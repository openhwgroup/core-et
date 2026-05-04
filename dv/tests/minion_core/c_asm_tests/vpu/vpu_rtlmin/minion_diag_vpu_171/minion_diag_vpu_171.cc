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
			 0xfb6c316c, /// 0x0
			 0x78c008ba, /// 0x4
			 0xfe23fca9, /// 0x8
			 0x544d02cc, /// 0xc
			 0x20fa901a, /// 0x10
			 0x625c9a25, /// 0x14
			 0x77e8119f, /// 0x18
			 0x75bafda9, /// 0x1c
			 0xf1ec427e, /// 0x20
			 0x42ce21be, /// 0x24
			 0x13729816, /// 0x28
			 0xfab206e3, /// 0x2c
			 0x90ba6406, /// 0x30
			 0x43665317, /// 0x34
			 0x747f66a9, /// 0x38
			 0x60a4bcdb, /// 0x3c
			 0x612545df, /// 0x40
			 0xd2a287a0, /// 0x44
			 0x81233412, /// 0x48
			 0xfb7cf347, /// 0x4c
			 0x278bc334, /// 0x50
			 0x7a7146f7, /// 0x54
			 0x36048886, /// 0x58
			 0x9f07f68f, /// 0x5c
			 0x5e7c4004, /// 0x60
			 0x04338045, /// 0x64
			 0x13d9566e, /// 0x68
			 0x249dc5bd, /// 0x6c
			 0x21c968a8, /// 0x70
			 0x34564a35, /// 0x74
			 0xfef24138, /// 0x78
			 0x90f0d487, /// 0x7c
			 0xdb06d5b8, /// 0x80
			 0x3c5dc034, /// 0x84
			 0x23505edd, /// 0x88
			 0x600b245f, /// 0x8c
			 0x7de0933b, /// 0x90
			 0x6ea6faca, /// 0x94
			 0x38300af8, /// 0x98
			 0x30313aad, /// 0x9c
			 0xcb0aeff0, /// 0xa0
			 0xa3061634, /// 0xa4
			 0xc6bc17a4, /// 0xa8
			 0xc49632cf, /// 0xac
			 0xb46b4a22, /// 0xb0
			 0x5b491169, /// 0xb4
			 0xbfd88271, /// 0xb8
			 0x59a603de, /// 0xbc
			 0x048181d8, /// 0xc0
			 0xbc9191ac, /// 0xc4
			 0x09fb7e8a, /// 0xc8
			 0xdc81e12d, /// 0xcc
			 0x8817fc29, /// 0xd0
			 0xca57ae18, /// 0xd4
			 0xe8d1e3ca, /// 0xd8
			 0xbe472cab, /// 0xdc
			 0x372104a1, /// 0xe0
			 0xa285af77, /// 0xe4
			 0xf780035b, /// 0xe8
			 0x987df7c5, /// 0xec
			 0x2776b087, /// 0xf0
			 0xdc9a10d0, /// 0xf4
			 0xaf6408bb, /// 0xf8
			 0x78d8ed72, /// 0xfc
			 0xbc607647, /// 0x100
			 0x2002ea51, /// 0x104
			 0xdc483558, /// 0x108
			 0x4194529b, /// 0x10c
			 0x341df34f, /// 0x110
			 0x4b981596, /// 0x114
			 0x5b6c4d73, /// 0x118
			 0x104ad08a, /// 0x11c
			 0xefd222b5, /// 0x120
			 0x6800469d, /// 0x124
			 0x61423598, /// 0x128
			 0xfff2cbe8, /// 0x12c
			 0xca80907b, /// 0x130
			 0x275318e4, /// 0x134
			 0xd4193adb, /// 0x138
			 0x35de17a9, /// 0x13c
			 0xc0183833, /// 0x140
			 0xb3537362, /// 0x144
			 0xd830ef7e, /// 0x148
			 0x9faf0540, /// 0x14c
			 0x8fe6f6d4, /// 0x150
			 0x9a726922, /// 0x154
			 0x170bd171, /// 0x158
			 0xc2754094, /// 0x15c
			 0xcea6b27a, /// 0x160
			 0xe505504f, /// 0x164
			 0xa36208af, /// 0x168
			 0x74673cc6, /// 0x16c
			 0x8139bbcd, /// 0x170
			 0x76288a36, /// 0x174
			 0xbedfebf5, /// 0x178
			 0xeae7292b, /// 0x17c
			 0x7d9bb041, /// 0x180
			 0x8890049b, /// 0x184
			 0x40f0d8d1, /// 0x188
			 0x60a62306, /// 0x18c
			 0xd5ab5f7b, /// 0x190
			 0xbc4eb1a7, /// 0x194
			 0x33f124dd, /// 0x198
			 0x9765203f, /// 0x19c
			 0x03024025, /// 0x1a0
			 0x698fcb81, /// 0x1a4
			 0x90c56353, /// 0x1a8
			 0x579c4f2a, /// 0x1ac
			 0x6f0d6481, /// 0x1b0
			 0xdc293008, /// 0x1b4
			 0x8e342380, /// 0x1b8
			 0x022f901c, /// 0x1bc
			 0xf259fa52, /// 0x1c0
			 0x25547b23, /// 0x1c4
			 0x02da9882, /// 0x1c8
			 0xcaa7c915, /// 0x1cc
			 0x2eb12042, /// 0x1d0
			 0xd07f58a7, /// 0x1d4
			 0x69c142b5, /// 0x1d8
			 0x106e8b76, /// 0x1dc
			 0x36b9d185, /// 0x1e0
			 0x98774ed8, /// 0x1e4
			 0x59701452, /// 0x1e8
			 0x702a1181, /// 0x1ec
			 0xbaa5a765, /// 0x1f0
			 0xe06b8c9b, /// 0x1f4
			 0x78c0bdbe, /// 0x1f8
			 0xcfeaa2b2, /// 0x1fc
			 0xe0c93d49, /// 0x200
			 0xc85a67c6, /// 0x204
			 0x032b920e, /// 0x208
			 0x9bc035d5, /// 0x20c
			 0xeed59caf, /// 0x210
			 0x22db1e20, /// 0x214
			 0xee733e8a, /// 0x218
			 0x9df6cb4d, /// 0x21c
			 0xee8f598f, /// 0x220
			 0x75cf7eea, /// 0x224
			 0x5545bc68, /// 0x228
			 0xcbe6eea7, /// 0x22c
			 0x81d6125b, /// 0x230
			 0x961bf735, /// 0x234
			 0x96611a68, /// 0x238
			 0x8c5d2606, /// 0x23c
			 0xab88e70d, /// 0x240
			 0x6f156817, /// 0x244
			 0x3e090f41, /// 0x248
			 0x33c63c48, /// 0x24c
			 0xd8af66aa, /// 0x250
			 0x26ffac8e, /// 0x254
			 0xe4941953, /// 0x258
			 0x3c521bbe, /// 0x25c
			 0x1c032273, /// 0x260
			 0xcdc134c0, /// 0x264
			 0xfbac1570, /// 0x268
			 0x9111a43c, /// 0x26c
			 0xf9eb4366, /// 0x270
			 0x6941a497, /// 0x274
			 0xed359254, /// 0x278
			 0x76a24cf6, /// 0x27c
			 0x7ec47358, /// 0x280
			 0xd1ce0fb3, /// 0x284
			 0x9dde0c86, /// 0x288
			 0x939754a7, /// 0x28c
			 0x9fd7e941, /// 0x290
			 0x0be416d2, /// 0x294
			 0xf27d2905, /// 0x298
			 0xb0742e65, /// 0x29c
			 0x5b8c52d4, /// 0x2a0
			 0x26f06b89, /// 0x2a4
			 0x48bf9c69, /// 0x2a8
			 0xa97e681f, /// 0x2ac
			 0x832bb24e, /// 0x2b0
			 0x0c356a17, /// 0x2b4
			 0x574a829b, /// 0x2b8
			 0x89776f40, /// 0x2bc
			 0x18ce8264, /// 0x2c0
			 0x21e066d5, /// 0x2c4
			 0xf1406019, /// 0x2c8
			 0x3bc3745d, /// 0x2cc
			 0xb91886c2, /// 0x2d0
			 0xe9e7bcf1, /// 0x2d4
			 0x219b95c0, /// 0x2d8
			 0xe94ebab8, /// 0x2dc
			 0xd20e4c65, /// 0x2e0
			 0xb7b37c61, /// 0x2e4
			 0x498f5b07, /// 0x2e8
			 0xb5f98d40, /// 0x2ec
			 0xa7b05bca, /// 0x2f0
			 0x061a5e76, /// 0x2f4
			 0x5bafa045, /// 0x2f8
			 0xd8ff5a54, /// 0x2fc
			 0x0c741b62, /// 0x300
			 0xe55f7942, /// 0x304
			 0xf2403bf3, /// 0x308
			 0x4ff84e16, /// 0x30c
			 0xc3069991, /// 0x310
			 0x46e4c936, /// 0x314
			 0xd9cbf5a5, /// 0x318
			 0xa196141c, /// 0x31c
			 0x61870093, /// 0x320
			 0x2a622f5d, /// 0x324
			 0x52b26e70, /// 0x328
			 0xa7af6074, /// 0x32c
			 0xccc4273f, /// 0x330
			 0xba139bb7, /// 0x334
			 0x85bcf72e, /// 0x338
			 0xe531172c, /// 0x33c
			 0xcd97ac29, /// 0x340
			 0x87eba8f0, /// 0x344
			 0xa84d4a7e, /// 0x348
			 0x1d57ee0e, /// 0x34c
			 0x235b19f5, /// 0x350
			 0x52d11f2a, /// 0x354
			 0x4f81b344, /// 0x358
			 0xcf0855ab, /// 0x35c
			 0xa3a28459, /// 0x360
			 0x80dcf6c5, /// 0x364
			 0xc17ce586, /// 0x368
			 0xa49f503f, /// 0x36c
			 0x149cdc61, /// 0x370
			 0x68272d0d, /// 0x374
			 0x601aa4a5, /// 0x378
			 0x802e97bc, /// 0x37c
			 0x5dd18f5b, /// 0x380
			 0x2b470166, /// 0x384
			 0xea9b81d6, /// 0x388
			 0x9ced92cc, /// 0x38c
			 0xb9dd6861, /// 0x390
			 0x4e14c8a9, /// 0x394
			 0x916136c5, /// 0x398
			 0xb9a20037, /// 0x39c
			 0x1a73d76a, /// 0x3a0
			 0x3b9a2012, /// 0x3a4
			 0x0fc65eab, /// 0x3a8
			 0x88e26ce4, /// 0x3ac
			 0x2e814fa1, /// 0x3b0
			 0x71556b56, /// 0x3b4
			 0xfaaba9e9, /// 0x3b8
			 0x27ab1343, /// 0x3bc
			 0x67006a42, /// 0x3c0
			 0xce0dffaa, /// 0x3c4
			 0x98eb7f74, /// 0x3c8
			 0xfa45079b, /// 0x3cc
			 0xe9a5e8b4, /// 0x3d0
			 0xc17e9477, /// 0x3d4
			 0x0e5a7416, /// 0x3d8
			 0x51468801, /// 0x3dc
			 0xb7688dbc, /// 0x3e0
			 0x2276ef70, /// 0x3e4
			 0xaa950c36, /// 0x3e8
			 0xbc9fa473, /// 0x3ec
			 0xbe809f1f, /// 0x3f0
			 0x1bd80e0b, /// 0x3f4
			 0x7bd0eb0e, /// 0x3f8
			 0x839c7c3f, /// 0x3fc
			 0xdd70b554, /// 0x400
			 0x0cfb5c19, /// 0x404
			 0xfedc8ffa, /// 0x408
			 0x3aca2949, /// 0x40c
			 0xb8606060, /// 0x410
			 0x63e1e3a5, /// 0x414
			 0xedb32bb4, /// 0x418
			 0x83752b0c, /// 0x41c
			 0x2542d432, /// 0x420
			 0x99cd1c3a, /// 0x424
			 0xec265b43, /// 0x428
			 0x102ed16f, /// 0x42c
			 0x77578d44, /// 0x430
			 0xb16a1080, /// 0x434
			 0xa3240109, /// 0x438
			 0x5dacfb79, /// 0x43c
			 0x17ed5d4a, /// 0x440
			 0x352cf38d, /// 0x444
			 0xe89dbb0e, /// 0x448
			 0xfd64d0a0, /// 0x44c
			 0x09313071, /// 0x450
			 0x56567732, /// 0x454
			 0xfc5c53fc, /// 0x458
			 0x9b41ec43, /// 0x45c
			 0xf26479a4, /// 0x460
			 0x7a1399ba, /// 0x464
			 0x8ef68b3e, /// 0x468
			 0xe6256de3, /// 0x46c
			 0x1401189e, /// 0x470
			 0x13e9c6e0, /// 0x474
			 0x1377c005, /// 0x478
			 0xaaafb388, /// 0x47c
			 0x9f86aa2e, /// 0x480
			 0xc2a43abf, /// 0x484
			 0x04f3dded, /// 0x488
			 0x53eed2c3, /// 0x48c
			 0x417c959f, /// 0x490
			 0xac389e2d, /// 0x494
			 0x25c23798, /// 0x498
			 0x14e079f9, /// 0x49c
			 0x92260777, /// 0x4a0
			 0xc4eb239b, /// 0x4a4
			 0xecfb5266, /// 0x4a8
			 0xb9519f95, /// 0x4ac
			 0x96647ae1, /// 0x4b0
			 0xd07c4b3f, /// 0x4b4
			 0x6b049a7d, /// 0x4b8
			 0x0a24d00d, /// 0x4bc
			 0xfedaf816, /// 0x4c0
			 0xe6eb6594, /// 0x4c4
			 0xdfee3d06, /// 0x4c8
			 0xb28dce32, /// 0x4cc
			 0x5ec73543, /// 0x4d0
			 0x69d686a5, /// 0x4d4
			 0x0096acec, /// 0x4d8
			 0xbc3a0153, /// 0x4dc
			 0xbbcf4379, /// 0x4e0
			 0x75ed64ea, /// 0x4e4
			 0x05c02293, /// 0x4e8
			 0xe6aac0c7, /// 0x4ec
			 0xbcbe8680, /// 0x4f0
			 0x2703094d, /// 0x4f4
			 0xc70ad954, /// 0x4f8
			 0x4912f4c6, /// 0x4fc
			 0xe0987ba7, /// 0x500
			 0xce853b88, /// 0x504
			 0x91feb1f8, /// 0x508
			 0x8d2948fe, /// 0x50c
			 0x51b59b82, /// 0x510
			 0x44a2c932, /// 0x514
			 0xf1621f45, /// 0x518
			 0x7f4d3ae1, /// 0x51c
			 0xe78fd00b, /// 0x520
			 0x30735030, /// 0x524
			 0xa570f1d8, /// 0x528
			 0x954c4cae, /// 0x52c
			 0x96f839ba, /// 0x530
			 0xd87982f5, /// 0x534
			 0x66a50ea8, /// 0x538
			 0x7d2e31b2, /// 0x53c
			 0xefb443a6, /// 0x540
			 0xe785cd0b, /// 0x544
			 0xfcb29970, /// 0x548
			 0x8924fc9b, /// 0x54c
			 0x69e0b423, /// 0x550
			 0xb2e17f71, /// 0x554
			 0xf35a3eff, /// 0x558
			 0x9367fe09, /// 0x55c
			 0xa5dcde07, /// 0x560
			 0x9c0516bc, /// 0x564
			 0xbb2662f0, /// 0x568
			 0x8e445cc4, /// 0x56c
			 0x50566649, /// 0x570
			 0x939635b0, /// 0x574
			 0x9995e57f, /// 0x578
			 0x0f166a23, /// 0x57c
			 0x4d9c88da, /// 0x580
			 0x9833b99d, /// 0x584
			 0x514ee2bc, /// 0x588
			 0x2e1990e3, /// 0x58c
			 0xc5a5adbe, /// 0x590
			 0xfc8a586d, /// 0x594
			 0xeb57a5d1, /// 0x598
			 0x8480a7d3, /// 0x59c
			 0xca5b04d8, /// 0x5a0
			 0x2eb363af, /// 0x5a4
			 0x1353e2dc, /// 0x5a8
			 0xa5c67d67, /// 0x5ac
			 0x936ad80d, /// 0x5b0
			 0x06588ad2, /// 0x5b4
			 0xeec86b03, /// 0x5b8
			 0x1021b598, /// 0x5bc
			 0x8ffeca2d, /// 0x5c0
			 0x6a92363b, /// 0x5c4
			 0x28ad8f8f, /// 0x5c8
			 0x1ef84240, /// 0x5cc
			 0xab9598e5, /// 0x5d0
			 0xc307eacc, /// 0x5d4
			 0x85f6ff31, /// 0x5d8
			 0xe5bb354a, /// 0x5dc
			 0x2c5c64c2, /// 0x5e0
			 0xa9c0fc88, /// 0x5e4
			 0x044e157a, /// 0x5e8
			 0xc69583d9, /// 0x5ec
			 0x628505ca, /// 0x5f0
			 0x7dce9306, /// 0x5f4
			 0x18e1a614, /// 0x5f8
			 0xfbcd4a5b, /// 0x5fc
			 0x7b6879d0, /// 0x600
			 0x9167fd1b, /// 0x604
			 0x9f1f2243, /// 0x608
			 0x6799da2d, /// 0x60c
			 0x8348c5eb, /// 0x610
			 0xba7af8a5, /// 0x614
			 0x2dea4dc1, /// 0x618
			 0x329c1f55, /// 0x61c
			 0x3fe8a34a, /// 0x620
			 0xb7947b1c, /// 0x624
			 0x37ba099e, /// 0x628
			 0xdc9cb023, /// 0x62c
			 0xed159bf0, /// 0x630
			 0x3f2f1352, /// 0x634
			 0x9ae236fc, /// 0x638
			 0x4fcea6cf, /// 0x63c
			 0x1a8a9881, /// 0x640
			 0x44d841b0, /// 0x644
			 0xa4549784, /// 0x648
			 0xacf10962, /// 0x64c
			 0x0109a6eb, /// 0x650
			 0xf352c05d, /// 0x654
			 0x17870a11, /// 0x658
			 0xb0168bb4, /// 0x65c
			 0x206e9212, /// 0x660
			 0x99f45d2d, /// 0x664
			 0x9c3869e0, /// 0x668
			 0x3d4d1518, /// 0x66c
			 0x956513fe, /// 0x670
			 0x40daa6f1, /// 0x674
			 0xf7fbbb34, /// 0x678
			 0xdd051471, /// 0x67c
			 0x3b863101, /// 0x680
			 0x6772d601, /// 0x684
			 0xa14eacef, /// 0x688
			 0x5d68f1ac, /// 0x68c
			 0x0d982973, /// 0x690
			 0x5dad23ab, /// 0x694
			 0x2df914a4, /// 0x698
			 0x22950fe2, /// 0x69c
			 0xe284c53e, /// 0x6a0
			 0xcc2f97e0, /// 0x6a4
			 0x5b853e88, /// 0x6a8
			 0xf999f5ed, /// 0x6ac
			 0x3161e2b7, /// 0x6b0
			 0x4cc33007, /// 0x6b4
			 0xa1cab797, /// 0x6b8
			 0x9173df22, /// 0x6bc
			 0xf41e8698, /// 0x6c0
			 0x694205f3, /// 0x6c4
			 0xd9279db1, /// 0x6c8
			 0x25e42213, /// 0x6cc
			 0x15f84da7, /// 0x6d0
			 0x3313d9dd, /// 0x6d4
			 0x575ed746, /// 0x6d8
			 0x850b03f8, /// 0x6dc
			 0x5557d38c, /// 0x6e0
			 0x0ec067aa, /// 0x6e4
			 0x7cdd76fd, /// 0x6e8
			 0xe30ff68f, /// 0x6ec
			 0xebe9c8f7, /// 0x6f0
			 0x910c5c28, /// 0x6f4
			 0xaa017bf6, /// 0x6f8
			 0x1274e32a, /// 0x6fc
			 0xdaed06b2, /// 0x700
			 0xb42347fb, /// 0x704
			 0xce22cb17, /// 0x708
			 0x4eb460e1, /// 0x70c
			 0xdf051e8a, /// 0x710
			 0xced786b4, /// 0x714
			 0xca484992, /// 0x718
			 0xe65ac964, /// 0x71c
			 0xb004a738, /// 0x720
			 0x7d6be82b, /// 0x724
			 0x2e46fb42, /// 0x728
			 0xb2f6d7ec, /// 0x72c
			 0x1cd20ae6, /// 0x730
			 0x81d0088a, /// 0x734
			 0xee55a2e1, /// 0x738
			 0x73561ede, /// 0x73c
			 0x35d901bf, /// 0x740
			 0x4ed22fef, /// 0x744
			 0x3e75d07b, /// 0x748
			 0xf2b8a592, /// 0x74c
			 0x61ff2ac1, /// 0x750
			 0x62057906, /// 0x754
			 0x90d1507a, /// 0x758
			 0x8ebd4d7b, /// 0x75c
			 0x4c663ce2, /// 0x760
			 0xffe666e1, /// 0x764
			 0xcf8e9577, /// 0x768
			 0xf989f367, /// 0x76c
			 0x35fcbafa, /// 0x770
			 0x295b490e, /// 0x774
			 0xe132d028, /// 0x778
			 0x1e3a5d33, /// 0x77c
			 0x4c55a25f, /// 0x780
			 0x9cae7f0b, /// 0x784
			 0xe170a118, /// 0x788
			 0x7001adb5, /// 0x78c
			 0xeb3b4d80, /// 0x790
			 0xa280fe9c, /// 0x794
			 0x67a5ce2a, /// 0x798
			 0xbfa7de50, /// 0x79c
			 0x0270ff44, /// 0x7a0
			 0x4a29ef60, /// 0x7a4
			 0x4dced0fd, /// 0x7a8
			 0x1b4e8662, /// 0x7ac
			 0x31ec0ab8, /// 0x7b0
			 0xb1d88d58, /// 0x7b4
			 0xac662a91, /// 0x7b8
			 0x94f90b83, /// 0x7bc
			 0x2c777d44, /// 0x7c0
			 0xb1a78c85, /// 0x7c4
			 0x378542bf, /// 0x7c8
			 0xc57eb858, /// 0x7cc
			 0xef2bd6a7, /// 0x7d0
			 0x9b7455ef, /// 0x7d4
			 0x2e530312, /// 0x7d8
			 0x5d9f14a8, /// 0x7dc
			 0xddff6102, /// 0x7e0
			 0x2af3623f, /// 0x7e4
			 0x8f516c19, /// 0x7e8
			 0xb2ba6cd5, /// 0x7ec
			 0x8cf7cb43, /// 0x7f0
			 0x15a32616, /// 0x7f4
			 0x535828c2, /// 0x7f8
			 0xbe1f71e8, /// 0x7fc
			 0x6d3dbd41, /// 0x800
			 0x919c175b, /// 0x804
			 0x3f6af103, /// 0x808
			 0xceb8a688, /// 0x80c
			 0xe55e2ed2, /// 0x810
			 0xa20658a8, /// 0x814
			 0x789244a9, /// 0x818
			 0x19fb6fa7, /// 0x81c
			 0x877e4ec3, /// 0x820
			 0xfba04cef, /// 0x824
			 0xcf04a4c4, /// 0x828
			 0x93b22d5f, /// 0x82c
			 0x730451ba, /// 0x830
			 0xcf8db098, /// 0x834
			 0x61c1726c, /// 0x838
			 0x039677ba, /// 0x83c
			 0xa7f1a397, /// 0x840
			 0xadb5ff7a, /// 0x844
			 0x44831998, /// 0x848
			 0xf01e90b7, /// 0x84c
			 0x4c763884, /// 0x850
			 0xecf38f07, /// 0x854
			 0x635c43c1, /// 0x858
			 0x7120d62b, /// 0x85c
			 0x23efd189, /// 0x860
			 0xc042baf0, /// 0x864
			 0x328520aa, /// 0x868
			 0xb37b4beb, /// 0x86c
			 0x28a1d8cd, /// 0x870
			 0xb0c9c3c2, /// 0x874
			 0x97d0f6ec, /// 0x878
			 0x2a67aaf3, /// 0x87c
			 0xcdbed2e4, /// 0x880
			 0x4e2badc0, /// 0x884
			 0x43e99aa6, /// 0x888
			 0x6d276711, /// 0x88c
			 0x6b676b77, /// 0x890
			 0xed732c53, /// 0x894
			 0xff500f5a, /// 0x898
			 0xc7085623, /// 0x89c
			 0x51dbcbc1, /// 0x8a0
			 0xf338b490, /// 0x8a4
			 0x2932913c, /// 0x8a8
			 0x92a335e2, /// 0x8ac
			 0xb8579281, /// 0x8b0
			 0x9317c455, /// 0x8b4
			 0xbef77b22, /// 0x8b8
			 0xdf432bec, /// 0x8bc
			 0x6e7cba98, /// 0x8c0
			 0x59c2bacb, /// 0x8c4
			 0x8e931550, /// 0x8c8
			 0xea8ff7bc, /// 0x8cc
			 0x4a090431, /// 0x8d0
			 0x1d4a9f95, /// 0x8d4
			 0x7129d903, /// 0x8d8
			 0x2e8d861f, /// 0x8dc
			 0x404da56a, /// 0x8e0
			 0x5bd62909, /// 0x8e4
			 0xaecbb60b, /// 0x8e8
			 0xc0a85198, /// 0x8ec
			 0xe3e28078, /// 0x8f0
			 0xeec9b619, /// 0x8f4
			 0xd029123e, /// 0x8f8
			 0x690e5876, /// 0x8fc
			 0x1c589ccd, /// 0x900
			 0xcf9c8ce6, /// 0x904
			 0x4e1e1a63, /// 0x908
			 0x6564c436, /// 0x90c
			 0xfb22aa4a, /// 0x910
			 0xfdaa99da, /// 0x914
			 0xa7b8e45b, /// 0x918
			 0x2a18073a, /// 0x91c
			 0x80960dda, /// 0x920
			 0x02a85bd1, /// 0x924
			 0x7de2e74a, /// 0x928
			 0xe32f7f8f, /// 0x92c
			 0x22c2cd87, /// 0x930
			 0x960f0314, /// 0x934
			 0x0a835e18, /// 0x938
			 0x18ff686c, /// 0x93c
			 0x89799549, /// 0x940
			 0x3649b3eb, /// 0x944
			 0xda3090fb, /// 0x948
			 0xfc7c9d0b, /// 0x94c
			 0xff8cc171, /// 0x950
			 0xefedd89b, /// 0x954
			 0x694f9355, /// 0x958
			 0x7a54a437, /// 0x95c
			 0xdaf16e63, /// 0x960
			 0xb79ec505, /// 0x964
			 0xb14979ad, /// 0x968
			 0x386fac73, /// 0x96c
			 0x5435b1bb, /// 0x970
			 0x33788f15, /// 0x974
			 0x9c70a27d, /// 0x978
			 0x09aa0649, /// 0x97c
			 0xe9cf3815, /// 0x980
			 0x68a6bea5, /// 0x984
			 0x019a5dd0, /// 0x988
			 0x9c954d70, /// 0x98c
			 0x95c57a70, /// 0x990
			 0x98644a0f, /// 0x994
			 0xaf107a0f, /// 0x998
			 0x213a95a0, /// 0x99c
			 0x52b1db1c, /// 0x9a0
			 0xda37a41e, /// 0x9a4
			 0x4d4b43fc, /// 0x9a8
			 0x9e8fc556, /// 0x9ac
			 0x789c1b15, /// 0x9b0
			 0x6be9a768, /// 0x9b4
			 0x2f47dcf1, /// 0x9b8
			 0x4334b877, /// 0x9bc
			 0xf4956173, /// 0x9c0
			 0x54270ddc, /// 0x9c4
			 0xa3a33b24, /// 0x9c8
			 0x3d42c6e2, /// 0x9cc
			 0x295797e7, /// 0x9d0
			 0xd8b784ec, /// 0x9d4
			 0xce41e0cf, /// 0x9d8
			 0x40fdc762, /// 0x9dc
			 0x2e4a68f1, /// 0x9e0
			 0xfb20d0f7, /// 0x9e4
			 0x190bbd61, /// 0x9e8
			 0x0046b938, /// 0x9ec
			 0xc0154e80, /// 0x9f0
			 0xdf6c7cca, /// 0x9f4
			 0x9d76e751, /// 0x9f8
			 0x312d39bf, /// 0x9fc
			 0x6d922678, /// 0xa00
			 0x5e2e1f30, /// 0xa04
			 0x7304e930, /// 0xa08
			 0xc816b977, /// 0xa0c
			 0x052f33e1, /// 0xa10
			 0x14527f5e, /// 0xa14
			 0x40e8e44a, /// 0xa18
			 0xf8925926, /// 0xa1c
			 0x8eff9ba0, /// 0xa20
			 0xecdd4bd2, /// 0xa24
			 0xd4453f43, /// 0xa28
			 0xf7d1a0f2, /// 0xa2c
			 0xe164907d, /// 0xa30
			 0x2c89ef84, /// 0xa34
			 0xa8f8d01b, /// 0xa38
			 0x8c830f79, /// 0xa3c
			 0xc4d71ad3, /// 0xa40
			 0x153a2e27, /// 0xa44
			 0x4e98ab95, /// 0xa48
			 0x07387845, /// 0xa4c
			 0x5e88d6a5, /// 0xa50
			 0xc30b4254, /// 0xa54
			 0x7b8791fd, /// 0xa58
			 0xf26dcfa1, /// 0xa5c
			 0xe64187bc, /// 0xa60
			 0x15cc002a, /// 0xa64
			 0x3bf102c2, /// 0xa68
			 0x16b4f42e, /// 0xa6c
			 0xb06effff, /// 0xa70
			 0x989c8b0f, /// 0xa74
			 0x37ff6272, /// 0xa78
			 0x71bb673a, /// 0xa7c
			 0x2f30e87d, /// 0xa80
			 0xd2ea809a, /// 0xa84
			 0x79ec72e3, /// 0xa88
			 0xd338bf9c, /// 0xa8c
			 0xc2f0a2ff, /// 0xa90
			 0xe20587a0, /// 0xa94
			 0xfc548244, /// 0xa98
			 0x13da72f6, /// 0xa9c
			 0x19b54d14, /// 0xaa0
			 0xea522a8f, /// 0xaa4
			 0xffdceae5, /// 0xaa8
			 0x546737b6, /// 0xaac
			 0xb6041c73, /// 0xab0
			 0x9d67b214, /// 0xab4
			 0x19021f7e, /// 0xab8
			 0x02dc9164, /// 0xabc
			 0x80b04ff7, /// 0xac0
			 0x81afbb8b, /// 0xac4
			 0x0e93e2bf, /// 0xac8
			 0xd610b6b1, /// 0xacc
			 0xad59fa45, /// 0xad0
			 0x550ea083, /// 0xad4
			 0x60b74f97, /// 0xad8
			 0x4ab4c84a, /// 0xadc
			 0xa7027f2b, /// 0xae0
			 0x214c716d, /// 0xae4
			 0x227db421, /// 0xae8
			 0x9eb30a4e, /// 0xaec
			 0x3a974fd7, /// 0xaf0
			 0x1446d788, /// 0xaf4
			 0xcb97eb37, /// 0xaf8
			 0x8ee5bf19, /// 0xafc
			 0x76319278, /// 0xb00
			 0xec1dd340, /// 0xb04
			 0x4a7ac80f, /// 0xb08
			 0x73a34bbb, /// 0xb0c
			 0x39b9c20a, /// 0xb10
			 0x23995148, /// 0xb14
			 0xbb06e472, /// 0xb18
			 0x5526dc7b, /// 0xb1c
			 0x7759035d, /// 0xb20
			 0x6e0483d8, /// 0xb24
			 0x209ca003, /// 0xb28
			 0x5fc698bc, /// 0xb2c
			 0xab571d34, /// 0xb30
			 0x5d17c5b6, /// 0xb34
			 0xb34b3cc5, /// 0xb38
			 0xd257bf84, /// 0xb3c
			 0xc00f6a10, /// 0xb40
			 0x7f54678f, /// 0xb44
			 0xcd0b3f01, /// 0xb48
			 0x7f910a94, /// 0xb4c
			 0xeca08992, /// 0xb50
			 0x7107558a, /// 0xb54
			 0x9c4be7bc, /// 0xb58
			 0x41fbeabf, /// 0xb5c
			 0xddbc8169, /// 0xb60
			 0x9e3a08c8, /// 0xb64
			 0x947cbd12, /// 0xb68
			 0x3ea858b7, /// 0xb6c
			 0x3a252525, /// 0xb70
			 0x7093328a, /// 0xb74
			 0xced61338, /// 0xb78
			 0x9cb87846, /// 0xb7c
			 0xd913e706, /// 0xb80
			 0x0ce7a06c, /// 0xb84
			 0x83f75abc, /// 0xb88
			 0xb2e25359, /// 0xb8c
			 0x225c6792, /// 0xb90
			 0xa09900af, /// 0xb94
			 0xc3d6fd0d, /// 0xb98
			 0x43778965, /// 0xb9c
			 0x91b33122, /// 0xba0
			 0x175ad3b4, /// 0xba4
			 0x9d3db39e, /// 0xba8
			 0x7757c248, /// 0xbac
			 0xadfc7dee, /// 0xbb0
			 0x220c6805, /// 0xbb4
			 0x7609dfb5, /// 0xbb8
			 0x7f63157d, /// 0xbbc
			 0xb308606a, /// 0xbc0
			 0xfd8bef13, /// 0xbc4
			 0xefc65c78, /// 0xbc8
			 0x64b436fd, /// 0xbcc
			 0xf903b715, /// 0xbd0
			 0xa05cb91c, /// 0xbd4
			 0xc618913d, /// 0xbd8
			 0x3a2a14dc, /// 0xbdc
			 0x9441faff, /// 0xbe0
			 0xa292b697, /// 0xbe4
			 0xf8b01d5b, /// 0xbe8
			 0x5d00a13b, /// 0xbec
			 0x0aac2290, /// 0xbf0
			 0xef3b987b, /// 0xbf4
			 0x243411e2, /// 0xbf8
			 0xb7e3cf14, /// 0xbfc
			 0xad5cc52c, /// 0xc00
			 0x349de97b, /// 0xc04
			 0x285c0271, /// 0xc08
			 0x0adfd951, /// 0xc0c
			 0xc2234618, /// 0xc10
			 0xe4cf3b6e, /// 0xc14
			 0x60a93db7, /// 0xc18
			 0x53d8b724, /// 0xc1c
			 0x99e79d92, /// 0xc20
			 0x08dfee44, /// 0xc24
			 0xb8f4272b, /// 0xc28
			 0xdde6696a, /// 0xc2c
			 0xce1f824b, /// 0xc30
			 0xaa23a5bc, /// 0xc34
			 0xe2446714, /// 0xc38
			 0xa477da19, /// 0xc3c
			 0x45918b54, /// 0xc40
			 0x6c94f393, /// 0xc44
			 0x626fce0d, /// 0xc48
			 0xc20d217e, /// 0xc4c
			 0xb24082d1, /// 0xc50
			 0xa3684d45, /// 0xc54
			 0xb9076c2f, /// 0xc58
			 0xdd36e54a, /// 0xc5c
			 0x4ed2f383, /// 0xc60
			 0x2f55b265, /// 0xc64
			 0x5bd9f3d9, /// 0xc68
			 0x5db0275b, /// 0xc6c
			 0x030ebd34, /// 0xc70
			 0xc5359b31, /// 0xc74
			 0x69fce58b, /// 0xc78
			 0xfc82520c, /// 0xc7c
			 0x7f8dfdec, /// 0xc80
			 0x0a1cbf86, /// 0xc84
			 0x60144d15, /// 0xc88
			 0x7444abdf, /// 0xc8c
			 0xca52a75f, /// 0xc90
			 0x6849fb09, /// 0xc94
			 0x5b87fda8, /// 0xc98
			 0x88af353d, /// 0xc9c
			 0x920bed59, /// 0xca0
			 0x41c88562, /// 0xca4
			 0x4365a002, /// 0xca8
			 0x042c1cdc, /// 0xcac
			 0x28ecca8f, /// 0xcb0
			 0xdf578f3d, /// 0xcb4
			 0x10f124f9, /// 0xcb8
			 0x636e8ca1, /// 0xcbc
			 0x0d72b492, /// 0xcc0
			 0x26970a15, /// 0xcc4
			 0x3bb57f3e, /// 0xcc8
			 0xce70fa8d, /// 0xccc
			 0x5db52fc2, /// 0xcd0
			 0x9338cc53, /// 0xcd4
			 0xfce24516, /// 0xcd8
			 0x62021087, /// 0xcdc
			 0x9ace28cf, /// 0xce0
			 0x7c9481f4, /// 0xce4
			 0x41dadcc4, /// 0xce8
			 0x16c7414b, /// 0xcec
			 0x1bea7409, /// 0xcf0
			 0x9ae565c0, /// 0xcf4
			 0x131e10cd, /// 0xcf8
			 0x978acb1d, /// 0xcfc
			 0x4decaee3, /// 0xd00
			 0x1a79d6ce, /// 0xd04
			 0xbceea4cd, /// 0xd08
			 0x7667138b, /// 0xd0c
			 0xcb6d67f0, /// 0xd10
			 0x900fe8f5, /// 0xd14
			 0xe1f0f0f0, /// 0xd18
			 0xd703e371, /// 0xd1c
			 0xced7d038, /// 0xd20
			 0x131ff45f, /// 0xd24
			 0x6aa185c3, /// 0xd28
			 0xe8744267, /// 0xd2c
			 0x1ab7347d, /// 0xd30
			 0xf98e1c6f, /// 0xd34
			 0x50bb0bd3, /// 0xd38
			 0x372b57e0, /// 0xd3c
			 0x885f53c4, /// 0xd40
			 0x65a24a3e, /// 0xd44
			 0x85763259, /// 0xd48
			 0x19e1d540, /// 0xd4c
			 0x5591852c, /// 0xd50
			 0x2f0277af, /// 0xd54
			 0xe8c1561b, /// 0xd58
			 0x96ccf9f9, /// 0xd5c
			 0x6ff6f131, /// 0xd60
			 0x1f4294fb, /// 0xd64
			 0xd8240c84, /// 0xd68
			 0x5b7a8279, /// 0xd6c
			 0xd98d5dba, /// 0xd70
			 0x89d6a2f6, /// 0xd74
			 0x2149afe3, /// 0xd78
			 0x14059673, /// 0xd7c
			 0xf259f4c5, /// 0xd80
			 0xf93fd571, /// 0xd84
			 0xbe8c5af3, /// 0xd88
			 0xe4a15682, /// 0xd8c
			 0xc9b9fb06, /// 0xd90
			 0x29bf66f7, /// 0xd94
			 0x318c9f70, /// 0xd98
			 0x1aaedd82, /// 0xd9c
			 0x0709fe59, /// 0xda0
			 0xeee07fab, /// 0xda4
			 0xc31bb630, /// 0xda8
			 0x1995a7bb, /// 0xdac
			 0x668767ae, /// 0xdb0
			 0xb1aa8da4, /// 0xdb4
			 0x36a7f30f, /// 0xdb8
			 0x0ffbafba, /// 0xdbc
			 0x75a66ea9, /// 0xdc0
			 0x4112eb55, /// 0xdc4
			 0x2c843ffa, /// 0xdc8
			 0x6994b1d6, /// 0xdcc
			 0x3160c659, /// 0xdd0
			 0x16e92ebc, /// 0xdd4
			 0xbec8786f, /// 0xdd8
			 0xa2adeff4, /// 0xddc
			 0x09e8b2da, /// 0xde0
			 0x05b0933f, /// 0xde4
			 0xa0a2de98, /// 0xde8
			 0xc6ce1ddd, /// 0xdec
			 0x3a6908dd, /// 0xdf0
			 0xa37e79c4, /// 0xdf4
			 0xc8eb036c, /// 0xdf8
			 0x5ffc00f9, /// 0xdfc
			 0x2c9c55dd, /// 0xe00
			 0xc5b745b3, /// 0xe04
			 0x857c4e64, /// 0xe08
			 0x58c40ba3, /// 0xe0c
			 0x705c599a, /// 0xe10
			 0xd2f22500, /// 0xe14
			 0x0a6612c2, /// 0xe18
			 0x89383869, /// 0xe1c
			 0x0923c465, /// 0xe20
			 0xb0ff4562, /// 0xe24
			 0xa9e1f007, /// 0xe28
			 0x9fa716ec, /// 0xe2c
			 0xaf40fa27, /// 0xe30
			 0xdd023837, /// 0xe34
			 0x0c820ab1, /// 0xe38
			 0x046da87d, /// 0xe3c
			 0xf87b99e2, /// 0xe40
			 0x6d159455, /// 0xe44
			 0xa0bb7cef, /// 0xe48
			 0x7eca3e2a, /// 0xe4c
			 0x0b571d58, /// 0xe50
			 0xdc621901, /// 0xe54
			 0x5613bc82, /// 0xe58
			 0x97491052, /// 0xe5c
			 0x0fafa384, /// 0xe60
			 0x57456953, /// 0xe64
			 0x858064cb, /// 0xe68
			 0xad23c016, /// 0xe6c
			 0x85f06741, /// 0xe70
			 0x8bcc87e8, /// 0xe74
			 0x29a213cd, /// 0xe78
			 0x7dcf9d2b, /// 0xe7c
			 0xe57cb6d4, /// 0xe80
			 0x4f97fdc8, /// 0xe84
			 0x9da60b08, /// 0xe88
			 0x0963e383, /// 0xe8c
			 0xef810f3e, /// 0xe90
			 0x0f7aad8e, /// 0xe94
			 0xcc08c428, /// 0xe98
			 0x3c73e6ec, /// 0xe9c
			 0x8a006c9f, /// 0xea0
			 0x73ae3051, /// 0xea4
			 0xe9917182, /// 0xea8
			 0xfe4bc802, /// 0xeac
			 0xa6d60dd1, /// 0xeb0
			 0xe3e930f4, /// 0xeb4
			 0x7e357221, /// 0xeb8
			 0xbb6f028c, /// 0xebc
			 0x53815fd4, /// 0xec0
			 0xa092eff7, /// 0xec4
			 0x794aac76, /// 0xec8
			 0xbee4e687, /// 0xecc
			 0x4ae0b241, /// 0xed0
			 0xe2557ba0, /// 0xed4
			 0x3d224243, /// 0xed8
			 0xea5c029e, /// 0xedc
			 0xa64c149a, /// 0xee0
			 0xc80d9ee7, /// 0xee4
			 0xc4aaad06, /// 0xee8
			 0xb6d8b6be, /// 0xeec
			 0x704f2171, /// 0xef0
			 0xa8554bf5, /// 0xef4
			 0xec1e807b, /// 0xef8
			 0x51c3bbd0, /// 0xefc
			 0x987b343f, /// 0xf00
			 0x94a36cef, /// 0xf04
			 0x9d672105, /// 0xf08
			 0x0f44c41e, /// 0xf0c
			 0xdf3dc24c, /// 0xf10
			 0x64edf01f, /// 0xf14
			 0x5d7a8047, /// 0xf18
			 0xd003735f, /// 0xf1c
			 0x16adbe00, /// 0xf20
			 0x60fcf6f2, /// 0xf24
			 0xaf2f431f, /// 0xf28
			 0xe39998c2, /// 0xf2c
			 0x7d83f222, /// 0xf30
			 0xc421b2d3, /// 0xf34
			 0x39d10892, /// 0xf38
			 0xc65fb316, /// 0xf3c
			 0xea882c88, /// 0xf40
			 0x9b6e2564, /// 0xf44
			 0x830c30c5, /// 0xf48
			 0xb173b999, /// 0xf4c
			 0x0989f3cd, /// 0xf50
			 0x8a50386d, /// 0xf54
			 0x82f5c33a, /// 0xf58
			 0x75bc37b3, /// 0xf5c
			 0xc5267e7c, /// 0xf60
			 0xf1fc705f, /// 0xf64
			 0x27f335be, /// 0xf68
			 0x954841c7, /// 0xf6c
			 0x5534ff33, /// 0xf70
			 0xeabf4da9, /// 0xf74
			 0x5fd66cb1, /// 0xf78
			 0x085707ed, /// 0xf7c
			 0x25d86778, /// 0xf80
			 0x673f75dd, /// 0xf84
			 0xd15e6798, /// 0xf88
			 0x2e646b02, /// 0xf8c
			 0x2f60231e, /// 0xf90
			 0xe16963fa, /// 0xf94
			 0x8a51958e, /// 0xf98
			 0xda4c317d, /// 0xf9c
			 0x9a3f837d, /// 0xfa0
			 0x245f3c38, /// 0xfa4
			 0xbeaa0e99, /// 0xfa8
			 0x60f854ec, /// 0xfac
			 0x019c54e8, /// 0xfb0
			 0x2ac863d2, /// 0xfb4
			 0xcd387561, /// 0xfb8
			 0x2b47341c, /// 0xfbc
			 0x5233cbac, /// 0xfc0
			 0x29859054, /// 0xfc4
			 0x09886ea7, /// 0xfc8
			 0xb5143d17, /// 0xfcc
			 0xd9c4b32b, /// 0xfd0
			 0xe0ae2d1b, /// 0xfd4
			 0x32e5fbbe, /// 0xfd8
			 0x37fa2681, /// 0xfdc
			 0xf9f03f44, /// 0xfe0
			 0x0cac8988, /// 0xfe4
			 0xe751065a, /// 0xfe8
			 0x439b7e47, /// 0xfec
			 0xd12b1ae2, /// 0xff0
			 0xb3ab1ad3, /// 0xff4
			 0xa178abf7, /// 0xff8
			 0xe795376e /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x8382f687, /// 0x0
			 0x5df6b9b2, /// 0x4
			 0x10d95ff0, /// 0x8
			 0x2bce26e4, /// 0xc
			 0xb1df09e0, /// 0x10
			 0x65ee5cb3, /// 0x14
			 0x2e077b27, /// 0x18
			 0x4a99bd10, /// 0x1c
			 0xc484c89b, /// 0x20
			 0xaa1655bb, /// 0x24
			 0x2f54ec73, /// 0x28
			 0x0a2c36c6, /// 0x2c
			 0x87c4e075, /// 0x30
			 0x553173e2, /// 0x34
			 0xc46fdf05, /// 0x38
			 0x10f535ed, /// 0x3c
			 0xa76ff908, /// 0x40
			 0x0d4fa90b, /// 0x44
			 0xb28e1410, /// 0x48
			 0x5bcd5a6e, /// 0x4c
			 0x5bbbe654, /// 0x50
			 0x248dce60, /// 0x54
			 0x9d7339d4, /// 0x58
			 0x0c411e3c, /// 0x5c
			 0x4d2e8cfb, /// 0x60
			 0x325f0305, /// 0x64
			 0xdeb99c79, /// 0x68
			 0x419899e8, /// 0x6c
			 0x87e095bd, /// 0x70
			 0x715a8ef7, /// 0x74
			 0x8f170729, /// 0x78
			 0xbfd70178, /// 0x7c
			 0xa6e9e727, /// 0x80
			 0x29fbde25, /// 0x84
			 0x95480713, /// 0x88
			 0xa80204ab, /// 0x8c
			 0x5c2ff802, /// 0x90
			 0xc7e4d3d9, /// 0x94
			 0xe8c52ae4, /// 0x98
			 0xf5b55e59, /// 0x9c
			 0x6e268953, /// 0xa0
			 0x5432e396, /// 0xa4
			 0xc0bc05c6, /// 0xa8
			 0x608f1a0f, /// 0xac
			 0x707c6049, /// 0xb0
			 0x748cf294, /// 0xb4
			 0x4c78712d, /// 0xb8
			 0xc7fc5d00, /// 0xbc
			 0xccf1706c, /// 0xc0
			 0xde2c96a4, /// 0xc4
			 0x8f3da776, /// 0xc8
			 0x76b11b83, /// 0xcc
			 0xefe3ceec, /// 0xd0
			 0xc2ac3c4a, /// 0xd4
			 0x34f42514, /// 0xd8
			 0x3f283666, /// 0xdc
			 0x7048de1e, /// 0xe0
			 0x4685d2ed, /// 0xe4
			 0x9177f044, /// 0xe8
			 0x6da207c7, /// 0xec
			 0x0af661a2, /// 0xf0
			 0x6f669025, /// 0xf4
			 0x15bb3d90, /// 0xf8
			 0xc1fd6c8d, /// 0xfc
			 0x5b3b2fd6, /// 0x100
			 0xdea290a1, /// 0x104
			 0x84bb495b, /// 0x108
			 0xa5cabb52, /// 0x10c
			 0x7313a9a9, /// 0x110
			 0x1b27e4e3, /// 0x114
			 0x65c0de76, /// 0x118
			 0x9a319382, /// 0x11c
			 0x3c51d8a6, /// 0x120
			 0xcaf09a06, /// 0x124
			 0xd5613677, /// 0x128
			 0xb0cb7774, /// 0x12c
			 0xf3ea1bca, /// 0x130
			 0x0d8c0db7, /// 0x134
			 0x687d2cac, /// 0x138
			 0xcda7618d, /// 0x13c
			 0xb11051df, /// 0x140
			 0x5dbd9244, /// 0x144
			 0xa3594e4b, /// 0x148
			 0x61c19ea7, /// 0x14c
			 0xb385cdeb, /// 0x150
			 0x1a4bda13, /// 0x154
			 0x65e9ac09, /// 0x158
			 0x75a31068, /// 0x15c
			 0x34d1e5dd, /// 0x160
			 0x8236fe55, /// 0x164
			 0x4c8c0c30, /// 0x168
			 0xa72da131, /// 0x16c
			 0x4de2cd3e, /// 0x170
			 0xbe502bd4, /// 0x174
			 0xeb655b5f, /// 0x178
			 0x2b1be8a0, /// 0x17c
			 0x599dbaa4, /// 0x180
			 0x3f91416e, /// 0x184
			 0x80549b96, /// 0x188
			 0xdaf5e614, /// 0x18c
			 0x9a38e86b, /// 0x190
			 0x33479207, /// 0x194
			 0xcb84fe6c, /// 0x198
			 0xeb55287c, /// 0x19c
			 0x887726bd, /// 0x1a0
			 0x0fef48be, /// 0x1a4
			 0xdf9d2a78, /// 0x1a8
			 0xe3abad72, /// 0x1ac
			 0x4b2634aa, /// 0x1b0
			 0xa2dce261, /// 0x1b4
			 0xa4ca1597, /// 0x1b8
			 0xcf561177, /// 0x1bc
			 0x5c18df84, /// 0x1c0
			 0xa2e57ca5, /// 0x1c4
			 0xa121f2ff, /// 0x1c8
			 0x2e73751e, /// 0x1cc
			 0xa4b6b981, /// 0x1d0
			 0x241a8f83, /// 0x1d4
			 0x83cfa234, /// 0x1d8
			 0x824f8ba1, /// 0x1dc
			 0x930e2f0b, /// 0x1e0
			 0xe21f8c7d, /// 0x1e4
			 0x184dd382, /// 0x1e8
			 0xa574c954, /// 0x1ec
			 0x48a3eb56, /// 0x1f0
			 0x8cbdec3c, /// 0x1f4
			 0xb16a6b81, /// 0x1f8
			 0x11c917b9, /// 0x1fc
			 0x6debecfb, /// 0x200
			 0x537849ba, /// 0x204
			 0xef9525c7, /// 0x208
			 0xc8ea145a, /// 0x20c
			 0xfbd33ada, /// 0x210
			 0x39d43875, /// 0x214
			 0x4069442c, /// 0x218
			 0x918ae3da, /// 0x21c
			 0x1e314628, /// 0x220
			 0xf3143565, /// 0x224
			 0xc47c04fa, /// 0x228
			 0x0f36b982, /// 0x22c
			 0x1d147e90, /// 0x230
			 0x12457d35, /// 0x234
			 0xe7ea3bdf, /// 0x238
			 0x8fc666c1, /// 0x23c
			 0xa23f7e62, /// 0x240
			 0x94638a58, /// 0x244
			 0xc6ccf5c0, /// 0x248
			 0xe2dbb014, /// 0x24c
			 0x2c96a867, /// 0x250
			 0x0f101e35, /// 0x254
			 0x1c14f45f, /// 0x258
			 0x26669117, /// 0x25c
			 0xc837196d, /// 0x260
			 0x34fa6d4d, /// 0x264
			 0xc54e6867, /// 0x268
			 0xc297092a, /// 0x26c
			 0xa9fd576c, /// 0x270
			 0xd0ab1bae, /// 0x274
			 0xc590e26f, /// 0x278
			 0x73c5ea47, /// 0x27c
			 0x69d4f3c5, /// 0x280
			 0x6b666c60, /// 0x284
			 0xbbaa5f0c, /// 0x288
			 0x4655acf6, /// 0x28c
			 0x9d09bed7, /// 0x290
			 0x351c5a3f, /// 0x294
			 0x694dbf23, /// 0x298
			 0x34a63f28, /// 0x29c
			 0x3b7c66da, /// 0x2a0
			 0xb16be6f3, /// 0x2a4
			 0xbaef8658, /// 0x2a8
			 0xbe6b54ff, /// 0x2ac
			 0x8bb20bcd, /// 0x2b0
			 0x6ea7ae84, /// 0x2b4
			 0x13829d73, /// 0x2b8
			 0x7e603deb, /// 0x2bc
			 0xc370e0d9, /// 0x2c0
			 0x48d9d15b, /// 0x2c4
			 0xd82564de, /// 0x2c8
			 0xd984f60b, /// 0x2cc
			 0x4e51cbfb, /// 0x2d0
			 0x0a28b029, /// 0x2d4
			 0xc06d20d7, /// 0x2d8
			 0xfd535077, /// 0x2dc
			 0xf228cb75, /// 0x2e0
			 0xf5f7d7c0, /// 0x2e4
			 0x60b13783, /// 0x2e8
			 0x17dfad43, /// 0x2ec
			 0x7826c6b0, /// 0x2f0
			 0x75de7ca1, /// 0x2f4
			 0x6fdcfc7b, /// 0x2f8
			 0x621f3c72, /// 0x2fc
			 0x7b96d858, /// 0x300
			 0x585a4e9e, /// 0x304
			 0xff162cff, /// 0x308
			 0x5fee46a4, /// 0x30c
			 0x81bc4baf, /// 0x310
			 0xbd2361ca, /// 0x314
			 0x845c3059, /// 0x318
			 0x67301825, /// 0x31c
			 0x9cda9bd4, /// 0x320
			 0xb2f90206, /// 0x324
			 0x3bb9f078, /// 0x328
			 0xdccc81d2, /// 0x32c
			 0x4f286717, /// 0x330
			 0x114f6d7b, /// 0x334
			 0x6c28b616, /// 0x338
			 0x7f595d24, /// 0x33c
			 0x36beab2f, /// 0x340
			 0x0d6a7ca7, /// 0x344
			 0xfce0ecf1, /// 0x348
			 0xc4ebd7bd, /// 0x34c
			 0x01c22208, /// 0x350
			 0xc455010d, /// 0x354
			 0x9b44bd18, /// 0x358
			 0x8f62afcd, /// 0x35c
			 0x678a3d61, /// 0x360
			 0xfacf033f, /// 0x364
			 0x463278d9, /// 0x368
			 0x7533ef7c, /// 0x36c
			 0x8653ab03, /// 0x370
			 0x73c78806, /// 0x374
			 0x58d1dfa2, /// 0x378
			 0x94ad512d, /// 0x37c
			 0x063d79d5, /// 0x380
			 0x3d5117c0, /// 0x384
			 0xd7f7a52f, /// 0x388
			 0xbc380f34, /// 0x38c
			 0xcfd1d7a5, /// 0x390
			 0xc4336ca8, /// 0x394
			 0xd04e70fd, /// 0x398
			 0xd155e8af, /// 0x39c
			 0xed5651c2, /// 0x3a0
			 0x61fd8215, /// 0x3a4
			 0x668c532b, /// 0x3a8
			 0xd3c11842, /// 0x3ac
			 0x6c3cdf9a, /// 0x3b0
			 0xa8e763a0, /// 0x3b4
			 0x13924ff0, /// 0x3b8
			 0x3bcc207e, /// 0x3bc
			 0xfa35b9ac, /// 0x3c0
			 0x51837943, /// 0x3c4
			 0xef79f9c5, /// 0x3c8
			 0xcb78488e, /// 0x3cc
			 0xbc83b1dd, /// 0x3d0
			 0x9adec38a, /// 0x3d4
			 0xa5217d35, /// 0x3d8
			 0xfbdc6584, /// 0x3dc
			 0xcddcf43b, /// 0x3e0
			 0xdc77fedc, /// 0x3e4
			 0x9246f626, /// 0x3e8
			 0x2e503de2, /// 0x3ec
			 0xe37575f0, /// 0x3f0
			 0x90e4562c, /// 0x3f4
			 0x21043889, /// 0x3f8
			 0x38545b8b, /// 0x3fc
			 0xfbcfbc39, /// 0x400
			 0x5f17ed17, /// 0x404
			 0xc619da98, /// 0x408
			 0x685f3e31, /// 0x40c
			 0xf319f88b, /// 0x410
			 0x3fcd1932, /// 0x414
			 0x334afe66, /// 0x418
			 0x5cb67d64, /// 0x41c
			 0xd6b70765, /// 0x420
			 0xa79ba10b, /// 0x424
			 0x61cd174b, /// 0x428
			 0x38fa0e0f, /// 0x42c
			 0x993d01ea, /// 0x430
			 0x2496bf66, /// 0x434
			 0xefac288b, /// 0x438
			 0xedb761d0, /// 0x43c
			 0xf4575385, /// 0x440
			 0x48289b1c, /// 0x444
			 0xa1b70f86, /// 0x448
			 0x1729a93c, /// 0x44c
			 0x1c7e4c1e, /// 0x450
			 0x0a6d5852, /// 0x454
			 0x1d0ec6a7, /// 0x458
			 0x175b3aa7, /// 0x45c
			 0x8190e51f, /// 0x460
			 0x8598b51a, /// 0x464
			 0x5aeab958, /// 0x468
			 0x0cd4f832, /// 0x46c
			 0x15e9fdaf, /// 0x470
			 0x591dbb99, /// 0x474
			 0xdc7e6553, /// 0x478
			 0x60bfbdf3, /// 0x47c
			 0xffc1b5ed, /// 0x480
			 0x819aed5e, /// 0x484
			 0xa6b7b4ad, /// 0x488
			 0x3c77e872, /// 0x48c
			 0x7aeedb8d, /// 0x490
			 0x47a9cf74, /// 0x494
			 0xde60383d, /// 0x498
			 0x13e00e92, /// 0x49c
			 0xc9da079c, /// 0x4a0
			 0x57454d90, /// 0x4a4
			 0xca394cb9, /// 0x4a8
			 0x839bfe04, /// 0x4ac
			 0x3d540352, /// 0x4b0
			 0xee2e9c0e, /// 0x4b4
			 0x9e5eeca1, /// 0x4b8
			 0xb3a8483f, /// 0x4bc
			 0x09c64675, /// 0x4c0
			 0x9bfa556e, /// 0x4c4
			 0x3f5b99db, /// 0x4c8
			 0xb4053018, /// 0x4cc
			 0xf8157852, /// 0x4d0
			 0xf9c1215f, /// 0x4d4
			 0x7e0d114a, /// 0x4d8
			 0xab1b4ca3, /// 0x4dc
			 0x1927f759, /// 0x4e0
			 0xa4f931df, /// 0x4e4
			 0xa9c263aa, /// 0x4e8
			 0x25493f37, /// 0x4ec
			 0x2e6117c0, /// 0x4f0
			 0xb48aa7e2, /// 0x4f4
			 0x6fad4a40, /// 0x4f8
			 0x6417c4a3, /// 0x4fc
			 0xc3a028cb, /// 0x500
			 0x70e30739, /// 0x504
			 0xf8b5a62c, /// 0x508
			 0x79c7bf6a, /// 0x50c
			 0x6ee3ec07, /// 0x510
			 0x3ebb334b, /// 0x514
			 0xe878ce78, /// 0x518
			 0xf5b0c263, /// 0x51c
			 0x6ae908af, /// 0x520
			 0x016539a3, /// 0x524
			 0xf564d2fc, /// 0x528
			 0xe09ed932, /// 0x52c
			 0x734309e7, /// 0x530
			 0xb8d715b5, /// 0x534
			 0xabe56fa8, /// 0x538
			 0x154764be, /// 0x53c
			 0x382b8ad1, /// 0x540
			 0xc7e2dc90, /// 0x544
			 0x161e11c4, /// 0x548
			 0xd1b08a1a, /// 0x54c
			 0x4f3b11af, /// 0x550
			 0xedd71fa7, /// 0x554
			 0x77458634, /// 0x558
			 0x57760155, /// 0x55c
			 0x1a447cb8, /// 0x560
			 0x2ed96bb8, /// 0x564
			 0xc7357321, /// 0x568
			 0xf558b98a, /// 0x56c
			 0x22710a4f, /// 0x570
			 0xb0211846, /// 0x574
			 0x9ee8f899, /// 0x578
			 0x373b671b, /// 0x57c
			 0x9f2babca, /// 0x580
			 0xfd1ec328, /// 0x584
			 0x626b0ca4, /// 0x588
			 0x09102d96, /// 0x58c
			 0x46ce67fc, /// 0x590
			 0xcdb63944, /// 0x594
			 0x42256be0, /// 0x598
			 0xefd77567, /// 0x59c
			 0x61cb4a87, /// 0x5a0
			 0xb98801a8, /// 0x5a4
			 0x78e1e92b, /// 0x5a8
			 0x0e74ec91, /// 0x5ac
			 0xb15a3c16, /// 0x5b0
			 0x20171fa7, /// 0x5b4
			 0x0063598c, /// 0x5b8
			 0xc6ec81fe, /// 0x5bc
			 0x226c4a97, /// 0x5c0
			 0xc5e23e98, /// 0x5c4
			 0x7b203f49, /// 0x5c8
			 0xd34b16a8, /// 0x5cc
			 0x1598e530, /// 0x5d0
			 0x198ea14d, /// 0x5d4
			 0xcd595185, /// 0x5d8
			 0x2dfe2089, /// 0x5dc
			 0x55200281, /// 0x5e0
			 0xe380fa9b, /// 0x5e4
			 0xb3cbd89b, /// 0x5e8
			 0xb1e3e304, /// 0x5ec
			 0xa722649e, /// 0x5f0
			 0x369c591f, /// 0x5f4
			 0x1a7c1579, /// 0x5f8
			 0xcbdad88c, /// 0x5fc
			 0x5af84802, /// 0x600
			 0x63115dbe, /// 0x604
			 0xa129db89, /// 0x608
			 0x72d9e81d, /// 0x60c
			 0xd32f062c, /// 0x610
			 0x1c5790b1, /// 0x614
			 0x4d56dac7, /// 0x618
			 0x62610d21, /// 0x61c
			 0x5903d989, /// 0x620
			 0x53eeee19, /// 0x624
			 0x035e38dc, /// 0x628
			 0x9a5f13e4, /// 0x62c
			 0x60fc67eb, /// 0x630
			 0x48b3005c, /// 0x634
			 0x077de1ee, /// 0x638
			 0xcf4b59bc, /// 0x63c
			 0x397f3ef1, /// 0x640
			 0xf27252b4, /// 0x644
			 0x2b068f88, /// 0x648
			 0x1e22b5ad, /// 0x64c
			 0x3a283a9d, /// 0x650
			 0xb8fdf3cd, /// 0x654
			 0x3b08bd5c, /// 0x658
			 0x88869181, /// 0x65c
			 0x7ab9435b, /// 0x660
			 0xb423425c, /// 0x664
			 0x64fe801c, /// 0x668
			 0x2aa4ce12, /// 0x66c
			 0x40a93756, /// 0x670
			 0x33e5424c, /// 0x674
			 0xd36d5f57, /// 0x678
			 0xae5a8ad0, /// 0x67c
			 0x2f698838, /// 0x680
			 0xc9c40b94, /// 0x684
			 0x6613c742, /// 0x688
			 0x53a0620d, /// 0x68c
			 0x361f351c, /// 0x690
			 0x37dd852f, /// 0x694
			 0xcd4f0682, /// 0x698
			 0xec6f6101, /// 0x69c
			 0x2653b778, /// 0x6a0
			 0x65b86d5e, /// 0x6a4
			 0x14c7b589, /// 0x6a8
			 0x0efcc562, /// 0x6ac
			 0xf590692a, /// 0x6b0
			 0xcdda85b1, /// 0x6b4
			 0xb6079ee0, /// 0x6b8
			 0x6c58c07c, /// 0x6bc
			 0xbfd8937a, /// 0x6c0
			 0x19d5bf41, /// 0x6c4
			 0x0a1ab711, /// 0x6c8
			 0xac489674, /// 0x6cc
			 0x14f52c02, /// 0x6d0
			 0xb7cff985, /// 0x6d4
			 0x0901b709, /// 0x6d8
			 0x204a0989, /// 0x6dc
			 0x9e1a4443, /// 0x6e0
			 0x225114cd, /// 0x6e4
			 0x28f53f3c, /// 0x6e8
			 0x756720ef, /// 0x6ec
			 0x555b9ea3, /// 0x6f0
			 0xa24fc5e3, /// 0x6f4
			 0xcfdaeea2, /// 0x6f8
			 0x2a06e317, /// 0x6fc
			 0x6d8c3195, /// 0x700
			 0x81f64a91, /// 0x704
			 0x4277472b, /// 0x708
			 0x290fee51, /// 0x70c
			 0xb8b627e5, /// 0x710
			 0x48c8249f, /// 0x714
			 0xd7998ac9, /// 0x718
			 0x422c83ef, /// 0x71c
			 0xa04d9943, /// 0x720
			 0xc914606e, /// 0x724
			 0xb3601ec5, /// 0x728
			 0x13c363b4, /// 0x72c
			 0x822be0c5, /// 0x730
			 0x9dae7679, /// 0x734
			 0x37bc3fd5, /// 0x738
			 0xd7025a60, /// 0x73c
			 0x529cae0c, /// 0x740
			 0xc651d1f0, /// 0x744
			 0xc1ae7b66, /// 0x748
			 0xe62fe5c2, /// 0x74c
			 0x727c6cea, /// 0x750
			 0x0e0e6fcc, /// 0x754
			 0xaa4b443d, /// 0x758
			 0x790da4dc, /// 0x75c
			 0xcf08e252, /// 0x760
			 0xed278842, /// 0x764
			 0x68be9a15, /// 0x768
			 0x86a120da, /// 0x76c
			 0xd460e2ee, /// 0x770
			 0xfff51d28, /// 0x774
			 0x354ed8bb, /// 0x778
			 0xddd209e1, /// 0x77c
			 0x3b629257, /// 0x780
			 0xd00ea1b2, /// 0x784
			 0x7dc4a327, /// 0x788
			 0x44282066, /// 0x78c
			 0x06a67e03, /// 0x790
			 0x33979eff, /// 0x794
			 0x0c63075b, /// 0x798
			 0xec23e145, /// 0x79c
			 0x671e37c9, /// 0x7a0
			 0x7acbd03d, /// 0x7a4
			 0x96dbc30c, /// 0x7a8
			 0xfedb8b9b, /// 0x7ac
			 0x8749ac23, /// 0x7b0
			 0x2688ebd7, /// 0x7b4
			 0xd96bd0ee, /// 0x7b8
			 0x1cfbb30f, /// 0x7bc
			 0x9e12453c, /// 0x7c0
			 0xcb1859bc, /// 0x7c4
			 0x2a4451e1, /// 0x7c8
			 0x62c563f2, /// 0x7cc
			 0x6ba222b5, /// 0x7d0
			 0x1bcb5e85, /// 0x7d4
			 0xbb88de77, /// 0x7d8
			 0x9271ffc8, /// 0x7dc
			 0xe033fc74, /// 0x7e0
			 0x897d344c, /// 0x7e4
			 0x1adca0f8, /// 0x7e8
			 0x447efaba, /// 0x7ec
			 0xf5deabc6, /// 0x7f0
			 0x2c7b4526, /// 0x7f4
			 0xd89423da, /// 0x7f8
			 0x130910ac, /// 0x7fc
			 0xaa8b975f, /// 0x800
			 0xfcc3feae, /// 0x804
			 0x82d74a1b, /// 0x808
			 0x9062746f, /// 0x80c
			 0x05bcbf8d, /// 0x810
			 0x94d8f340, /// 0x814
			 0xfe170773, /// 0x818
			 0xb56de42e, /// 0x81c
			 0x02fbce29, /// 0x820
			 0xa35e1226, /// 0x824
			 0xab2ff6ee, /// 0x828
			 0x22be11b3, /// 0x82c
			 0x3c843758, /// 0x830
			 0x7c4a6d5a, /// 0x834
			 0x56520f91, /// 0x838
			 0xc4e3d534, /// 0x83c
			 0x7c9bd587, /// 0x840
			 0xc05c6660, /// 0x844
			 0x17b7e385, /// 0x848
			 0xb51f0b13, /// 0x84c
			 0x3795314a, /// 0x850
			 0xb8fa9b3a, /// 0x854
			 0x7940ab83, /// 0x858
			 0xbb4a0ca1, /// 0x85c
			 0xc52e8022, /// 0x860
			 0x46a94958, /// 0x864
			 0xac6ae2db, /// 0x868
			 0x47a2fce4, /// 0x86c
			 0x0cf296aa, /// 0x870
			 0xd7e8e645, /// 0x874
			 0x79f7ccac, /// 0x878
			 0x00ae825b, /// 0x87c
			 0x71560c77, /// 0x880
			 0x866daa61, /// 0x884
			 0x852bf453, /// 0x888
			 0x12204c65, /// 0x88c
			 0xd6310221, /// 0x890
			 0x1ff9c026, /// 0x894
			 0x49d6a68c, /// 0x898
			 0x5ace573c, /// 0x89c
			 0x8d2f0ad3, /// 0x8a0
			 0x27a445b1, /// 0x8a4
			 0xc1be6468, /// 0x8a8
			 0xc82f2335, /// 0x8ac
			 0xfb4e1132, /// 0x8b0
			 0x76c6756e, /// 0x8b4
			 0x48cc7b3e, /// 0x8b8
			 0x8c529e57, /// 0x8bc
			 0x7b68b709, /// 0x8c0
			 0x4f9c38a1, /// 0x8c4
			 0xb5146eee, /// 0x8c8
			 0x95900863, /// 0x8cc
			 0x098e1ba6, /// 0x8d0
			 0x9d0e7e06, /// 0x8d4
			 0x43896e88, /// 0x8d8
			 0x6f22b33f, /// 0x8dc
			 0x44e118f6, /// 0x8e0
			 0xb705d282, /// 0x8e4
			 0x8dc5fc06, /// 0x8e8
			 0xb7932209, /// 0x8ec
			 0x34f6a9ca, /// 0x8f0
			 0xb07dc916, /// 0x8f4
			 0x6d726ccc, /// 0x8f8
			 0x1a6f2b3f, /// 0x8fc
			 0xcc1d627b, /// 0x900
			 0xcef53cc0, /// 0x904
			 0xafe05717, /// 0x908
			 0x3e6c2622, /// 0x90c
			 0x0c8a4b9d, /// 0x910
			 0x757a3e6c, /// 0x914
			 0x45a3bc7a, /// 0x918
			 0xff6c51ea, /// 0x91c
			 0x9aa8401d, /// 0x920
			 0xa00c8497, /// 0x924
			 0xe0aa03ef, /// 0x928
			 0x005bdbfe, /// 0x92c
			 0x51accf30, /// 0x930
			 0x893d3631, /// 0x934
			 0x49b96ee9, /// 0x938
			 0xb7321c15, /// 0x93c
			 0x4802933b, /// 0x940
			 0x47e7f316, /// 0x944
			 0xa3532927, /// 0x948
			 0x7434a453, /// 0x94c
			 0xf8d11c97, /// 0x950
			 0xe4ed5a1a, /// 0x954
			 0x95b33496, /// 0x958
			 0xcc7f45af, /// 0x95c
			 0x8170adad, /// 0x960
			 0x2833a1f3, /// 0x964
			 0x38173d5f, /// 0x968
			 0xd91c3244, /// 0x96c
			 0xccc275ca, /// 0x970
			 0x77510fdb, /// 0x974
			 0xbc1eef6e, /// 0x978
			 0x83bae39b, /// 0x97c
			 0x78beb24a, /// 0x980
			 0x8fdaa4eb, /// 0x984
			 0xcc724dac, /// 0x988
			 0xd8d808b4, /// 0x98c
			 0x0a70c163, /// 0x990
			 0x76c97766, /// 0x994
			 0x7b23efc7, /// 0x998
			 0x2db0ab00, /// 0x99c
			 0xd9a4ab97, /// 0x9a0
			 0x2343cfc0, /// 0x9a4
			 0x5132447c, /// 0x9a8
			 0xe81627da, /// 0x9ac
			 0x353f6eaa, /// 0x9b0
			 0xd112df75, /// 0x9b4
			 0xd10aa764, /// 0x9b8
			 0x0e2c109a, /// 0x9bc
			 0x438787b5, /// 0x9c0
			 0x0983c76c, /// 0x9c4
			 0xc988d711, /// 0x9c8
			 0x53eee09c, /// 0x9cc
			 0xf673990c, /// 0x9d0
			 0xd46b7bcf, /// 0x9d4
			 0x3cd9c686, /// 0x9d8
			 0x1e1bbf36, /// 0x9dc
			 0xca16abd0, /// 0x9e0
			 0x3d58d22c, /// 0x9e4
			 0x39231184, /// 0x9e8
			 0xc0288267, /// 0x9ec
			 0xd57ed90d, /// 0x9f0
			 0xf9ad5c1a, /// 0x9f4
			 0xb4a0fcca, /// 0x9f8
			 0xaf8bbec0, /// 0x9fc
			 0xd761814e, /// 0xa00
			 0xd7731a72, /// 0xa04
			 0x3fdb56b1, /// 0xa08
			 0xf48572f2, /// 0xa0c
			 0xce4935a9, /// 0xa10
			 0x32630601, /// 0xa14
			 0x280122f3, /// 0xa18
			 0xb6c68fc3, /// 0xa1c
			 0xdcdf725b, /// 0xa20
			 0xaab502bd, /// 0xa24
			 0x5931fc31, /// 0xa28
			 0xabbd9b9e, /// 0xa2c
			 0x457e995e, /// 0xa30
			 0x1f0502d5, /// 0xa34
			 0x384f1981, /// 0xa38
			 0x81268e41, /// 0xa3c
			 0x0c097b3c, /// 0xa40
			 0xfefc8a03, /// 0xa44
			 0x8f9d12ae, /// 0xa48
			 0x00326bd7, /// 0xa4c
			 0xfe9d0665, /// 0xa50
			 0x369b3fda, /// 0xa54
			 0xda305f7d, /// 0xa58
			 0x421baa7d, /// 0xa5c
			 0xf99ab397, /// 0xa60
			 0xf61afd2a, /// 0xa64
			 0x3df6b3c6, /// 0xa68
			 0xed106fdc, /// 0xa6c
			 0xe30b79a7, /// 0xa70
			 0x392d9108, /// 0xa74
			 0xa8b596d8, /// 0xa78
			 0xaff58a88, /// 0xa7c
			 0x86f8d259, /// 0xa80
			 0xe9f1bb69, /// 0xa84
			 0xef1f1073, /// 0xa88
			 0xd5230ebe, /// 0xa8c
			 0xe9a3990f, /// 0xa90
			 0xaf0d3361, /// 0xa94
			 0xf94c24c9, /// 0xa98
			 0x1636ad1f, /// 0xa9c
			 0x1a0791ef, /// 0xaa0
			 0x49dd61ef, /// 0xaa4
			 0xf9599fb3, /// 0xaa8
			 0xfadb51ee, /// 0xaac
			 0xff34a0d7, /// 0xab0
			 0x42b78670, /// 0xab4
			 0xd3e9c7a6, /// 0xab8
			 0x2617aade, /// 0xabc
			 0xa8efac94, /// 0xac0
			 0x7e939e2f, /// 0xac4
			 0xd2dc97c7, /// 0xac8
			 0x64294ba6, /// 0xacc
			 0x07254c04, /// 0xad0
			 0xb23f518f, /// 0xad4
			 0x3d499801, /// 0xad8
			 0x22ec48c5, /// 0xadc
			 0x5ac65a65, /// 0xae0
			 0x681dfa6a, /// 0xae4
			 0x4a4fb7b9, /// 0xae8
			 0x6f4076d3, /// 0xaec
			 0x574f4a31, /// 0xaf0
			 0xca26e4b2, /// 0xaf4
			 0x8d548601, /// 0xaf8
			 0xd160433c, /// 0xafc
			 0xaa361f69, /// 0xb00
			 0x6e39d4e7, /// 0xb04
			 0xe813fa77, /// 0xb08
			 0x1104025f, /// 0xb0c
			 0x9062e8a8, /// 0xb10
			 0x2ab9d398, /// 0xb14
			 0xa9698605, /// 0xb18
			 0x741e9051, /// 0xb1c
			 0xb7ae7640, /// 0xb20
			 0x28828b7d, /// 0xb24
			 0x950e5cea, /// 0xb28
			 0xb6008fb0, /// 0xb2c
			 0x499569c3, /// 0xb30
			 0x02166a48, /// 0xb34
			 0xfcae1245, /// 0xb38
			 0x4ef0bba3, /// 0xb3c
			 0xae3e9c57, /// 0xb40
			 0x7f28b296, /// 0xb44
			 0x8622e64f, /// 0xb48
			 0x5955a92b, /// 0xb4c
			 0x20866a51, /// 0xb50
			 0x4546badb, /// 0xb54
			 0xf0dc5367, /// 0xb58
			 0x94f43460, /// 0xb5c
			 0x057acbea, /// 0xb60
			 0x455453a1, /// 0xb64
			 0x0d1eba61, /// 0xb68
			 0x000cdf33, /// 0xb6c
			 0xe736d29d, /// 0xb70
			 0x374dd55d, /// 0xb74
			 0x5f1670ea, /// 0xb78
			 0x711e6614, /// 0xb7c
			 0x04ef174c, /// 0xb80
			 0x2905d881, /// 0xb84
			 0xfeb11e05, /// 0xb88
			 0x3ba446d4, /// 0xb8c
			 0x2c8d465d, /// 0xb90
			 0x52164f80, /// 0xb94
			 0x33983c2c, /// 0xb98
			 0x75766bf5, /// 0xb9c
			 0xb7089c3b, /// 0xba0
			 0x4b17198a, /// 0xba4
			 0xd20437ac, /// 0xba8
			 0x2a99367e, /// 0xbac
			 0xf1240b67, /// 0xbb0
			 0xd37d3446, /// 0xbb4
			 0xc3efc53b, /// 0xbb8
			 0x6ae45ea6, /// 0xbbc
			 0x359a69ef, /// 0xbc0
			 0x8e7c95e4, /// 0xbc4
			 0x2f2186e4, /// 0xbc8
			 0xcbdcb9f7, /// 0xbcc
			 0xc6678568, /// 0xbd0
			 0x6427da36, /// 0xbd4
			 0x1afe7aa3, /// 0xbd8
			 0x507745c2, /// 0xbdc
			 0xdcd89c0c, /// 0xbe0
			 0xf217244c, /// 0xbe4
			 0xf314dc9f, /// 0xbe8
			 0x6bbda052, /// 0xbec
			 0x56a4a341, /// 0xbf0
			 0x305d957e, /// 0xbf4
			 0xa4cd9745, /// 0xbf8
			 0x65d60ea6, /// 0xbfc
			 0x316775ec, /// 0xc00
			 0x4995f859, /// 0xc04
			 0xcdc040ca, /// 0xc08
			 0x9bb4b21f, /// 0xc0c
			 0xa1b6ec3d, /// 0xc10
			 0x76cccd5a, /// 0xc14
			 0x18590a6e, /// 0xc18
			 0x7b2889bf, /// 0xc1c
			 0x8de95161, /// 0xc20
			 0x446a6da0, /// 0xc24
			 0x5dde9ca9, /// 0xc28
			 0x3f09232e, /// 0xc2c
			 0x6992b108, /// 0xc30
			 0xebba6829, /// 0xc34
			 0x13fe0de7, /// 0xc38
			 0x41d7ee66, /// 0xc3c
			 0x68d75822, /// 0xc40
			 0x892da793, /// 0xc44
			 0x3fcbea4d, /// 0xc48
			 0x1d133b8b, /// 0xc4c
			 0x3ca2f270, /// 0xc50
			 0x6b02df36, /// 0xc54
			 0x50885f31, /// 0xc58
			 0x9e4c6054, /// 0xc5c
			 0x64547b1a, /// 0xc60
			 0xd0a15dcc, /// 0xc64
			 0x0640b010, /// 0xc68
			 0x85128a0e, /// 0xc6c
			 0xa7989ac3, /// 0xc70
			 0x55d0e634, /// 0xc74
			 0xec47f22f, /// 0xc78
			 0x6b6fcd3a, /// 0xc7c
			 0x82868277, /// 0xc80
			 0xc7f84f19, /// 0xc84
			 0x4a75d301, /// 0xc88
			 0x84eda7f1, /// 0xc8c
			 0x3306c596, /// 0xc90
			 0x19db974b, /// 0xc94
			 0x1c356f6c, /// 0xc98
			 0xd763a206, /// 0xc9c
			 0x1567058c, /// 0xca0
			 0x9f5fa766, /// 0xca4
			 0xdb02924f, /// 0xca8
			 0xf57653e7, /// 0xcac
			 0x70eaf369, /// 0xcb0
			 0x00da0b41, /// 0xcb4
			 0xe4ffb236, /// 0xcb8
			 0x3da3c980, /// 0xcbc
			 0x1d7d11f3, /// 0xcc0
			 0x1147ebff, /// 0xcc4
			 0x4b00d654, /// 0xcc8
			 0x2dabfdec, /// 0xccc
			 0xba12e8a1, /// 0xcd0
			 0x7ad758d4, /// 0xcd4
			 0x27807a51, /// 0xcd8
			 0x5eaaf48a, /// 0xcdc
			 0xddf585f8, /// 0xce0
			 0x9c9f1598, /// 0xce4
			 0xf70d71d1, /// 0xce8
			 0x67155f7f, /// 0xcec
			 0x2b6e164c, /// 0xcf0
			 0x8a87d83e, /// 0xcf4
			 0x2ad42db9, /// 0xcf8
			 0xaa547d90, /// 0xcfc
			 0x6919fc99, /// 0xd00
			 0x613ad90a, /// 0xd04
			 0xb15d1d53, /// 0xd08
			 0xfb507998, /// 0xd0c
			 0x8bcd1827, /// 0xd10
			 0x02c6bcbc, /// 0xd14
			 0x0b7bb71d, /// 0xd18
			 0xa5b0d9f9, /// 0xd1c
			 0xc5387faa, /// 0xd20
			 0xdc3bcb5a, /// 0xd24
			 0x1c10cb13, /// 0xd28
			 0x75c97c2b, /// 0xd2c
			 0x6cd76924, /// 0xd30
			 0x5d4924a4, /// 0xd34
			 0xbc3e3633, /// 0xd38
			 0xb084d00f, /// 0xd3c
			 0x69d82975, /// 0xd40
			 0x74b07c09, /// 0xd44
			 0x154dbeec, /// 0xd48
			 0xb70f77cf, /// 0xd4c
			 0x593f28dc, /// 0xd50
			 0x1ae518fb, /// 0xd54
			 0x80a0f2b7, /// 0xd58
			 0x255b0f44, /// 0xd5c
			 0x610786f1, /// 0xd60
			 0x15e2b6a1, /// 0xd64
			 0x5c49fd11, /// 0xd68
			 0x9e8846f3, /// 0xd6c
			 0xf9d4a516, /// 0xd70
			 0x320a0345, /// 0xd74
			 0x40670f05, /// 0xd78
			 0x305cf814, /// 0xd7c
			 0xf84f51b9, /// 0xd80
			 0xc5ff78cf, /// 0xd84
			 0x460de864, /// 0xd88
			 0x50d6cb20, /// 0xd8c
			 0xb8a42c83, /// 0xd90
			 0x42ebbeb3, /// 0xd94
			 0x7cf2cba1, /// 0xd98
			 0xde992d09, /// 0xd9c
			 0xcb3ff67f, /// 0xda0
			 0x0aab4091, /// 0xda4
			 0x0a5857c8, /// 0xda8
			 0x57d0c92d, /// 0xdac
			 0xfb8e4242, /// 0xdb0
			 0x7c3c73ea, /// 0xdb4
			 0x62937323, /// 0xdb8
			 0xe76022cb, /// 0xdbc
			 0x8f40f6ea, /// 0xdc0
			 0xb0337cdb, /// 0xdc4
			 0x9fbc62b5, /// 0xdc8
			 0xf1076782, /// 0xdcc
			 0xae6cde11, /// 0xdd0
			 0xaf234077, /// 0xdd4
			 0x429cdfda, /// 0xdd8
			 0x5e56b30c, /// 0xddc
			 0x3a643a92, /// 0xde0
			 0x947c1cdb, /// 0xde4
			 0xdfc9414e, /// 0xde8
			 0x986833de, /// 0xdec
			 0xcca9ce6a, /// 0xdf0
			 0x3fd92c48, /// 0xdf4
			 0xf031bd03, /// 0xdf8
			 0x19cd6f27, /// 0xdfc
			 0x01fedc81, /// 0xe00
			 0xcf3d1e21, /// 0xe04
			 0x0a8f96be, /// 0xe08
			 0x53d21cbc, /// 0xe0c
			 0xaeee04a2, /// 0xe10
			 0xd5b1dc2d, /// 0xe14
			 0xdd8d7cf8, /// 0xe18
			 0x4794f7ba, /// 0xe1c
			 0x9d9509b3, /// 0xe20
			 0xe3d06560, /// 0xe24
			 0xbd264333, /// 0xe28
			 0x1b460676, /// 0xe2c
			 0x6a5679d4, /// 0xe30
			 0xdb910f63, /// 0xe34
			 0xc3b2b9f2, /// 0xe38
			 0x87b90035, /// 0xe3c
			 0x9f34da25, /// 0xe40
			 0xf75f0bf5, /// 0xe44
			 0xe23259ba, /// 0xe48
			 0x9e7b17aa, /// 0xe4c
			 0xe2df4287, /// 0xe50
			 0x77c47fac, /// 0xe54
			 0x7770bd2a, /// 0xe58
			 0x22099c1f, /// 0xe5c
			 0x08ceb6bc, /// 0xe60
			 0xf5cc6ec2, /// 0xe64
			 0x66f06ee0, /// 0xe68
			 0x16f24bdd, /// 0xe6c
			 0x65e05c5d, /// 0xe70
			 0x69efc19f, /// 0xe74
			 0xde4b0a56, /// 0xe78
			 0x84d38631, /// 0xe7c
			 0x0173a57e, /// 0xe80
			 0xf2f97fc6, /// 0xe84
			 0x4b4b15d8, /// 0xe88
			 0xac58dac6, /// 0xe8c
			 0x8bf51565, /// 0xe90
			 0xcdb048d0, /// 0xe94
			 0x52727476, /// 0xe98
			 0xbb239a83, /// 0xe9c
			 0xde5a9292, /// 0xea0
			 0xe09b8b3f, /// 0xea4
			 0x9685be14, /// 0xea8
			 0x85394e6a, /// 0xeac
			 0xfd6ac030, /// 0xeb0
			 0xc1f4c010, /// 0xeb4
			 0x0b917473, /// 0xeb8
			 0xfc22f380, /// 0xebc
			 0xa35fd249, /// 0xec0
			 0x2aeab879, /// 0xec4
			 0x88f85e1c, /// 0xec8
			 0x3268566b, /// 0xecc
			 0xc08238c9, /// 0xed0
			 0x922ffe2a, /// 0xed4
			 0xf24c31f7, /// 0xed8
			 0x1452fa8e, /// 0xedc
			 0xa5fe1e4f, /// 0xee0
			 0xe1c86e8d, /// 0xee4
			 0xea5382ef, /// 0xee8
			 0xcab70f65, /// 0xeec
			 0xb6cd857b, /// 0xef0
			 0xe067a899, /// 0xef4
			 0x55e7b2c3, /// 0xef8
			 0x7225de92, /// 0xefc
			 0xced194d4, /// 0xf00
			 0x7f2884fd, /// 0xf04
			 0xd14f14bc, /// 0xf08
			 0x05a0eba5, /// 0xf0c
			 0x75a9e1d9, /// 0xf10
			 0x7bc7f615, /// 0xf14
			 0xe5d8c78a, /// 0xf18
			 0x93c1dcf1, /// 0xf1c
			 0xc22bb9f6, /// 0xf20
			 0x06c420d7, /// 0xf24
			 0xb82bc6c1, /// 0xf28
			 0x60637ff4, /// 0xf2c
			 0x12f68c29, /// 0xf30
			 0x2ef00246, /// 0xf34
			 0xb85dd075, /// 0xf38
			 0xa44e79a5, /// 0xf3c
			 0x7751ebc0, /// 0xf40
			 0x43d30f33, /// 0xf44
			 0x0f284bc4, /// 0xf48
			 0x5a416d99, /// 0xf4c
			 0x0e2b1aa4, /// 0xf50
			 0x9171e9ac, /// 0xf54
			 0x7557159c, /// 0xf58
			 0x53010d90, /// 0xf5c
			 0xf931731b, /// 0xf60
			 0xf20cbc0c, /// 0xf64
			 0x67972008, /// 0xf68
			 0x9d0fe510, /// 0xf6c
			 0x3c056ca6, /// 0xf70
			 0xe9015fbf, /// 0xf74
			 0x78c6b2a2, /// 0xf78
			 0x6cbbe281, /// 0xf7c
			 0xb889226a, /// 0xf80
			 0xe13716f4, /// 0xf84
			 0x8bf7038c, /// 0xf88
			 0x8f41e300, /// 0xf8c
			 0xa38279b2, /// 0xf90
			 0xf417c20e, /// 0xf94
			 0x074c452d, /// 0xf98
			 0xe4e5f8b1, /// 0xf9c
			 0x3bf04845, /// 0xfa0
			 0x2526eab2, /// 0xfa4
			 0x0de7490f, /// 0xfa8
			 0x48fe660e, /// 0xfac
			 0x4412e448, /// 0xfb0
			 0x2ca740e9, /// 0xfb4
			 0x7a269e3b, /// 0xfb8
			 0xd5e91b86, /// 0xfbc
			 0x18fef6cf, /// 0xfc0
			 0x91f67386, /// 0xfc4
			 0xcdb8b367, /// 0xfc8
			 0x061f273c, /// 0xfcc
			 0xdfdcc0b8, /// 0xfd0
			 0x813c71a7, /// 0xfd4
			 0x725dfd51, /// 0xfd8
			 0xd36d0146, /// 0xfdc
			 0x28d8a0f1, /// 0xfe0
			 0xa7c0dba6, /// 0xfe4
			 0xfde15ed0, /// 0xfe8
			 0x4d576830, /// 0xfec
			 0xb6de0e1d, /// 0xff0
			 0x39fe98a4, /// 0xff4
			 0xf7688d58, /// 0xff8
			 0x374a1433 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x921be2cb, /// 0x0
			 0x28ce892f, /// 0x4
			 0x8e858aa5, /// 0x8
			 0x133f879c, /// 0xc
			 0x666cc68c, /// 0x10
			 0x01d7a355, /// 0x14
			 0xdabd5b8a, /// 0x18
			 0x50773bb6, /// 0x1c
			 0xa685cb71, /// 0x20
			 0x4024c547, /// 0x24
			 0x18ce56eb, /// 0x28
			 0x9403bef0, /// 0x2c
			 0x17f507fc, /// 0x30
			 0x8e856cba, /// 0x34
			 0xa13dbc33, /// 0x38
			 0x3db94c9c, /// 0x3c
			 0xeb12c241, /// 0x40
			 0xac573d54, /// 0x44
			 0x7336d9c4, /// 0x48
			 0xaf596e9d, /// 0x4c
			 0xd6d38e19, /// 0x50
			 0x9eb73f86, /// 0x54
			 0xcb2fdccc, /// 0x58
			 0x539bdb41, /// 0x5c
			 0x1924c49d, /// 0x60
			 0xefd5073e, /// 0x64
			 0x81d5297f, /// 0x68
			 0x9e3345bb, /// 0x6c
			 0xecddb1bb, /// 0x70
			 0x0fa665e8, /// 0x74
			 0x16c26d94, /// 0x78
			 0xf059c3f0, /// 0x7c
			 0x7958bc43, /// 0x80
			 0x22f2e03d, /// 0x84
			 0xd6a993e3, /// 0x88
			 0xc520a772, /// 0x8c
			 0x665f2a63, /// 0x90
			 0xc14dbeb5, /// 0x94
			 0x1806ece6, /// 0x98
			 0x3138120d, /// 0x9c
			 0x1224adf6, /// 0xa0
			 0xb7b3ccc0, /// 0xa4
			 0xdc2caaa7, /// 0xa8
			 0x2faff4fa, /// 0xac
			 0x4820ad10, /// 0xb0
			 0x73ec0f51, /// 0xb4
			 0x219dc670, /// 0xb8
			 0x5184aa2c, /// 0xbc
			 0x439a6005, /// 0xc0
			 0x7f0a3b7a, /// 0xc4
			 0x815de22b, /// 0xc8
			 0x0e94fad8, /// 0xcc
			 0x069e9117, /// 0xd0
			 0x2596ca35, /// 0xd4
			 0xe7d76370, /// 0xd8
			 0xfd784a26, /// 0xdc
			 0x73b8a888, /// 0xe0
			 0x8b4e9198, /// 0xe4
			 0x4fb9a93a, /// 0xe8
			 0x58c28b8e, /// 0xec
			 0x129011fc, /// 0xf0
			 0x22568276, /// 0xf4
			 0x060c080a, /// 0xf8
			 0x2ed17c72, /// 0xfc
			 0x3a1e5eac, /// 0x100
			 0x9e313cf6, /// 0x104
			 0x20478e99, /// 0x108
			 0x91cece27, /// 0x10c
			 0xcf63adb9, /// 0x110
			 0x3c880a3e, /// 0x114
			 0x39c14836, /// 0x118
			 0x17537c39, /// 0x11c
			 0xcdba97ee, /// 0x120
			 0xc393de3b, /// 0x124
			 0xbcdf98c3, /// 0x128
			 0x29a14652, /// 0x12c
			 0xc2e478d2, /// 0x130
			 0x30836df2, /// 0x134
			 0x9961ce0c, /// 0x138
			 0xbcb4f73d, /// 0x13c
			 0xb30a0048, /// 0x140
			 0xddfa8d74, /// 0x144
			 0x5d80a86e, /// 0x148
			 0x54208b1e, /// 0x14c
			 0xb3e17c51, /// 0x150
			 0xf24b67e5, /// 0x154
			 0x985db41f, /// 0x158
			 0xe9681661, /// 0x15c
			 0xe738cc07, /// 0x160
			 0x6debb00d, /// 0x164
			 0x6649e648, /// 0x168
			 0xfcb81626, /// 0x16c
			 0xbbbee81f, /// 0x170
			 0x8cac146f, /// 0x174
			 0xbbc7aa01, /// 0x178
			 0x2270d8cc, /// 0x17c
			 0x253fbbac, /// 0x180
			 0x613db825, /// 0x184
			 0xdf7b6ffb, /// 0x188
			 0x03676031, /// 0x18c
			 0xa9382862, /// 0x190
			 0xf376ca3d, /// 0x194
			 0x55098fb6, /// 0x198
			 0x006d3251, /// 0x19c
			 0xc2706000, /// 0x1a0
			 0x0ecda438, /// 0x1a4
			 0x133e3770, /// 0x1a8
			 0xae403e20, /// 0x1ac
			 0x7e338e67, /// 0x1b0
			 0x447ccc2b, /// 0x1b4
			 0xce2353e0, /// 0x1b8
			 0x1a6c97e5, /// 0x1bc
			 0xa362c386, /// 0x1c0
			 0xcac09554, /// 0x1c4
			 0x94126336, /// 0x1c8
			 0xca2d797a, /// 0x1cc
			 0xa85fa53e, /// 0x1d0
			 0xa2c4c244, /// 0x1d4
			 0x4c8afdb9, /// 0x1d8
			 0x808d4b0a, /// 0x1dc
			 0xa18d9d51, /// 0x1e0
			 0x429306ce, /// 0x1e4
			 0x66a98c1b, /// 0x1e8
			 0x37bc6cbe, /// 0x1ec
			 0xcd7acc2f, /// 0x1f0
			 0x00df5c60, /// 0x1f4
			 0x8d32b40e, /// 0x1f8
			 0x6bfae894, /// 0x1fc
			 0x38774487, /// 0x200
			 0xc46b31d3, /// 0x204
			 0x68d6566d, /// 0x208
			 0x338c837c, /// 0x20c
			 0x255269ec, /// 0x210
			 0xd4d02e64, /// 0x214
			 0x789d5afa, /// 0x218
			 0x06e44427, /// 0x21c
			 0xb59c13e0, /// 0x220
			 0x20ff0fa1, /// 0x224
			 0x1ea7e6b4, /// 0x228
			 0x1217ba20, /// 0x22c
			 0x50ff4b87, /// 0x230
			 0xfed60e6f, /// 0x234
			 0xdb5d8284, /// 0x238
			 0xdea557c6, /// 0x23c
			 0xfd7689c2, /// 0x240
			 0x6c5cdda5, /// 0x244
			 0x448b3754, /// 0x248
			 0x1ae9ea9c, /// 0x24c
			 0x0ecb4176, /// 0x250
			 0x34ea46c3, /// 0x254
			 0xc419cfee, /// 0x258
			 0x38f56dd9, /// 0x25c
			 0xeb89b6f5, /// 0x260
			 0xd92f3193, /// 0x264
			 0x7bdd808f, /// 0x268
			 0xad49ec0f, /// 0x26c
			 0xc6c637aa, /// 0x270
			 0x8a38394b, /// 0x274
			 0x27e053be, /// 0x278
			 0xcc8e1982, /// 0x27c
			 0x4fb8767c, /// 0x280
			 0xb5b12058, /// 0x284
			 0xcc2926ae, /// 0x288
			 0x055cc474, /// 0x28c
			 0x2572e088, /// 0x290
			 0x4e571fb5, /// 0x294
			 0x9c156746, /// 0x298
			 0x41e46e20, /// 0x29c
			 0x49b3329d, /// 0x2a0
			 0x73491f34, /// 0x2a4
			 0x998d160c, /// 0x2a8
			 0x135b9ce2, /// 0x2ac
			 0x899af57b, /// 0x2b0
			 0xb0e2cbda, /// 0x2b4
			 0x7b4fb173, /// 0x2b8
			 0x830935da, /// 0x2bc
			 0xb6e6f5ff, /// 0x2c0
			 0xa5c75d94, /// 0x2c4
			 0xb5be7a5f, /// 0x2c8
			 0xea57c113, /// 0x2cc
			 0x7080c5c4, /// 0x2d0
			 0x05ba9230, /// 0x2d4
			 0x840efe6d, /// 0x2d8
			 0xb5d75b7d, /// 0x2dc
			 0x441a21ce, /// 0x2e0
			 0x18f463d1, /// 0x2e4
			 0x0711ef00, /// 0x2e8
			 0xe5fa0cb8, /// 0x2ec
			 0x678fda23, /// 0x2f0
			 0x9977ddfa, /// 0x2f4
			 0x96f48bb5, /// 0x2f8
			 0x5568bc0f, /// 0x2fc
			 0x4ab5e33b, /// 0x300
			 0x9a1e6daa, /// 0x304
			 0x69b9d736, /// 0x308
			 0x74169a83, /// 0x30c
			 0x04c65489, /// 0x310
			 0xfb444415, /// 0x314
			 0xdbf6bb8f, /// 0x318
			 0xd273faf6, /// 0x31c
			 0xe5b59a9e, /// 0x320
			 0xdee4010f, /// 0x324
			 0x556f868c, /// 0x328
			 0x8e6b0aed, /// 0x32c
			 0x39e61164, /// 0x330
			 0xafea80a1, /// 0x334
			 0xda62d53d, /// 0x338
			 0x5d0aa46b, /// 0x33c
			 0xadf41840, /// 0x340
			 0x53966bd0, /// 0x344
			 0xe0020280, /// 0x348
			 0xbdd09446, /// 0x34c
			 0x73f9d02e, /// 0x350
			 0x51f73c0d, /// 0x354
			 0xc5d97182, /// 0x358
			 0x2687e98c, /// 0x35c
			 0x62b2e207, /// 0x360
			 0xbae2bceb, /// 0x364
			 0x8a3eb965, /// 0x368
			 0xcd780e47, /// 0x36c
			 0x3366d422, /// 0x370
			 0x5f272cd0, /// 0x374
			 0xf135843c, /// 0x378
			 0xd953d587, /// 0x37c
			 0x861c2713, /// 0x380
			 0x1a7e2822, /// 0x384
			 0x939a3d9b, /// 0x388
			 0x32e4942a, /// 0x38c
			 0x47f12cc0, /// 0x390
			 0xb634ff46, /// 0x394
			 0xc3f7ac2b, /// 0x398
			 0x8bebe23d, /// 0x39c
			 0xd710353d, /// 0x3a0
			 0xa784e0f8, /// 0x3a4
			 0x0abbdb6d, /// 0x3a8
			 0x69811b40, /// 0x3ac
			 0xf7923125, /// 0x3b0
			 0x1f891476, /// 0x3b4
			 0x735f40b6, /// 0x3b8
			 0xf4a3a442, /// 0x3bc
			 0x416e0825, /// 0x3c0
			 0xe960394a, /// 0x3c4
			 0x0125be12, /// 0x3c8
			 0xf014cf58, /// 0x3cc
			 0xdc1a0159, /// 0x3d0
			 0x29a748a2, /// 0x3d4
			 0xf25b8b46, /// 0x3d8
			 0xdaa1149b, /// 0x3dc
			 0x58674390, /// 0x3e0
			 0x5317a920, /// 0x3e4
			 0xdfa5b358, /// 0x3e8
			 0xd05de415, /// 0x3ec
			 0x21de9184, /// 0x3f0
			 0x831666f4, /// 0x3f4
			 0xb76fc656, /// 0x3f8
			 0xcabd0fe1, /// 0x3fc
			 0x7216d9c9, /// 0x400
			 0x18f84f4d, /// 0x404
			 0x8cfa8739, /// 0x408
			 0x20a43de3, /// 0x40c
			 0x806830c5, /// 0x410
			 0x584074e0, /// 0x414
			 0x2fd4b6cf, /// 0x418
			 0x959269b5, /// 0x41c
			 0xc1f3df1a, /// 0x420
			 0x1338e662, /// 0x424
			 0xc6350263, /// 0x428
			 0x089fc5d3, /// 0x42c
			 0x87577c06, /// 0x430
			 0x2d14aee6, /// 0x434
			 0xe9f509e8, /// 0x438
			 0x8951ec17, /// 0x43c
			 0xb442c3ea, /// 0x440
			 0x9870c14b, /// 0x444
			 0x24cdbd7b, /// 0x448
			 0x99efc5ec, /// 0x44c
			 0x66e9e567, /// 0x450
			 0x87c1dba5, /// 0x454
			 0x6aae2bfc, /// 0x458
			 0xe008ee1b, /// 0x45c
			 0xef782058, /// 0x460
			 0x4d379d0d, /// 0x464
			 0xce5d827f, /// 0x468
			 0xbb97f7ac, /// 0x46c
			 0x070cc7fe, /// 0x470
			 0xd7eaf588, /// 0x474
			 0x7cfb39b0, /// 0x478
			 0xe1549b1d, /// 0x47c
			 0x540d3dec, /// 0x480
			 0xbac3b5d6, /// 0x484
			 0xe8ce7a88, /// 0x488
			 0xda28c3a1, /// 0x48c
			 0x0ed6a499, /// 0x490
			 0x99dfa012, /// 0x494
			 0x2428e2cc, /// 0x498
			 0x7b627d7e, /// 0x49c
			 0xf84dc723, /// 0x4a0
			 0xa224f2c2, /// 0x4a4
			 0x78a88976, /// 0x4a8
			 0x99c922d7, /// 0x4ac
			 0x50e8ef65, /// 0x4b0
			 0x8d5731b3, /// 0x4b4
			 0x6ff66850, /// 0x4b8
			 0x32b4c699, /// 0x4bc
			 0x634986ab, /// 0x4c0
			 0xa524bffc, /// 0x4c4
			 0x695c05b6, /// 0x4c8
			 0x6a507f6b, /// 0x4cc
			 0xfbadd9cc, /// 0x4d0
			 0x8a98db06, /// 0x4d4
			 0xd2925849, /// 0x4d8
			 0x436cfc08, /// 0x4dc
			 0x80cc647b, /// 0x4e0
			 0xc766ed61, /// 0x4e4
			 0x353e5538, /// 0x4e8
			 0x0a5c7acc, /// 0x4ec
			 0xcd669e25, /// 0x4f0
			 0x25e2e349, /// 0x4f4
			 0xb8f45ac1, /// 0x4f8
			 0xeb6704d3, /// 0x4fc
			 0xe9301213, /// 0x500
			 0x71013d2a, /// 0x504
			 0x5366d4b1, /// 0x508
			 0x506f94ab, /// 0x50c
			 0xc3886244, /// 0x510
			 0xf01bd373, /// 0x514
			 0xd31e9195, /// 0x518
			 0x2ff8a907, /// 0x51c
			 0x0c1d6e7e, /// 0x520
			 0x633b0ac1, /// 0x524
			 0x2e37cb8d, /// 0x528
			 0xe7cbaaca, /// 0x52c
			 0x1da7c0a3, /// 0x530
			 0x4e0ea8d7, /// 0x534
			 0x9d2a42f4, /// 0x538
			 0x11abba1a, /// 0x53c
			 0xafd7ce82, /// 0x540
			 0x24e3d24e, /// 0x544
			 0xcf71c207, /// 0x548
			 0x41a6b2d8, /// 0x54c
			 0xcf8bf17f, /// 0x550
			 0xdf75f22b, /// 0x554
			 0xf924f7b1, /// 0x558
			 0x69939652, /// 0x55c
			 0x48687e2d, /// 0x560
			 0x26eb3f83, /// 0x564
			 0xb4ab3b1c, /// 0x568
			 0x0fcea87c, /// 0x56c
			 0x8cc439f9, /// 0x570
			 0xffb452e1, /// 0x574
			 0xfeda2815, /// 0x578
			 0xa9f9f980, /// 0x57c
			 0x64602185, /// 0x580
			 0xa4dcf9aa, /// 0x584
			 0xd925d438, /// 0x588
			 0x940839d7, /// 0x58c
			 0x1d3064d8, /// 0x590
			 0x3a9e4f1a, /// 0x594
			 0x9354dd48, /// 0x598
			 0x2406ac4c, /// 0x59c
			 0xd7594df7, /// 0x5a0
			 0xa9fd4381, /// 0x5a4
			 0x33a410bb, /// 0x5a8
			 0x0d98f545, /// 0x5ac
			 0x344a3a80, /// 0x5b0
			 0x09cf9dfd, /// 0x5b4
			 0x6d9ec41d, /// 0x5b8
			 0xd5ee4256, /// 0x5bc
			 0x3727b229, /// 0x5c0
			 0x49d3ae75, /// 0x5c4
			 0xe6f61f91, /// 0x5c8
			 0x22599cc7, /// 0x5cc
			 0xff81051a, /// 0x5d0
			 0x5eb3734f, /// 0x5d4
			 0x77b7e8db, /// 0x5d8
			 0xe6198c8d, /// 0x5dc
			 0x8173abaf, /// 0x5e0
			 0x10c97b02, /// 0x5e4
			 0x2aeaad9b, /// 0x5e8
			 0xe176db9c, /// 0x5ec
			 0xf3483184, /// 0x5f0
			 0xe1a2fa01, /// 0x5f4
			 0x87d35ba4, /// 0x5f8
			 0xbd368716, /// 0x5fc
			 0xca255a02, /// 0x600
			 0xb892dfac, /// 0x604
			 0x0ccdda11, /// 0x608
			 0x95e8863a, /// 0x60c
			 0xaaf95753, /// 0x610
			 0xbe1c1b75, /// 0x614
			 0x7ad690ae, /// 0x618
			 0xbcaa5cfc, /// 0x61c
			 0xe65a9ac0, /// 0x620
			 0x27d9c484, /// 0x624
			 0x71ee9ee8, /// 0x628
			 0x5ff4f0a6, /// 0x62c
			 0xb6caee79, /// 0x630
			 0x08223d75, /// 0x634
			 0x070d4ea3, /// 0x638
			 0xa12c4749, /// 0x63c
			 0xa5e98d12, /// 0x640
			 0xfa0898b9, /// 0x644
			 0x47c4c34b, /// 0x648
			 0x302721b5, /// 0x64c
			 0x74914b23, /// 0x650
			 0xb0412db9, /// 0x654
			 0x0ac3b82e, /// 0x658
			 0xc7e46dd8, /// 0x65c
			 0x7c262e0e, /// 0x660
			 0xbf653df5, /// 0x664
			 0x22d22005, /// 0x668
			 0x1a580ff4, /// 0x66c
			 0xcba9e630, /// 0x670
			 0xeccb54e2, /// 0x674
			 0xc977d4ae, /// 0x678
			 0xd2cfd1d1, /// 0x67c
			 0xdd2074b7, /// 0x680
			 0x3814cc17, /// 0x684
			 0x84e3b1c3, /// 0x688
			 0x3699c9d8, /// 0x68c
			 0xd926861b, /// 0x690
			 0x5ef2c0cd, /// 0x694
			 0x461ea981, /// 0x698
			 0xfbeed474, /// 0x69c
			 0x7f2321eb, /// 0x6a0
			 0xfc39920c, /// 0x6a4
			 0xc8dfbf7b, /// 0x6a8
			 0x14ef7b48, /// 0x6ac
			 0x45e09291, /// 0x6b0
			 0xef00adc1, /// 0x6b4
			 0xf8a719ab, /// 0x6b8
			 0x381da02e, /// 0x6bc
			 0x6ebe3598, /// 0x6c0
			 0xd743994c, /// 0x6c4
			 0x2af437d3, /// 0x6c8
			 0x45bc80ef, /// 0x6cc
			 0xa7e5b14c, /// 0x6d0
			 0x41a0664c, /// 0x6d4
			 0xb0d72b3f, /// 0x6d8
			 0x4986d6c3, /// 0x6dc
			 0x70638829, /// 0x6e0
			 0x13c428b5, /// 0x6e4
			 0x7c594acc, /// 0x6e8
			 0x3fe885cd, /// 0x6ec
			 0xdde9aa6b, /// 0x6f0
			 0x338ec593, /// 0x6f4
			 0x28b68682, /// 0x6f8
			 0xefa217a1, /// 0x6fc
			 0xc6f7fafe, /// 0x700
			 0x9082c076, /// 0x704
			 0xdb172ba3, /// 0x708
			 0xc955054d, /// 0x70c
			 0x5b757de2, /// 0x710
			 0x5bbfff73, /// 0x714
			 0xdc814b8d, /// 0x718
			 0xafa5a15c, /// 0x71c
			 0xd58cd82b, /// 0x720
			 0xbbd77bca, /// 0x724
			 0xccf8b76e, /// 0x728
			 0x30d697f3, /// 0x72c
			 0x8bb38e18, /// 0x730
			 0x0d3ed7e0, /// 0x734
			 0x086983e3, /// 0x738
			 0x3a01d3f7, /// 0x73c
			 0xe2448ce6, /// 0x740
			 0x96597b35, /// 0x744
			 0x12b14c7d, /// 0x748
			 0xac7cb079, /// 0x74c
			 0xc1c3db86, /// 0x750
			 0x45ba63c3, /// 0x754
			 0xe50b3b46, /// 0x758
			 0x31723c32, /// 0x75c
			 0x822dd5cd, /// 0x760
			 0x357dfa81, /// 0x764
			 0xad7e7acd, /// 0x768
			 0xb3b1de7a, /// 0x76c
			 0xa009dc7f, /// 0x770
			 0xa3a4b3ff, /// 0x774
			 0xc91044c3, /// 0x778
			 0xad7ce3b0, /// 0x77c
			 0x6b4cb701, /// 0x780
			 0xaffaba6e, /// 0x784
			 0xd00fc2db, /// 0x788
			 0xc6003156, /// 0x78c
			 0x62a81b19, /// 0x790
			 0xd6ee2051, /// 0x794
			 0x1cdc5821, /// 0x798
			 0x5d947e5b, /// 0x79c
			 0x174a0945, /// 0x7a0
			 0xbafc8f96, /// 0x7a4
			 0xdefea8c8, /// 0x7a8
			 0x404b5565, /// 0x7ac
			 0x351f6106, /// 0x7b0
			 0x7f2ce063, /// 0x7b4
			 0x443e7b32, /// 0x7b8
			 0xb4146138, /// 0x7bc
			 0x51a71e08, /// 0x7c0
			 0xc1d384c0, /// 0x7c4
			 0x5220f125, /// 0x7c8
			 0x5f477b5d, /// 0x7cc
			 0xecb494ca, /// 0x7d0
			 0x5c59571f, /// 0x7d4
			 0x99a90fdb, /// 0x7d8
			 0x4fd71580, /// 0x7dc
			 0x0c786cc5, /// 0x7e0
			 0xa9da56ad, /// 0x7e4
			 0x0d1b1895, /// 0x7e8
			 0xfb4a83e5, /// 0x7ec
			 0x7afe95f3, /// 0x7f0
			 0x65029b2d, /// 0x7f4
			 0x121290c1, /// 0x7f8
			 0xbf329340, /// 0x7fc
			 0x1040f238, /// 0x800
			 0x5b258399, /// 0x804
			 0x81e9875d, /// 0x808
			 0x3b3ac3eb, /// 0x80c
			 0x04324fc0, /// 0x810
			 0x1fc30718, /// 0x814
			 0xe45fc142, /// 0x818
			 0x3ec6f9b9, /// 0x81c
			 0x29f91809, /// 0x820
			 0xd192da95, /// 0x824
			 0xd31b635d, /// 0x828
			 0x746b646d, /// 0x82c
			 0xe37b6b14, /// 0x830
			 0xe8c660ae, /// 0x834
			 0xf4a0bf2c, /// 0x838
			 0x6d167934, /// 0x83c
			 0x0b3247ed, /// 0x840
			 0xdd197584, /// 0x844
			 0xb0a27165, /// 0x848
			 0xa6424fc7, /// 0x84c
			 0xe9fdc02a, /// 0x850
			 0x047c4ea6, /// 0x854
			 0x7320f369, /// 0x858
			 0x440eb14d, /// 0x85c
			 0x9618a4ca, /// 0x860
			 0xacc2d4cc, /// 0x864
			 0x8bf06fab, /// 0x868
			 0x27cb57fd, /// 0x86c
			 0xcab998e5, /// 0x870
			 0xd4c16abe, /// 0x874
			 0x7065c9ea, /// 0x878
			 0x062438a9, /// 0x87c
			 0xd65acfac, /// 0x880
			 0x22b74e96, /// 0x884
			 0xb863e472, /// 0x888
			 0xe673a9d3, /// 0x88c
			 0x044449cd, /// 0x890
			 0x92d27203, /// 0x894
			 0x72ed8ed1, /// 0x898
			 0x56236669, /// 0x89c
			 0xf52193b7, /// 0x8a0
			 0xb8c327a2, /// 0x8a4
			 0xa9a55d2f, /// 0x8a8
			 0xe4552e69, /// 0x8ac
			 0x9e9d6e64, /// 0x8b0
			 0xf59b4379, /// 0x8b4
			 0x635632f9, /// 0x8b8
			 0x65bbcc1a, /// 0x8bc
			 0xa477a34e, /// 0x8c0
			 0x7dfbcb36, /// 0x8c4
			 0x7f7a6428, /// 0x8c8
			 0x46376a27, /// 0x8cc
			 0xc013870e, /// 0x8d0
			 0xfe4667e8, /// 0x8d4
			 0x3b35e825, /// 0x8d8
			 0x49276ab7, /// 0x8dc
			 0xd1d87ebb, /// 0x8e0
			 0x7a775c98, /// 0x8e4
			 0xe064970a, /// 0x8e8
			 0xc720f7c3, /// 0x8ec
			 0x830a74cc, /// 0x8f0
			 0xd7a43200, /// 0x8f4
			 0x4742a73d, /// 0x8f8
			 0xf4a47f94, /// 0x8fc
			 0xb3a5c0ad, /// 0x900
			 0xdd078482, /// 0x904
			 0x10281e68, /// 0x908
			 0x443fcc5c, /// 0x90c
			 0xb0ee20d8, /// 0x910
			 0xe0e6338f, /// 0x914
			 0xe3cd9863, /// 0x918
			 0x79f425e7, /// 0x91c
			 0xf12bb7dd, /// 0x920
			 0xfb611607, /// 0x924
			 0x5ea8e564, /// 0x928
			 0x6eabec1c, /// 0x92c
			 0xf6beab5c, /// 0x930
			 0x24c70f2e, /// 0x934
			 0x8a31b9bd, /// 0x938
			 0x107bc4a9, /// 0x93c
			 0x39b47740, /// 0x940
			 0x97f9bff9, /// 0x944
			 0xb0898d2d, /// 0x948
			 0x06d5cd70, /// 0x94c
			 0x47f857de, /// 0x950
			 0xdc0cb9f8, /// 0x954
			 0xddec64f6, /// 0x958
			 0x0483697c, /// 0x95c
			 0xa910611f, /// 0x960
			 0x3cecbdef, /// 0x964
			 0xb47d8962, /// 0x968
			 0xa78a8fa3, /// 0x96c
			 0xeee28f9b, /// 0x970
			 0xc9e7bdff, /// 0x974
			 0xd5beaa85, /// 0x978
			 0x2c6eb5b5, /// 0x97c
			 0x53a24496, /// 0x980
			 0xdefdf91c, /// 0x984
			 0xc2d52f91, /// 0x988
			 0xdebf1cb9, /// 0x98c
			 0xd5aaf986, /// 0x990
			 0x6505846b, /// 0x994
			 0x53afc6f3, /// 0x998
			 0x3baa2c26, /// 0x99c
			 0xc349e9a0, /// 0x9a0
			 0x09216925, /// 0x9a4
			 0xec400ae6, /// 0x9a8
			 0x50e32c3e, /// 0x9ac
			 0x15387f80, /// 0x9b0
			 0xc7385644, /// 0x9b4
			 0x3507088f, /// 0x9b8
			 0x61acd2b3, /// 0x9bc
			 0x0e43f798, /// 0x9c0
			 0xf0d32277, /// 0x9c4
			 0x5bd1b3d4, /// 0x9c8
			 0x1aefd350, /// 0x9cc
			 0xadc3699e, /// 0x9d0
			 0xb5545835, /// 0x9d4
			 0xf4d36f87, /// 0x9d8
			 0x7eea0518, /// 0x9dc
			 0x66b55ad4, /// 0x9e0
			 0x5f292134, /// 0x9e4
			 0x1954a085, /// 0x9e8
			 0x00a8f8cf, /// 0x9ec
			 0x54d8658b, /// 0x9f0
			 0x25206938, /// 0x9f4
			 0x85ba063b, /// 0x9f8
			 0x65353e79, /// 0x9fc
			 0x624bad5d, /// 0xa00
			 0x68bb2157, /// 0xa04
			 0x64c80009, /// 0xa08
			 0xf6d69369, /// 0xa0c
			 0x7fa662fd, /// 0xa10
			 0x4b5082a3, /// 0xa14
			 0xb155932c, /// 0xa18
			 0xb7a18ab7, /// 0xa1c
			 0xcbb95323, /// 0xa20
			 0x0f41267d, /// 0xa24
			 0x75181fd3, /// 0xa28
			 0xbc8846ea, /// 0xa2c
			 0xc60c00c1, /// 0xa30
			 0x0dc56944, /// 0xa34
			 0xb4d072b6, /// 0xa38
			 0xe0ae4050, /// 0xa3c
			 0xef721566, /// 0xa40
			 0x2f28b243, /// 0xa44
			 0x70f0ddd5, /// 0xa48
			 0x54d7624f, /// 0xa4c
			 0xa866966b, /// 0xa50
			 0x1e649ef8, /// 0xa54
			 0x6e0aefc6, /// 0xa58
			 0xfad25d03, /// 0xa5c
			 0xe5654918, /// 0xa60
			 0x4b82d969, /// 0xa64
			 0xcdcc2860, /// 0xa68
			 0x9e081d9c, /// 0xa6c
			 0xab8eb455, /// 0xa70
			 0x179161a6, /// 0xa74
			 0x775b6c6e, /// 0xa78
			 0xc6879f43, /// 0xa7c
			 0x164bd67f, /// 0xa80
			 0x33a8caa3, /// 0xa84
			 0x66a8bca0, /// 0xa88
			 0x69c3ee01, /// 0xa8c
			 0x407215df, /// 0xa90
			 0xfae01b22, /// 0xa94
			 0xce3a8170, /// 0xa98
			 0x1017e709, /// 0xa9c
			 0x167ede5d, /// 0xaa0
			 0x6ac03f95, /// 0xaa4
			 0x43d6f6e8, /// 0xaa8
			 0xbdbf99b5, /// 0xaac
			 0x54f1158d, /// 0xab0
			 0xd247ddde, /// 0xab4
			 0x47b73388, /// 0xab8
			 0xdce16bbb, /// 0xabc
			 0x7447745f, /// 0xac0
			 0x05be507b, /// 0xac4
			 0x218e6d73, /// 0xac8
			 0x88a16ce4, /// 0xacc
			 0x33bdb63a, /// 0xad0
			 0x4afc38a1, /// 0xad4
			 0x5e4f07ef, /// 0xad8
			 0xe547473c, /// 0xadc
			 0x87247989, /// 0xae0
			 0x8c05457c, /// 0xae4
			 0xfc1f8bfe, /// 0xae8
			 0x46e75df2, /// 0xaec
			 0x1ac40e68, /// 0xaf0
			 0x723c265e, /// 0xaf4
			 0xcf7a48d9, /// 0xaf8
			 0xa1aec551, /// 0xafc
			 0xd2bcd9f3, /// 0xb00
			 0x3fa1d9e7, /// 0xb04
			 0x88134885, /// 0xb08
			 0x482fa6c1, /// 0xb0c
			 0x1e9db165, /// 0xb10
			 0x86de0cac, /// 0xb14
			 0xfacfb30d, /// 0xb18
			 0x2d3dc126, /// 0xb1c
			 0xcd9820c4, /// 0xb20
			 0x1962550f, /// 0xb24
			 0x98fa6c33, /// 0xb28
			 0x3539a249, /// 0xb2c
			 0x87186a6f, /// 0xb30
			 0x02cbf972, /// 0xb34
			 0x685ef4ca, /// 0xb38
			 0xcfa0c714, /// 0xb3c
			 0x7332ed10, /// 0xb40
			 0x6be4d80c, /// 0xb44
			 0x1e35fc46, /// 0xb48
			 0x3ae10b6a, /// 0xb4c
			 0x33bd34a7, /// 0xb50
			 0xa218291a, /// 0xb54
			 0xa70128d0, /// 0xb58
			 0x5ec81066, /// 0xb5c
			 0x758bbdb2, /// 0xb60
			 0xb71b527f, /// 0xb64
			 0x1fdfe474, /// 0xb68
			 0x16351475, /// 0xb6c
			 0x12af8bba, /// 0xb70
			 0x871c1f07, /// 0xb74
			 0x6aee2b4c, /// 0xb78
			 0xe05d7e96, /// 0xb7c
			 0x5608a1eb, /// 0xb80
			 0xe1959385, /// 0xb84
			 0x470113fe, /// 0xb88
			 0x8fe41e61, /// 0xb8c
			 0xeddd298c, /// 0xb90
			 0x05dad282, /// 0xb94
			 0x2729019e, /// 0xb98
			 0x79029d62, /// 0xb9c
			 0x0f3cda8c, /// 0xba0
			 0xb739e4dc, /// 0xba4
			 0x803df1d7, /// 0xba8
			 0xbf898271, /// 0xbac
			 0xc69b2597, /// 0xbb0
			 0x0529bc82, /// 0xbb4
			 0xc32a9e66, /// 0xbb8
			 0x1907b7d7, /// 0xbbc
			 0x94c6a9d1, /// 0xbc0
			 0xda15c35c, /// 0xbc4
			 0x74c02d3e, /// 0xbc8
			 0x28f70f4f, /// 0xbcc
			 0x2fd6cdd5, /// 0xbd0
			 0x04b00983, /// 0xbd4
			 0xbf6ae78c, /// 0xbd8
			 0x9ffa36c1, /// 0xbdc
			 0x87e914e5, /// 0xbe0
			 0xe7258b8d, /// 0xbe4
			 0x5a4fe05b, /// 0xbe8
			 0x057acd3d, /// 0xbec
			 0x0386ba02, /// 0xbf0
			 0xa01fa1bd, /// 0xbf4
			 0xf24d9880, /// 0xbf8
			 0x21151c1c, /// 0xbfc
			 0x1419e5be, /// 0xc00
			 0x31ae8748, /// 0xc04
			 0xb8d913eb, /// 0xc08
			 0x69f9d6fb, /// 0xc0c
			 0xa8f27a14, /// 0xc10
			 0x5c2bca0c, /// 0xc14
			 0xafd6d381, /// 0xc18
			 0xf736a5c0, /// 0xc1c
			 0xc09e2bbb, /// 0xc20
			 0xc3e00f57, /// 0xc24
			 0x5f62096e, /// 0xc28
			 0xbfa36580, /// 0xc2c
			 0x5ea35a5f, /// 0xc30
			 0xb2a81606, /// 0xc34
			 0x2fea7caa, /// 0xc38
			 0xafc0cea1, /// 0xc3c
			 0xe491d78f, /// 0xc40
			 0xf72f451f, /// 0xc44
			 0xf4d44094, /// 0xc48
			 0x096f9005, /// 0xc4c
			 0x810dd755, /// 0xc50
			 0x2e211f16, /// 0xc54
			 0x49e1b316, /// 0xc58
			 0xe6f1dcc4, /// 0xc5c
			 0xaf0e114a, /// 0xc60
			 0x85507f48, /// 0xc64
			 0x672a850f, /// 0xc68
			 0x9eaf4984, /// 0xc6c
			 0x2090c5fe, /// 0xc70
			 0x126e9971, /// 0xc74
			 0x70941d96, /// 0xc78
			 0x6f80c0c6, /// 0xc7c
			 0x5b11291a, /// 0xc80
			 0xf83f2c7f, /// 0xc84
			 0x34dd30b8, /// 0xc88
			 0x6fa9b15a, /// 0xc8c
			 0x249cb4ef, /// 0xc90
			 0xcc5dfbf9, /// 0xc94
			 0x6a00c36e, /// 0xc98
			 0xc81183a6, /// 0xc9c
			 0x88ae9bac, /// 0xca0
			 0x70c9c0d6, /// 0xca4
			 0x3d20f5b4, /// 0xca8
			 0x35b0e3a0, /// 0xcac
			 0x0cab5094, /// 0xcb0
			 0x8016b2ed, /// 0xcb4
			 0xb8a58a73, /// 0xcb8
			 0x56822725, /// 0xcbc
			 0x77af7bdc, /// 0xcc0
			 0xc39dfebd, /// 0xcc4
			 0xbf9d87f9, /// 0xcc8
			 0x5d3241be, /// 0xccc
			 0xad2ae813, /// 0xcd0
			 0x056530b6, /// 0xcd4
			 0xf262b9f0, /// 0xcd8
			 0xd30737c9, /// 0xcdc
			 0x77803b73, /// 0xce0
			 0x70cf59ea, /// 0xce4
			 0xbeef2ce6, /// 0xce8
			 0x0580066c, /// 0xcec
			 0xcc0afcdf, /// 0xcf0
			 0x4a9fef91, /// 0xcf4
			 0xf10c91ae, /// 0xcf8
			 0x574115e2, /// 0xcfc
			 0xbc211d08, /// 0xd00
			 0x257e32b7, /// 0xd04
			 0x60b28da9, /// 0xd08
			 0xc5e475e3, /// 0xd0c
			 0xd09ab8c4, /// 0xd10
			 0x053d9d12, /// 0xd14
			 0x842d3daf, /// 0xd18
			 0x7588f922, /// 0xd1c
			 0x3a4a7453, /// 0xd20
			 0xc18eeb27, /// 0xd24
			 0x8160153c, /// 0xd28
			 0x8651c2c6, /// 0xd2c
			 0x3eabe3c2, /// 0xd30
			 0xcc782341, /// 0xd34
			 0xcd57901a, /// 0xd38
			 0xb54dc54e, /// 0xd3c
			 0xee291693, /// 0xd40
			 0x87788e18, /// 0xd44
			 0x35761b18, /// 0xd48
			 0xa1fa3cc8, /// 0xd4c
			 0x2d8aa9c1, /// 0xd50
			 0xc994fc23, /// 0xd54
			 0x66fa6e9a, /// 0xd58
			 0x4847d370, /// 0xd5c
			 0x2d704a13, /// 0xd60
			 0xd1c4f033, /// 0xd64
			 0x22f6700c, /// 0xd68
			 0xfb8f97e1, /// 0xd6c
			 0xc9e736f7, /// 0xd70
			 0xf897603d, /// 0xd74
			 0x8b59f4c2, /// 0xd78
			 0x138b26ff, /// 0xd7c
			 0x279f7e81, /// 0xd80
			 0x66ae8a5d, /// 0xd84
			 0x3d814bba, /// 0xd88
			 0x8f5f7ce9, /// 0xd8c
			 0xedb0c438, /// 0xd90
			 0x079b6750, /// 0xd94
			 0x6e78c398, /// 0xd98
			 0x405497a2, /// 0xd9c
			 0x4d5a0544, /// 0xda0
			 0x985cf9bd, /// 0xda4
			 0x5b4f7f1d, /// 0xda8
			 0x3ccedaae, /// 0xdac
			 0xfa329852, /// 0xdb0
			 0xb24aa797, /// 0xdb4
			 0xeda38b4d, /// 0xdb8
			 0x671b6770, /// 0xdbc
			 0x313a1665, /// 0xdc0
			 0x48bcd719, /// 0xdc4
			 0x58382bc0, /// 0xdc8
			 0xed9dd17b, /// 0xdcc
			 0xab6da70e, /// 0xdd0
			 0x624e18b8, /// 0xdd4
			 0x48c5e226, /// 0xdd8
			 0x2a184569, /// 0xddc
			 0x34d4e499, /// 0xde0
			 0x762f79ab, /// 0xde4
			 0x977bb0f9, /// 0xde8
			 0xb6fb6ad5, /// 0xdec
			 0xb2cf4446, /// 0xdf0
			 0xd2f13f8f, /// 0xdf4
			 0x527e9d6c, /// 0xdf8
			 0x41f65e51, /// 0xdfc
			 0x61e42e04, /// 0xe00
			 0xa3912344, /// 0xe04
			 0x099c7e51, /// 0xe08
			 0xa9e72406, /// 0xe0c
			 0xc985f4c6, /// 0xe10
			 0x98eab34c, /// 0xe14
			 0x0be91a0f, /// 0xe18
			 0x84c6bb24, /// 0xe1c
			 0xe00f6cc3, /// 0xe20
			 0xe6c2b962, /// 0xe24
			 0x69583d1a, /// 0xe28
			 0xee688545, /// 0xe2c
			 0x03332130, /// 0xe30
			 0xaceadcb1, /// 0xe34
			 0x4058d9a7, /// 0xe38
			 0x6eb58fe2, /// 0xe3c
			 0x243f0b0c, /// 0xe40
			 0xf45d82b6, /// 0xe44
			 0x23dc7d9f, /// 0xe48
			 0xc023ddde, /// 0xe4c
			 0x727d15c0, /// 0xe50
			 0xa4b918ed, /// 0xe54
			 0xf24a7355, /// 0xe58
			 0x61c2b7d3, /// 0xe5c
			 0xc5e7e4d4, /// 0xe60
			 0xa1ed8cd9, /// 0xe64
			 0xf3f9351e, /// 0xe68
			 0x893250b2, /// 0xe6c
			 0xd9639f97, /// 0xe70
			 0x4304efcf, /// 0xe74
			 0xca10bc60, /// 0xe78
			 0x494c1857, /// 0xe7c
			 0x549fb638, /// 0xe80
			 0xf0a66dac, /// 0xe84
			 0x3cba6ad6, /// 0xe88
			 0x09fb394e, /// 0xe8c
			 0xe17e85a0, /// 0xe90
			 0xf3f8744f, /// 0xe94
			 0xfca10b07, /// 0xe98
			 0x6d9562a9, /// 0xe9c
			 0x1244e2a2, /// 0xea0
			 0x23710a75, /// 0xea4
			 0x89d05279, /// 0xea8
			 0x8382f867, /// 0xeac
			 0x5519293c, /// 0xeb0
			 0x6019182f, /// 0xeb4
			 0xd071c905, /// 0xeb8
			 0x36239a86, /// 0xebc
			 0x10523cbe, /// 0xec0
			 0x53c07c11, /// 0xec4
			 0x18c3e4ef, /// 0xec8
			 0x98a4a2aa, /// 0xecc
			 0xaf5c9f81, /// 0xed0
			 0xa063f81d, /// 0xed4
			 0x98aea7d3, /// 0xed8
			 0xfa6fcd4f, /// 0xedc
			 0xbf66a759, /// 0xee0
			 0x67f880b8, /// 0xee4
			 0xa992b3bd, /// 0xee8
			 0xb6d751c6, /// 0xeec
			 0x6581c093, /// 0xef0
			 0x56e6d711, /// 0xef4
			 0x20d6bc6f, /// 0xef8
			 0x08e5d870, /// 0xefc
			 0x3fe38bac, /// 0xf00
			 0xbb2a5484, /// 0xf04
			 0xbc0d4b4f, /// 0xf08
			 0x4528a266, /// 0xf0c
			 0x6ec08770, /// 0xf10
			 0x4d2fc509, /// 0xf14
			 0x2fe2d04d, /// 0xf18
			 0x5e58d6ad, /// 0xf1c
			 0x68022c2b, /// 0xf20
			 0xe60e965e, /// 0xf24
			 0x1ffa2932, /// 0xf28
			 0x642adfbd, /// 0xf2c
			 0xf7cebb03, /// 0xf30
			 0xaffeb95f, /// 0xf34
			 0x3a7c19de, /// 0xf38
			 0xb0dcf408, /// 0xf3c
			 0x96a1568d, /// 0xf40
			 0x914ee4de, /// 0xf44
			 0x0a04db7c, /// 0xf48
			 0xd83a09fb, /// 0xf4c
			 0x0c4c0083, /// 0xf50
			 0x7331fce0, /// 0xf54
			 0x95259abc, /// 0xf58
			 0x6302d370, /// 0xf5c
			 0x3fdbe1fa, /// 0xf60
			 0xfcd3beac, /// 0xf64
			 0x90ddc0ae, /// 0xf68
			 0x333d0d5a, /// 0xf6c
			 0x209396fd, /// 0xf70
			 0x533fdff6, /// 0xf74
			 0xf11f0f16, /// 0xf78
			 0x5223a450, /// 0xf7c
			 0x3d26295c, /// 0xf80
			 0x1bd8a35f, /// 0xf84
			 0xcf8fad83, /// 0xf88
			 0x5a7eef86, /// 0xf8c
			 0x183f5c70, /// 0xf90
			 0x9285dfba, /// 0xf94
			 0x7a9785fd, /// 0xf98
			 0x5f9fcc41, /// 0xf9c
			 0xd21f45f8, /// 0xfa0
			 0x7dcac266, /// 0xfa4
			 0xe07dddad, /// 0xfa8
			 0xa79a867c, /// 0xfac
			 0xb1aa45ef, /// 0xfb0
			 0x0fabc4e0, /// 0xfb4
			 0x80108e4c, /// 0xfb8
			 0x902c5a55, /// 0xfbc
			 0xcc4b48d1, /// 0xfc0
			 0x5b2550b3, /// 0xfc4
			 0x49cd1f00, /// 0xfc8
			 0x0cd7b6ba, /// 0xfcc
			 0x34576469, /// 0xfd0
			 0x779a6bd2, /// 0xfd4
			 0x2621409c, /// 0xfd8
			 0x2302f5cf, /// 0xfdc
			 0x324e64a2, /// 0xfe0
			 0x4e75eae3, /// 0xfe4
			 0x5eb6a892, /// 0xfe8
			 0x4dd6df00, /// 0xfec
			 0x248c81ec, /// 0xff0
			 0xe1739c7f, /// 0xff4
			 0xb233c79c, /// 0xff8
			 0xb4987ea0 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00000
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00004
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00010
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0xff800000,                                                  // -inf                                        /// 00018
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0001c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00024
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0002c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00030
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00034
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00038
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00044
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00048
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00050
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00058
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0005c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00060
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00068
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00070
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00074
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00078
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0007c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00080
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00088
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0008c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00090
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00094
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0009c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000dc
			 0xffc00001,                                                  // -signaling NaN                              /// 000e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00100
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00104
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00108
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00114
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0011c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00124
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0012c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00130
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00138
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0013c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00144
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00150
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00158
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00160
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00164
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00168
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0016c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00174
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00178
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0017c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00180
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00198
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00011111,                                                  // 9.7958E-41                                  /// 001a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x7fc00001,                                                  // signaling NaN                               /// 001e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0xbf028f5c,                                                  // -0.51                                       /// 001e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00204
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0020c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00214
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00218
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0021c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00220
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00224
			 0x0c600000,                                                  // Leading 1s:                                 /// 00228
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00230
			 0x80000000,                                                  // -zero                                       /// 00234
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0e000007,                                                  // Trailing 1s:                                /// 0023c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00248
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0024c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00250
			 0xff800000,                                                  // -inf                                        /// 00254
			 0x7fc00001,                                                  // signaling NaN                               /// 00258
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0025c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00260
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00264
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00268
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00270
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00274
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00278
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00284
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0028c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00290
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00294
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00298
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0029c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002c4
			 0xbf028f5c,                                                  // -0.51                                       /// 002c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002f0
			 0x00000000,                                                  // zero                                        /// 002f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00300
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00304
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0030c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00310
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00318
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00328
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0032c
			 0xbf028f5c,                                                  // -0.51                                       /// 00330
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00334
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00338
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00344
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00348
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00354
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00358
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0035c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00360
			 0x00000000,                                                  // zero                                        /// 00364
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0036c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0c700000,                                                  // Leading 1s:                                 /// 00374
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00378
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0037c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00380
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00388
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00390
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00398
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0039c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x7f800000,                                                  // inf                                         /// 003b4
			 0xcb000000,                                                  // -8388608.0                                  /// 003b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003dc
			 0x4b000000,                                                  // 8388608.0                                   /// 003e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00400
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00404
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00408
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0041c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00420
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00428
			 0x0c600000,                                                  // Leading 1s:                                 /// 0042c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00430
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00434
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00440
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00448
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00450
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0045c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00464
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00468
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x55555555,                                                  // 4 random values                             /// 00470
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00478
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0047c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00480
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00484
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0048c
			 0x33333333,                                                  // 4 random values                             /// 00490
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00494
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00498
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0049c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00500
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00504
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0050c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00514
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0051c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00520
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00528
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0052c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00530
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x3f028f5c,                                                  // 0.51                                        /// 00538
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0053c
			 0x33333333,                                                  // 4 random values                             /// 00540
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00548
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0054c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00550
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00554
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00558
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00560
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00564
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x33333333,                                                  // 4 random values                             /// 0056c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00574
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0057c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0058c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0c400000,                                                  // Leading 1s:                                 /// 00594
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00598
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00600
			 0x4b000000,                                                  // 8388608.0                                   /// 00604
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00610
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00614
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00618
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00620
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00624
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0c600000,                                                  // Leading 1s:                                 /// 0062c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00630
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00634
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0xbf028f5c,                                                  // -0.51                                       /// 0063c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00640
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00650
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00658
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0065c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00660
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00664
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00668
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00670
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0067c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00680
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00688
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00690
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00694
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00698
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x33333333,                                                  // 4 random values                             /// 006a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x7f800000,                                                  // inf                                         /// 006b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00700
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00708
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00710
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00714
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00728
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00734
			 0x0c780000,                                                  // Leading 1s:                                 /// 00738
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0073c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00740
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00748
			 0x55555555,                                                  // 4 random values                             /// 0074c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00750
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00754
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00760
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00764
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00768
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0076c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00770
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00774
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00778
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0077c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00780
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00788
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0078c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00790
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00794
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b8
			 0xff800000,                                                  // -inf                                        /// 007bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x4b000000,                                                  // 8388608.0                                   /// 007e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00000000,                                                  // zero                                        /// 00804
			 0x00011111,                                                  // 9.7958E-41                                  /// 00808
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0080c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0081c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00820
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00824
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xff800000,                                                  // -inf                                        /// 00830
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00834
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00840
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00844
			 0xffc00001,                                                  // -signaling NaN                              /// 00848
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0084c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00850
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00860
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00864
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00868
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x00000000,                                                  // zero                                        /// 00874
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00878
			 0x00011111,                                                  // 9.7958E-41                                  /// 0087c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00888
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00890
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00894
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00898
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0089c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008a8
			 0x3f028f5c,                                                  // 0.51                                        /// 008ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x7fc00001,                                                  // signaling NaN                               /// 008dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008f0
			 0x00000000,                                                  // zero                                        /// 008f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00900
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00904
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0090c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00914
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00918
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0091c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00920
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00924
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0092c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00930
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00934
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0093c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00940
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00944
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00948
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0094c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00950
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00954
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00958
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x7f800000,                                                  // inf                                         /// 00960
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00968
			 0x4b000000,                                                  // 8388608.0                                   /// 0096c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00970
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00978
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0097c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00980
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00988
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0098c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00990
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00998
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0099c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x4b000000,                                                  // 8388608.0                                   /// 009c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x3f028f5c,                                                  // 0.51                                        /// 00a44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00aac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ab4
			 0x80000000,                                                  // -zero                                       /// 00ab8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ac4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ac8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ad4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x3f028f5c,                                                  // 0.51                                        /// 00adc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b38
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b44
			 0x7f800000,                                                  // inf                                         /// 00b48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x7f800000,                                                  // inf                                         /// 00b54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b74
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ba8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x55555555,                                                  // 4 random values                             /// 00bc0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd4
			 0xffc00001,                                                  // -signaling NaN                              /// 00bd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bdc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x3f028f5c,                                                  // 0.51                                        /// 00be4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bf4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x33333333,                                                  // 4 random values                             /// 00c00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c10
			 0x80000000,                                                  // -zero                                       /// 00c14
			 0xcb000000,                                                  // -8388608.0                                  /// 00c18
			 0xff800000,                                                  // -inf                                        /// 00c1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xff800000,                                                  // -inf                                        /// 00c6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ca8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb0
			 0x55555555,                                                  // 4 random values                             /// 00cb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ce0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ce8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cfc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00000000,                                                  // zero                                        /// 00d04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d44
			 0xbf028f5c,                                                  // -0.51                                       /// 00d48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xcb000000,                                                  // -8388608.0                                  /// 00d5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00da0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00da8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00db0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00db4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000000,                                                  // zero                                        /// 00dc8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00dd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00de8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00df8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0xbf028f5c,                                                  // -0.51                                       /// 00e0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e60
			 0xff800000,                                                  // -inf                                        /// 00e64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e80
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00eb8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ebc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ec8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ef4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00efc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x7f800000,                                                  // inf                                         /// 00f10
			 0x3f028f5c,                                                  // 0.51                                        /// 00f14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fc0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fdc
			 0xcb000000,                                                  // -8388608.0                                  /// 00fe0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ff0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00000001                                                  // 1.4E-45 (+denorm)                           /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0b7ac527, /// 0x0
			 0xb3fefe24, /// 0x4
			 0x13963f41, /// 0x8
			 0xd356daa9, /// 0xc
			 0xb9d7df6f, /// 0x10
			 0x32bcb5b7, /// 0x14
			 0x5d099b3b, /// 0x18
			 0xfe0a529a, /// 0x1c
			 0xc2443bab, /// 0x20
			 0x8ed76964, /// 0x24
			 0xc928906e, /// 0x28
			 0x157c953c, /// 0x2c
			 0x867681ff, /// 0x30
			 0xae939acc, /// 0x34
			 0xff5f585f, /// 0x38
			 0x3ffff5f7, /// 0x3c
			 0x9ff3aa80, /// 0x40
			 0xaf2a672d, /// 0x44
			 0xb33a47ac, /// 0x48
			 0x3b117a47, /// 0x4c
			 0xad58cdda, /// 0x50
			 0x51ac0a31, /// 0x54
			 0x0680d604, /// 0x58
			 0xf6522fe7, /// 0x5c
			 0x138d3ff7, /// 0x60
			 0x3103b1a2, /// 0x64
			 0xcc305068, /// 0x68
			 0xe7dc523c, /// 0x6c
			 0x8ae3b7c1, /// 0x70
			 0xa0207236, /// 0x74
			 0x4abe0c29, /// 0x78
			 0x83291ddc, /// 0x7c
			 0xd877d42d, /// 0x80
			 0x4a989464, /// 0x84
			 0xb56d4661, /// 0x88
			 0x87f56692, /// 0x8c
			 0xa7fdc8d7, /// 0x90
			 0x5e0e0646, /// 0x94
			 0x5eed12c4, /// 0x98
			 0x816dba3e, /// 0x9c
			 0x0d851f5a, /// 0xa0
			 0xdec05acb, /// 0xa4
			 0x8ffd811b, /// 0xa8
			 0x8abc5235, /// 0xac
			 0x6af69fb7, /// 0xb0
			 0xaf992b79, /// 0xb4
			 0x7882a377, /// 0xb8
			 0x896fd7cb, /// 0xbc
			 0x29c7ca2e, /// 0xc0
			 0xb13178e3, /// 0xc4
			 0x164adb1d, /// 0xc8
			 0xad8b968b, /// 0xcc
			 0xb30d5b74, /// 0xd0
			 0x39f4cace, /// 0xd4
			 0x23f94f4c, /// 0xd8
			 0x99519479, /// 0xdc
			 0xd0b1b16d, /// 0xe0
			 0xd5f4b4db, /// 0xe4
			 0xbda4557d, /// 0xe8
			 0xe6a67480, /// 0xec
			 0xc692ec29, /// 0xf0
			 0xc510f831, /// 0xf4
			 0xe1622616, /// 0xf8
			 0xe0c58d7e, /// 0xfc
			 0x3ecbc603, /// 0x100
			 0x43de7b4f, /// 0x104
			 0xd9d466e6, /// 0x108
			 0xdf56db6a, /// 0x10c
			 0x5bd3c456, /// 0x110
			 0x22412dc2, /// 0x114
			 0x1114ba65, /// 0x118
			 0xd8416158, /// 0x11c
			 0x7ea2456d, /// 0x120
			 0x4e218fda, /// 0x124
			 0xfd1a3530, /// 0x128
			 0xda65b98f, /// 0x12c
			 0x01c92d3e, /// 0x130
			 0xe08188f5, /// 0x134
			 0xf49ceea8, /// 0x138
			 0x1146c53a, /// 0x13c
			 0x8951c0c4, /// 0x140
			 0x39cff046, /// 0x144
			 0x80653fad, /// 0x148
			 0x7281d5b2, /// 0x14c
			 0x4f5c508b, /// 0x150
			 0x1e3915ed, /// 0x154
			 0x8d30becf, /// 0x158
			 0xf338ad4a, /// 0x15c
			 0x527fbc32, /// 0x160
			 0x1d7d2b25, /// 0x164
			 0x692e5760, /// 0x168
			 0x539acd0e, /// 0x16c
			 0x04b6e374, /// 0x170
			 0xd86b3b8c, /// 0x174
			 0x4a5ffc8b, /// 0x178
			 0x9ff2d563, /// 0x17c
			 0xd7f56bf2, /// 0x180
			 0x23c584db, /// 0x184
			 0x11397790, /// 0x188
			 0x2ef4c1c8, /// 0x18c
			 0x5331bf01, /// 0x190
			 0x73e12acf, /// 0x194
			 0x58bf4e9a, /// 0x198
			 0xfb3359b0, /// 0x19c
			 0x1d8de342, /// 0x1a0
			 0xbae20f53, /// 0x1a4
			 0xfbc1e3da, /// 0x1a8
			 0x7fa8d3e0, /// 0x1ac
			 0x6499af49, /// 0x1b0
			 0xa60c4ecf, /// 0x1b4
			 0x97cfb388, /// 0x1b8
			 0x1064090d, /// 0x1bc
			 0xdcfe4e6e, /// 0x1c0
			 0x18a3455f, /// 0x1c4
			 0x9d565e28, /// 0x1c8
			 0xf5138eb9, /// 0x1cc
			 0x61e5966b, /// 0x1d0
			 0x91a4fece, /// 0x1d4
			 0xedb223f5, /// 0x1d8
			 0x21aedc87, /// 0x1dc
			 0x6ca39b0c, /// 0x1e0
			 0x56759d56, /// 0x1e4
			 0x279adc02, /// 0x1e8
			 0xd7d91ff4, /// 0x1ec
			 0x2c44eda5, /// 0x1f0
			 0x2fda8452, /// 0x1f4
			 0xd78ef62b, /// 0x1f8
			 0xc3f0969a, /// 0x1fc
			 0x79e47b8f, /// 0x200
			 0x6c922777, /// 0x204
			 0xbc179161, /// 0x208
			 0xc3957dfb, /// 0x20c
			 0xe9323a29, /// 0x210
			 0x36b55abe, /// 0x214
			 0xc3292fcc, /// 0x218
			 0x0359434e, /// 0x21c
			 0x15c3fe2a, /// 0x220
			 0x9a700766, /// 0x224
			 0xf2c6be42, /// 0x228
			 0xf990deca, /// 0x22c
			 0x2006caa5, /// 0x230
			 0x8b0de929, /// 0x234
			 0xf5e4f2fb, /// 0x238
			 0xaa2ff725, /// 0x23c
			 0x2d8cc97b, /// 0x240
			 0xc6614fdc, /// 0x244
			 0x077728f8, /// 0x248
			 0xfeb43a52, /// 0x24c
			 0xf6570d41, /// 0x250
			 0x7ffbf89a, /// 0x254
			 0x7565fcc6, /// 0x258
			 0x1a809dc4, /// 0x25c
			 0x6c559791, /// 0x260
			 0xb01c7e49, /// 0x264
			 0xd345d020, /// 0x268
			 0xed4b73bb, /// 0x26c
			 0xba9a6812, /// 0x270
			 0xb6738419, /// 0x274
			 0x5ba2d9ad, /// 0x278
			 0x825debf8, /// 0x27c
			 0x4797c057, /// 0x280
			 0x9cd145ba, /// 0x284
			 0xe1192130, /// 0x288
			 0x34bc5688, /// 0x28c
			 0xf0be7833, /// 0x290
			 0x99ac5685, /// 0x294
			 0x3e78d96f, /// 0x298
			 0xc75e8b36, /// 0x29c
			 0x656594a5, /// 0x2a0
			 0x508e9ab8, /// 0x2a4
			 0xd2e85fdb, /// 0x2a8
			 0xca422086, /// 0x2ac
			 0x35365ec9, /// 0x2b0
			 0x8465edd9, /// 0x2b4
			 0x3d03aa98, /// 0x2b8
			 0x803ff6d7, /// 0x2bc
			 0x1a665ea3, /// 0x2c0
			 0xddbbb6c6, /// 0x2c4
			 0x7de1d0c3, /// 0x2c8
			 0xf8ab0104, /// 0x2cc
			 0x42ebfb8f, /// 0x2d0
			 0xdea093ad, /// 0x2d4
			 0xfe5d7dc3, /// 0x2d8
			 0xb25a0984, /// 0x2dc
			 0xa31b4a60, /// 0x2e0
			 0xb6bd61b8, /// 0x2e4
			 0xaf8ae7a2, /// 0x2e8
			 0x61f460fe, /// 0x2ec
			 0x8dc0bdef, /// 0x2f0
			 0xd5123806, /// 0x2f4
			 0xc77a1c9b, /// 0x2f8
			 0xacf89814, /// 0x2fc
			 0x6ee33d65, /// 0x300
			 0x71904e23, /// 0x304
			 0x094578f6, /// 0x308
			 0x3b541542, /// 0x30c
			 0x61ff5578, /// 0x310
			 0x9ee77d8a, /// 0x314
			 0xdc791e61, /// 0x318
			 0x9392c7f5, /// 0x31c
			 0x297f695c, /// 0x320
			 0xca93a2df, /// 0x324
			 0xbdc615e4, /// 0x328
			 0xe7f03992, /// 0x32c
			 0xa28d05e8, /// 0x330
			 0x9ec6e5c8, /// 0x334
			 0x4f16601f, /// 0x338
			 0x08c96cc9, /// 0x33c
			 0x73cb5ae9, /// 0x340
			 0x261ae31b, /// 0x344
			 0xd4e3cd54, /// 0x348
			 0x719b4789, /// 0x34c
			 0xb04451f5, /// 0x350
			 0x2a23e191, /// 0x354
			 0xf54cc804, /// 0x358
			 0x5b31593d, /// 0x35c
			 0x269cbc91, /// 0x360
			 0xf4f62a68, /// 0x364
			 0xcd770b7b, /// 0x368
			 0x6cc0dfff, /// 0x36c
			 0x2eed4f9e, /// 0x370
			 0x992886fc, /// 0x374
			 0x34afc736, /// 0x378
			 0xc405db54, /// 0x37c
			 0xe7acfbcc, /// 0x380
			 0xad6b7bca, /// 0x384
			 0x6f75cef2, /// 0x388
			 0x85ea59aa, /// 0x38c
			 0xc16c9482, /// 0x390
			 0x8149c66b, /// 0x394
			 0x839de0f3, /// 0x398
			 0x98ab22ce, /// 0x39c
			 0x7fece993, /// 0x3a0
			 0x2358ca2b, /// 0x3a4
			 0x9c04a421, /// 0x3a8
			 0x6b48beca, /// 0x3ac
			 0xfa3c69c0, /// 0x3b0
			 0xa6870db0, /// 0x3b4
			 0x15f096f1, /// 0x3b8
			 0xe11f22f4, /// 0x3bc
			 0x288aec9e, /// 0x3c0
			 0x53d5e5f2, /// 0x3c4
			 0x92e1b3b7, /// 0x3c8
			 0x5f353c09, /// 0x3cc
			 0x2c6206d4, /// 0x3d0
			 0xcb6e7835, /// 0x3d4
			 0x0a4a0040, /// 0x3d8
			 0x636d76de, /// 0x3dc
			 0x7d0f070c, /// 0x3e0
			 0x61b7673e, /// 0x3e4
			 0xfbc6c8af, /// 0x3e8
			 0x81a3aba2, /// 0x3ec
			 0xb9d68a9a, /// 0x3f0
			 0x10154801, /// 0x3f4
			 0x268194b7, /// 0x3f8
			 0x7944d25e, /// 0x3fc
			 0x1631f586, /// 0x400
			 0x3f4fd736, /// 0x404
			 0xf2ad6f2b, /// 0x408
			 0xb2b2751b, /// 0x40c
			 0x7c099ad7, /// 0x410
			 0x233cb193, /// 0x414
			 0x493ab5b2, /// 0x418
			 0xf71e75ee, /// 0x41c
			 0x524916a1, /// 0x420
			 0x2e2d7a9a, /// 0x424
			 0xca87b5e2, /// 0x428
			 0x1522e83f, /// 0x42c
			 0x083309e4, /// 0x430
			 0x42ab6a9e, /// 0x434
			 0x2d8ff76b, /// 0x438
			 0x5de88381, /// 0x43c
			 0xb5e65ad5, /// 0x440
			 0x10563e29, /// 0x444
			 0xa1ca9b27, /// 0x448
			 0x601fc13a, /// 0x44c
			 0x509a5397, /// 0x450
			 0xb81bd169, /// 0x454
			 0x9b732cdd, /// 0x458
			 0x83973661, /// 0x45c
			 0xa1aa229e, /// 0x460
			 0x30d957a5, /// 0x464
			 0xb856a4c6, /// 0x468
			 0xed859f05, /// 0x46c
			 0xf686209b, /// 0x470
			 0x139130b3, /// 0x474
			 0xeb9d0d70, /// 0x478
			 0x73eeba53, /// 0x47c
			 0x8eb57775, /// 0x480
			 0x6ac1b8d5, /// 0x484
			 0x91b6b1ee, /// 0x488
			 0x91dae172, /// 0x48c
			 0x392ef15a, /// 0x490
			 0xc20861f7, /// 0x494
			 0x5d33d90a, /// 0x498
			 0x58ac9fd0, /// 0x49c
			 0x7a9b596d, /// 0x4a0
			 0xe2f6f313, /// 0x4a4
			 0x774de269, /// 0x4a8
			 0xa9065721, /// 0x4ac
			 0x8bca5a92, /// 0x4b0
			 0x1be1d7ca, /// 0x4b4
			 0x650f8684, /// 0x4b8
			 0xa0fd28a8, /// 0x4bc
			 0x5733551a, /// 0x4c0
			 0x117a150e, /// 0x4c4
			 0x086ce21e, /// 0x4c8
			 0xcd47339a, /// 0x4cc
			 0xab74ca1c, /// 0x4d0
			 0xa55525c6, /// 0x4d4
			 0x37fb5d2b, /// 0x4d8
			 0xb1ad3039, /// 0x4dc
			 0xce16e461, /// 0x4e0
			 0x89010159, /// 0x4e4
			 0x6e886b9c, /// 0x4e8
			 0x0d0bb4fc, /// 0x4ec
			 0x8e398081, /// 0x4f0
			 0x86d2d672, /// 0x4f4
			 0xfdcdd2e0, /// 0x4f8
			 0x6f0c526f, /// 0x4fc
			 0x9560db60, /// 0x500
			 0xc093f1a7, /// 0x504
			 0x1b20f41f, /// 0x508
			 0x0029534a, /// 0x50c
			 0x5b6bf233, /// 0x510
			 0x6bd53437, /// 0x514
			 0x5fbcd1a9, /// 0x518
			 0xc0aaa565, /// 0x51c
			 0xd3816aeb, /// 0x520
			 0x3404b951, /// 0x524
			 0xc46fe63d, /// 0x528
			 0x6477814b, /// 0x52c
			 0xf841eb77, /// 0x530
			 0xf2b157ab, /// 0x534
			 0xc0587930, /// 0x538
			 0xf1925559, /// 0x53c
			 0x086d4693, /// 0x540
			 0xe75ef86d, /// 0x544
			 0x6c89269b, /// 0x548
			 0xdf64fb88, /// 0x54c
			 0x25e1aaae, /// 0x550
			 0xe6f25d9f, /// 0x554
			 0x1a43bc7c, /// 0x558
			 0x3ca3bd83, /// 0x55c
			 0x9e0d3ab0, /// 0x560
			 0x4c65f017, /// 0x564
			 0xaac78137, /// 0x568
			 0x5bce2c22, /// 0x56c
			 0xa2702d00, /// 0x570
			 0xf9665401, /// 0x574
			 0xe9f9740a, /// 0x578
			 0xdcccfbcd, /// 0x57c
			 0xbbdf7b90, /// 0x580
			 0x9e84cdf5, /// 0x584
			 0xc720627a, /// 0x588
			 0xf9399431, /// 0x58c
			 0x6ae141e0, /// 0x590
			 0x88d99318, /// 0x594
			 0x96f7a496, /// 0x598
			 0x27213722, /// 0x59c
			 0xd2cefc06, /// 0x5a0
			 0x717f2230, /// 0x5a4
			 0x91a4dd10, /// 0x5a8
			 0x902cbfc8, /// 0x5ac
			 0xe6eee423, /// 0x5b0
			 0xc96423e9, /// 0x5b4
			 0x778549ae, /// 0x5b8
			 0x074de0b1, /// 0x5bc
			 0x4d7422aa, /// 0x5c0
			 0x1f39e29a, /// 0x5c4
			 0x35577df7, /// 0x5c8
			 0x8f33d378, /// 0x5cc
			 0xeeadee58, /// 0x5d0
			 0x3613a48e, /// 0x5d4
			 0xc9d4858d, /// 0x5d8
			 0xdc993fe0, /// 0x5dc
			 0xee20133d, /// 0x5e0
			 0xf5c8388a, /// 0x5e4
			 0x95537fa6, /// 0x5e8
			 0xd1657435, /// 0x5ec
			 0x0e828f5c, /// 0x5f0
			 0x0e3e42c7, /// 0x5f4
			 0xa7429357, /// 0x5f8
			 0x71d58579, /// 0x5fc
			 0xd4c9c44f, /// 0x600
			 0xd6f062e9, /// 0x604
			 0xde69b43b, /// 0x608
			 0xff2d6d49, /// 0x60c
			 0x6a28e417, /// 0x610
			 0xc884ab94, /// 0x614
			 0x7e190401, /// 0x618
			 0x0074d0d5, /// 0x61c
			 0xd136b530, /// 0x620
			 0x0fff7a5a, /// 0x624
			 0xe2411c0c, /// 0x628
			 0xa7bd2cea, /// 0x62c
			 0xc0364a57, /// 0x630
			 0xf3c9e179, /// 0x634
			 0xcf9f3cad, /// 0x638
			 0xec1b2971, /// 0x63c
			 0x7fd3cffd, /// 0x640
			 0x63d7ca2c, /// 0x644
			 0x4616baeb, /// 0x648
			 0x45323b33, /// 0x64c
			 0x73097900, /// 0x650
			 0x958ba780, /// 0x654
			 0xe665ed0f, /// 0x658
			 0x82f56939, /// 0x65c
			 0xa87cc54a, /// 0x660
			 0x655847f2, /// 0x664
			 0xab2f9fa6, /// 0x668
			 0xcbdca0cc, /// 0x66c
			 0xe99bf376, /// 0x670
			 0x33a410e4, /// 0x674
			 0x30a35862, /// 0x678
			 0xb9d0b44e, /// 0x67c
			 0x4fa8031d, /// 0x680
			 0xf1debd74, /// 0x684
			 0xd626db49, /// 0x688
			 0xc43ca5c0, /// 0x68c
			 0x4fd1a124, /// 0x690
			 0x39a146bf, /// 0x694
			 0x824c34db, /// 0x698
			 0x87cf3630, /// 0x69c
			 0x51f46555, /// 0x6a0
			 0x8bf91280, /// 0x6a4
			 0x0fcc052f, /// 0x6a8
			 0x7bf79b9c, /// 0x6ac
			 0x481d9edf, /// 0x6b0
			 0xabb9b8d9, /// 0x6b4
			 0xb465c4b4, /// 0x6b8
			 0xd6ad9442, /// 0x6bc
			 0x0c16d334, /// 0x6c0
			 0x78c50af6, /// 0x6c4
			 0xb3d9325e, /// 0x6c8
			 0x802acf18, /// 0x6cc
			 0xf658318a, /// 0x6d0
			 0x3ce8197f, /// 0x6d4
			 0x12b95fd0, /// 0x6d8
			 0xde8cf0ae, /// 0x6dc
			 0x5b8cc9e5, /// 0x6e0
			 0xc6989016, /// 0x6e4
			 0x1848d6ca, /// 0x6e8
			 0x4a82df0b, /// 0x6ec
			 0xe7749ec7, /// 0x6f0
			 0x5b479394, /// 0x6f4
			 0x7483f1e5, /// 0x6f8
			 0x9299d69a, /// 0x6fc
			 0xa455d2d1, /// 0x700
			 0xfdb76d44, /// 0x704
			 0xd90650e6, /// 0x708
			 0x7ad4e51e, /// 0x70c
			 0x2692b7f8, /// 0x710
			 0x7f400f7c, /// 0x714
			 0x8e2a2373, /// 0x718
			 0xd1b3f181, /// 0x71c
			 0x2d9d048c, /// 0x720
			 0x585368fd, /// 0x724
			 0x97e78b69, /// 0x728
			 0x6f0a374a, /// 0x72c
			 0x124c8a79, /// 0x730
			 0x02c4d429, /// 0x734
			 0xf2c5764c, /// 0x738
			 0xd0d8d126, /// 0x73c
			 0x8af5e025, /// 0x740
			 0xd8e863a6, /// 0x744
			 0xf00c7a88, /// 0x748
			 0x4f80161a, /// 0x74c
			 0x24b5a50a, /// 0x750
			 0x287cc493, /// 0x754
			 0x5138eae8, /// 0x758
			 0x78bd53cb, /// 0x75c
			 0x25f341d5, /// 0x760
			 0xd67ec87d, /// 0x764
			 0x131fa1c9, /// 0x768
			 0x7d964a77, /// 0x76c
			 0xc9ac2d8a, /// 0x770
			 0x267ccb19, /// 0x774
			 0xcd28413a, /// 0x778
			 0x3121dbe8, /// 0x77c
			 0x733871fe, /// 0x780
			 0xa7cbedf7, /// 0x784
			 0xfa783025, /// 0x788
			 0x0528d62a, /// 0x78c
			 0xb237d8e8, /// 0x790
			 0x9bf2715c, /// 0x794
			 0x8bdb9bc2, /// 0x798
			 0x52f318ee, /// 0x79c
			 0xb494f3df, /// 0x7a0
			 0x37216a66, /// 0x7a4
			 0x18d17bfe, /// 0x7a8
			 0xe389ee5d, /// 0x7ac
			 0xc1fb188f, /// 0x7b0
			 0xe5f6ba5f, /// 0x7b4
			 0xc30c972d, /// 0x7b8
			 0xd3a8dcc6, /// 0x7bc
			 0x52f83e78, /// 0x7c0
			 0xda895e11, /// 0x7c4
			 0xc97d705b, /// 0x7c8
			 0xf0461bf3, /// 0x7cc
			 0x6031f9a1, /// 0x7d0
			 0xbc48e3ba, /// 0x7d4
			 0xcfe157c2, /// 0x7d8
			 0xc58d20cb, /// 0x7dc
			 0xf8ec35a4, /// 0x7e0
			 0x19921077, /// 0x7e4
			 0x6e61bc4c, /// 0x7e8
			 0x91a58b7c, /// 0x7ec
			 0x8aa80cc8, /// 0x7f0
			 0x7543f421, /// 0x7f4
			 0x59f08ac9, /// 0x7f8
			 0x0880e9c0, /// 0x7fc
			 0x0b648046, /// 0x800
			 0x780e7cbe, /// 0x804
			 0x33472a37, /// 0x808
			 0xd49f3433, /// 0x80c
			 0xe51b800a, /// 0x810
			 0xda293e7f, /// 0x814
			 0xccbc8ac9, /// 0x818
			 0x9182ceb5, /// 0x81c
			 0xd3f3ab0e, /// 0x820
			 0x121283cc, /// 0x824
			 0xd539a2ac, /// 0x828
			 0x94e421d7, /// 0x82c
			 0xe2407988, /// 0x830
			 0xf819ffbc, /// 0x834
			 0xe2f4482c, /// 0x838
			 0xfd9fb0f5, /// 0x83c
			 0xed04700c, /// 0x840
			 0x45c36902, /// 0x844
			 0x936bc184, /// 0x848
			 0x1f62b101, /// 0x84c
			 0x77df557f, /// 0x850
			 0xde6bfef3, /// 0x854
			 0x93f0b48d, /// 0x858
			 0x85490727, /// 0x85c
			 0x67bc27f2, /// 0x860
			 0xf59769dc, /// 0x864
			 0x069e1c8e, /// 0x868
			 0x64afe112, /// 0x86c
			 0x926c36a6, /// 0x870
			 0x8293e8f5, /// 0x874
			 0x56c5b2c3, /// 0x878
			 0xb09812f3, /// 0x87c
			 0x9a5a4250, /// 0x880
			 0xe2be2f87, /// 0x884
			 0x7fd86cfc, /// 0x888
			 0x5f97167b, /// 0x88c
			 0xf8fa8b00, /// 0x890
			 0x3b974104, /// 0x894
			 0xf9805177, /// 0x898
			 0x8b8aee5e, /// 0x89c
			 0xf419a5b3, /// 0x8a0
			 0x92459f86, /// 0x8a4
			 0x544110a9, /// 0x8a8
			 0xf9fd6395, /// 0x8ac
			 0x0212fe52, /// 0x8b0
			 0x9ae2888f, /// 0x8b4
			 0x729ea45f, /// 0x8b8
			 0x1487d3c3, /// 0x8bc
			 0x0177651b, /// 0x8c0
			 0x1292a61e, /// 0x8c4
			 0x5447fd46, /// 0x8c8
			 0xa51d4502, /// 0x8cc
			 0xccb5c488, /// 0x8d0
			 0x5c6e8239, /// 0x8d4
			 0xf4b63af1, /// 0x8d8
			 0x2d31c165, /// 0x8dc
			 0x62b8c737, /// 0x8e0
			 0xf1b7d0a3, /// 0x8e4
			 0xd9f6abbe, /// 0x8e8
			 0xaddd1f6d, /// 0x8ec
			 0xb1ec043e, /// 0x8f0
			 0x18566bad, /// 0x8f4
			 0xbe68ec6e, /// 0x8f8
			 0x8092f35c, /// 0x8fc
			 0xf820d5c9, /// 0x900
			 0xe1d54586, /// 0x904
			 0x92dc0a9b, /// 0x908
			 0x0f3f7976, /// 0x90c
			 0x8b3aa6b6, /// 0x910
			 0x40001e5b, /// 0x914
			 0xe02574df, /// 0x918
			 0xbc4f349d, /// 0x91c
			 0x31178312, /// 0x920
			 0x48c10fc9, /// 0x924
			 0x9612fa7b, /// 0x928
			 0x47450149, /// 0x92c
			 0x01f6408f, /// 0x930
			 0xcf18d157, /// 0x934
			 0x94ffba1e, /// 0x938
			 0x8a049572, /// 0x93c
			 0x380be6e9, /// 0x940
			 0x83709af6, /// 0x944
			 0x598054f2, /// 0x948
			 0x79686904, /// 0x94c
			 0x8e8b5466, /// 0x950
			 0x555c8289, /// 0x954
			 0x870fbdd0, /// 0x958
			 0xd4d4ff68, /// 0x95c
			 0xf55b416a, /// 0x960
			 0x7f954fd9, /// 0x964
			 0xa27562e2, /// 0x968
			 0x228c35a4, /// 0x96c
			 0x65d76912, /// 0x970
			 0x3c8a1f95, /// 0x974
			 0x1b54ad93, /// 0x978
			 0x68e68996, /// 0x97c
			 0x02c050dd, /// 0x980
			 0x6335924b, /// 0x984
			 0x551f7db2, /// 0x988
			 0x75ebcd43, /// 0x98c
			 0x43b8963c, /// 0x990
			 0xeb9457ce, /// 0x994
			 0x2cbb9724, /// 0x998
			 0x0750d55c, /// 0x99c
			 0xc87dfaea, /// 0x9a0
			 0x334a28c8, /// 0x9a4
			 0x8c34467d, /// 0x9a8
			 0x3e72f8ba, /// 0x9ac
			 0x44656b27, /// 0x9b0
			 0xef6b01e7, /// 0x9b4
			 0x98093f96, /// 0x9b8
			 0x30a35a63, /// 0x9bc
			 0xa65eff78, /// 0x9c0
			 0x22712a66, /// 0x9c4
			 0xdb2acc57, /// 0x9c8
			 0x9b23d965, /// 0x9cc
			 0xfbe62fea, /// 0x9d0
			 0x1671efa6, /// 0x9d4
			 0xa809fa31, /// 0x9d8
			 0x2d4fb62e, /// 0x9dc
			 0xd515241c, /// 0x9e0
			 0x537c41fc, /// 0x9e4
			 0xbbd88e20, /// 0x9e8
			 0xde7d3433, /// 0x9ec
			 0x63cd194d, /// 0x9f0
			 0xb314c542, /// 0x9f4
			 0x28ee51e4, /// 0x9f8
			 0xd2b540b8, /// 0x9fc
			 0x9adb0417, /// 0xa00
			 0x338be6e5, /// 0xa04
			 0x8c250259, /// 0xa08
			 0x7a566242, /// 0xa0c
			 0x5c079ca5, /// 0xa10
			 0xb758e586, /// 0xa14
			 0x89fff780, /// 0xa18
			 0x5a4944d5, /// 0xa1c
			 0x5895250a, /// 0xa20
			 0xd1d7a753, /// 0xa24
			 0x7f29cd6c, /// 0xa28
			 0x13a40e26, /// 0xa2c
			 0x8923620c, /// 0xa30
			 0xe9adc6db, /// 0xa34
			 0x72ae571a, /// 0xa38
			 0x3ab7c160, /// 0xa3c
			 0xaaf3c135, /// 0xa40
			 0x40f26ada, /// 0xa44
			 0x25308c18, /// 0xa48
			 0xb940532b, /// 0xa4c
			 0x02942f52, /// 0xa50
			 0x8d36742b, /// 0xa54
			 0xbe238e68, /// 0xa58
			 0x53583e39, /// 0xa5c
			 0x810c038e, /// 0xa60
			 0x837800ce, /// 0xa64
			 0x9eed225a, /// 0xa68
			 0xa62f2105, /// 0xa6c
			 0x35fc78e2, /// 0xa70
			 0x07452e6d, /// 0xa74
			 0xa55ee939, /// 0xa78
			 0x7f7c25a8, /// 0xa7c
			 0x3e8fd19a, /// 0xa80
			 0x162c1196, /// 0xa84
			 0xea1689d2, /// 0xa88
			 0x4616c22c, /// 0xa8c
			 0xc473095a, /// 0xa90
			 0xd816cc52, /// 0xa94
			 0x27a9c887, /// 0xa98
			 0x9ddf0689, /// 0xa9c
			 0x08f7accf, /// 0xaa0
			 0x55544ff1, /// 0xaa4
			 0x1851f7b7, /// 0xaa8
			 0x511a1639, /// 0xaac
			 0xa9b80faa, /// 0xab0
			 0x09270bb0, /// 0xab4
			 0xe412a3ee, /// 0xab8
			 0xcf59e9e4, /// 0xabc
			 0x00a4156a, /// 0xac0
			 0x5979516c, /// 0xac4
			 0xa0be9dd7, /// 0xac8
			 0x82d91cc1, /// 0xacc
			 0x9bd52c48, /// 0xad0
			 0xed412961, /// 0xad4
			 0x0a2d84a4, /// 0xad8
			 0x1e45d448, /// 0xadc
			 0xdf98f90b, /// 0xae0
			 0x9f97547a, /// 0xae4
			 0x71943f5f, /// 0xae8
			 0xb750fadd, /// 0xaec
			 0x501387bb, /// 0xaf0
			 0xc51d05bb, /// 0xaf4
			 0xba3771a2, /// 0xaf8
			 0xb15d58d8, /// 0xafc
			 0x9fc42dfe, /// 0xb00
			 0x14d9c874, /// 0xb04
			 0xb25b35b4, /// 0xb08
			 0x56c33105, /// 0xb0c
			 0x1f13aafd, /// 0xb10
			 0x27575bdd, /// 0xb14
			 0x1f410773, /// 0xb18
			 0xfc648e75, /// 0xb1c
			 0x1fae6adf, /// 0xb20
			 0x2c102028, /// 0xb24
			 0xce56a162, /// 0xb28
			 0xee2d0834, /// 0xb2c
			 0xeb1cc0af, /// 0xb30
			 0xcb8fb790, /// 0xb34
			 0xaa2c2f7b, /// 0xb38
			 0x5a649405, /// 0xb3c
			 0x2cdcc6ed, /// 0xb40
			 0x5abcf2f9, /// 0xb44
			 0x7758ee18, /// 0xb48
			 0x95026ea8, /// 0xb4c
			 0x546300b5, /// 0xb50
			 0x404a4e7b, /// 0xb54
			 0x20c1c05f, /// 0xb58
			 0xc4d82d4e, /// 0xb5c
			 0x2ffd8c93, /// 0xb60
			 0xef16c563, /// 0xb64
			 0xf911fc36, /// 0xb68
			 0x383353e0, /// 0xb6c
			 0x69a45f09, /// 0xb70
			 0xd7391b02, /// 0xb74
			 0xb4be5f93, /// 0xb78
			 0x44354adb, /// 0xb7c
			 0x5f64ec35, /// 0xb80
			 0x20689050, /// 0xb84
			 0xeb87aac0, /// 0xb88
			 0xbc43a457, /// 0xb8c
			 0x24aa9ebb, /// 0xb90
			 0x136240a9, /// 0xb94
			 0xecfc725b, /// 0xb98
			 0x7040da22, /// 0xb9c
			 0xeeb5de65, /// 0xba0
			 0xc440ccd3, /// 0xba4
			 0xa9559016, /// 0xba8
			 0x8d26f147, /// 0xbac
			 0xc8997c8c, /// 0xbb0
			 0xd0745ab0, /// 0xbb4
			 0x9c0dbeb6, /// 0xbb8
			 0x0a973d3e, /// 0xbbc
			 0xd72d9333, /// 0xbc0
			 0x0e87312f, /// 0xbc4
			 0xb3f9b4d6, /// 0xbc8
			 0x92d0433b, /// 0xbcc
			 0x4f4b6770, /// 0xbd0
			 0x72876549, /// 0xbd4
			 0xfbf35e19, /// 0xbd8
			 0xa0f3d0ec, /// 0xbdc
			 0x6b55fca3, /// 0xbe0
			 0x6c010669, /// 0xbe4
			 0xea3b2bd5, /// 0xbe8
			 0xcf7ba4cd, /// 0xbec
			 0x178e8c7f, /// 0xbf0
			 0x525dd114, /// 0xbf4
			 0xe55ee649, /// 0xbf8
			 0xadd2e312, /// 0xbfc
			 0xa4a1b8aa, /// 0xc00
			 0x748bd0a3, /// 0xc04
			 0xb8e25477, /// 0xc08
			 0x03cc6471, /// 0xc0c
			 0xbf629d9d, /// 0xc10
			 0xcd1c4e86, /// 0xc14
			 0x20a635d5, /// 0xc18
			 0xf0506a37, /// 0xc1c
			 0x8896dc0f, /// 0xc20
			 0x2ff513dd, /// 0xc24
			 0x0d7ab1fe, /// 0xc28
			 0xab111cfd, /// 0xc2c
			 0x93455a2e, /// 0xc30
			 0x7c9a83c2, /// 0xc34
			 0xcf975272, /// 0xc38
			 0xcafbcb7c, /// 0xc3c
			 0x65eff59a, /// 0xc40
			 0x21a1a8b1, /// 0xc44
			 0x2cb41c3b, /// 0xc48
			 0x25ec7a13, /// 0xc4c
			 0x3dd43fc4, /// 0xc50
			 0x3bf86f94, /// 0xc54
			 0x3b91a18f, /// 0xc58
			 0x6ec917a5, /// 0xc5c
			 0x97aa4d77, /// 0xc60
			 0xe5ce3467, /// 0xc64
			 0xcddef061, /// 0xc68
			 0xda329f5f, /// 0xc6c
			 0xc7219bef, /// 0xc70
			 0x6eabc917, /// 0xc74
			 0x947d6726, /// 0xc78
			 0x9043d787, /// 0xc7c
			 0x4e09b9de, /// 0xc80
			 0x362ce9f2, /// 0xc84
			 0x43b03ca2, /// 0xc88
			 0x4f960126, /// 0xc8c
			 0xd179ee34, /// 0xc90
			 0xb721c046, /// 0xc94
			 0x11a43855, /// 0xc98
			 0xdb871c29, /// 0xc9c
			 0x468aec43, /// 0xca0
			 0x2819f0c6, /// 0xca4
			 0xf4af519e, /// 0xca8
			 0x613caba5, /// 0xcac
			 0x5aac111a, /// 0xcb0
			 0x8f5b0421, /// 0xcb4
			 0x28ac3608, /// 0xcb8
			 0x79f27bf5, /// 0xcbc
			 0xb5f600a3, /// 0xcc0
			 0xd87c3dff, /// 0xcc4
			 0x66eac228, /// 0xcc8
			 0x2c6d433c, /// 0xccc
			 0x685cba7f, /// 0xcd0
			 0xdd816064, /// 0xcd4
			 0x379a408f, /// 0xcd8
			 0x90aa7271, /// 0xcdc
			 0xfe6e1907, /// 0xce0
			 0x4c6f069e, /// 0xce4
			 0x5d34d50f, /// 0xce8
			 0xdaaa2c95, /// 0xcec
			 0xbd65210d, /// 0xcf0
			 0x1459e746, /// 0xcf4
			 0x8589897b, /// 0xcf8
			 0xf300478e, /// 0xcfc
			 0xdf38d1b1, /// 0xd00
			 0x49dc46c2, /// 0xd04
			 0x0062b4c8, /// 0xd08
			 0xee9bb64b, /// 0xd0c
			 0x708a5af9, /// 0xd10
			 0x325f0976, /// 0xd14
			 0xb403bfb0, /// 0xd18
			 0xb8b2e00e, /// 0xd1c
			 0x3ac8fda9, /// 0xd20
			 0x7eb009de, /// 0xd24
			 0x403076c5, /// 0xd28
			 0x23c11b74, /// 0xd2c
			 0xa3526f53, /// 0xd30
			 0x809cafea, /// 0xd34
			 0x0d3964c8, /// 0xd38
			 0xbaf54804, /// 0xd3c
			 0x31b378a6, /// 0xd40
			 0xb6272f56, /// 0xd44
			 0x35dccff0, /// 0xd48
			 0x38caaabb, /// 0xd4c
			 0x0a807389, /// 0xd50
			 0xf0f4976c, /// 0xd54
			 0x6f8f7599, /// 0xd58
			 0x87520387, /// 0xd5c
			 0xea70456f, /// 0xd60
			 0xda445af1, /// 0xd64
			 0xa142e0b7, /// 0xd68
			 0x67e5fef5, /// 0xd6c
			 0xa1e5c454, /// 0xd70
			 0xc34e0fe8, /// 0xd74
			 0xe5896f6c, /// 0xd78
			 0x4a8510e3, /// 0xd7c
			 0xbf6c1408, /// 0xd80
			 0x7ab15143, /// 0xd84
			 0xdfdad753, /// 0xd88
			 0x1d224e75, /// 0xd8c
			 0xcac3bad5, /// 0xd90
			 0x3f7f6b95, /// 0xd94
			 0xd54849b7, /// 0xd98
			 0x010bc2d8, /// 0xd9c
			 0xb39e624f, /// 0xda0
			 0x1b26f277, /// 0xda4
			 0x35ee6bc4, /// 0xda8
			 0x2438ca21, /// 0xdac
			 0x41dbfc83, /// 0xdb0
			 0x68caca82, /// 0xdb4
			 0xc9ed6f14, /// 0xdb8
			 0x5790dbfe, /// 0xdbc
			 0xc073547c, /// 0xdc0
			 0xab40caf0, /// 0xdc4
			 0xcd84c8b3, /// 0xdc8
			 0xd4a30904, /// 0xdcc
			 0x401e6159, /// 0xdd0
			 0x6220267a, /// 0xdd4
			 0x7d51798b, /// 0xdd8
			 0xd0f273e3, /// 0xddc
			 0xad388b42, /// 0xde0
			 0xeb66dbef, /// 0xde4
			 0xd31e349c, /// 0xde8
			 0xa13db6f4, /// 0xdec
			 0x3fa2b4f3, /// 0xdf0
			 0xb096776f, /// 0xdf4
			 0x772b81d0, /// 0xdf8
			 0xa88fbfdb, /// 0xdfc
			 0xf50ca935, /// 0xe00
			 0x1cb15b8e, /// 0xe04
			 0x74560124, /// 0xe08
			 0xb7ff2239, /// 0xe0c
			 0x2ffadfbe, /// 0xe10
			 0x75281341, /// 0xe14
			 0x391998fb, /// 0xe18
			 0xbf76c25d, /// 0xe1c
			 0x45ab562b, /// 0xe20
			 0xeaa84b2c, /// 0xe24
			 0xec3c1c55, /// 0xe28
			 0x734f2fa1, /// 0xe2c
			 0x58baed47, /// 0xe30
			 0x1d984fa3, /// 0xe34
			 0x3e61ecce, /// 0xe38
			 0xafadc2b6, /// 0xe3c
			 0xbd113210, /// 0xe40
			 0x2c39f23a, /// 0xe44
			 0x4472de9e, /// 0xe48
			 0xdbb0877c, /// 0xe4c
			 0x13620aed, /// 0xe50
			 0x101d0c2c, /// 0xe54
			 0x90a29d7d, /// 0xe58
			 0x74f81afd, /// 0xe5c
			 0xa9c27a68, /// 0xe60
			 0x4fb7c020, /// 0xe64
			 0x44e7a4e0, /// 0xe68
			 0x1953bdaa, /// 0xe6c
			 0x31047bda, /// 0xe70
			 0x448e4e0f, /// 0xe74
			 0x389abe30, /// 0xe78
			 0xa944f295, /// 0xe7c
			 0xe8e1538a, /// 0xe80
			 0x6e67f11a, /// 0xe84
			 0x9f9547fa, /// 0xe88
			 0xf403d171, /// 0xe8c
			 0xbf9357b4, /// 0xe90
			 0x0db813fc, /// 0xe94
			 0xe3a0b55c, /// 0xe98
			 0xf2994a2e, /// 0xe9c
			 0x44751a08, /// 0xea0
			 0x9b40883f, /// 0xea4
			 0x7abe05ce, /// 0xea8
			 0x64fc86c3, /// 0xeac
			 0x52a50619, /// 0xeb0
			 0xe7ee08a3, /// 0xeb4
			 0xfc6a6ff9, /// 0xeb8
			 0x862d4e82, /// 0xebc
			 0x7537df56, /// 0xec0
			 0xf875962f, /// 0xec4
			 0xc5ea8ae5, /// 0xec8
			 0x9cedc72f, /// 0xecc
			 0x5198246c, /// 0xed0
			 0x39543bb5, /// 0xed4
			 0x20555a24, /// 0xed8
			 0x0fc2e4d1, /// 0xedc
			 0x04868d7a, /// 0xee0
			 0xf1733ebd, /// 0xee4
			 0x837ab02b, /// 0xee8
			 0xbd08c265, /// 0xeec
			 0x9a4943b6, /// 0xef0
			 0xbbdf65b3, /// 0xef4
			 0x9921570e, /// 0xef8
			 0x118a823a, /// 0xefc
			 0x9bf17f4f, /// 0xf00
			 0x101142ca, /// 0xf04
			 0x8b0dcc4e, /// 0xf08
			 0xd511c35e, /// 0xf0c
			 0x7edd4bf1, /// 0xf10
			 0x1883891a, /// 0xf14
			 0xe6e970b0, /// 0xf18
			 0x2164c150, /// 0xf1c
			 0x123296d5, /// 0xf20
			 0x9a8e1d73, /// 0xf24
			 0xd174bf8a, /// 0xf28
			 0x13c7446f, /// 0xf2c
			 0x4cd86550, /// 0xf30
			 0xf7d9e4f8, /// 0xf34
			 0xcd6b5356, /// 0xf38
			 0x7d8ce720, /// 0xf3c
			 0x8f10996e, /// 0xf40
			 0xe3274d60, /// 0xf44
			 0xb1c777fe, /// 0xf48
			 0x4e0357bf, /// 0xf4c
			 0x5cf84bc2, /// 0xf50
			 0x260d4588, /// 0xf54
			 0xed1dd3d5, /// 0xf58
			 0xadc97f8d, /// 0xf5c
			 0xdda8cfc0, /// 0xf60
			 0xd281acdd, /// 0xf64
			 0xf164e761, /// 0xf68
			 0xdeb4c8f2, /// 0xf6c
			 0x0be87392, /// 0xf70
			 0x1d4d209e, /// 0xf74
			 0x7bc46758, /// 0xf78
			 0x03651fbe, /// 0xf7c
			 0x4611b3e3, /// 0xf80
			 0x7bdbad01, /// 0xf84
			 0xf2536c80, /// 0xf88
			 0x421f940e, /// 0xf8c
			 0x5e8b2d6f, /// 0xf90
			 0xe00de82d, /// 0xf94
			 0x327aad2b, /// 0xf98
			 0xc8839fca, /// 0xf9c
			 0x1ecae1b7, /// 0xfa0
			 0x47da81e1, /// 0xfa4
			 0x53c97b17, /// 0xfa8
			 0x39867c2d, /// 0xfac
			 0x3cb8e147, /// 0xfb0
			 0x864f50bc, /// 0xfb4
			 0x806a9701, /// 0xfb8
			 0xf58de85c, /// 0xfbc
			 0x72172a58, /// 0xfc0
			 0x0b2f225c, /// 0xfc4
			 0xd7fbb2aa, /// 0xfc8
			 0x5c09b171, /// 0xfcc
			 0x9f7b7c10, /// 0xfd0
			 0x9e268a7c, /// 0xfd4
			 0xc50c4dae, /// 0xfd8
			 0x4008a066, /// 0xfdc
			 0x5065605b, /// 0xfe0
			 0x0b41f8b4, /// 0xfe4
			 0x56ef3e77, /// 0xfe8
			 0x016a30d3, /// 0xfec
			 0x90e14fe7, /// 0xff0
			 0x1af531c8, /// 0xff4
			 0xe10e2086, /// 0xff8
			 0xab1a3c46 /// last
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
			 0x000005d0,
			 0x000005c8,
			 0x00000144,
			 0x000005b0,
			 0x00000460,
			 0x000006f8,
			 0x000002f8,
			 0x00000358,

			 /// vpu register f2
			 0x41000000,
			 0x41f80000,
			 0x41500000,
			 0x41900000,
			 0x41000000,
			 0x41880000,
			 0x41d00000,
			 0x41b00000,

			 /// vpu register f3
			 0x40c00000,
			 0x41880000,
			 0x41e80000,
			 0x41e00000,
			 0x41d00000,
			 0x41a00000,
			 0x41600000,
			 0x41400000,

			 /// vpu register f4
			 0x41880000,
			 0x40a00000,
			 0x41300000,
			 0x41b80000,
			 0x41600000,
			 0x41b00000,
			 0x41200000,
			 0x41a00000,

			 /// vpu register f5
			 0x40400000,
			 0x41880000,
			 0x41b80000,
			 0x3f800000,
			 0x41e00000,
			 0x40000000,
			 0x41c00000,
			 0x41f80000,

			 /// vpu register f6
			 0x42000000,
			 0x40e00000,
			 0x41980000,
			 0x41a00000,
			 0x41900000,
			 0x41f80000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f7
			 0x40800000,
			 0x41b00000,
			 0x41a80000,
			 0x42000000,
			 0x41b80000,
			 0x41400000,
			 0x41300000,
			 0x41c00000,

			 /// vpu register f8
			 0x42000000,
			 0x41a80000,
			 0x41c80000,
			 0x41f00000,
			 0x41100000,
			 0x42000000,
			 0x40a00000,
			 0x42000000,

			 /// vpu register f9
			 0x41d00000,
			 0x3f800000,
			 0x40e00000,
			 0x41300000,
			 0x41b80000,
			 0x41f80000,
			 0x41a80000,
			 0x41a00000,

			 /// vpu register f10
			 0x41a80000,
			 0x41300000,
			 0x41880000,
			 0x41700000,
			 0x41c00000,
			 0x40400000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f11
			 0x41e80000,
			 0x41a00000,
			 0x41d80000,
			 0x41e00000,
			 0x41c00000,
			 0x41100000,
			 0x41c80000,
			 0x41800000,

			 /// vpu register f12
			 0x41b00000,
			 0x41900000,
			 0x41800000,
			 0x3f800000,
			 0x41900000,
			 0x41d00000,
			 0x41000000,
			 0x3f800000,

			 /// vpu register f13
			 0x40800000,
			 0x41e00000,
			 0x41d00000,
			 0x42000000,
			 0x40000000,
			 0x41100000,
			 0x41000000,
			 0x41900000,

			 /// vpu register f14
			 0x41100000,
			 0x40800000,
			 0x41a00000,
			 0x40000000,
			 0x41980000,
			 0x41000000,
			 0x40800000,
			 0x41c00000,

			 /// vpu register f15
			 0x41f00000,
			 0x41400000,
			 0x41800000,
			 0x40400000,
			 0x41b80000,
			 0x41b80000,
			 0x41300000,
			 0x41100000,

			 /// vpu register f16
			 0x41f80000,
			 0x40e00000,
			 0x41100000,
			 0x41100000,
			 0x41100000,
			 0x41d80000,
			 0x41600000,
			 0x41880000,

			 /// vpu register f17
			 0x41980000,
			 0x3f800000,
			 0x40c00000,
			 0x41000000,
			 0x41000000,
			 0x40c00000,
			 0x41000000,
			 0x41a80000,

			 /// vpu register f18
			 0x41e00000,
			 0x41900000,
			 0x3f800000,
			 0x41400000,
			 0x41c00000,
			 0x41300000,
			 0x41d80000,
			 0x41700000,

			 /// vpu register f19
			 0x41980000,
			 0x41400000,
			 0x41100000,
			 0x41600000,
			 0x41700000,
			 0x41100000,
			 0x41a00000,
			 0x41b00000,

			 /// vpu register f20
			 0x41400000,
			 0x41e00000,
			 0x42000000,
			 0x40000000,
			 0x41f80000,
			 0x40c00000,
			 0x40a00000,
			 0x41200000,

			 /// vpu register f21
			 0x41400000,
			 0x41600000,
			 0x41700000,
			 0x41100000,
			 0x3f800000,
			 0x41e80000,
			 0x41b80000,
			 0x41880000,

			 /// vpu register f22
			 0x41a00000,
			 0x41100000,
			 0x41a00000,
			 0x42000000,
			 0x41700000,
			 0x41f80000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f23
			 0x41a00000,
			 0x41a00000,
			 0x41000000,
			 0x42000000,
			 0x41600000,
			 0x41100000,
			 0x41200000,
			 0x41d00000,

			 /// vpu register f24
			 0x41f00000,
			 0x41400000,
			 0x40c00000,
			 0x41600000,
			 0x41200000,
			 0x42000000,
			 0x41e00000,
			 0x41400000,

			 /// vpu register f25
			 0x41f80000,
			 0x41a00000,
			 0x40a00000,
			 0x41200000,
			 0x41980000,
			 0x41f00000,
			 0x41d00000,
			 0x40800000,

			 /// vpu register f26
			 0x41800000,
			 0x41400000,
			 0x40a00000,
			 0x42000000,
			 0x41c00000,
			 0x41880000,
			 0x41e80000,
			 0x41900000,

			 /// vpu register f27
			 0x42000000,
			 0x41000000,
			 0x40800000,
			 0x41900000,
			 0x41980000,
			 0x41600000,
			 0x41b80000,
			 0x41f00000,

			 /// vpu register f28
			 0x41200000,
			 0x41880000,
			 0x41f00000,
			 0x41e80000,
			 0x41700000,
			 0x40a00000,
			 0x41a80000,
			 0x41a80000,

			 /// vpu register f29
			 0x41200000,
			 0x42000000,
			 0x40400000,
			 0x40800000,
			 0x40a00000,
			 0x41300000,
			 0x41b80000,
			 0x41000000,

			 /// vpu register f30
			 0x41d80000,
			 0x41a00000,
			 0x40e00000,
			 0x41c00000,
			 0x40e00000,
			 0x41a80000,
			 0x40e00000,
			 0x41c00000,

			 /// vpu register f31
			 0x41400000,
			 0x41b80000,
			 0x41400000,
			 0x41880000,
			 0x41d80000,
			 0x41d00000,
			 0x40400000,
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
		"famomaxul.pi f3, f1 (x14)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f16, f1 (x15)\n"                        ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f2, f1 (x13)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f5, f1 (x14)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f9, f1 (x13)\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f7, f1 (x15)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f11, f1 (x12)\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f5, f1 (x15)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f5, f1 (x13)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f17, f1 (x13)\n"                        ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f9, f1 (x12)\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f9, f1 (x14)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f20, f1 (x15)\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f22, f1 (x11)\n"                        ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f30, f1 (x13)\n"                        ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f14, f1 (x11)\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f6, f1 (x15)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f3, f1 (x13)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f6, f1 (x14)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f30, f1 (x14)\n"                        ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f2, f1 (x11)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f26, f1 (x12)\n"                        ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f19, f1 (x12)\n"                        ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f12, f1 (x12)\n"                        ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f4, f1 (x15)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f22, f1 (x12)\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f29, f1 (x15)\n"                        ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f17, f1 (x14)\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f6, f1 (x13)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f24, f1 (x11)\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f4, f1 (x15)\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f14, f1 (x15)\n"                        ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f17, f1 (x15)\n"                        ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f9, f1 (x12)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f18, f1 (x11)\n"                        ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f17, f1 (x12)\n"                        ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f15, f1 (x14)\n"                        ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f22, f1 (x12)\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f4, f1 (x13)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f2, f1 (x11)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f28, f1 (x11)\n"                        ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f2, f1 (x15)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f8, f1 (x14)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f15, f1 (x13)\n"                        ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f2, f1 (x14)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f16, f1 (x15)\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f23, f1 (x11)\n"                        ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f22, f1 (x15)\n"                        ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f8, f1 (x11)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f3, f1 (x11)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f6, f1 (x11)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f5, f1 (x13)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f25, f1 (x11)\n"                        ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f23, f1 (x14)\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f21, f1 (x12)\n"                        ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f10, f1 (x11)\n"                        ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f4, f1 (x14)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f17, f1 (x13)\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f12, f1 (x14)\n"                        ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f10, f1 (x11)\n"                        ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f8, f1 (x12)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f9, f1 (x15)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f19, f1 (x15)\n"                        ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f3, f1 (x13)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f24, f1 (x14)\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f6, f1 (x14)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f22, f1 (x15)\n"                        ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f28, f1 (x12)\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f30, f1 (x12)\n"                        ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f16, f1 (x14)\n"                        ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f13, f1 (x11)\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f28, f1 (x11)\n"                        ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f28, f1 (x14)\n"                        ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f26, f1 (x11)\n"                        ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f27, f1 (x13)\n"                        ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f29, f1 (x11)\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f30, f1 (x14)\n"                        ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f12, f1 (x11)\n"                        ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f13, f1 (x12)\n"                        ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f9, f1 (x12)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f20, f1 (x11)\n"                        ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f13, f1 (x12)\n"                        ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f30, f1 (x12)\n"                        ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f23, f1 (x15)\n"                        ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f11, f1 (x12)\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f13, f1 (x12)\n"                        ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f12, f1 (x13)\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f23, f1 (x12)\n"                        ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f24, f1 (x12)\n"                        ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f23, f1 (x15)\n"                        ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f21, f1 (x12)\n"                        ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f24, f1 (x12)\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f23, f1 (x12)\n"                        ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f3, f1 (x11)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f28, f1 (x12)\n"                        ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f22, f1 (x15)\n"                        ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f19, f1 (x15)\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f19, f1 (x13)\n"                        ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxul.pi f26, f1 (x15)\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
