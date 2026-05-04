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
			 0xe5449ba5, /// 0x0
			 0xa9e1afe1, /// 0x4
			 0x1d8c0cd0, /// 0x8
			 0xb3f7e7a3, /// 0xc
			 0x18b76aaa, /// 0x10
			 0xc02444d4, /// 0x14
			 0xf0d26c23, /// 0x18
			 0x0b06a055, /// 0x1c
			 0xc6a69cd0, /// 0x20
			 0x4094b3cc, /// 0x24
			 0x20da3a87, /// 0x28
			 0x6752d0c0, /// 0x2c
			 0xb2c771fa, /// 0x30
			 0xe9f15177, /// 0x34
			 0x9a5251c2, /// 0x38
			 0xbce3d78c, /// 0x3c
			 0x10eb34a5, /// 0x40
			 0x1c982a13, /// 0x44
			 0xd44c6833, /// 0x48
			 0xcc554cdf, /// 0x4c
			 0xeab9ecff, /// 0x50
			 0x6e6268e1, /// 0x54
			 0x8d58045d, /// 0x58
			 0xe0d35f74, /// 0x5c
			 0x34384baa, /// 0x60
			 0x4ef7cc86, /// 0x64
			 0xa3e24497, /// 0x68
			 0xf214794e, /// 0x6c
			 0xaff1a5ec, /// 0x70
			 0xeb702b04, /// 0x74
			 0xbc96bbf7, /// 0x78
			 0xa1af4a4e, /// 0x7c
			 0x248cf30c, /// 0x80
			 0x6721cdcc, /// 0x84
			 0x02bec23b, /// 0x88
			 0xf4118c59, /// 0x8c
			 0x55c8493f, /// 0x90
			 0x66ed7a54, /// 0x94
			 0xdd4c69de, /// 0x98
			 0x4b9f8cbf, /// 0x9c
			 0x022e40c8, /// 0xa0
			 0x24941f30, /// 0xa4
			 0x07369233, /// 0xa8
			 0x81c04833, /// 0xac
			 0xe7f5f5a0, /// 0xb0
			 0xec0a67da, /// 0xb4
			 0xab343159, /// 0xb8
			 0x616c3049, /// 0xbc
			 0xb8fa4376, /// 0xc0
			 0x60b732bb, /// 0xc4
			 0x036e2aed, /// 0xc8
			 0x9ee29322, /// 0xcc
			 0x12689a28, /// 0xd0
			 0xd533bc5d, /// 0xd4
			 0x7d0eb230, /// 0xd8
			 0xaa11fb69, /// 0xdc
			 0x8718c5db, /// 0xe0
			 0x03698044, /// 0xe4
			 0xf0b0ed5b, /// 0xe8
			 0xeaabe7d5, /// 0xec
			 0xaab7817a, /// 0xf0
			 0xc2421a54, /// 0xf4
			 0x937f78f6, /// 0xf8
			 0x3e69643a, /// 0xfc
			 0x6413e45b, /// 0x100
			 0x06f84ca5, /// 0x104
			 0xccc6d17f, /// 0x108
			 0x3341bb5e, /// 0x10c
			 0x5e4a8f8e, /// 0x110
			 0x142ff6db, /// 0x114
			 0x8f72d704, /// 0x118
			 0x47eb2139, /// 0x11c
			 0xc04590fa, /// 0x120
			 0xfdca0fcc, /// 0x124
			 0xc2d48273, /// 0x128
			 0x218cf788, /// 0x12c
			 0x218656c5, /// 0x130
			 0x07a08f14, /// 0x134
			 0x7c5845ff, /// 0x138
			 0xa759095a, /// 0x13c
			 0xd469b536, /// 0x140
			 0x061f22be, /// 0x144
			 0x84d394b3, /// 0x148
			 0x1bbd8884, /// 0x14c
			 0xc453e7b9, /// 0x150
			 0x4765b35f, /// 0x154
			 0xe7f05421, /// 0x158
			 0xe7ef1538, /// 0x15c
			 0x5eaabee6, /// 0x160
			 0xe0dac18f, /// 0x164
			 0x2c75516f, /// 0x168
			 0x73edffa5, /// 0x16c
			 0x2c16d5ef, /// 0x170
			 0x1834673d, /// 0x174
			 0xe022e5bb, /// 0x178
			 0x6137cecb, /// 0x17c
			 0x17d6b4a3, /// 0x180
			 0x9ec2fae1, /// 0x184
			 0x34f2f6c8, /// 0x188
			 0xdab5053c, /// 0x18c
			 0x1da88bec, /// 0x190
			 0x042cd530, /// 0x194
			 0xfaa75201, /// 0x198
			 0x49afb9c3, /// 0x19c
			 0x2787aed4, /// 0x1a0
			 0xc16a4d90, /// 0x1a4
			 0xff6d5cc8, /// 0x1a8
			 0xa816b7c8, /// 0x1ac
			 0xe1869050, /// 0x1b0
			 0xd6de9d4d, /// 0x1b4
			 0xf1f061f5, /// 0x1b8
			 0x6a2c009f, /// 0x1bc
			 0x8a7bd319, /// 0x1c0
			 0xb1cca904, /// 0x1c4
			 0x4860ab26, /// 0x1c8
			 0x2a64d29b, /// 0x1cc
			 0x350aaaa3, /// 0x1d0
			 0xb4e168cb, /// 0x1d4
			 0xd11b5893, /// 0x1d8
			 0x43013615, /// 0x1dc
			 0x397f0473, /// 0x1e0
			 0x271119cb, /// 0x1e4
			 0x0b347423, /// 0x1e8
			 0xa093f179, /// 0x1ec
			 0x361a3ee5, /// 0x1f0
			 0x5a5ae26d, /// 0x1f4
			 0x67894b3e, /// 0x1f8
			 0xa94ce737, /// 0x1fc
			 0xce28d88a, /// 0x200
			 0x4d2e267f, /// 0x204
			 0x38d0546f, /// 0x208
			 0xf3f3418d, /// 0x20c
			 0x1d9185e8, /// 0x210
			 0x0db40bec, /// 0x214
			 0x81441418, /// 0x218
			 0x9f81dc6e, /// 0x21c
			 0xa0e7b01a, /// 0x220
			 0x336852d7, /// 0x224
			 0x5bc23f45, /// 0x228
			 0x012b4988, /// 0x22c
			 0xdfcbaeca, /// 0x230
			 0x24774999, /// 0x234
			 0x4d3520b7, /// 0x238
			 0x78564ca9, /// 0x23c
			 0x67f8608d, /// 0x240
			 0x7dbfea7c, /// 0x244
			 0x75e99254, /// 0x248
			 0xde6b2d15, /// 0x24c
			 0x2486e99c, /// 0x250
			 0x2e3a68c3, /// 0x254
			 0x874c400e, /// 0x258
			 0x195a8ebb, /// 0x25c
			 0xb677a35c, /// 0x260
			 0x4c59ba65, /// 0x264
			 0xebe0fe14, /// 0x268
			 0x5fa2696f, /// 0x26c
			 0x61a9c8b0, /// 0x270
			 0xc6c1462e, /// 0x274
			 0xfa93bb62, /// 0x278
			 0x4426521a, /// 0x27c
			 0x961d9d9b, /// 0x280
			 0xfbb8fbac, /// 0x284
			 0x09985ee7, /// 0x288
			 0x8d628e22, /// 0x28c
			 0xa1b8f87f, /// 0x290
			 0xc3084a17, /// 0x294
			 0x02a06f50, /// 0x298
			 0x738d9347, /// 0x29c
			 0xd1ba4f3f, /// 0x2a0
			 0x1a13e876, /// 0x2a4
			 0xcd7bcf56, /// 0x2a8
			 0x5da487a0, /// 0x2ac
			 0x6d4b7386, /// 0x2b0
			 0x5ebfc8c5, /// 0x2b4
			 0xd37c1a9b, /// 0x2b8
			 0x9ae87230, /// 0x2bc
			 0x20abf7c5, /// 0x2c0
			 0xaf2f4d07, /// 0x2c4
			 0x164c6d29, /// 0x2c8
			 0x2ffd5ed5, /// 0x2cc
			 0xfcaef387, /// 0x2d0
			 0xeda24b47, /// 0x2d4
			 0x7174884a, /// 0x2d8
			 0x618ac2ab, /// 0x2dc
			 0xc95fab18, /// 0x2e0
			 0x64cfcae9, /// 0x2e4
			 0x9b6d4b0d, /// 0x2e8
			 0xbb87aa03, /// 0x2ec
			 0x7fb92fbb, /// 0x2f0
			 0x49e53b75, /// 0x2f4
			 0x300891c9, /// 0x2f8
			 0xed9c60e8, /// 0x2fc
			 0x4670bd6d, /// 0x300
			 0xc91eb1bb, /// 0x304
			 0x815a7fff, /// 0x308
			 0xe980357d, /// 0x30c
			 0x5974f781, /// 0x310
			 0xd6b8f7fd, /// 0x314
			 0xf842f791, /// 0x318
			 0x037f335c, /// 0x31c
			 0xe897171f, /// 0x320
			 0x97fea0eb, /// 0x324
			 0xa931dc01, /// 0x328
			 0xc0dddfbc, /// 0x32c
			 0x858a9c33, /// 0x330
			 0xea308a12, /// 0x334
			 0x19e1fa3a, /// 0x338
			 0x31c61769, /// 0x33c
			 0x855b56b1, /// 0x340
			 0x530f172a, /// 0x344
			 0xed3e4b79, /// 0x348
			 0x966bae11, /// 0x34c
			 0x4d76c53d, /// 0x350
			 0x869c6557, /// 0x354
			 0x514fd209, /// 0x358
			 0x0dd3c057, /// 0x35c
			 0x0161d69a, /// 0x360
			 0xe1122c90, /// 0x364
			 0x343d52fc, /// 0x368
			 0x921212b3, /// 0x36c
			 0x1ee789bc, /// 0x370
			 0xb23c0a11, /// 0x374
			 0x4afdd7c0, /// 0x378
			 0x037eab69, /// 0x37c
			 0x0c992af6, /// 0x380
			 0x0af50373, /// 0x384
			 0xb53a352e, /// 0x388
			 0xa6e9d43f, /// 0x38c
			 0xbb4866e3, /// 0x390
			 0x6af43e3a, /// 0x394
			 0xb5de8b82, /// 0x398
			 0x9495db1d, /// 0x39c
			 0x359d1251, /// 0x3a0
			 0x3c254cd8, /// 0x3a4
			 0xbafde1cb, /// 0x3a8
			 0x419fe962, /// 0x3ac
			 0x6aa26ecb, /// 0x3b0
			 0x597f4a43, /// 0x3b4
			 0xcd1c979b, /// 0x3b8
			 0xf4002974, /// 0x3bc
			 0x8c1aa404, /// 0x3c0
			 0x4a9a6ffb, /// 0x3c4
			 0xf5698aff, /// 0x3c8
			 0xf67cd655, /// 0x3cc
			 0xb7dc62de, /// 0x3d0
			 0xbdf3cc2c, /// 0x3d4
			 0x2808c496, /// 0x3d8
			 0x31990eb7, /// 0x3dc
			 0x5bdd5a96, /// 0x3e0
			 0x122c3562, /// 0x3e4
			 0x9aa61951, /// 0x3e8
			 0xb469dbc6, /// 0x3ec
			 0x7030abfe, /// 0x3f0
			 0xdc484771, /// 0x3f4
			 0x4059586d, /// 0x3f8
			 0xe6938f1c, /// 0x3fc
			 0x31797415, /// 0x400
			 0xb5d80fb9, /// 0x404
			 0xdffcfba6, /// 0x408
			 0xcd92db3a, /// 0x40c
			 0x8b552eac, /// 0x410
			 0xeb649ead, /// 0x414
			 0x79bd9187, /// 0x418
			 0xc276f421, /// 0x41c
			 0xd3810ec4, /// 0x420
			 0x5db5c894, /// 0x424
			 0x0085d726, /// 0x428
			 0xf44ab06f, /// 0x42c
			 0xe47345e8, /// 0x430
			 0x6cac5ba2, /// 0x434
			 0xcf51b566, /// 0x438
			 0x72c1aef0, /// 0x43c
			 0x8d51c9d7, /// 0x440
			 0x6057285b, /// 0x444
			 0xa66f0726, /// 0x448
			 0x2a64aba8, /// 0x44c
			 0x10bd2a19, /// 0x450
			 0xce391807, /// 0x454
			 0x250f73fa, /// 0x458
			 0x85638b8e, /// 0x45c
			 0x4a20547e, /// 0x460
			 0xd985b34f, /// 0x464
			 0x006445d0, /// 0x468
			 0x036a9c40, /// 0x46c
			 0xc6ff9e41, /// 0x470
			 0x9122c84f, /// 0x474
			 0x7a04cd94, /// 0x478
			 0x7443abbc, /// 0x47c
			 0x806f7875, /// 0x480
			 0xfec9e41a, /// 0x484
			 0xf7719d33, /// 0x488
			 0x7bd4a1e5, /// 0x48c
			 0x603d31d8, /// 0x490
			 0x47d74991, /// 0x494
			 0xefbdb50c, /// 0x498
			 0x71d631e5, /// 0x49c
			 0x2eb8b372, /// 0x4a0
			 0xed30a764, /// 0x4a4
			 0x2806b7f3, /// 0x4a8
			 0xc97a2288, /// 0x4ac
			 0x31f4fdaa, /// 0x4b0
			 0x9ea9b9ef, /// 0x4b4
			 0x08de1122, /// 0x4b8
			 0xf81803d9, /// 0x4bc
			 0x812091db, /// 0x4c0
			 0x066b7d93, /// 0x4c4
			 0x7d6d80c1, /// 0x4c8
			 0xfb0925e5, /// 0x4cc
			 0x0d988a8d, /// 0x4d0
			 0x04687012, /// 0x4d4
			 0x01d5b877, /// 0x4d8
			 0x62002654, /// 0x4dc
			 0x10c55d21, /// 0x4e0
			 0x27ead332, /// 0x4e4
			 0x5bc92e99, /// 0x4e8
			 0x8c03e632, /// 0x4ec
			 0xc0279796, /// 0x4f0
			 0x36e85593, /// 0x4f4
			 0x09a1f384, /// 0x4f8
			 0xdc0e5faa, /// 0x4fc
			 0x803cd41f, /// 0x500
			 0x4faf7934, /// 0x504
			 0xcd585d95, /// 0x508
			 0xf00bb550, /// 0x50c
			 0x581a4c37, /// 0x510
			 0x17bca812, /// 0x514
			 0x87093de5, /// 0x518
			 0xd00aff46, /// 0x51c
			 0x4a805d3d, /// 0x520
			 0xe4c6f0b2, /// 0x524
			 0x8569322b, /// 0x528
			 0x44cb1108, /// 0x52c
			 0xf95e2780, /// 0x530
			 0x24b1499d, /// 0x534
			 0x047c491f, /// 0x538
			 0x0bffd60c, /// 0x53c
			 0xc770379a, /// 0x540
			 0xe8a52789, /// 0x544
			 0xd1514c61, /// 0x548
			 0xf749df17, /// 0x54c
			 0x6b2f44d7, /// 0x550
			 0x6fbf6894, /// 0x554
			 0xb3b0e143, /// 0x558
			 0xc3d93193, /// 0x55c
			 0x5902e8a2, /// 0x560
			 0xf9fccc6d, /// 0x564
			 0x6088c994, /// 0x568
			 0x8b3caba7, /// 0x56c
			 0x9afab0ec, /// 0x570
			 0x86f2cfc7, /// 0x574
			 0xe1fb05ed, /// 0x578
			 0x2b597712, /// 0x57c
			 0xe9143b6e, /// 0x580
			 0x99986973, /// 0x584
			 0x29e26525, /// 0x588
			 0x660e1729, /// 0x58c
			 0x2975ee46, /// 0x590
			 0x3bf6b152, /// 0x594
			 0x0a1c95b3, /// 0x598
			 0xc0a1d04c, /// 0x59c
			 0xff947daf, /// 0x5a0
			 0xd786d062, /// 0x5a4
			 0xa32c3fcf, /// 0x5a8
			 0x0a1e12e2, /// 0x5ac
			 0x1ac236ee, /// 0x5b0
			 0xd3d3023a, /// 0x5b4
			 0x377fecae, /// 0x5b8
			 0x69b66bca, /// 0x5bc
			 0x29639432, /// 0x5c0
			 0x529824d7, /// 0x5c4
			 0x45b4698c, /// 0x5c8
			 0x9b4bcc16, /// 0x5cc
			 0x2d815444, /// 0x5d0
			 0x6a20996c, /// 0x5d4
			 0xf4281769, /// 0x5d8
			 0x1eb84f63, /// 0x5dc
			 0x86b85c75, /// 0x5e0
			 0x722320bc, /// 0x5e4
			 0xcb0cadf9, /// 0x5e8
			 0xe2ffb812, /// 0x5ec
			 0x185fcaaa, /// 0x5f0
			 0xeff353b3, /// 0x5f4
			 0xa874466f, /// 0x5f8
			 0x0190b627, /// 0x5fc
			 0x22ae88f0, /// 0x600
			 0x6c073f5f, /// 0x604
			 0xd3a69622, /// 0x608
			 0xdab08c77, /// 0x60c
			 0x57a9970b, /// 0x610
			 0xec9c85c0, /// 0x614
			 0xe70ab8c1, /// 0x618
			 0x6ac62234, /// 0x61c
			 0xb23a47dd, /// 0x620
			 0xe2a2d27f, /// 0x624
			 0xb3cb6cb4, /// 0x628
			 0x05f885eb, /// 0x62c
			 0x0e9da277, /// 0x630
			 0x4642d4ec, /// 0x634
			 0x90abebf4, /// 0x638
			 0x1dd09707, /// 0x63c
			 0xd15a62de, /// 0x640
			 0x9728b95c, /// 0x644
			 0x6dec8117, /// 0x648
			 0x979916d6, /// 0x64c
			 0xaeef3d28, /// 0x650
			 0x9f9d7371, /// 0x654
			 0x7e9212fa, /// 0x658
			 0x7f0531d7, /// 0x65c
			 0xacd38a8d, /// 0x660
			 0xea06ce48, /// 0x664
			 0xf4f82aba, /// 0x668
			 0x4ed2315e, /// 0x66c
			 0x8cab3318, /// 0x670
			 0x1301e29f, /// 0x674
			 0x58354d36, /// 0x678
			 0x19df5948, /// 0x67c
			 0xb4a7f702, /// 0x680
			 0x0f622515, /// 0x684
			 0xb8fd5de6, /// 0x688
			 0xbb92c886, /// 0x68c
			 0xdcd708ec, /// 0x690
			 0xbfbf4db2, /// 0x694
			 0xa8ba224e, /// 0x698
			 0xdbc1fead, /// 0x69c
			 0x52c29637, /// 0x6a0
			 0x9155d959, /// 0x6a4
			 0xa5f599b4, /// 0x6a8
			 0x4fef6535, /// 0x6ac
			 0x091add8a, /// 0x6b0
			 0x9fd1d776, /// 0x6b4
			 0x3ce95b84, /// 0x6b8
			 0x1030e052, /// 0x6bc
			 0x0f48cad5, /// 0x6c0
			 0x864b6de8, /// 0x6c4
			 0x15d5fb15, /// 0x6c8
			 0x7e7da60c, /// 0x6cc
			 0xef7a11d6, /// 0x6d0
			 0x18dc7fcb, /// 0x6d4
			 0x90a38a2e, /// 0x6d8
			 0xbf4b905f, /// 0x6dc
			 0x78356314, /// 0x6e0
			 0x04568f93, /// 0x6e4
			 0x878915f7, /// 0x6e8
			 0xbd3260ae, /// 0x6ec
			 0xd1312c36, /// 0x6f0
			 0x239c3e0f, /// 0x6f4
			 0x4a2af6b5, /// 0x6f8
			 0x147d8e3c, /// 0x6fc
			 0x6ed8bcaf, /// 0x700
			 0xc1a14995, /// 0x704
			 0xa78fa05b, /// 0x708
			 0xb923ce4f, /// 0x70c
			 0xaa5948d4, /// 0x710
			 0x57e94b74, /// 0x714
			 0xdc5d2ea5, /// 0x718
			 0x8b2113ae, /// 0x71c
			 0x586f9e6c, /// 0x720
			 0xa8e7d0b8, /// 0x724
			 0x880f669a, /// 0x728
			 0x460b4bac, /// 0x72c
			 0x7cd8727d, /// 0x730
			 0x09b8129b, /// 0x734
			 0xdf390c53, /// 0x738
			 0x8dbe8c5a, /// 0x73c
			 0x269f2b8a, /// 0x740
			 0xf55e9eec, /// 0x744
			 0x9ab559e1, /// 0x748
			 0xcde6cc1d, /// 0x74c
			 0x8eeeffb6, /// 0x750
			 0x13688ea0, /// 0x754
			 0x1c147444, /// 0x758
			 0xca8c8976, /// 0x75c
			 0xaae86bbb, /// 0x760
			 0x7b0364eb, /// 0x764
			 0x88565629, /// 0x768
			 0xf403ff13, /// 0x76c
			 0x2079740d, /// 0x770
			 0x0ddfc62c, /// 0x774
			 0xd983591f, /// 0x778
			 0xc2da1962, /// 0x77c
			 0x0065b4b2, /// 0x780
			 0x94ff5e88, /// 0x784
			 0xa71c8d2a, /// 0x788
			 0x5a7abab9, /// 0x78c
			 0xfbb6d400, /// 0x790
			 0x74cc747e, /// 0x794
			 0xde6a76d0, /// 0x798
			 0x59158e4b, /// 0x79c
			 0x57c0d553, /// 0x7a0
			 0x46935913, /// 0x7a4
			 0x2269eada, /// 0x7a8
			 0x329de100, /// 0x7ac
			 0x045ec138, /// 0x7b0
			 0xd6efbe37, /// 0x7b4
			 0x446acb28, /// 0x7b8
			 0x5bc41f01, /// 0x7bc
			 0x691a57fc, /// 0x7c0
			 0x382dc772, /// 0x7c4
			 0x194dd52d, /// 0x7c8
			 0x1f687756, /// 0x7cc
			 0xf7da3392, /// 0x7d0
			 0x8ab9e361, /// 0x7d4
			 0x2af24736, /// 0x7d8
			 0xbd130084, /// 0x7dc
			 0x7cc15aa9, /// 0x7e0
			 0xe96c53ef, /// 0x7e4
			 0xfde40bc7, /// 0x7e8
			 0xd64b09d7, /// 0x7ec
			 0x982ffa95, /// 0x7f0
			 0xcaf49884, /// 0x7f4
			 0x511d360c, /// 0x7f8
			 0x979d21bd, /// 0x7fc
			 0xfe98f9d8, /// 0x800
			 0x63953f87, /// 0x804
			 0x8a29bb9d, /// 0x808
			 0xc92043c7, /// 0x80c
			 0xffa5447e, /// 0x810
			 0x6ef69bba, /// 0x814
			 0x70079621, /// 0x818
			 0x8225d6ce, /// 0x81c
			 0x6b51f448, /// 0x820
			 0x594cd438, /// 0x824
			 0xcd5dcd3e, /// 0x828
			 0x35d62e9c, /// 0x82c
			 0xb2e39464, /// 0x830
			 0xce94b659, /// 0x834
			 0x8497ea82, /// 0x838
			 0xa35e0029, /// 0x83c
			 0xe4cd6f52, /// 0x840
			 0xb0929e2d, /// 0x844
			 0x1f9ab595, /// 0x848
			 0xb0339a1f, /// 0x84c
			 0x39000b65, /// 0x850
			 0x16c5a0d4, /// 0x854
			 0x424a5ae4, /// 0x858
			 0x20cac72d, /// 0x85c
			 0x46ffe590, /// 0x860
			 0x48ef2937, /// 0x864
			 0x01e1f5ee, /// 0x868
			 0xc0dbcca2, /// 0x86c
			 0x117e3cbd, /// 0x870
			 0x6e1ea5e2, /// 0x874
			 0x5f924f1e, /// 0x878
			 0x310fb114, /// 0x87c
			 0xc5380084, /// 0x880
			 0x1908ac97, /// 0x884
			 0x45be5336, /// 0x888
			 0xd4f650ed, /// 0x88c
			 0xffe12f7d, /// 0x890
			 0x941b0fe5, /// 0x894
			 0xebc530f3, /// 0x898
			 0x7ca79b94, /// 0x89c
			 0x51b640af, /// 0x8a0
			 0xb8899d80, /// 0x8a4
			 0x750f1717, /// 0x8a8
			 0xd94a119d, /// 0x8ac
			 0x15feeb87, /// 0x8b0
			 0xd572299e, /// 0x8b4
			 0xd14f7b3b, /// 0x8b8
			 0xdc787cdc, /// 0x8bc
			 0x9b1cb307, /// 0x8c0
			 0x676b1c2f, /// 0x8c4
			 0x22468a27, /// 0x8c8
			 0xa3cf7b86, /// 0x8cc
			 0xa9d662c2, /// 0x8d0
			 0xe4909c5f, /// 0x8d4
			 0x00ff2daa, /// 0x8d8
			 0x77648a09, /// 0x8dc
			 0x2ec74acc, /// 0x8e0
			 0x5246b155, /// 0x8e4
			 0xaa1c7271, /// 0x8e8
			 0x09871e50, /// 0x8ec
			 0x48041a56, /// 0x8f0
			 0x3ec233b7, /// 0x8f4
			 0x90d108c9, /// 0x8f8
			 0xcd09f07b, /// 0x8fc
			 0x70b8a131, /// 0x900
			 0x1e1e2e57, /// 0x904
			 0xa234f9bd, /// 0x908
			 0x59abd0b4, /// 0x90c
			 0xf1d7571c, /// 0x910
			 0x4e855939, /// 0x914
			 0x97148ea6, /// 0x918
			 0x91440fa5, /// 0x91c
			 0x2627e302, /// 0x920
			 0xf27e9974, /// 0x924
			 0x71af0851, /// 0x928
			 0xd2e0556d, /// 0x92c
			 0x305e9a59, /// 0x930
			 0xdba784d2, /// 0x934
			 0xdddd766e, /// 0x938
			 0x844cd3cb, /// 0x93c
			 0x5851ee08, /// 0x940
			 0xcd7fa177, /// 0x944
			 0xfe3671a7, /// 0x948
			 0xe0239c8d, /// 0x94c
			 0xdbaf001a, /// 0x950
			 0x2734864f, /// 0x954
			 0xa07874cf, /// 0x958
			 0x995a5e95, /// 0x95c
			 0x1595b655, /// 0x960
			 0x92acc04d, /// 0x964
			 0xe6caffb6, /// 0x968
			 0x0a4ea14f, /// 0x96c
			 0xc800265e, /// 0x970
			 0xa1713aea, /// 0x974
			 0x66dd63e2, /// 0x978
			 0x9ec77333, /// 0x97c
			 0x7bb007a7, /// 0x980
			 0x1cdbd24f, /// 0x984
			 0x7bdcdef4, /// 0x988
			 0x5c649d0f, /// 0x98c
			 0x5fa369e7, /// 0x990
			 0x696b4f70, /// 0x994
			 0x4f834ebd, /// 0x998
			 0xbf20acfe, /// 0x99c
			 0x3888f300, /// 0x9a0
			 0x899ab512, /// 0x9a4
			 0x4af1642d, /// 0x9a8
			 0x1a5a7166, /// 0x9ac
			 0x9b056055, /// 0x9b0
			 0x67ec9039, /// 0x9b4
			 0x6cda8737, /// 0x9b8
			 0xd8c9d8a3, /// 0x9bc
			 0xa1e7b617, /// 0x9c0
			 0x95eb25a1, /// 0x9c4
			 0x4472d11b, /// 0x9c8
			 0x8f23ef6f, /// 0x9cc
			 0xca3c3899, /// 0x9d0
			 0xdc4bbb7c, /// 0x9d4
			 0x097e5ca3, /// 0x9d8
			 0x791c23a6, /// 0x9dc
			 0xf1e7835e, /// 0x9e0
			 0x0aa05aa1, /// 0x9e4
			 0x7116d722, /// 0x9e8
			 0xaa9693d5, /// 0x9ec
			 0xf3099b88, /// 0x9f0
			 0x13ec25cd, /// 0x9f4
			 0x5e822f37, /// 0x9f8
			 0x76e44fe9, /// 0x9fc
			 0x67e55eeb, /// 0xa00
			 0x510409f3, /// 0xa04
			 0x7edb52ff, /// 0xa08
			 0xcb40b972, /// 0xa0c
			 0xada2c62d, /// 0xa10
			 0xac1ce868, /// 0xa14
			 0x41dbaca2, /// 0xa18
			 0x8736c23d, /// 0xa1c
			 0xc7085f2f, /// 0xa20
			 0xd9a91a92, /// 0xa24
			 0x261ad457, /// 0xa28
			 0xe17c5056, /// 0xa2c
			 0xa3f0ae5f, /// 0xa30
			 0xe70df51e, /// 0xa34
			 0x93d68329, /// 0xa38
			 0x28e61f34, /// 0xa3c
			 0xf56dcb1f, /// 0xa40
			 0x3078be8d, /// 0xa44
			 0xc56a3fba, /// 0xa48
			 0xb48a25fd, /// 0xa4c
			 0x7489be06, /// 0xa50
			 0x87cd14b0, /// 0xa54
			 0xab03f29e, /// 0xa58
			 0x565606eb, /// 0xa5c
			 0x11d6322a, /// 0xa60
			 0xf78393d8, /// 0xa64
			 0xbe2cebfe, /// 0xa68
			 0x2c2882f9, /// 0xa6c
			 0xc0b5f18b, /// 0xa70
			 0x521721ae, /// 0xa74
			 0x4a5af32c, /// 0xa78
			 0xb461448c, /// 0xa7c
			 0xbf500108, /// 0xa80
			 0x7f2cd2e0, /// 0xa84
			 0x7e7d239c, /// 0xa88
			 0x778de960, /// 0xa8c
			 0x7a77ad95, /// 0xa90
			 0x78d1289e, /// 0xa94
			 0x33d3b55e, /// 0xa98
			 0xb4035bbb, /// 0xa9c
			 0xaae4529e, /// 0xaa0
			 0x451e7ddb, /// 0xaa4
			 0xf5f846b1, /// 0xaa8
			 0x86826683, /// 0xaac
			 0x134b0399, /// 0xab0
			 0x47993887, /// 0xab4
			 0x0e7b06c7, /// 0xab8
			 0xb139065a, /// 0xabc
			 0xa1182493, /// 0xac0
			 0xb3e1002a, /// 0xac4
			 0x2e76b44d, /// 0xac8
			 0x976dc2b4, /// 0xacc
			 0xa81bafd9, /// 0xad0
			 0x1f0abc91, /// 0xad4
			 0xa528684a, /// 0xad8
			 0x2fa8dc0e, /// 0xadc
			 0x0aac4774, /// 0xae0
			 0x566783a6, /// 0xae4
			 0xaa82bbcb, /// 0xae8
			 0xa3f85f76, /// 0xaec
			 0x87cd27d5, /// 0xaf0
			 0xd046749b, /// 0xaf4
			 0xff09f8c6, /// 0xaf8
			 0x88060445, /// 0xafc
			 0xac841a60, /// 0xb00
			 0x31aa7d2e, /// 0xb04
			 0xafa87e3d, /// 0xb08
			 0x2846aa28, /// 0xb0c
			 0xefee8618, /// 0xb10
			 0xab92961f, /// 0xb14
			 0x43f572a1, /// 0xb18
			 0x7ed2249e, /// 0xb1c
			 0x53b47800, /// 0xb20
			 0x870ae81a, /// 0xb24
			 0x36cc1cae, /// 0xb28
			 0x3a4e107d, /// 0xb2c
			 0x85cda1a8, /// 0xb30
			 0xf4bf6eda, /// 0xb34
			 0xf0c99f71, /// 0xb38
			 0x638ce110, /// 0xb3c
			 0xb8ee0b11, /// 0xb40
			 0x03f68d81, /// 0xb44
			 0xa0491e78, /// 0xb48
			 0xfd820fd0, /// 0xb4c
			 0xed2dc94a, /// 0xb50
			 0x0faac7f9, /// 0xb54
			 0xb3c2710f, /// 0xb58
			 0x8d50506d, /// 0xb5c
			 0xabd8e02f, /// 0xb60
			 0x495ae6c3, /// 0xb64
			 0x055ce550, /// 0xb68
			 0x24a0142d, /// 0xb6c
			 0xba71b525, /// 0xb70
			 0xd1e5ec61, /// 0xb74
			 0xf28da61c, /// 0xb78
			 0x94bc7518, /// 0xb7c
			 0xe4d80194, /// 0xb80
			 0x44ef38ab, /// 0xb84
			 0x9fc84891, /// 0xb88
			 0xafdf4d4b, /// 0xb8c
			 0x6a6eff5c, /// 0xb90
			 0x4ab8c45c, /// 0xb94
			 0xdfab8e67, /// 0xb98
			 0xca59e8b7, /// 0xb9c
			 0xe18462bf, /// 0xba0
			 0x0a16375b, /// 0xba4
			 0xd6f651b0, /// 0xba8
			 0x41d61cca, /// 0xbac
			 0x7acc4c4b, /// 0xbb0
			 0x1c39b8dc, /// 0xbb4
			 0xea76bed1, /// 0xbb8
			 0xef253451, /// 0xbbc
			 0x8ecec4f5, /// 0xbc0
			 0x4896e02e, /// 0xbc4
			 0xebf3a233, /// 0xbc8
			 0x86a35c68, /// 0xbcc
			 0x8ad2271d, /// 0xbd0
			 0xbdf715e1, /// 0xbd4
			 0x601a4a87, /// 0xbd8
			 0xa087f7de, /// 0xbdc
			 0x9e587c33, /// 0xbe0
			 0xd366d0da, /// 0xbe4
			 0xd0294e7f, /// 0xbe8
			 0x7ff3f317, /// 0xbec
			 0x9ccebfa6, /// 0xbf0
			 0x8a66ab95, /// 0xbf4
			 0x3c4f22e7, /// 0xbf8
			 0xe9499f60, /// 0xbfc
			 0xb3702cac, /// 0xc00
			 0x74a9cf09, /// 0xc04
			 0x3627d07d, /// 0xc08
			 0x0fa66965, /// 0xc0c
			 0xa3c3daba, /// 0xc10
			 0xf002c930, /// 0xc14
			 0x5efb91c4, /// 0xc18
			 0xf8c73346, /// 0xc1c
			 0x55d2b6da, /// 0xc20
			 0x76f3cbec, /// 0xc24
			 0x9d39637b, /// 0xc28
			 0x1d0725e6, /// 0xc2c
			 0xc6764098, /// 0xc30
			 0xcb056e34, /// 0xc34
			 0xb3879646, /// 0xc38
			 0x8306a128, /// 0xc3c
			 0xa8b5f62f, /// 0xc40
			 0x1e875358, /// 0xc44
			 0xddb5dd60, /// 0xc48
			 0x05ff28a9, /// 0xc4c
			 0xca8b7073, /// 0xc50
			 0x8f25a3fc, /// 0xc54
			 0xe3f7f651, /// 0xc58
			 0x82bd1026, /// 0xc5c
			 0xff25d78c, /// 0xc60
			 0xa749a625, /// 0xc64
			 0x8a3e4e63, /// 0xc68
			 0x2b248f2e, /// 0xc6c
			 0xbcf4dc9f, /// 0xc70
			 0xf4bac88d, /// 0xc74
			 0x0ec7d362, /// 0xc78
			 0x10b9946c, /// 0xc7c
			 0xdc3800e4, /// 0xc80
			 0x92bd701c, /// 0xc84
			 0xb89b0fd0, /// 0xc88
			 0x5fe5dd77, /// 0xc8c
			 0xb45357e4, /// 0xc90
			 0xc4d3b25f, /// 0xc94
			 0x78673878, /// 0xc98
			 0x70ffdb2d, /// 0xc9c
			 0x84662fef, /// 0xca0
			 0x04208b38, /// 0xca4
			 0x78d32872, /// 0xca8
			 0x19ee84a8, /// 0xcac
			 0xc28d76a8, /// 0xcb0
			 0x150aa730, /// 0xcb4
			 0x5c7bece5, /// 0xcb8
			 0xb774ca2d, /// 0xcbc
			 0x88b56b28, /// 0xcc0
			 0x2c341ce1, /// 0xcc4
			 0xd99b5826, /// 0xcc8
			 0x1af2ff27, /// 0xccc
			 0xa8bd51b1, /// 0xcd0
			 0x5beec910, /// 0xcd4
			 0x270852f3, /// 0xcd8
			 0xae943973, /// 0xcdc
			 0x857e9b69, /// 0xce0
			 0xbc2039d7, /// 0xce4
			 0xafc2f58d, /// 0xce8
			 0x5ce268ac, /// 0xcec
			 0x73d0e3a9, /// 0xcf0
			 0xf9d9c91a, /// 0xcf4
			 0xb0cd4c7a, /// 0xcf8
			 0xd2702dae, /// 0xcfc
			 0xa5f33aec, /// 0xd00
			 0x886fe221, /// 0xd04
			 0x21a19a49, /// 0xd08
			 0x883083e6, /// 0xd0c
			 0x46d1b2e6, /// 0xd10
			 0xd27724c3, /// 0xd14
			 0xc3121426, /// 0xd18
			 0xe6841086, /// 0xd1c
			 0xdf27175f, /// 0xd20
			 0xdb62d91c, /// 0xd24
			 0x31d21e86, /// 0xd28
			 0x9e508403, /// 0xd2c
			 0xd4daebaf, /// 0xd30
			 0xfcd829d0, /// 0xd34
			 0xefb38586, /// 0xd38
			 0xfc253d9f, /// 0xd3c
			 0xebc8b55a, /// 0xd40
			 0x9c9990e3, /// 0xd44
			 0x4468aaf9, /// 0xd48
			 0x5b1158ec, /// 0xd4c
			 0x290b57bb, /// 0xd50
			 0x5db1a802, /// 0xd54
			 0xfbf57f8e, /// 0xd58
			 0x138983a6, /// 0xd5c
			 0x236b44ff, /// 0xd60
			 0x02f5344c, /// 0xd64
			 0x28a5aa4d, /// 0xd68
			 0x8f5fd2de, /// 0xd6c
			 0x37ce6a48, /// 0xd70
			 0x8de0abb8, /// 0xd74
			 0x0e06d8a7, /// 0xd78
			 0xa55e6821, /// 0xd7c
			 0x803b193e, /// 0xd80
			 0x9efa8564, /// 0xd84
			 0x258e0b63, /// 0xd88
			 0x0d4ebff6, /// 0xd8c
			 0xa10fa605, /// 0xd90
			 0x29cedc82, /// 0xd94
			 0x4c1b362c, /// 0xd98
			 0x0e2d57a5, /// 0xd9c
			 0x7132a51a, /// 0xda0
			 0x6f80bf8f, /// 0xda4
			 0x80aca5f7, /// 0xda8
			 0x65c1daaa, /// 0xdac
			 0xb84eda7c, /// 0xdb0
			 0xb374d57a, /// 0xdb4
			 0xa99e3cbe, /// 0xdb8
			 0xbc82be18, /// 0xdbc
			 0x41bd969d, /// 0xdc0
			 0xd84061ca, /// 0xdc4
			 0x3b3f1add, /// 0xdc8
			 0x8e7ce297, /// 0xdcc
			 0x145c753c, /// 0xdd0
			 0xad28002b, /// 0xdd4
			 0xff7f2f62, /// 0xdd8
			 0x55a95d64, /// 0xddc
			 0x8c78b097, /// 0xde0
			 0x52f5d0f2, /// 0xde4
			 0x47382785, /// 0xde8
			 0xf1ec8e0d, /// 0xdec
			 0x3c5f4448, /// 0xdf0
			 0x2b6d98f2, /// 0xdf4
			 0x2de7bc65, /// 0xdf8
			 0xdddc27c8, /// 0xdfc
			 0xad0eee7b, /// 0xe00
			 0xb0cdb393, /// 0xe04
			 0x43934a5d, /// 0xe08
			 0x67142885, /// 0xe0c
			 0xa845ee55, /// 0xe10
			 0xb8ee0186, /// 0xe14
			 0x208ec419, /// 0xe18
			 0xee4bdcd9, /// 0xe1c
			 0x6b4e3bd8, /// 0xe20
			 0xbd0727a2, /// 0xe24
			 0xc4f59edd, /// 0xe28
			 0x8e8ebc97, /// 0xe2c
			 0x1d64f157, /// 0xe30
			 0x2728629b, /// 0xe34
			 0x0b478d05, /// 0xe38
			 0x4c0bf8d9, /// 0xe3c
			 0xd1104e6d, /// 0xe40
			 0x243d63b1, /// 0xe44
			 0x1ae79074, /// 0xe48
			 0x7c662a72, /// 0xe4c
			 0x395cc479, /// 0xe50
			 0x883f502a, /// 0xe54
			 0xd347cadd, /// 0xe58
			 0x463b7284, /// 0xe5c
			 0xac561723, /// 0xe60
			 0x2ebd0744, /// 0xe64
			 0x69e66e20, /// 0xe68
			 0x7b636d9d, /// 0xe6c
			 0x651ff8a3, /// 0xe70
			 0xd82add1f, /// 0xe74
			 0x32cced88, /// 0xe78
			 0xdcf9d31e, /// 0xe7c
			 0x2c41f3bf, /// 0xe80
			 0x45eedfcd, /// 0xe84
			 0x0aaaa4a2, /// 0xe88
			 0x134901c6, /// 0xe8c
			 0x3a8d2124, /// 0xe90
			 0xedabc185, /// 0xe94
			 0xcd8d58f1, /// 0xe98
			 0x37d0f0e6, /// 0xe9c
			 0x35585fc7, /// 0xea0
			 0x236b3377, /// 0xea4
			 0xc86ebb4d, /// 0xea8
			 0x34d1133e, /// 0xeac
			 0x33cfa4e7, /// 0xeb0
			 0xc6f92a5d, /// 0xeb4
			 0x8247535a, /// 0xeb8
			 0x41994579, /// 0xebc
			 0xfb1cea9f, /// 0xec0
			 0x1a513b07, /// 0xec4
			 0xe8993577, /// 0xec8
			 0xc7121f6b, /// 0xecc
			 0xe5496e04, /// 0xed0
			 0x8b3520c0, /// 0xed4
			 0x26c83f85, /// 0xed8
			 0x335da8c1, /// 0xedc
			 0xa6ac596f, /// 0xee0
			 0x9b225c29, /// 0xee4
			 0x9fdcf04b, /// 0xee8
			 0xb3548ec4, /// 0xeec
			 0x2aa1c8fc, /// 0xef0
			 0x2c086def, /// 0xef4
			 0xa478af31, /// 0xef8
			 0x3dfc97ec, /// 0xefc
			 0xaab951aa, /// 0xf00
			 0xac2dd7b9, /// 0xf04
			 0x500cadd8, /// 0xf08
			 0xbf0fd451, /// 0xf0c
			 0xc7e735ab, /// 0xf10
			 0x0993f609, /// 0xf14
			 0x7696b021, /// 0xf18
			 0xc4d34acf, /// 0xf1c
			 0x0b9291a0, /// 0xf20
			 0x2f27becb, /// 0xf24
			 0x7410e217, /// 0xf28
			 0x3b813bf9, /// 0xf2c
			 0xe744e59f, /// 0xf30
			 0x86b1f4a0, /// 0xf34
			 0xd06c384b, /// 0xf38
			 0xe286581a, /// 0xf3c
			 0x17bbff24, /// 0xf40
			 0x68ecbed3, /// 0xf44
			 0xa737da38, /// 0xf48
			 0x37788b61, /// 0xf4c
			 0x6e45cd87, /// 0xf50
			 0x6bd65339, /// 0xf54
			 0x0cb9c074, /// 0xf58
			 0xe7809959, /// 0xf5c
			 0xbaeb0280, /// 0xf60
			 0x97b12ba1, /// 0xf64
			 0x3f28bf54, /// 0xf68
			 0x8c035efe, /// 0xf6c
			 0x84b05eb1, /// 0xf70
			 0x64e417f1, /// 0xf74
			 0xd56cc37e, /// 0xf78
			 0xff67560b, /// 0xf7c
			 0xd41c2468, /// 0xf80
			 0x1629fc2e, /// 0xf84
			 0x6e121705, /// 0xf88
			 0xcd3398ca, /// 0xf8c
			 0xd07bdf48, /// 0xf90
			 0xe735e9b4, /// 0xf94
			 0xf2ce5800, /// 0xf98
			 0x43325b4b, /// 0xf9c
			 0x43a46a73, /// 0xfa0
			 0xd7042187, /// 0xfa4
			 0xff4678ac, /// 0xfa8
			 0x7c4dd7c5, /// 0xfac
			 0xd63c1be9, /// 0xfb0
			 0xe037b6f8, /// 0xfb4
			 0x675bfbf5, /// 0xfb8
			 0x4c5fd95b, /// 0xfbc
			 0xc85ee11b, /// 0xfc0
			 0x52b81e95, /// 0xfc4
			 0x2e8c95da, /// 0xfc8
			 0xdc01d146, /// 0xfcc
			 0x21b50eaf, /// 0xfd0
			 0x98b66d3b, /// 0xfd4
			 0x7a0fac74, /// 0xfd8
			 0xc893edb1, /// 0xfdc
			 0xf9459f69, /// 0xfe0
			 0x2a8301ed, /// 0xfe4
			 0x012ca226, /// 0xfe8
			 0x9f8ae9c3, /// 0xfec
			 0x51204ee1, /// 0xff0
			 0xc8ba409d, /// 0xff4
			 0x4e2d46ee, /// 0xff8
			 0xdff58d8b /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xd315455b, /// 0x0
			 0xc197cfef, /// 0x4
			 0x7e3b8817, /// 0x8
			 0x5fb59376, /// 0xc
			 0x9527a6be, /// 0x10
			 0x263d21eb, /// 0x14
			 0x55cdd23e, /// 0x18
			 0xe3a0883d, /// 0x1c
			 0x7f56fa62, /// 0x20
			 0x5785e9b5, /// 0x24
			 0x26226fd7, /// 0x28
			 0x729ae34f, /// 0x2c
			 0x0a260c72, /// 0x30
			 0x4ba67842, /// 0x34
			 0x1abed692, /// 0x38
			 0xc6a6911f, /// 0x3c
			 0x3f6fdacd, /// 0x40
			 0x518445d2, /// 0x44
			 0x1de65767, /// 0x48
			 0x9ed799b6, /// 0x4c
			 0x44e7f0b9, /// 0x50
			 0x8ed68d05, /// 0x54
			 0x891f59f6, /// 0x58
			 0xcf19dd53, /// 0x5c
			 0xf177615b, /// 0x60
			 0xe07364c3, /// 0x64
			 0x4bd48aab, /// 0x68
			 0xf0da6898, /// 0x6c
			 0xf23e2d56, /// 0x70
			 0x3490b14c, /// 0x74
			 0x5b14fed2, /// 0x78
			 0x13f38495, /// 0x7c
			 0xd990a27c, /// 0x80
			 0x18ca488e, /// 0x84
			 0x47449117, /// 0x88
			 0x5341776e, /// 0x8c
			 0xe9e13ac5, /// 0x90
			 0x2045ea85, /// 0x94
			 0xd29104b6, /// 0x98
			 0x3fd2fbbf, /// 0x9c
			 0xc5b9e410, /// 0xa0
			 0x82afe39b, /// 0xa4
			 0x0d309bd7, /// 0xa8
			 0x59bc3e0b, /// 0xac
			 0x1577b4cf, /// 0xb0
			 0xab1fae58, /// 0xb4
			 0xdcb0d102, /// 0xb8
			 0x93ff4d97, /// 0xbc
			 0x1f4117c0, /// 0xc0
			 0x21a1e346, /// 0xc4
			 0x78779630, /// 0xc8
			 0x70b021fb, /// 0xcc
			 0xcaf6b3f7, /// 0xd0
			 0x6f7c242b, /// 0xd4
			 0x28514a02, /// 0xd8
			 0xf472a99e, /// 0xdc
			 0xbf44fb49, /// 0xe0
			 0xfb2c3b4b, /// 0xe4
			 0x85f54a6c, /// 0xe8
			 0x63efa1ba, /// 0xec
			 0xd1290282, /// 0xf0
			 0x69e654d1, /// 0xf4
			 0xb0126469, /// 0xf8
			 0xcf15a278, /// 0xfc
			 0x9273ccdd, /// 0x100
			 0x644e1852, /// 0x104
			 0x655b212c, /// 0x108
			 0x8d6a6ec8, /// 0x10c
			 0x3558064c, /// 0x110
			 0x1aab90da, /// 0x114
			 0x4e36c2f6, /// 0x118
			 0x6707c972, /// 0x11c
			 0xd27859c5, /// 0x120
			 0x5aea3433, /// 0x124
			 0x5be7baaa, /// 0x128
			 0x5e397c4f, /// 0x12c
			 0x485a240e, /// 0x130
			 0xc1f6501e, /// 0x134
			 0x0baabe28, /// 0x138
			 0x2c1c5150, /// 0x13c
			 0xbf8e0526, /// 0x140
			 0x6d9f3c73, /// 0x144
			 0xa8d182bd, /// 0x148
			 0x2e6c38f1, /// 0x14c
			 0xdb96b161, /// 0x150
			 0xe054dc2c, /// 0x154
			 0xfe25a795, /// 0x158
			 0xb2493d73, /// 0x15c
			 0x6a64c552, /// 0x160
			 0x6bc43d2c, /// 0x164
			 0x8698d7e1, /// 0x168
			 0xb902f380, /// 0x16c
			 0xfb5bab73, /// 0x170
			 0xaa705647, /// 0x174
			 0xd12e4976, /// 0x178
			 0xd85512d4, /// 0x17c
			 0x4240ca12, /// 0x180
			 0xbcb73927, /// 0x184
			 0x1a2cad4f, /// 0x188
			 0xb32dd658, /// 0x18c
			 0x89836a5a, /// 0x190
			 0xedced1b3, /// 0x194
			 0x77cd7c2d, /// 0x198
			 0x09cb2489, /// 0x19c
			 0x0eac42f1, /// 0x1a0
			 0x306038a2, /// 0x1a4
			 0x94e97e9e, /// 0x1a8
			 0x831239b3, /// 0x1ac
			 0x217f6cf5, /// 0x1b0
			 0xc4ff42fa, /// 0x1b4
			 0x920b716c, /// 0x1b8
			 0xa5b9b570, /// 0x1bc
			 0xc6633287, /// 0x1c0
			 0xa11fb4d5, /// 0x1c4
			 0x89e8fdd6, /// 0x1c8
			 0x5b356d05, /// 0x1cc
			 0xa2d2cf83, /// 0x1d0
			 0xafb845e9, /// 0x1d4
			 0x3ae6e1ba, /// 0x1d8
			 0xed99bc25, /// 0x1dc
			 0xeafb0276, /// 0x1e0
			 0x1ac8c8b2, /// 0x1e4
			 0xb4e9a634, /// 0x1e8
			 0x84fab8ba, /// 0x1ec
			 0xce6be812, /// 0x1f0
			 0x9810c6fb, /// 0x1f4
			 0x75e38dc8, /// 0x1f8
			 0xdd722530, /// 0x1fc
			 0x4fa0f2c3, /// 0x200
			 0x2104ec20, /// 0x204
			 0x316bc909, /// 0x208
			 0x17c28001, /// 0x20c
			 0xb7ed4239, /// 0x210
			 0xfa3648b9, /// 0x214
			 0x061cae6c, /// 0x218
			 0x4c6e36cd, /// 0x21c
			 0x731b8880, /// 0x220
			 0x797c57ea, /// 0x224
			 0xf27ece38, /// 0x228
			 0x7958fbf5, /// 0x22c
			 0x428231f6, /// 0x230
			 0x2cda79dd, /// 0x234
			 0x1e4d1bd4, /// 0x238
			 0xcd776381, /// 0x23c
			 0xcf53baa9, /// 0x240
			 0xbc2efe5c, /// 0x244
			 0xd5eee15d, /// 0x248
			 0xc183d2ae, /// 0x24c
			 0x4009791a, /// 0x250
			 0x81918d17, /// 0x254
			 0xafbe906a, /// 0x258
			 0x340bfa72, /// 0x25c
			 0x6768f7a9, /// 0x260
			 0xc07f1345, /// 0x264
			 0xa19d5805, /// 0x268
			 0xe3877a4d, /// 0x26c
			 0xe95001b0, /// 0x270
			 0x324a860c, /// 0x274
			 0x71366d69, /// 0x278
			 0x58cd3596, /// 0x27c
			 0x0283faac, /// 0x280
			 0xb811c495, /// 0x284
			 0x282d2221, /// 0x288
			 0xe40fef6c, /// 0x28c
			 0x72c5f940, /// 0x290
			 0xc967d058, /// 0x294
			 0xc7b264f0, /// 0x298
			 0x09af58cd, /// 0x29c
			 0xc15953f1, /// 0x2a0
			 0x0e6df230, /// 0x2a4
			 0xb13ef1b4, /// 0x2a8
			 0xbf578846, /// 0x2ac
			 0x3368a66c, /// 0x2b0
			 0xace2605a, /// 0x2b4
			 0x73e6ac88, /// 0x2b8
			 0xce67a4ec, /// 0x2bc
			 0x79bc9fd5, /// 0x2c0
			 0x282ae1ad, /// 0x2c4
			 0x443a3c8a, /// 0x2c8
			 0xded7bc8a, /// 0x2cc
			 0x34d7966b, /// 0x2d0
			 0x624778b0, /// 0x2d4
			 0x25fa9b72, /// 0x2d8
			 0x25a179a1, /// 0x2dc
			 0x6fd24237, /// 0x2e0
			 0xe3a32ce7, /// 0x2e4
			 0xdd42d08d, /// 0x2e8
			 0xe403c98c, /// 0x2ec
			 0x9049d3fa, /// 0x2f0
			 0x41a9dec1, /// 0x2f4
			 0xa508c8b8, /// 0x2f8
			 0x9ade9293, /// 0x2fc
			 0x6af16c69, /// 0x300
			 0x5216b3d6, /// 0x304
			 0xeb33f1c7, /// 0x308
			 0x7edf155b, /// 0x30c
			 0x8b460e71, /// 0x310
			 0xc11f2438, /// 0x314
			 0x7f2c61b5, /// 0x318
			 0xc8f2227f, /// 0x31c
			 0x8c75800f, /// 0x320
			 0x17be429c, /// 0x324
			 0x91bab2f5, /// 0x328
			 0x015fcbf6, /// 0x32c
			 0x408a1d64, /// 0x330
			 0xb3f0663b, /// 0x334
			 0x50f14852, /// 0x338
			 0xb8472a24, /// 0x33c
			 0xca7ec16d, /// 0x340
			 0x8c7d7ec9, /// 0x344
			 0xdba6bf30, /// 0x348
			 0xdf12f602, /// 0x34c
			 0xe14f19fb, /// 0x350
			 0xe892b982, /// 0x354
			 0x06190364, /// 0x358
			 0x75554b74, /// 0x35c
			 0x5db7bfe6, /// 0x360
			 0x51fdce9f, /// 0x364
			 0x7c57ab23, /// 0x368
			 0x5c8804fb, /// 0x36c
			 0xee3aaaa6, /// 0x370
			 0x4eb5f412, /// 0x374
			 0x01958533, /// 0x378
			 0x7f4e3690, /// 0x37c
			 0xf4aee6c2, /// 0x380
			 0xaaf7400a, /// 0x384
			 0x5aa41945, /// 0x388
			 0x11a9d1a4, /// 0x38c
			 0xa0eb136f, /// 0x390
			 0x4eab557d, /// 0x394
			 0x34551910, /// 0x398
			 0x69f111fd, /// 0x39c
			 0x10c99fdb, /// 0x3a0
			 0x08d62f11, /// 0x3a4
			 0x32f32e9e, /// 0x3a8
			 0x7372e3f8, /// 0x3ac
			 0x508a514d, /// 0x3b0
			 0x456251c2, /// 0x3b4
			 0x05557fa9, /// 0x3b8
			 0xa9fbf2a1, /// 0x3bc
			 0xe9eb2722, /// 0x3c0
			 0x32130eba, /// 0x3c4
			 0x765efb7d, /// 0x3c8
			 0x8b996617, /// 0x3cc
			 0x4063f7ed, /// 0x3d0
			 0x1158c445, /// 0x3d4
			 0x73fad5e3, /// 0x3d8
			 0xbdfecbe0, /// 0x3dc
			 0xc35cdc56, /// 0x3e0
			 0x77430898, /// 0x3e4
			 0xdf1de39e, /// 0x3e8
			 0xa8225f7f, /// 0x3ec
			 0x4dc666ae, /// 0x3f0
			 0x489ca9a7, /// 0x3f4
			 0x67276509, /// 0x3f8
			 0x79484913, /// 0x3fc
			 0x991b8d76, /// 0x400
			 0x2283f8f4, /// 0x404
			 0x43a9d9ac, /// 0x408
			 0xd030a2f4, /// 0x40c
			 0xf6e1463a, /// 0x410
			 0xab640895, /// 0x414
			 0x1dcb5761, /// 0x418
			 0x9169657d, /// 0x41c
			 0xa4b4679d, /// 0x420
			 0x06598c4f, /// 0x424
			 0x26188d78, /// 0x428
			 0x9e91b611, /// 0x42c
			 0x75fc4d53, /// 0x430
			 0xd188d3d9, /// 0x434
			 0x8f6aa96e, /// 0x438
			 0xe1a8d2d7, /// 0x43c
			 0xc5e077cc, /// 0x440
			 0x83fb0d6f, /// 0x444
			 0x0c335849, /// 0x448
			 0x76493052, /// 0x44c
			 0x43e95581, /// 0x450
			 0xb91acbfa, /// 0x454
			 0x23a6b28a, /// 0x458
			 0x886e47b4, /// 0x45c
			 0x1dd3f98f, /// 0x460
			 0xb8eea46a, /// 0x464
			 0xce088d64, /// 0x468
			 0x075788d2, /// 0x46c
			 0x33e40b74, /// 0x470
			 0x67191453, /// 0x474
			 0x83a16f97, /// 0x478
			 0xfa06e13c, /// 0x47c
			 0x0d228c32, /// 0x480
			 0x912ac5fb, /// 0x484
			 0x52822ab5, /// 0x488
			 0x3363ad17, /// 0x48c
			 0x1f831c7e, /// 0x490
			 0x538d7789, /// 0x494
			 0x0200201d, /// 0x498
			 0x27acbb80, /// 0x49c
			 0x2b191163, /// 0x4a0
			 0xf89b0cac, /// 0x4a4
			 0x8559b53d, /// 0x4a8
			 0x38ec7847, /// 0x4ac
			 0xa0a68a92, /// 0x4b0
			 0xcfde5801, /// 0x4b4
			 0x98cff91b, /// 0x4b8
			 0x746f96ac, /// 0x4bc
			 0x9fbbc044, /// 0x4c0
			 0x60d833a1, /// 0x4c4
			 0x3e7c9927, /// 0x4c8
			 0x9857e176, /// 0x4cc
			 0x416d08e0, /// 0x4d0
			 0xf02567f6, /// 0x4d4
			 0x07e9144a, /// 0x4d8
			 0x81a0a179, /// 0x4dc
			 0x0243b343, /// 0x4e0
			 0xb6f8b216, /// 0x4e4
			 0xd2de3197, /// 0x4e8
			 0xabb14705, /// 0x4ec
			 0xcfe793e1, /// 0x4f0
			 0x5bb392b0, /// 0x4f4
			 0xc242d331, /// 0x4f8
			 0x3b34962d, /// 0x4fc
			 0xee87732f, /// 0x500
			 0xe3f5ed1b, /// 0x504
			 0x71914ecd, /// 0x508
			 0x18bb4fe7, /// 0x50c
			 0x48f63135, /// 0x510
			 0x13727242, /// 0x514
			 0x6fe283af, /// 0x518
			 0x4218947f, /// 0x51c
			 0xfb672eb1, /// 0x520
			 0xbb1e14ef, /// 0x524
			 0xa0f6cae8, /// 0x528
			 0x748a9b03, /// 0x52c
			 0x7728755f, /// 0x530
			 0x573f9ba3, /// 0x534
			 0x3018d231, /// 0x538
			 0x87b64348, /// 0x53c
			 0x56b55e29, /// 0x540
			 0x4f82aa8d, /// 0x544
			 0x9e069920, /// 0x548
			 0xaf0ac641, /// 0x54c
			 0xfc9e46cf, /// 0x550
			 0x4251fb62, /// 0x554
			 0x795dd771, /// 0x558
			 0xcff67f66, /// 0x55c
			 0xb2aea368, /// 0x560
			 0x3525ee43, /// 0x564
			 0xbc5fb806, /// 0x568
			 0xb4f60cc7, /// 0x56c
			 0x7c0b390b, /// 0x570
			 0x37a9661f, /// 0x574
			 0xe7eacd47, /// 0x578
			 0x481f673e, /// 0x57c
			 0x3d7b4765, /// 0x580
			 0x84057790, /// 0x584
			 0xf5d4c753, /// 0x588
			 0xfb31de41, /// 0x58c
			 0xb33a5034, /// 0x590
			 0xafe6416f, /// 0x594
			 0xfe48d498, /// 0x598
			 0x1dd960a6, /// 0x59c
			 0xe2326357, /// 0x5a0
			 0xecb8dfcd, /// 0x5a4
			 0xa182883a, /// 0x5a8
			 0x3a1e600f, /// 0x5ac
			 0xb94cf99b, /// 0x5b0
			 0xe4409c00, /// 0x5b4
			 0xa8bf9d48, /// 0x5b8
			 0xb83086e4, /// 0x5bc
			 0x1be1df6a, /// 0x5c0
			 0x527a72b4, /// 0x5c4
			 0xb0e85c70, /// 0x5c8
			 0x53833ce3, /// 0x5cc
			 0xa3f371ae, /// 0x5d0
			 0xe6f0cbb6, /// 0x5d4
			 0xd4c6ed17, /// 0x5d8
			 0x0c45bfca, /// 0x5dc
			 0xff4efebf, /// 0x5e0
			 0x3df75736, /// 0x5e4
			 0x6339c993, /// 0x5e8
			 0x94ec14f4, /// 0x5ec
			 0x5c22a2b7, /// 0x5f0
			 0x52e715af, /// 0x5f4
			 0xf3ec6381, /// 0x5f8
			 0x2bd45f40, /// 0x5fc
			 0x8bcedc30, /// 0x600
			 0xf9283be0, /// 0x604
			 0x264134f4, /// 0x608
			 0xf1bb05de, /// 0x60c
			 0xbaba566d, /// 0x610
			 0xd9c80967, /// 0x614
			 0x63cc4214, /// 0x618
			 0xf77674d9, /// 0x61c
			 0x30dc0d36, /// 0x620
			 0xf5e0acfd, /// 0x624
			 0xab02efcd, /// 0x628
			 0x32cc09c8, /// 0x62c
			 0xc9260642, /// 0x630
			 0xa556e7ed, /// 0x634
			 0x032bf94b, /// 0x638
			 0x73a7fc01, /// 0x63c
			 0x103c8531, /// 0x640
			 0xbd73fcbc, /// 0x644
			 0xcb917a80, /// 0x648
			 0xf8c336cf, /// 0x64c
			 0x513d385e, /// 0x650
			 0x2104e198, /// 0x654
			 0x9a57d427, /// 0x658
			 0xa7faea11, /// 0x65c
			 0x259207b1, /// 0x660
			 0x9d34bf42, /// 0x664
			 0x4e126efa, /// 0x668
			 0x57ba090e, /// 0x66c
			 0x333e3489, /// 0x670
			 0x2eb77361, /// 0x674
			 0x46385567, /// 0x678
			 0x38dd8d21, /// 0x67c
			 0x47a424db, /// 0x680
			 0xe473184f, /// 0x684
			 0xe955b00e, /// 0x688
			 0x467a457c, /// 0x68c
			 0xea0d8bc1, /// 0x690
			 0x581da458, /// 0x694
			 0xd220cdaa, /// 0x698
			 0x4071643b, /// 0x69c
			 0x279d6997, /// 0x6a0
			 0xfeadc53f, /// 0x6a4
			 0x2d2d6df8, /// 0x6a8
			 0xbde828f2, /// 0x6ac
			 0xe13a5971, /// 0x6b0
			 0xde08bb9e, /// 0x6b4
			 0x391472ab, /// 0x6b8
			 0xd9bf35ea, /// 0x6bc
			 0x7758dffa, /// 0x6c0
			 0x6ec73cc3, /// 0x6c4
			 0x01f7e451, /// 0x6c8
			 0x3eb0194e, /// 0x6cc
			 0x3af82f4a, /// 0x6d0
			 0x6f2bb75b, /// 0x6d4
			 0x7e8a98e9, /// 0x6d8
			 0xe3fc24cc, /// 0x6dc
			 0xe26c185c, /// 0x6e0
			 0x532b72a1, /// 0x6e4
			 0x94d3338e, /// 0x6e8
			 0x5c15f51e, /// 0x6ec
			 0x24c459bc, /// 0x6f0
			 0xdb4f8a53, /// 0x6f4
			 0xa2292dde, /// 0x6f8
			 0x65385e8f, /// 0x6fc
			 0x2ed0d5b7, /// 0x700
			 0xf88e06e7, /// 0x704
			 0x8eae7512, /// 0x708
			 0xf3edb803, /// 0x70c
			 0xa63f1d78, /// 0x710
			 0x57b836cb, /// 0x714
			 0x63b32548, /// 0x718
			 0x8027182c, /// 0x71c
			 0xc9ae0158, /// 0x720
			 0x0678395b, /// 0x724
			 0x24e8d228, /// 0x728
			 0x05b2cc85, /// 0x72c
			 0xb6035068, /// 0x730
			 0xab674e05, /// 0x734
			 0x1095622d, /// 0x738
			 0x1f9f6dc6, /// 0x73c
			 0xa7764e99, /// 0x740
			 0xf5345649, /// 0x744
			 0xee0a94da, /// 0x748
			 0x91fc974a, /// 0x74c
			 0x0db90b33, /// 0x750
			 0x48269ecd, /// 0x754
			 0x63d2c2e4, /// 0x758
			 0xc0b089ab, /// 0x75c
			 0x213cd5e0, /// 0x760
			 0x92ea70d4, /// 0x764
			 0x0c1b291a, /// 0x768
			 0x9cc3b263, /// 0x76c
			 0x9767eda6, /// 0x770
			 0xf09c52ea, /// 0x774
			 0x4d396a1a, /// 0x778
			 0x0bf31562, /// 0x77c
			 0x9dff7a23, /// 0x780
			 0xffbbc960, /// 0x784
			 0x23f72a4b, /// 0x788
			 0xc363048e, /// 0x78c
			 0xbcf915f6, /// 0x790
			 0xaf1fa571, /// 0x794
			 0x6e293a40, /// 0x798
			 0x29e28ac9, /// 0x79c
			 0xc77b6d9a, /// 0x7a0
			 0x35ead199, /// 0x7a4
			 0xd9b06aa5, /// 0x7a8
			 0x051c2c91, /// 0x7ac
			 0x22ee49e0, /// 0x7b0
			 0x9ae0833a, /// 0x7b4
			 0xdf528d39, /// 0x7b8
			 0x1c9fbc25, /// 0x7bc
			 0x04aae116, /// 0x7c0
			 0x5f02dac3, /// 0x7c4
			 0x637dc05e, /// 0x7c8
			 0x73f09b73, /// 0x7cc
			 0x6b41e5de, /// 0x7d0
			 0x6e8c4ba2, /// 0x7d4
			 0x6f6b66e9, /// 0x7d8
			 0xe2f15317, /// 0x7dc
			 0xff53eb01, /// 0x7e0
			 0xe2ec599b, /// 0x7e4
			 0x6e9c72b3, /// 0x7e8
			 0x24efa722, /// 0x7ec
			 0x75b28b7b, /// 0x7f0
			 0x23d6a8b3, /// 0x7f4
			 0x468dfdbf, /// 0x7f8
			 0x436ce255, /// 0x7fc
			 0x34609524, /// 0x800
			 0x7c4d0aff, /// 0x804
			 0xe4e9bbde, /// 0x808
			 0xff53a856, /// 0x80c
			 0xe80eed59, /// 0x810
			 0xad010a24, /// 0x814
			 0x4cbe9442, /// 0x818
			 0x66709adf, /// 0x81c
			 0x12690e11, /// 0x820
			 0xa30fb0f8, /// 0x824
			 0x7ad56fbb, /// 0x828
			 0xe385732a, /// 0x82c
			 0xc7b1e6d4, /// 0x830
			 0x0168b2b4, /// 0x834
			 0x3f2f2a02, /// 0x838
			 0x1a5365ab, /// 0x83c
			 0x0339605b, /// 0x840
			 0xe4e346a6, /// 0x844
			 0x9c20b819, /// 0x848
			 0xd7e4f25c, /// 0x84c
			 0x9efe242a, /// 0x850
			 0x8e70bccf, /// 0x854
			 0x4a2212d2, /// 0x858
			 0xa93433ee, /// 0x85c
			 0x02a15ce0, /// 0x860
			 0x9606dff0, /// 0x864
			 0x5e18bfde, /// 0x868
			 0xaa1e0b0d, /// 0x86c
			 0x2a7f8677, /// 0x870
			 0xa39b31b8, /// 0x874
			 0x8cfbaca9, /// 0x878
			 0xb599edff, /// 0x87c
			 0xa266c30f, /// 0x880
			 0xabc6010e, /// 0x884
			 0x60c0236c, /// 0x888
			 0x337b83b4, /// 0x88c
			 0x4cb13653, /// 0x890
			 0x89166358, /// 0x894
			 0xcfa72a2e, /// 0x898
			 0xabdb988b, /// 0x89c
			 0xbde172da, /// 0x8a0
			 0x91263958, /// 0x8a4
			 0x73d07b7a, /// 0x8a8
			 0x7ad74795, /// 0x8ac
			 0x4246263e, /// 0x8b0
			 0x8654200f, /// 0x8b4
			 0x7eca2d04, /// 0x8b8
			 0xb54b72d5, /// 0x8bc
			 0x727e08c8, /// 0x8c0
			 0xe406e548, /// 0x8c4
			 0xf5089e0d, /// 0x8c8
			 0x9fec50fd, /// 0x8cc
			 0xa00821b5, /// 0x8d0
			 0x279654c1, /// 0x8d4
			 0xa0868e38, /// 0x8d8
			 0x72187243, /// 0x8dc
			 0x6911bf6a, /// 0x8e0
			 0xeeab6ed1, /// 0x8e4
			 0x67384c00, /// 0x8e8
			 0x708ab919, /// 0x8ec
			 0x337afede, /// 0x8f0
			 0x36680c7d, /// 0x8f4
			 0xe2c1523a, /// 0x8f8
			 0xa571db94, /// 0x8fc
			 0x4c91ace4, /// 0x900
			 0xfb16b21c, /// 0x904
			 0x1f8f9bb8, /// 0x908
			 0x2daaf38f, /// 0x90c
			 0x9ee168ca, /// 0x910
			 0x63e3eaad, /// 0x914
			 0xc9671a51, /// 0x918
			 0x388dd937, /// 0x91c
			 0xbe6a3165, /// 0x920
			 0xf3e6863f, /// 0x924
			 0x6ccbb47d, /// 0x928
			 0xd210c5fb, /// 0x92c
			 0x0275878d, /// 0x930
			 0x21a66590, /// 0x934
			 0xa85e37c4, /// 0x938
			 0xc04c5859, /// 0x93c
			 0x54514fc8, /// 0x940
			 0x296336b8, /// 0x944
			 0xfa7cf47a, /// 0x948
			 0x2d64ed22, /// 0x94c
			 0xc7bf3881, /// 0x950
			 0x54fd667e, /// 0x954
			 0x56dc4a5f, /// 0x958
			 0x82d3ac5e, /// 0x95c
			 0xb98cba49, /// 0x960
			 0xbdf7d92e, /// 0x964
			 0x0b09c957, /// 0x968
			 0x93043260, /// 0x96c
			 0xb0e8f3e3, /// 0x970
			 0xe6921a77, /// 0x974
			 0xe53ec53b, /// 0x978
			 0x94188754, /// 0x97c
			 0x6b1cef32, /// 0x980
			 0x2e0a6c1b, /// 0x984
			 0x70242e06, /// 0x988
			 0xa0ea5b76, /// 0x98c
			 0xfdb81540, /// 0x990
			 0xdfd93e24, /// 0x994
			 0xe8adfa36, /// 0x998
			 0x9cdf26bb, /// 0x99c
			 0x09323ac2, /// 0x9a0
			 0x2e117ffc, /// 0x9a4
			 0x51c1c04d, /// 0x9a8
			 0x54f2e429, /// 0x9ac
			 0xb4fbb49a, /// 0x9b0
			 0x375ed1ba, /// 0x9b4
			 0xd76977d9, /// 0x9b8
			 0xc60c5707, /// 0x9bc
			 0x1bbbba36, /// 0x9c0
			 0x67e7de7c, /// 0x9c4
			 0x60a0736e, /// 0x9c8
			 0x3d91422f, /// 0x9cc
			 0x3057eb91, /// 0x9d0
			 0x7cb98c02, /// 0x9d4
			 0x8d8d6e76, /// 0x9d8
			 0xbdfef2f4, /// 0x9dc
			 0x99414cc8, /// 0x9e0
			 0x42ffc192, /// 0x9e4
			 0xe19a081b, /// 0x9e8
			 0x6449a1b1, /// 0x9ec
			 0xdb158a8c, /// 0x9f0
			 0x193f47a8, /// 0x9f4
			 0xe5815103, /// 0x9f8
			 0xe6827b35, /// 0x9fc
			 0x4352c0fd, /// 0xa00
			 0x86877e9e, /// 0xa04
			 0xc748fd36, /// 0xa08
			 0x9e819f9e, /// 0xa0c
			 0x9379081a, /// 0xa10
			 0xa2fb7eed, /// 0xa14
			 0xf4e49381, /// 0xa18
			 0xfa6eeb54, /// 0xa1c
			 0x9901c4e1, /// 0xa20
			 0xe9314dc5, /// 0xa24
			 0xfec4d614, /// 0xa28
			 0x72d6afe5, /// 0xa2c
			 0x9335a9dc, /// 0xa30
			 0xbaa8dd85, /// 0xa34
			 0x6192e5b4, /// 0xa38
			 0x24a57600, /// 0xa3c
			 0xdb6d7ee6, /// 0xa40
			 0xcc38b0ec, /// 0xa44
			 0x1eb64ee3, /// 0xa48
			 0x56f8b5a3, /// 0xa4c
			 0x904d925a, /// 0xa50
			 0x98b0d80d, /// 0xa54
			 0xe3055b04, /// 0xa58
			 0xc82e196b, /// 0xa5c
			 0x4cf33e21, /// 0xa60
			 0xa4f8761e, /// 0xa64
			 0x5d3c7762, /// 0xa68
			 0xa84a1e2d, /// 0xa6c
			 0x348a4826, /// 0xa70
			 0xdca20c0c, /// 0xa74
			 0x47bc6fdb, /// 0xa78
			 0x668318ac, /// 0xa7c
			 0xc6ef062a, /// 0xa80
			 0x00288ff3, /// 0xa84
			 0x75da428a, /// 0xa88
			 0x999be931, /// 0xa8c
			 0x1ff41438, /// 0xa90
			 0x318a318b, /// 0xa94
			 0x9883e883, /// 0xa98
			 0x4f92b731, /// 0xa9c
			 0x95aaece1, /// 0xaa0
			 0x556960fa, /// 0xaa4
			 0x00f68329, /// 0xaa8
			 0x979ac388, /// 0xaac
			 0x4bd83e9e, /// 0xab0
			 0x091974c8, /// 0xab4
			 0x59b7fdf2, /// 0xab8
			 0x5f9265f1, /// 0xabc
			 0xc11faf50, /// 0xac0
			 0x332e32e3, /// 0xac4
			 0x11bf1063, /// 0xac8
			 0xed30523e, /// 0xacc
			 0x46c028f3, /// 0xad0
			 0x2fbaaed4, /// 0xad4
			 0xee431158, /// 0xad8
			 0x765d8c25, /// 0xadc
			 0x48c17d32, /// 0xae0
			 0x5cf4609f, /// 0xae4
			 0xee85775a, /// 0xae8
			 0xffb0f168, /// 0xaec
			 0x9909f7b2, /// 0xaf0
			 0xdfc59307, /// 0xaf4
			 0x5dab1d80, /// 0xaf8
			 0x2d58b865, /// 0xafc
			 0x17e3a63e, /// 0xb00
			 0x72b65277, /// 0xb04
			 0x3993966a, /// 0xb08
			 0x9967ca37, /// 0xb0c
			 0x4ea5095c, /// 0xb10
			 0xc5a734fc, /// 0xb14
			 0x128bbceb, /// 0xb18
			 0xd1d44396, /// 0xb1c
			 0xa5cb5636, /// 0xb20
			 0x7b9b5308, /// 0xb24
			 0x961253f2, /// 0xb28
			 0x4bc1ac47, /// 0xb2c
			 0x9c9865d4, /// 0xb30
			 0x6f447cdc, /// 0xb34
			 0x0f7dd2fb, /// 0xb38
			 0x507e2567, /// 0xb3c
			 0xdefbe0f9, /// 0xb40
			 0x142a8340, /// 0xb44
			 0x466755d5, /// 0xb48
			 0x9a506900, /// 0xb4c
			 0x02477ee9, /// 0xb50
			 0x59254742, /// 0xb54
			 0x55379ab0, /// 0xb58
			 0xdda5479c, /// 0xb5c
			 0x4ba92847, /// 0xb60
			 0x0280383b, /// 0xb64
			 0x208bb8de, /// 0xb68
			 0xdf002809, /// 0xb6c
			 0x2800ecfc, /// 0xb70
			 0x7e530372, /// 0xb74
			 0x39ef6d3b, /// 0xb78
			 0xa12e85c8, /// 0xb7c
			 0xf9be7706, /// 0xb80
			 0x0b71d286, /// 0xb84
			 0xb0f5a3a4, /// 0xb88
			 0x54a7db43, /// 0xb8c
			 0x30b349b8, /// 0xb90
			 0x70852b93, /// 0xb94
			 0xe83f0c3a, /// 0xb98
			 0xaf63bd12, /// 0xb9c
			 0xe17b6f64, /// 0xba0
			 0x8f7b860e, /// 0xba4
			 0x718ef400, /// 0xba8
			 0x7be0ac51, /// 0xbac
			 0xe3eb2d80, /// 0xbb0
			 0x0a373004, /// 0xbb4
			 0xf8746f27, /// 0xbb8
			 0xafd39e2c, /// 0xbbc
			 0x53d6f143, /// 0xbc0
			 0x299739ae, /// 0xbc4
			 0xed12c088, /// 0xbc8
			 0xd4b22c47, /// 0xbcc
			 0x0c63a502, /// 0xbd0
			 0x416f973b, /// 0xbd4
			 0xddbcba81, /// 0xbd8
			 0x6dad51c3, /// 0xbdc
			 0xfc03feaf, /// 0xbe0
			 0x64115a73, /// 0xbe4
			 0x7d834a5b, /// 0xbe8
			 0x3442a649, /// 0xbec
			 0x4e46a8b6, /// 0xbf0
			 0x9bdc066f, /// 0xbf4
			 0x30fca33a, /// 0xbf8
			 0xe256340b, /// 0xbfc
			 0x6224381a, /// 0xc00
			 0xa5cfd7af, /// 0xc04
			 0x22c5a129, /// 0xc08
			 0x7301af18, /// 0xc0c
			 0xb9581f04, /// 0xc10
			 0x0f9c8540, /// 0xc14
			 0x6110fd88, /// 0xc18
			 0xdb45ff90, /// 0xc1c
			 0xe800fb2a, /// 0xc20
			 0xc5b328cc, /// 0xc24
			 0xb5b6d5b4, /// 0xc28
			 0x3115a828, /// 0xc2c
			 0x32f31fe7, /// 0xc30
			 0x8446200a, /// 0xc34
			 0x2d4946f1, /// 0xc38
			 0x38c27f9b, /// 0xc3c
			 0xc4b65c76, /// 0xc40
			 0x5d7a7578, /// 0xc44
			 0x6a5354d7, /// 0xc48
			 0xfb4ce797, /// 0xc4c
			 0xe13f0919, /// 0xc50
			 0xef2dc8a2, /// 0xc54
			 0x882be89c, /// 0xc58
			 0x29b05a78, /// 0xc5c
			 0x38812cae, /// 0xc60
			 0xad148ea5, /// 0xc64
			 0x79cb5e85, /// 0xc68
			 0x825b6a68, /// 0xc6c
			 0x87c7a210, /// 0xc70
			 0x962d48ef, /// 0xc74
			 0x43054971, /// 0xc78
			 0xfc8d9f6a, /// 0xc7c
			 0x0b524007, /// 0xc80
			 0xcffee492, /// 0xc84
			 0x102ba8ac, /// 0xc88
			 0xe4bcdd64, /// 0xc8c
			 0x24a6faef, /// 0xc90
			 0x0473acf0, /// 0xc94
			 0x72d6d46c, /// 0xc98
			 0x1a874b44, /// 0xc9c
			 0xad6c029c, /// 0xca0
			 0x4991f1ac, /// 0xca4
			 0xe0381f72, /// 0xca8
			 0xa09b0ea5, /// 0xcac
			 0x51bd68e4, /// 0xcb0
			 0xbf86dee2, /// 0xcb4
			 0xb209c47d, /// 0xcb8
			 0x0829e7a8, /// 0xcbc
			 0x8bdd0568, /// 0xcc0
			 0xaa3d5af7, /// 0xcc4
			 0xfb16c78b, /// 0xcc8
			 0x15659179, /// 0xccc
			 0x25652091, /// 0xcd0
			 0x5fb3b38a, /// 0xcd4
			 0x5cfc2638, /// 0xcd8
			 0xd1b0990c, /// 0xcdc
			 0x8f3e7887, /// 0xce0
			 0xd5c6030e, /// 0xce4
			 0x4381dd35, /// 0xce8
			 0x6a151332, /// 0xcec
			 0x9f0ea5e5, /// 0xcf0
			 0x5fd3e032, /// 0xcf4
			 0x62a4ac08, /// 0xcf8
			 0x31cf5ecd, /// 0xcfc
			 0xd40a2d7a, /// 0xd00
			 0xab145175, /// 0xd04
			 0xfab9a9b1, /// 0xd08
			 0x8826b114, /// 0xd0c
			 0x2a02950c, /// 0xd10
			 0xaa954577, /// 0xd14
			 0xe9395c9d, /// 0xd18
			 0xd77f2894, /// 0xd1c
			 0x9b10d263, /// 0xd20
			 0x02e532e9, /// 0xd24
			 0xb36fba54, /// 0xd28
			 0xcaf9d5da, /// 0xd2c
			 0xf31e3d93, /// 0xd30
			 0x72336c2c, /// 0xd34
			 0x0774b8ab, /// 0xd38
			 0x01a1d547, /// 0xd3c
			 0x2a8893ae, /// 0xd40
			 0x7e626a9b, /// 0xd44
			 0x3a19c16e, /// 0xd48
			 0x9eac9026, /// 0xd4c
			 0xd86a3605, /// 0xd50
			 0x85e3e62d, /// 0xd54
			 0x5c37127f, /// 0xd58
			 0x632cac7a, /// 0xd5c
			 0xf5a5a025, /// 0xd60
			 0x94b52f81, /// 0xd64
			 0x56e21c36, /// 0xd68
			 0x4eac7ea4, /// 0xd6c
			 0xbce95ecf, /// 0xd70
			 0xd7e0f236, /// 0xd74
			 0x6ff87a86, /// 0xd78
			 0x53363261, /// 0xd7c
			 0x5647adb3, /// 0xd80
			 0xcbcb9f18, /// 0xd84
			 0x0a6f206d, /// 0xd88
			 0x6b6568de, /// 0xd8c
			 0xea1a0dc2, /// 0xd90
			 0xccd72806, /// 0xd94
			 0xac0204ba, /// 0xd98
			 0x46f2dba0, /// 0xd9c
			 0x48cb4275, /// 0xda0
			 0x89e70f2c, /// 0xda4
			 0x49108ff1, /// 0xda8
			 0x9f7203cb, /// 0xdac
			 0x1ef58ff6, /// 0xdb0
			 0xae84c3ee, /// 0xdb4
			 0xea069214, /// 0xdb8
			 0xf99e2f35, /// 0xdbc
			 0xc48fd39f, /// 0xdc0
			 0x343b2972, /// 0xdc4
			 0x957e45ab, /// 0xdc8
			 0xdd9783dc, /// 0xdcc
			 0x6984b7f9, /// 0xdd0
			 0xc610b9d4, /// 0xdd4
			 0xc41c2e5f, /// 0xdd8
			 0x59f60e88, /// 0xddc
			 0xc522c2c1, /// 0xde0
			 0xeb6a90fc, /// 0xde4
			 0x3db77756, /// 0xde8
			 0x1a677237, /// 0xdec
			 0x70048142, /// 0xdf0
			 0xd76f4949, /// 0xdf4
			 0x89fe83b3, /// 0xdf8
			 0x7b704104, /// 0xdfc
			 0xc35b3a87, /// 0xe00
			 0xeb97ee18, /// 0xe04
			 0x5382ca30, /// 0xe08
			 0x74548785, /// 0xe0c
			 0x150bbb5c, /// 0xe10
			 0xb79115f7, /// 0xe14
			 0x3a0b24c8, /// 0xe18
			 0x0fbacf32, /// 0xe1c
			 0x7d8a584b, /// 0xe20
			 0x59ada2b0, /// 0xe24
			 0xe3b69ef4, /// 0xe28
			 0x1652d54f, /// 0xe2c
			 0x1ca3e437, /// 0xe30
			 0x3f96b623, /// 0xe34
			 0x25f63efa, /// 0xe38
			 0xb4a91bb2, /// 0xe3c
			 0x3899d234, /// 0xe40
			 0x2598c24c, /// 0xe44
			 0xad851131, /// 0xe48
			 0x3930f02a, /// 0xe4c
			 0xf987b7f6, /// 0xe50
			 0xae813960, /// 0xe54
			 0x3ad9d89d, /// 0xe58
			 0x9a7d7865, /// 0xe5c
			 0x1220d1f0, /// 0xe60
			 0x42ad5899, /// 0xe64
			 0xce2dcdd4, /// 0xe68
			 0x6ee39149, /// 0xe6c
			 0xd97fe0a0, /// 0xe70
			 0xdeaf0303, /// 0xe74
			 0x99ff987b, /// 0xe78
			 0x8e7b3444, /// 0xe7c
			 0xeff0970f, /// 0xe80
			 0x912d068a, /// 0xe84
			 0xcb1dbed2, /// 0xe88
			 0x69c30c20, /// 0xe8c
			 0x9cb28ce8, /// 0xe90
			 0x15c7fda2, /// 0xe94
			 0x43b99e5f, /// 0xe98
			 0x9942a5b1, /// 0xe9c
			 0x83cc88ce, /// 0xea0
			 0xa04adc22, /// 0xea4
			 0x8657a56b, /// 0xea8
			 0x3b952d01, /// 0xeac
			 0x1a2673de, /// 0xeb0
			 0x7dc60f30, /// 0xeb4
			 0x49f2d05c, /// 0xeb8
			 0xc73d2c05, /// 0xebc
			 0x8100e084, /// 0xec0
			 0x8f5df9d1, /// 0xec4
			 0x670720b4, /// 0xec8
			 0x1824c7d4, /// 0xecc
			 0xbcb98f52, /// 0xed0
			 0x66d5a776, /// 0xed4
			 0xa761f799, /// 0xed8
			 0x04053b19, /// 0xedc
			 0xd7f6c1b2, /// 0xee0
			 0x6a5ac74d, /// 0xee4
			 0x010d11c0, /// 0xee8
			 0x781f15d5, /// 0xeec
			 0xfe4a65d4, /// 0xef0
			 0x46560161, /// 0xef4
			 0x06869a53, /// 0xef8
			 0xa3c89d13, /// 0xefc
			 0xfb65a93a, /// 0xf00
			 0xa84f7e02, /// 0xf04
			 0x1388ede2, /// 0xf08
			 0xd8b99270, /// 0xf0c
			 0x4e89db98, /// 0xf10
			 0xe91a7106, /// 0xf14
			 0x1bf90211, /// 0xf18
			 0xabf2b54d, /// 0xf1c
			 0xaf9b291f, /// 0xf20
			 0xc5a35303, /// 0xf24
			 0x90c75d70, /// 0xf28
			 0xaa112f88, /// 0xf2c
			 0x66daa55b, /// 0xf30
			 0xcbf26d31, /// 0xf34
			 0xfbc4536a, /// 0xf38
			 0xf3e03b61, /// 0xf3c
			 0x5fe08cd4, /// 0xf40
			 0xe3310fb9, /// 0xf44
			 0xe39f32db, /// 0xf48
			 0x5d3e8a53, /// 0xf4c
			 0x52af0552, /// 0xf50
			 0x2b4cd84a, /// 0xf54
			 0xcda67137, /// 0xf58
			 0x8402953c, /// 0xf5c
			 0x76af22fd, /// 0xf60
			 0xedecf43c, /// 0xf64
			 0xae1adb50, /// 0xf68
			 0x9cf57016, /// 0xf6c
			 0x85004142, /// 0xf70
			 0x7255efb6, /// 0xf74
			 0xe9e35a12, /// 0xf78
			 0xa9b96518, /// 0xf7c
			 0x7c8f2ba4, /// 0xf80
			 0xb9acf36b, /// 0xf84
			 0xdd1b7109, /// 0xf88
			 0x700de73c, /// 0xf8c
			 0xdb61865b, /// 0xf90
			 0x548973b9, /// 0xf94
			 0x21dc14f6, /// 0xf98
			 0xeffc0f75, /// 0xf9c
			 0xe37afb10, /// 0xfa0
			 0xd201a85a, /// 0xfa4
			 0x512310d8, /// 0xfa8
			 0xf6040bce, /// 0xfac
			 0x11fbad46, /// 0xfb0
			 0x1d307997, /// 0xfb4
			 0x8c760b08, /// 0xfb8
			 0x6388e152, /// 0xfbc
			 0xdf8f4c3f, /// 0xfc0
			 0x048d94bd, /// 0xfc4
			 0x983c077b, /// 0xfc8
			 0x76bd4c33, /// 0xfcc
			 0xab7bc6d1, /// 0xfd0
			 0x6b8d16fa, /// 0xfd4
			 0xce9891ff, /// 0xfd8
			 0x097e5a56, /// 0xfdc
			 0xa81b3449, /// 0xfe0
			 0x59c24d81, /// 0xfe4
			 0xe391968a, /// 0xfe8
			 0x95e7f4b3, /// 0xfec
			 0x9179b891, /// 0xff0
			 0x85bbe783, /// 0xff4
			 0xa52d43e5, /// 0xff8
			 0xb663a1c7 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00000
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e000001,                                                  // Trailing 1s:                                /// 00008
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0000c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00018
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0001c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00020
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00024
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00028
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0002c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00030
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00040
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00048
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0004c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00050
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00054
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x0e000007,                                                  // Trailing 1s:                                /// 0005c
			 0xff800000,                                                  // -inf                                        /// 00060
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00068
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00070
			 0xcb000000,                                                  // -8388608.0                                  /// 00074
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00078
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0007c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00084
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0008c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0009c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x7f800000,                                                  // inf                                         /// 000b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00104
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0010c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00118
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0011c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00124
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00128
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0012c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0c780000,                                                  // Leading 1s:                                 /// 00134
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00138
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00144
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00148
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0014c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00154
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00158
			 0x55555555,                                                  // 4 random values                             /// 0015c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00160
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00168
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00170
			 0x7f800000,                                                  // inf                                         /// 00174
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0017c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e000007,                                                  // Trailing 1s:                                /// 00184
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00188
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0018c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00194
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00198
			 0x4b000000,                                                  // 8388608.0                                   /// 0019c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00204
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00208
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0020c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00210
			 0x4b000000,                                                  // 8388608.0                                   /// 00214
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0021c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00220
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00228
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00230
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00234
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0023c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00240
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00244
			 0xbf028f5c,                                                  // -0.51                                       /// 00248
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0024c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00250
			 0x0c400000,                                                  // Leading 1s:                                 /// 00254
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00258
			 0x80000000,                                                  // -zero                                       /// 0025c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00260
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00268
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00270
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00278
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0027c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00280
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00284
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00288
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00290
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00294
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00298
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0029c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xbf028f5c,                                                  // -0.51                                       /// 002a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00304
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00318
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0031c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00324
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00328
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x7fc00001,                                                  // signaling NaN                               /// 00330
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00334
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00338
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00340
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00348
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00368
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00011111,                                                  // 9.7958E-41                                  /// 00374
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0037c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x33333333,                                                  // 4 random values                             /// 00384
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0038c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00390
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00394
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0039c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x33333333,                                                  // 4 random values                             /// 003c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003e8
			 0x00000000,                                                  // zero                                        /// 003ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00400
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00408
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0040c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00414
			 0x00011111,                                                  // 9.7958E-41                                  /// 00418
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00420
			 0x0e000007,                                                  // Trailing 1s:                                /// 00424
			 0x0c600000,                                                  // Leading 1s:                                 /// 00428
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00430
			 0x0c400000,                                                  // Leading 1s:                                 /// 00434
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00438
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00440
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00444
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00448
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0044c
			 0xbf028f5c,                                                  // -0.51                                       /// 00450
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00458
			 0x0e000001,                                                  // Trailing 1s:                                /// 0045c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00460
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00478
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0047c
			 0x33333333,                                                  // 4 random values                             /// 00480
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00488
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0048c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00490
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00494
			 0x55555555,                                                  // 4 random values                             /// 00498
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0049c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x3f028f5c,                                                  // 0.51                                        /// 004c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004cc
			 0x00000000,                                                  // zero                                        /// 004d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00500
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00504
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00508
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0050c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00510
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0xff800000,                                                  // -inf                                        /// 00518
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00524
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00528
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0052c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00534
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x33333333,                                                  // 4 random values                             /// 0053c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00540
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00548
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00550
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00560
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00564
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00568
			 0x0c700000,                                                  // Leading 1s:                                 /// 0056c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00574
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0057c
			 0x80000000,                                                  // -zero                                       /// 00580
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0058c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00590
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00594
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00598
			 0x0c780000,                                                  // Leading 1s:                                 /// 0059c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005d8
			 0xff800000,                                                  // -inf                                        /// 005dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00604
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00608
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00610
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00614
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00618
			 0xbf028f5c,                                                  // -0.51                                       /// 0061c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00628
			 0xbf028f5c,                                                  // -0.51                                       /// 0062c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00630
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00638
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0063c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00640
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00648
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00650
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00654
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00658
			 0x00000000,                                                  // zero                                        /// 0065c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x0c600000,                                                  // Leading 1s:                                 /// 00664
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00668
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0066c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00670
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00674
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0067c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00680
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00684
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0068c
			 0xff800000,                                                  // -inf                                        /// 00690
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00698
			 0x4b000000,                                                  // 8388608.0                                   /// 0069c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00700
			 0x0c600000,                                                  // Leading 1s:                                 /// 00704
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00708
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0070c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00714
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00718
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00728
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0072c
			 0x7f800000,                                                  // inf                                         /// 00730
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00734
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00738
			 0x0c700000,                                                  // Leading 1s:                                 /// 0073c
			 0x7fc00001,                                                  // signaling NaN                               /// 00740
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00744
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00748
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00750
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00758
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0075c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00760
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00768
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0076c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00770
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00774
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00778
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0077c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x3f028f5c,                                                  // 0.51                                        /// 00788
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00790
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00794
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e000007,                                                  // Trailing 1s:                                /// 0079c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x7f800000,                                                  // inf                                         /// 007b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007bc
			 0x33333333,                                                  // 4 random values                             /// 007c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007d4
			 0xbf028f5c,                                                  // -0.51                                       /// 007d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00800
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00804
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00808
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0080c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00810
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00818
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0081c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0082c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00838
			 0xcb000000,                                                  // -8388608.0                                  /// 0083c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00844
			 0xcb000000,                                                  // -8388608.0                                  /// 00848
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0084c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00850
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00854
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00858
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0085c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00860
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00864
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00868
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00870
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00874
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0xff800000,                                                  // -inf                                        /// 0087c
			 0x33333333,                                                  // 4 random values                             /// 00880
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00884
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00888
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0088c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00890
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00894
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0089c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00904
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00908
			 0x0c780000,                                                  // Leading 1s:                                 /// 0090c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00914
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0092c
			 0x7f800000,                                                  // inf                                         /// 00930
			 0x33333333,                                                  // 4 random values                             /// 00934
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0093c
			 0x80000000,                                                  // -zero                                       /// 00940
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00944
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00948
			 0x7f800000,                                                  // inf                                         /// 0094c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00950
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00958
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0095c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00960
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00964
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0096c
			 0xbf028f5c,                                                  // -0.51                                       /// 00970
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00978
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0097c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00980
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00984
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00988
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00990
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00994
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x4b000000,                                                  // 8388608.0                                   /// 009a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a38
			 0x55555555,                                                  // 4 random values                             /// 00a3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aa0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ab4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ab8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ac0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ac4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x7f800000,                                                  // inf                                         /// 00ad0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ad4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ae0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00afc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x55555555,                                                  // 4 random values                             /// 00b28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80000000,                                                  // -zero                                       /// 00b30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x00000000,                                                  // zero                                        /// 00b78
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00be8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bfc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c10
			 0x33333333,                                                  // 4 random values                             /// 00c14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c50
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0xcb000000,                                                  // -8388608.0                                  /// 00c58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c74
			 0x3f028f5c,                                                  // 0.51                                        /// 00c78
			 0xcb000000,                                                  // -8388608.0                                  /// 00c7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ca8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cdc
			 0x55555555,                                                  // 4 random values                             /// 00ce0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cf0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0xff800000,                                                  // -inf                                        /// 00cf8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d04
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000000,                                                  // zero                                        /// 00d30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d54
			 0xbf028f5c,                                                  // -0.51                                       /// 00d58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000000,                                                  // zero                                        /// 00d60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d74
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x33333333,                                                  // 4 random values                             /// 00d84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00db4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00de0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00de8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00df4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00df8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dfc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00000000,                                                  // zero                                        /// 00e18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e24
			 0x3f028f5c,                                                  // 0.51                                        /// 00e28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e74
			 0x33333333,                                                  // 4 random values                             /// 00e78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eb4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eb8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ebc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000000,                                                  // -zero                                       /// 00ec8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ed8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00edc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ee4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ef4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ef8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f38
			 0x7fc00001,                                                  // signaling NaN                               /// 00f3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f5c
			 0x80000000,                                                  // -zero                                       /// 00f60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x7fc00001,                                                  // signaling NaN                               /// 00f74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f80
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f94
			 0xff800000,                                                  // -inf                                        /// 00f98
			 0x7f800000,                                                  // inf                                         /// 00f9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fa0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fa4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fcc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fd0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fe4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0xff7fffff // max norm                                      // max norm -ve                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00000
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00008
			 0xffc00001,                                                  // -signaling NaN                              /// 0000c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00014
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00020
			 0xffc00001,                                                  // -signaling NaN                              /// 00024
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00028
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0002c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00030
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00034
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0003c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00040
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00044
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff800000,                                                  // -inf                                        /// 0004c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00058
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00060
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00068
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00074
			 0x55555555,                                                  // 4 random values                             /// 00078
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00080
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00084
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0c400000,                                                  // Leading 1s:                                 /// 00094
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00098
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0009c
			 0x7f800000,                                                  // inf                                         /// 000a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000bc
			 0xffc00001,                                                  // -signaling NaN                              /// 000c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00100
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00108
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0010c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00114
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00120
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00124
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00128
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0012c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00130
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00134
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00138
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0013c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00140
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00144
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00148
			 0xff800000,                                                  // -inf                                        /// 0014c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00154
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00158
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00160
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0016c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0c780000,                                                  // Leading 1s:                                 /// 00174
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00178
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00000000,                                                  // zero                                        /// 00180
			 0x3f028f5c,                                                  // 0.51                                        /// 00184
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0018c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00190
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00198
			 0x0c780000,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e4
			 0x55555555,                                                  // 4 random values                             /// 001e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00000000,                                                  // zero                                        /// 001f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00200
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00204
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00208
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00210
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00214
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00218
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0021c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00220
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00224
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0022c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00230
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00238
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0023c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00244
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00248
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0024c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00250
			 0x4b000000,                                                  // 8388608.0                                   /// 00254
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00260
			 0x80011111,                                                  // -9.7958E-41                                 /// 00264
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00268
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0026c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00270
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00274
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00280
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00284
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00298
			 0x0e000003,                                                  // Trailing 1s:                                /// 0029c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 002b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00300
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00308
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0030c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00310
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00314
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00318
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0031c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00320
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00324
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00328
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0032c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00330
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0033c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00344
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00348
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0034c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x0c700000,                                                  // Leading 1s:                                 /// 00354
			 0xbf028f5c,                                                  // -0.51                                       /// 00358
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0035c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00360
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00364
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0036c
			 0xcb000000,                                                  // -8388608.0                                  /// 00370
			 0x80000000,                                                  // -zero                                       /// 00374
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0xff800000,                                                  // -inf                                        /// 00380
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00384
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0c780000,                                                  // Leading 1s:                                 /// 0038c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003a0
			 0x80000000,                                                  // -zero                                       /// 003a4
			 0x3f028f5c,                                                  // 0.51                                        /// 003a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0xffc00001,                                                  // -signaling NaN                              /// 003bc
			 0x33333333,                                                  // 4 random values                             /// 003c0
			 0xff800000,                                                  // -inf                                        /// 003c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d4
			 0xbf028f5c,                                                  // -0.51                                       /// 003d8
			 0x55555555,                                                  // 4 random values                             /// 003dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00000000,                                                  // zero                                        /// 003e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 003f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00404
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x0e000007,                                                  // Trailing 1s:                                /// 00418
			 0x0c400000,                                                  // Leading 1s:                                 /// 0041c
			 0x3f028f5c,                                                  // 0.51                                        /// 00420
			 0x00000000,                                                  // zero                                        /// 00424
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00428
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0042c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00430
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00444
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00448
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00454
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00458
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0045c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0c700000,                                                  // Leading 1s:                                 /// 00464
			 0xcb000000,                                                  // -8388608.0                                  /// 00468
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0046c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00470
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00478
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00480
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00484
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0048c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00490
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00494
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00498
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0049c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x3f028f5c,                                                  // 0.51                                        /// 004b8
			 0x7f800000,                                                  // inf                                         /// 004bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00500
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00508
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00510
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x3f028f5c,                                                  // 0.51                                        /// 00518
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00528
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0052c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00530
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00534
			 0x3f028f5c,                                                  // 0.51                                        /// 00538
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0053c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00544
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00550
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00554
			 0xcb000000,                                                  // -8388608.0                                  /// 00558
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0055c
			 0x4b000000,                                                  // 8388608.0                                   /// 00560
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00564
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00568
			 0xffc00001,                                                  // -signaling NaN                              /// 0056c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00570
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00578
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00580
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00584
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00588
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0058c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00590
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a8
			 0x00000000,                                                  // zero                                        /// 005ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0xff800000,                                                  // -inf                                        /// 005c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00000000,                                                  // zero                                        /// 005e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00600
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00618
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0061c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00624
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0e000003,                                                  // Trailing 1s:                                /// 0062c
			 0x00000000,                                                  // zero                                        /// 00630
			 0x7f800000,                                                  // inf                                         /// 00634
			 0x7fc00001,                                                  // signaling NaN                               /// 00638
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00640
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00644
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0e000007,                                                  // Trailing 1s:                                /// 00654
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00668
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00670
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00674
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x0c700000,                                                  // Leading 1s:                                 /// 00688
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0068c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00690
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00694
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00698
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0069c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006d0
			 0x55555555,                                                  // 4 random values                             /// 006d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00700
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00704
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0070c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e000001,                                                  // Trailing 1s:                                /// 00724
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00728
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00734
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00738
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0073c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00748
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0074c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00754
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00758
			 0x80000000,                                                  // -zero                                       /// 0075c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00760
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00764
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x33333333,                                                  // 4 random values                             /// 0076c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00770
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00774
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00784
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00788
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0078c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00794
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00798
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0079c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00800
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00804
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0080c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00810
			 0x0e000007,                                                  // Trailing 1s:                                /// 00814
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00818
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0081c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00820
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00828
			 0x4b000000,                                                  // 8388608.0                                   /// 0082c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00834
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00840
			 0x0e000003,                                                  // Trailing 1s:                                /// 00844
			 0x0e000003,                                                  // Trailing 1s:                                /// 00848
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0084c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00850
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00854
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0085c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00860
			 0x0e000003,                                                  // Trailing 1s:                                /// 00864
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00868
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0086c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00874
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x33333333,                                                  // 4 random values                             /// 0087c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00880
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00888
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00890
			 0xffc00001,                                                  // -signaling NaN                              /// 00894
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00898
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008a0
			 0x3f028f5c,                                                  // 0.51                                        /// 008a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00900
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00904
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00908
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00910
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00914
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00918
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00924
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00928
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00930
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00938
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00940
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00944
			 0x0e000003,                                                  // Trailing 1s:                                /// 00948
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0094c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00950
			 0x4b000000,                                                  // 8388608.0                                   /// 00954
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0095c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00964
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00968
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00970
			 0x0c700000,                                                  // Leading 1s:                                 /// 00974
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x3f028f5c,                                                  // 0.51                                        /// 0097c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00980
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00984
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00988
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0098c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00994
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00998
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009c8
			 0x3f028f5c,                                                  // 0.51                                        /// 009cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a24
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000000,                                                  // zero                                        /// 00a58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x55555555,                                                  // 4 random values                             /// 00a94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xbf028f5c,                                                  // -0.51                                       /// 00aa8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ab8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ac8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00acc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ae8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00af0
			 0x3f028f5c,                                                  // 0.51                                        /// 00af4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00af8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00afc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b10
			 0x00000000,                                                  // zero                                        /// 00b14
			 0x33333333,                                                  // 4 random values                             /// 00b18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ba4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ba8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00be4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00be8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bf0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bf4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x00000000,                                                  // zero                                        /// 00c54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c5c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c7c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca8
			 0x7f800000,                                                  // inf                                         /// 00cac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb8
			 0x33333333,                                                  // 4 random values                             /// 00cbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ccc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cd0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cd4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ce0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cf4
			 0xff800000,                                                  // -inf                                        /// 00cf8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d30
			 0x80000000,                                                  // -zero                                       /// 00d34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d94
			 0xcb000000,                                                  // -8388608.0                                  /// 00d98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da4
			 0x4b000000,                                                  // 8388608.0                                   /// 00da8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dac
			 0x33333333,                                                  // 4 random values                             /// 00db0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dc4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dd4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ddc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00de4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00df0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dfc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0xff800000,                                                  // -inf                                        /// 00e1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ebc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ec0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00edc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ee0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ee4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ef4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00efc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x80000000,                                                  // -zero                                       /// 00f10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f48
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0xffc00001,                                                  // -signaling NaN                              /// 00f58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fa8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fb4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fbc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fcc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fd4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ff4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0d00fff0                                                  // Set of 1s                                   /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xe9b08698, /// 0x0
			 0xee4ea77d, /// 0x4
			 0x73f10535, /// 0x8
			 0x5c300a7f, /// 0xc
			 0x5bf259e9, /// 0x10
			 0xb23942e5, /// 0x14
			 0x6de582ff, /// 0x18
			 0x55d8c769, /// 0x1c
			 0x73ebb143, /// 0x20
			 0x63ea3df2, /// 0x24
			 0x6c23c0e9, /// 0x28
			 0xfb7cbc99, /// 0x2c
			 0xa2848114, /// 0x30
			 0xb65d0b2d, /// 0x34
			 0x792af120, /// 0x38
			 0xbc9dd47d, /// 0x3c
			 0xdb8f9faf, /// 0x40
			 0x9c31bfeb, /// 0x44
			 0xcdcbc48f, /// 0x48
			 0x753a0fdb, /// 0x4c
			 0xdf23cc9a, /// 0x50
			 0x095744de, /// 0x54
			 0xa3428f93, /// 0x58
			 0x6758bd7a, /// 0x5c
			 0x390a3575, /// 0x60
			 0x7d746a91, /// 0x64
			 0x8b46400c, /// 0x68
			 0x1bd5bac2, /// 0x6c
			 0x2169dcbf, /// 0x70
			 0xbe5aba4b, /// 0x74
			 0x1c35fa71, /// 0x78
			 0x7f6cda2b, /// 0x7c
			 0x322a25b5, /// 0x80
			 0x52c0cc27, /// 0x84
			 0x7f722aef, /// 0x88
			 0xa48b2753, /// 0x8c
			 0xade45079, /// 0x90
			 0xa1d4320c, /// 0x94
			 0xaf0a04d1, /// 0x98
			 0x35b9f70c, /// 0x9c
			 0x691106c5, /// 0xa0
			 0xd55302b3, /// 0xa4
			 0x7f42bb8f, /// 0xa8
			 0x8a40a808, /// 0xac
			 0x17ed5954, /// 0xb0
			 0x258ebc69, /// 0xb4
			 0x589ec6ba, /// 0xb8
			 0xf87f5e05, /// 0xbc
			 0x328716b3, /// 0xc0
			 0x21bbb674, /// 0xc4
			 0x59e4d4ce, /// 0xc8
			 0xfc01933b, /// 0xcc
			 0x3e653eee, /// 0xd0
			 0x1b3c3c5a, /// 0xd4
			 0x48665f95, /// 0xd8
			 0x0d754d18, /// 0xdc
			 0x70b05ae3, /// 0xe0
			 0xc09f68fd, /// 0xe4
			 0x33e64ea5, /// 0xe8
			 0x26f0827d, /// 0xec
			 0xafa88af9, /// 0xf0
			 0x60abd54e, /// 0xf4
			 0x8621e836, /// 0xf8
			 0xd11b0c3d, /// 0xfc
			 0x571f5771, /// 0x100
			 0xa4490525, /// 0x104
			 0xa8498596, /// 0x108
			 0x0769efb1, /// 0x10c
			 0x6465270f, /// 0x110
			 0x5368e0cd, /// 0x114
			 0x3edd3c2c, /// 0x118
			 0x6219f47e, /// 0x11c
			 0xbf68c3b8, /// 0x120
			 0xdef19b4f, /// 0x124
			 0x5aab6965, /// 0x128
			 0xe61b7ff5, /// 0x12c
			 0x0f8cc13d, /// 0x130
			 0x79505a2f, /// 0x134
			 0x97d82c89, /// 0x138
			 0xf5853831, /// 0x13c
			 0x97a6d051, /// 0x140
			 0xd0af2626, /// 0x144
			 0xff3a9134, /// 0x148
			 0x7d496f98, /// 0x14c
			 0x64686f5f, /// 0x150
			 0x9f29acf7, /// 0x154
			 0x5cd95885, /// 0x158
			 0x68c6736a, /// 0x15c
			 0xa1d1c1ea, /// 0x160
			 0x1be8e7db, /// 0x164
			 0xb6362394, /// 0x168
			 0xa160bc27, /// 0x16c
			 0x13ab30e5, /// 0x170
			 0xeaf87c6d, /// 0x174
			 0x5273f3e9, /// 0x178
			 0x7ec2e2d5, /// 0x17c
			 0x26dc854c, /// 0x180
			 0x3be3ce6e, /// 0x184
			 0xefbb19f5, /// 0x188
			 0x46a3fa0e, /// 0x18c
			 0xeb396852, /// 0x190
			 0x2fab154a, /// 0x194
			 0x4f0af264, /// 0x198
			 0xd1ebf6da, /// 0x19c
			 0xd36c21b5, /// 0x1a0
			 0xb3dc8be1, /// 0x1a4
			 0xf58fb326, /// 0x1a8
			 0xce5894f3, /// 0x1ac
			 0x25e7a902, /// 0x1b0
			 0x111cd4b4, /// 0x1b4
			 0xa6e8417e, /// 0x1b8
			 0xf48be54b, /// 0x1bc
			 0xa0ba72d2, /// 0x1c0
			 0xf0c8df36, /// 0x1c4
			 0x9bdda577, /// 0x1c8
			 0x9ff2ca6f, /// 0x1cc
			 0x97d0fa1b, /// 0x1d0
			 0x29fdf1da, /// 0x1d4
			 0x179f4caf, /// 0x1d8
			 0xc5fa2226, /// 0x1dc
			 0xdaaa274b, /// 0x1e0
			 0xd78837f3, /// 0x1e4
			 0xb90b34e6, /// 0x1e8
			 0x7272a0ca, /// 0x1ec
			 0x6f87f77c, /// 0x1f0
			 0x75d14eeb, /// 0x1f4
			 0x787c27f0, /// 0x1f8
			 0x9f155b2f, /// 0x1fc
			 0xd47d1097, /// 0x200
			 0xbf677b02, /// 0x204
			 0xfa2da79f, /// 0x208
			 0x8685f735, /// 0x20c
			 0xe8c7ce15, /// 0x210
			 0x45b63022, /// 0x214
			 0x9a823e9b, /// 0x218
			 0x0e0cf44b, /// 0x21c
			 0x26960ed8, /// 0x220
			 0x6191e839, /// 0x224
			 0x652ec32a, /// 0x228
			 0x564cd437, /// 0x22c
			 0x253a8010, /// 0x230
			 0x9dbccd1e, /// 0x234
			 0x9471fffd, /// 0x238
			 0x1c608663, /// 0x23c
			 0x0a5ed94f, /// 0x240
			 0x0bfbc57e, /// 0x244
			 0x4512f0b1, /// 0x248
			 0x9f6cd7a6, /// 0x24c
			 0x5a66fc46, /// 0x250
			 0xa99428fc, /// 0x254
			 0xe9880f00, /// 0x258
			 0xe0aa2cae, /// 0x25c
			 0x6daeb234, /// 0x260
			 0xcb6da02f, /// 0x264
			 0x34768bfe, /// 0x268
			 0xe02fadca, /// 0x26c
			 0xa725a475, /// 0x270
			 0xafd14e62, /// 0x274
			 0xace991a3, /// 0x278
			 0x861d38f1, /// 0x27c
			 0x4a77ef2e, /// 0x280
			 0xb67fa6db, /// 0x284
			 0x7de9abb6, /// 0x288
			 0xc5f6cecf, /// 0x28c
			 0x21b5cf1e, /// 0x290
			 0xb54d77f6, /// 0x294
			 0x27eeb56a, /// 0x298
			 0x39977344, /// 0x29c
			 0xcd556d3b, /// 0x2a0
			 0x5c05e260, /// 0x2a4
			 0x1aba1889, /// 0x2a8
			 0x5f2a1808, /// 0x2ac
			 0xd65cdfdc, /// 0x2b0
			 0x1dff15fb, /// 0x2b4
			 0x5002bf05, /// 0x2b8
			 0xd8380097, /// 0x2bc
			 0xd1c111f7, /// 0x2c0
			 0xdc72d69a, /// 0x2c4
			 0x95455f66, /// 0x2c8
			 0xf9a6af87, /// 0x2cc
			 0x16a9696d, /// 0x2d0
			 0x00a03551, /// 0x2d4
			 0x8b2c3191, /// 0x2d8
			 0x69cd5eab, /// 0x2dc
			 0x4b8f6ed1, /// 0x2e0
			 0x22945b8c, /// 0x2e4
			 0xc9fd18eb, /// 0x2e8
			 0x70b4f454, /// 0x2ec
			 0x262dfa13, /// 0x2f0
			 0x748090bd, /// 0x2f4
			 0x50848f7c, /// 0x2f8
			 0x5b0f68a5, /// 0x2fc
			 0x6d146bd2, /// 0x300
			 0x971ddd4e, /// 0x304
			 0x1808319a, /// 0x308
			 0xaabb1388, /// 0x30c
			 0x9da2ca95, /// 0x310
			 0xab2034a1, /// 0x314
			 0x329d3ee1, /// 0x318
			 0x70d59f06, /// 0x31c
			 0x9e23f816, /// 0x320
			 0xa0b1f4f8, /// 0x324
			 0xdd162a08, /// 0x328
			 0x2c43f895, /// 0x32c
			 0x8a7e1b0b, /// 0x330
			 0x571c9bce, /// 0x334
			 0x8cbee329, /// 0x338
			 0xda506e38, /// 0x33c
			 0x976950c1, /// 0x340
			 0x8eb261b0, /// 0x344
			 0x1098e67f, /// 0x348
			 0x84a5b9b9, /// 0x34c
			 0x629e7426, /// 0x350
			 0xe632389d, /// 0x354
			 0x435631c4, /// 0x358
			 0x63bb791f, /// 0x35c
			 0x846448a0, /// 0x360
			 0x657b4a6e, /// 0x364
			 0xfa4b5d81, /// 0x368
			 0x622f559f, /// 0x36c
			 0x35c72c81, /// 0x370
			 0x9206eaae, /// 0x374
			 0x0f1f6936, /// 0x378
			 0x40ef1e5a, /// 0x37c
			 0xca009124, /// 0x380
			 0xa02aa4f7, /// 0x384
			 0x395aa2bb, /// 0x388
			 0xdb2d5cf1, /// 0x38c
			 0xf96c86ad, /// 0x390
			 0x160da5a2, /// 0x394
			 0xa4a7f8bc, /// 0x398
			 0xfb2ce4f1, /// 0x39c
			 0x75ac6510, /// 0x3a0
			 0x9e2506a5, /// 0x3a4
			 0xb68d8bad, /// 0x3a8
			 0xc51d33d5, /// 0x3ac
			 0x027518c9, /// 0x3b0
			 0x83ef2e30, /// 0x3b4
			 0x4e287a13, /// 0x3b8
			 0x45cc4f9f, /// 0x3bc
			 0xda24e0bf, /// 0x3c0
			 0x6e27ddc6, /// 0x3c4
			 0xa886548e, /// 0x3c8
			 0x402c733b, /// 0x3cc
			 0x26e1513c, /// 0x3d0
			 0x76b6648f, /// 0x3d4
			 0x6a622e31, /// 0x3d8
			 0xa9fff2ca, /// 0x3dc
			 0xa1712999, /// 0x3e0
			 0xa7c8d532, /// 0x3e4
			 0x973ce198, /// 0x3e8
			 0x22281b59, /// 0x3ec
			 0x12efc09a, /// 0x3f0
			 0x7cc9bce2, /// 0x3f4
			 0xb119ea5c, /// 0x3f8
			 0xfb1ce7d9, /// 0x3fc
			 0x5425bf02, /// 0x400
			 0x4e09e017, /// 0x404
			 0xb934b2f4, /// 0x408
			 0x356a6198, /// 0x40c
			 0x454ea475, /// 0x410
			 0x79b48fc7, /// 0x414
			 0xe1345c64, /// 0x418
			 0xd8eb6e04, /// 0x41c
			 0x369e2e6b, /// 0x420
			 0xd73bd73d, /// 0x424
			 0x3e1975f3, /// 0x428
			 0xf9ce1c10, /// 0x42c
			 0x2498b44d, /// 0x430
			 0xf160052e, /// 0x434
			 0xcbca4d50, /// 0x438
			 0x708c89c5, /// 0x43c
			 0xb4370991, /// 0x440
			 0xf9a57df9, /// 0x444
			 0xccda9447, /// 0x448
			 0x4389c24b, /// 0x44c
			 0x9f343ae0, /// 0x450
			 0xa48accaf, /// 0x454
			 0xaecdc1a2, /// 0x458
			 0xc57e3fdf, /// 0x45c
			 0xf902dd26, /// 0x460
			 0x95c0e356, /// 0x464
			 0xf125b585, /// 0x468
			 0x2845fb6e, /// 0x46c
			 0x1e80e563, /// 0x470
			 0xddd26fc3, /// 0x474
			 0xe59e262c, /// 0x478
			 0x0de03434, /// 0x47c
			 0x46d9b01d, /// 0x480
			 0x6b868231, /// 0x484
			 0x391c557f, /// 0x488
			 0x9aeeec39, /// 0x48c
			 0xb9b3aaa8, /// 0x490
			 0xb5ee2b4c, /// 0x494
			 0x898087b6, /// 0x498
			 0xb6d53230, /// 0x49c
			 0x0381d476, /// 0x4a0
			 0xbcc4f9b4, /// 0x4a4
			 0x5731511c, /// 0x4a8
			 0xaf71e13f, /// 0x4ac
			 0xd8380d78, /// 0x4b0
			 0x1082c5ab, /// 0x4b4
			 0xdccd179f, /// 0x4b8
			 0x56386aa7, /// 0x4bc
			 0xda705278, /// 0x4c0
			 0x5c1deab4, /// 0x4c4
			 0xec3574ff, /// 0x4c8
			 0xf48ccb19, /// 0x4cc
			 0x271560d8, /// 0x4d0
			 0x4077e833, /// 0x4d4
			 0xcea4bfc5, /// 0x4d8
			 0x106d4f2c, /// 0x4dc
			 0xe7df8d07, /// 0x4e0
			 0x0f9a9ba7, /// 0x4e4
			 0xd4db55a4, /// 0x4e8
			 0x13eb62dc, /// 0x4ec
			 0x464033e0, /// 0x4f0
			 0x0008ff48, /// 0x4f4
			 0xbd020ce7, /// 0x4f8
			 0xb21044af, /// 0x4fc
			 0xd4a75f5a, /// 0x500
			 0x58de468c, /// 0x504
			 0xe1ef1948, /// 0x508
			 0x19cea0a9, /// 0x50c
			 0xd1009a06, /// 0x510
			 0x1b10dd9c, /// 0x514
			 0x407a9cf4, /// 0x518
			 0x9bb45861, /// 0x51c
			 0x0c82f5de, /// 0x520
			 0x0e9e8e46, /// 0x524
			 0x0386a53a, /// 0x528
			 0x953d778f, /// 0x52c
			 0x0f426929, /// 0x530
			 0x6b80ef9d, /// 0x534
			 0xf43a7c16, /// 0x538
			 0x8b66f7a3, /// 0x53c
			 0x2fc3fd53, /// 0x540
			 0xf9692c82, /// 0x544
			 0xb4273c09, /// 0x548
			 0x5c4a08ad, /// 0x54c
			 0x73073006, /// 0x550
			 0x631995da, /// 0x554
			 0xcb3f5e58, /// 0x558
			 0x95bce644, /// 0x55c
			 0xffd676c4, /// 0x560
			 0x6f7b29c8, /// 0x564
			 0xe62713ea, /// 0x568
			 0xa5f1f1e3, /// 0x56c
			 0xfe7b5f5c, /// 0x570
			 0xfb806760, /// 0x574
			 0x9c0fe11b, /// 0x578
			 0xd6e0f82f, /// 0x57c
			 0x05209a60, /// 0x580
			 0xf9a1eeee, /// 0x584
			 0xab5dd175, /// 0x588
			 0x4ef5c79d, /// 0x58c
			 0xfae3a792, /// 0x590
			 0x8d14a18e, /// 0x594
			 0x991133b1, /// 0x598
			 0x1fcddf8f, /// 0x59c
			 0x8f509f0d, /// 0x5a0
			 0x9ee4d6db, /// 0x5a4
			 0x26591859, /// 0x5a8
			 0xf6a5b8d7, /// 0x5ac
			 0x85a6ddda, /// 0x5b0
			 0x0cd4fbf4, /// 0x5b4
			 0x9745f9b0, /// 0x5b8
			 0xdb4c301b, /// 0x5bc
			 0x19aa66d6, /// 0x5c0
			 0x05fcdf84, /// 0x5c4
			 0xe0a3e3b4, /// 0x5c8
			 0x622d834a, /// 0x5cc
			 0xdc49ce2a, /// 0x5d0
			 0xc278b0a1, /// 0x5d4
			 0x7d59c8d3, /// 0x5d8
			 0x9b5bf417, /// 0x5dc
			 0x5402f6e5, /// 0x5e0
			 0x066be60d, /// 0x5e4
			 0xc0e23aba, /// 0x5e8
			 0x793f6ae8, /// 0x5ec
			 0xb6e2cd03, /// 0x5f0
			 0x98db6e03, /// 0x5f4
			 0x72b1a441, /// 0x5f8
			 0x59222c8a, /// 0x5fc
			 0x591ad451, /// 0x600
			 0x3a85a691, /// 0x604
			 0x601f2a43, /// 0x608
			 0x63f03996, /// 0x60c
			 0xd884164a, /// 0x610
			 0x96f47c2c, /// 0x614
			 0x21118cf9, /// 0x618
			 0x7f18ba59, /// 0x61c
			 0xd4edd5e4, /// 0x620
			 0x65004ff3, /// 0x624
			 0xf9d17430, /// 0x628
			 0x3928cfa8, /// 0x62c
			 0xb37adb84, /// 0x630
			 0xe73039f5, /// 0x634
			 0xbc20c2b2, /// 0x638
			 0x1525d1a0, /// 0x63c
			 0x395d47e3, /// 0x640
			 0xaa110aab, /// 0x644
			 0x0fd9c64f, /// 0x648
			 0xd1883f37, /// 0x64c
			 0x7cd7a00f, /// 0x650
			 0xa660eda6, /// 0x654
			 0x1cc8db22, /// 0x658
			 0x42fb9155, /// 0x65c
			 0xf7a1d261, /// 0x660
			 0xe5cec9b5, /// 0x664
			 0x5940298f, /// 0x668
			 0x74df6fc3, /// 0x66c
			 0x43c23452, /// 0x670
			 0x76d82f73, /// 0x674
			 0x54700bbc, /// 0x678
			 0x79aa2d0d, /// 0x67c
			 0xd7bde489, /// 0x680
			 0x3a4d9cbd, /// 0x684
			 0xa6d004dd, /// 0x688
			 0xe7ce2b05, /// 0x68c
			 0x93af51d4, /// 0x690
			 0x0d75b643, /// 0x694
			 0x89fe254a, /// 0x698
			 0x46f585da, /// 0x69c
			 0xb97b7b3c, /// 0x6a0
			 0xaee07fe9, /// 0x6a4
			 0xd94e4ed3, /// 0x6a8
			 0x7917aa67, /// 0x6ac
			 0xc9267d4a, /// 0x6b0
			 0xa64e50e9, /// 0x6b4
			 0x4e87e4a3, /// 0x6b8
			 0xb62b214f, /// 0x6bc
			 0x0d6b293e, /// 0x6c0
			 0xf89e5775, /// 0x6c4
			 0x4d9e2595, /// 0x6c8
			 0x829feec7, /// 0x6cc
			 0xd4b64b32, /// 0x6d0
			 0x42230baf, /// 0x6d4
			 0x95c3cb9e, /// 0x6d8
			 0xd7dec08c, /// 0x6dc
			 0xfcb6f5e5, /// 0x6e0
			 0x194e9fe6, /// 0x6e4
			 0xc1bdf41d, /// 0x6e8
			 0xc1354f05, /// 0x6ec
			 0xbfdd9842, /// 0x6f0
			 0x37346275, /// 0x6f4
			 0x6cc3de24, /// 0x6f8
			 0xf81a2734, /// 0x6fc
			 0xe64f5eb3, /// 0x700
			 0x2bb66565, /// 0x704
			 0x3620d288, /// 0x708
			 0xbe019391, /// 0x70c
			 0x2998f5bc, /// 0x710
			 0x9702e1f8, /// 0x714
			 0xb26c621e, /// 0x718
			 0x599b385e, /// 0x71c
			 0x2ca5681f, /// 0x720
			 0x23597355, /// 0x724
			 0x4a2c6632, /// 0x728
			 0xa3dd7cf1, /// 0x72c
			 0xfea8d2c4, /// 0x730
			 0x791685c8, /// 0x734
			 0xfa84c18a, /// 0x738
			 0x2415b252, /// 0x73c
			 0x07494690, /// 0x740
			 0x2516d47c, /// 0x744
			 0x7bac576d, /// 0x748
			 0x9740e5f4, /// 0x74c
			 0xf30c7acb, /// 0x750
			 0x3c26103f, /// 0x754
			 0x55abce0c, /// 0x758
			 0x14accde4, /// 0x75c
			 0x23c7de09, /// 0x760
			 0xb1b7a546, /// 0x764
			 0xb44397a0, /// 0x768
			 0x7bf00813, /// 0x76c
			 0xf5cc9e0c, /// 0x770
			 0xcf8f5924, /// 0x774
			 0xa23443eb, /// 0x778
			 0x0c85cc30, /// 0x77c
			 0x956bd245, /// 0x780
			 0xec328617, /// 0x784
			 0x392d9aab, /// 0x788
			 0x331de513, /// 0x78c
			 0x571a90b4, /// 0x790
			 0x6f6c5f93, /// 0x794
			 0x93ca97e9, /// 0x798
			 0xfd9828dd, /// 0x79c
			 0x47bbeb44, /// 0x7a0
			 0x2e3d8dd9, /// 0x7a4
			 0x444805f6, /// 0x7a8
			 0x1f078060, /// 0x7ac
			 0x70cd9069, /// 0x7b0
			 0xaec01daf, /// 0x7b4
			 0x98e1ff6a, /// 0x7b8
			 0x3e767903, /// 0x7bc
			 0x0645339a, /// 0x7c0
			 0x397c9673, /// 0x7c4
			 0x96f83409, /// 0x7c8
			 0xc4d23ef9, /// 0x7cc
			 0x4b3bd14d, /// 0x7d0
			 0xcc04f59c, /// 0x7d4
			 0x6fb62ef5, /// 0x7d8
			 0x64cf77d4, /// 0x7dc
			 0xe2e15b74, /// 0x7e0
			 0x6ba1b891, /// 0x7e4
			 0xae7ec742, /// 0x7e8
			 0xa6f319b3, /// 0x7ec
			 0x0d1586f0, /// 0x7f0
			 0x1d3da263, /// 0x7f4
			 0xfb83e483, /// 0x7f8
			 0x94868383, /// 0x7fc
			 0xaee34af2, /// 0x800
			 0x341d6060, /// 0x804
			 0x11d24457, /// 0x808
			 0x2831de3d, /// 0x80c
			 0x5aca5788, /// 0x810
			 0xfa4ad018, /// 0x814
			 0xc2ed2472, /// 0x818
			 0xbf231af9, /// 0x81c
			 0x434a845b, /// 0x820
			 0x90161c76, /// 0x824
			 0x5bb5e3a5, /// 0x828
			 0x4191eb03, /// 0x82c
			 0x67b72eda, /// 0x830
			 0x0ca7d160, /// 0x834
			 0x8df1599b, /// 0x838
			 0x6796c8ed, /// 0x83c
			 0x6829bf07, /// 0x840
			 0x3554ea47, /// 0x844
			 0x8b22267a, /// 0x848
			 0xdd2579f2, /// 0x84c
			 0xb74b1573, /// 0x850
			 0x190a8c37, /// 0x854
			 0xd3f33038, /// 0x858
			 0x4fcd7262, /// 0x85c
			 0xaaae9112, /// 0x860
			 0xd53afd48, /// 0x864
			 0xf3853d27, /// 0x868
			 0x7a38a064, /// 0x86c
			 0xfb4984c7, /// 0x870
			 0x43ea707b, /// 0x874
			 0x18d0ffc4, /// 0x878
			 0x5e358065, /// 0x87c
			 0xdc491207, /// 0x880
			 0x607e72ef, /// 0x884
			 0x369d8db2, /// 0x888
			 0x752782d4, /// 0x88c
			 0xe54d9045, /// 0x890
			 0x14bff8f5, /// 0x894
			 0x651599e3, /// 0x898
			 0xdd0fa78c, /// 0x89c
			 0x6824664b, /// 0x8a0
			 0x5e447615, /// 0x8a4
			 0x7b745f59, /// 0x8a8
			 0xc8c332a5, /// 0x8ac
			 0xed351839, /// 0x8b0
			 0xfbf1980a, /// 0x8b4
			 0xf00aa5d4, /// 0x8b8
			 0xed68ab2c, /// 0x8bc
			 0xcf061304, /// 0x8c0
			 0xd52a0cfa, /// 0x8c4
			 0x1877f004, /// 0x8c8
			 0x8cf061df, /// 0x8cc
			 0x1411b03d, /// 0x8d0
			 0xef7af549, /// 0x8d4
			 0xaf8be41d, /// 0x8d8
			 0x5b7172fc, /// 0x8dc
			 0x2a1dc347, /// 0x8e0
			 0xb269fae2, /// 0x8e4
			 0x2e14a055, /// 0x8e8
			 0xe0fda31e, /// 0x8ec
			 0x59be1572, /// 0x8f0
			 0xc87897f8, /// 0x8f4
			 0xbbc3a397, /// 0x8f8
			 0x4928fd2f, /// 0x8fc
			 0x125c5cb2, /// 0x900
			 0xbfb2e3f6, /// 0x904
			 0xd441bb95, /// 0x908
			 0xd07ac346, /// 0x90c
			 0xff3edd5f, /// 0x910
			 0x71bfc9fd, /// 0x914
			 0x9af1901c, /// 0x918
			 0x36fa2898, /// 0x91c
			 0x8331b45b, /// 0x920
			 0x81422fcd, /// 0x924
			 0x69d42a1a, /// 0x928
			 0x7ea2110d, /// 0x92c
			 0x710fcbf7, /// 0x930
			 0x8726a107, /// 0x934
			 0x5161b928, /// 0x938
			 0xbf855f14, /// 0x93c
			 0xe0bd8ad3, /// 0x940
			 0x52e2d809, /// 0x944
			 0xbd8d687d, /// 0x948
			 0xde5a83ee, /// 0x94c
			 0x20708cfc, /// 0x950
			 0x5a3dd9b7, /// 0x954
			 0x8f75bb76, /// 0x958
			 0x9b46122a, /// 0x95c
			 0x57e915b0, /// 0x960
			 0x22c3f11a, /// 0x964
			 0xeb881a74, /// 0x968
			 0x32baa94f, /// 0x96c
			 0x13c11dfa, /// 0x970
			 0x4a8866e4, /// 0x974
			 0x53a6cfac, /// 0x978
			 0x535d9a41, /// 0x97c
			 0x1b01a3b1, /// 0x980
			 0xecab12db, /// 0x984
			 0x90e9a0dd, /// 0x988
			 0xa6e1aa1c, /// 0x98c
			 0xaebe7f7b, /// 0x990
			 0x248510ae, /// 0x994
			 0x7b281978, /// 0x998
			 0xcbdea2e1, /// 0x99c
			 0x4f7e354d, /// 0x9a0
			 0x5a1bb6dd, /// 0x9a4
			 0xc5da42a9, /// 0x9a8
			 0x30908dab, /// 0x9ac
			 0x0a01d62f, /// 0x9b0
			 0x7bad98aa, /// 0x9b4
			 0x2ac4a7c6, /// 0x9b8
			 0xaf1bff7f, /// 0x9bc
			 0xcca4aa3f, /// 0x9c0
			 0xc5ebab8f, /// 0x9c4
			 0x8224a289, /// 0x9c8
			 0xf9cc8993, /// 0x9cc
			 0x2492e062, /// 0x9d0
			 0x8b296b72, /// 0x9d4
			 0xac32eb03, /// 0x9d8
			 0x7c4b2b23, /// 0x9dc
			 0x0070da83, /// 0x9e0
			 0xf6caf246, /// 0x9e4
			 0x7a884692, /// 0x9e8
			 0xd3f70ba1, /// 0x9ec
			 0xf76647a1, /// 0x9f0
			 0x8dca2d9b, /// 0x9f4
			 0x3b981244, /// 0x9f8
			 0x57e82991, /// 0x9fc
			 0x891f4c8b, /// 0xa00
			 0x6ff0d4c9, /// 0xa04
			 0xedeb394b, /// 0xa08
			 0x62f95b2d, /// 0xa0c
			 0xcf80018c, /// 0xa10
			 0x709fd955, /// 0xa14
			 0x23f2b8ff, /// 0xa18
			 0xe136f30f, /// 0xa1c
			 0xcfbb9a96, /// 0xa20
			 0xf945f8ec, /// 0xa24
			 0xe69b1e8b, /// 0xa28
			 0x5ae8c56a, /// 0xa2c
			 0x7c14a5db, /// 0xa30
			 0x11d4f2bf, /// 0xa34
			 0x46e5ee6e, /// 0xa38
			 0x3bb9d3f5, /// 0xa3c
			 0x9e828695, /// 0xa40
			 0x494b0ec9, /// 0xa44
			 0x61aa95f0, /// 0xa48
			 0x171f84c3, /// 0xa4c
			 0xba44ef36, /// 0xa50
			 0x559c3a9b, /// 0xa54
			 0xb7c0ddcb, /// 0xa58
			 0xa230b820, /// 0xa5c
			 0xe397742c, /// 0xa60
			 0x065f5ec2, /// 0xa64
			 0xc0fdde39, /// 0xa68
			 0xf39def06, /// 0xa6c
			 0x87cf7e6e, /// 0xa70
			 0xd105a55d, /// 0xa74
			 0xc8bab674, /// 0xa78
			 0xccd89129, /// 0xa7c
			 0xd6e97960, /// 0xa80
			 0xb6908687, /// 0xa84
			 0x6bcdec2f, /// 0xa88
			 0x2666fd18, /// 0xa8c
			 0x1ddc42fc, /// 0xa90
			 0xa8852a7e, /// 0xa94
			 0x0e33a81c, /// 0xa98
			 0xe90f6b11, /// 0xa9c
			 0x06af18f9, /// 0xaa0
			 0xa06812e0, /// 0xaa4
			 0x6ad56dbe, /// 0xaa8
			 0xfaff36bf, /// 0xaac
			 0x0cda6a23, /// 0xab0
			 0xeeb5655e, /// 0xab4
			 0xef9162a2, /// 0xab8
			 0xe4017a02, /// 0xabc
			 0x3adeb318, /// 0xac0
			 0xba51d015, /// 0xac4
			 0x6defb011, /// 0xac8
			 0xb28b359c, /// 0xacc
			 0x61400d83, /// 0xad0
			 0x4cc99b53, /// 0xad4
			 0x4ea3d34a, /// 0xad8
			 0x4604df8b, /// 0xadc
			 0x62935a17, /// 0xae0
			 0xa8a463d6, /// 0xae4
			 0x8af4d3f8, /// 0xae8
			 0xb77fe98f, /// 0xaec
			 0xe53897b1, /// 0xaf0
			 0x80a199a1, /// 0xaf4
			 0xf6742f7e, /// 0xaf8
			 0x835427da, /// 0xafc
			 0x04c3ed1a, /// 0xb00
			 0x024853e2, /// 0xb04
			 0xf178d7e7, /// 0xb08
			 0xe0e696e9, /// 0xb0c
			 0x6d15e7a5, /// 0xb10
			 0x4925d33f, /// 0xb14
			 0xb357d2da, /// 0xb18
			 0x5733d65b, /// 0xb1c
			 0xf6688b5e, /// 0xb20
			 0x05ffeeed, /// 0xb24
			 0x9aa3d201, /// 0xb28
			 0x3d868fd9, /// 0xb2c
			 0x6328002e, /// 0xb30
			 0x2aa90f0f, /// 0xb34
			 0x09e3a1a9, /// 0xb38
			 0xaf738875, /// 0xb3c
			 0x6c90f876, /// 0xb40
			 0x79184261, /// 0xb44
			 0x3cda21de, /// 0xb48
			 0xcf9c6faa, /// 0xb4c
			 0xf57a9ce9, /// 0xb50
			 0xdbd58f0f, /// 0xb54
			 0x2c470a41, /// 0xb58
			 0x89419410, /// 0xb5c
			 0x310f98e9, /// 0xb60
			 0xffa12452, /// 0xb64
			 0xd661e1ca, /// 0xb68
			 0xd5d67f7f, /// 0xb6c
			 0x4e7398bb, /// 0xb70
			 0x1d4534b4, /// 0xb74
			 0xd52201c0, /// 0xb78
			 0x0625eb90, /// 0xb7c
			 0xe1e155d9, /// 0xb80
			 0xf4b3136e, /// 0xb84
			 0x6ac74214, /// 0xb88
			 0x8cbb3758, /// 0xb8c
			 0xb6609d28, /// 0xb90
			 0x55a53857, /// 0xb94
			 0xe580ab66, /// 0xb98
			 0x0c8a934a, /// 0xb9c
			 0xa9be2b73, /// 0xba0
			 0x117b4627, /// 0xba4
			 0xb7f0e83c, /// 0xba8
			 0xd3bd582e, /// 0xbac
			 0x25c15079, /// 0xbb0
			 0x85dc7c68, /// 0xbb4
			 0x6beb0c9f, /// 0xbb8
			 0xa21f9fa7, /// 0xbbc
			 0x602ca7aa, /// 0xbc0
			 0x24489d8b, /// 0xbc4
			 0xc9c342f0, /// 0xbc8
			 0x67c1b9c4, /// 0xbcc
			 0x7e051cb8, /// 0xbd0
			 0x7d05f5b5, /// 0xbd4
			 0x28bfef0f, /// 0xbd8
			 0x8c36144d, /// 0xbdc
			 0x7ad31d73, /// 0xbe0
			 0x8f090f04, /// 0xbe4
			 0x969894a0, /// 0xbe8
			 0x480fde4a, /// 0xbec
			 0xd3e61bf6, /// 0xbf0
			 0x75f83ede, /// 0xbf4
			 0xef7ee603, /// 0xbf8
			 0xe65a8bea, /// 0xbfc
			 0xa78992a9, /// 0xc00
			 0x0a05721f, /// 0xc04
			 0x4ea56c6e, /// 0xc08
			 0xa5eeb9f5, /// 0xc0c
			 0x4907e3f3, /// 0xc10
			 0x6816316b, /// 0xc14
			 0x74bc2951, /// 0xc18
			 0xa1e20e34, /// 0xc1c
			 0xe3574ca0, /// 0xc20
			 0x0a9481ee, /// 0xc24
			 0x4bbf05e7, /// 0xc28
			 0x8bef00b6, /// 0xc2c
			 0x035b817e, /// 0xc30
			 0x6f78cc19, /// 0xc34
			 0xad5ee949, /// 0xc38
			 0x841d32d9, /// 0xc3c
			 0x34be54e7, /// 0xc40
			 0x901b0665, /// 0xc44
			 0x7ca49dcc, /// 0xc48
			 0xc4f37991, /// 0xc4c
			 0xac295087, /// 0xc50
			 0xcbf1ab34, /// 0xc54
			 0xbee19eeb, /// 0xc58
			 0x2b3dfd97, /// 0xc5c
			 0xa4dfc4fd, /// 0xc60
			 0x3abf81b0, /// 0xc64
			 0x70d61bd0, /// 0xc68
			 0xba938226, /// 0xc6c
			 0x8aaeaac6, /// 0xc70
			 0xd5d12fca, /// 0xc74
			 0xb6d20bcd, /// 0xc78
			 0x652b94b6, /// 0xc7c
			 0x87e0d4ad, /// 0xc80
			 0x342d0f1f, /// 0xc84
			 0xe0c69bca, /// 0xc88
			 0x63ae063f, /// 0xc8c
			 0x9288fc53, /// 0xc90
			 0x19dc3c48, /// 0xc94
			 0xe43d592b, /// 0xc98
			 0x42fd30f0, /// 0xc9c
			 0x5e943378, /// 0xca0
			 0x7559250d, /// 0xca4
			 0x7ca99b48, /// 0xca8
			 0x754cfc12, /// 0xcac
			 0xbed19f59, /// 0xcb0
			 0x94caa899, /// 0xcb4
			 0x375aa2bf, /// 0xcb8
			 0x3de1e6d7, /// 0xcbc
			 0x4d73043a, /// 0xcc0
			 0x0ba27d3f, /// 0xcc4
			 0xbdc2327c, /// 0xcc8
			 0x718f07fc, /// 0xccc
			 0x68f52fed, /// 0xcd0
			 0x14d66d6f, /// 0xcd4
			 0x1d26cc6b, /// 0xcd8
			 0xe62816b5, /// 0xcdc
			 0xb6987ed8, /// 0xce0
			 0x79798bae, /// 0xce4
			 0x68f2c2b1, /// 0xce8
			 0xc26eac76, /// 0xcec
			 0x505a762b, /// 0xcf0
			 0xdae77e3d, /// 0xcf4
			 0x41683a3c, /// 0xcf8
			 0x4fbce8d4, /// 0xcfc
			 0xe76a354c, /// 0xd00
			 0xa9d2c7dd, /// 0xd04
			 0xb64412df, /// 0xd08
			 0x2859aee6, /// 0xd0c
			 0x0a22460a, /// 0xd10
			 0x4a819f51, /// 0xd14
			 0x2cebd7c4, /// 0xd18
			 0x7b5b6cc4, /// 0xd1c
			 0xa79a22be, /// 0xd20
			 0x330788cd, /// 0xd24
			 0x1bc38f79, /// 0xd28
			 0x2e0c22b9, /// 0xd2c
			 0x22151642, /// 0xd30
			 0xa4007cd8, /// 0xd34
			 0x5da9f8da, /// 0xd38
			 0xdab62bd9, /// 0xd3c
			 0xf05d43b9, /// 0xd40
			 0x9333789f, /// 0xd44
			 0x4672f179, /// 0xd48
			 0x545b2618, /// 0xd4c
			 0x62adecba, /// 0xd50
			 0x31afed29, /// 0xd54
			 0x8b7a9a5e, /// 0xd58
			 0x44eb6dfb, /// 0xd5c
			 0xdcd47885, /// 0xd60
			 0xf2a6c15f, /// 0xd64
			 0xfa867796, /// 0xd68
			 0x1c00428d, /// 0xd6c
			 0x18a6381e, /// 0xd70
			 0x702803d6, /// 0xd74
			 0x877fd723, /// 0xd78
			 0x79d825a7, /// 0xd7c
			 0x4428aac6, /// 0xd80
			 0x158cd4d3, /// 0xd84
			 0x4f857a31, /// 0xd88
			 0xe04da3e5, /// 0xd8c
			 0xf2b9d03e, /// 0xd90
			 0xb0b5e035, /// 0xd94
			 0xc6fd1ac4, /// 0xd98
			 0xbd16586f, /// 0xd9c
			 0x4700cb1b, /// 0xda0
			 0x47f757e6, /// 0xda4
			 0x5a63d239, /// 0xda8
			 0x0496e9e0, /// 0xdac
			 0x1e6f8cc8, /// 0xdb0
			 0xe57e8bc7, /// 0xdb4
			 0xe44ca110, /// 0xdb8
			 0x38cc1163, /// 0xdbc
			 0x98b427cb, /// 0xdc0
			 0x120744c6, /// 0xdc4
			 0x145a5afc, /// 0xdc8
			 0x108e78f2, /// 0xdcc
			 0x9077efc5, /// 0xdd0
			 0x97f8d46b, /// 0xdd4
			 0x61011e54, /// 0xdd8
			 0x8baabccf, /// 0xddc
			 0x1bdeeee9, /// 0xde0
			 0x4e229e4b, /// 0xde4
			 0xaa080c3f, /// 0xde8
			 0x4191c403, /// 0xdec
			 0x25c6c56f, /// 0xdf0
			 0xcde6aa7d, /// 0xdf4
			 0x606e4e2a, /// 0xdf8
			 0x8d292a55, /// 0xdfc
			 0x666d86c7, /// 0xe00
			 0x5ced5582, /// 0xe04
			 0x09094914, /// 0xe08
			 0xd8e4c7f2, /// 0xe0c
			 0x8803d7c7, /// 0xe10
			 0x8acb3a9b, /// 0xe14
			 0x70ccc1b5, /// 0xe18
			 0x3b6c8d98, /// 0xe1c
			 0x8c75d863, /// 0xe20
			 0x2b59cba9, /// 0xe24
			 0xbd69c6b8, /// 0xe28
			 0x48e5fdc5, /// 0xe2c
			 0x860957ad, /// 0xe30
			 0x9ccc899c, /// 0xe34
			 0xd555a0b0, /// 0xe38
			 0xff1ee9c1, /// 0xe3c
			 0x85d0a18c, /// 0xe40
			 0x3933a06f, /// 0xe44
			 0x8cf574e2, /// 0xe48
			 0xfe952b4e, /// 0xe4c
			 0xdfbae4eb, /// 0xe50
			 0x88738538, /// 0xe54
			 0x4a23181d, /// 0xe58
			 0x3c55e172, /// 0xe5c
			 0x4acd5c47, /// 0xe60
			 0x7f02899e, /// 0xe64
			 0x0c991b49, /// 0xe68
			 0x9a1cd542, /// 0xe6c
			 0x28a7bcc0, /// 0xe70
			 0xbe3fccff, /// 0xe74
			 0x8cfe0c02, /// 0xe78
			 0x2ed2f5fb, /// 0xe7c
			 0xabcbd0a3, /// 0xe80
			 0x5bf6fdff, /// 0xe84
			 0xc80a1a85, /// 0xe88
			 0x13aeaa14, /// 0xe8c
			 0x8d4b855e, /// 0xe90
			 0xcd19f17b, /// 0xe94
			 0x54da2435, /// 0xe98
			 0xc54cf099, /// 0xe9c
			 0xf9f7d2ec, /// 0xea0
			 0x39e53532, /// 0xea4
			 0x1439c021, /// 0xea8
			 0x3b865324, /// 0xeac
			 0xde109c5c, /// 0xeb0
			 0x2c276fd4, /// 0xeb4
			 0xdc8a5c1e, /// 0xeb8
			 0xa932dac9, /// 0xebc
			 0xd17d118f, /// 0xec0
			 0x00dea7a1, /// 0xec4
			 0x601baad8, /// 0xec8
			 0x27b2a617, /// 0xecc
			 0xbefc2f81, /// 0xed0
			 0x8adf1507, /// 0xed4
			 0x53ffc3ea, /// 0xed8
			 0x5eb0ed34, /// 0xedc
			 0x93e6d2d3, /// 0xee0
			 0xb09ae668, /// 0xee4
			 0x4246c81e, /// 0xee8
			 0x73e4c746, /// 0xeec
			 0xca034959, /// 0xef0
			 0xa06e890e, /// 0xef4
			 0x882dec07, /// 0xef8
			 0x85becc87, /// 0xefc
			 0xf9c1095f, /// 0xf00
			 0xbb527336, /// 0xf04
			 0xa0e4d7d4, /// 0xf08
			 0x43c21a48, /// 0xf0c
			 0x3ef24d79, /// 0xf10
			 0x50e668ee, /// 0xf14
			 0xd6675cfa, /// 0xf18
			 0xebd85833, /// 0xf1c
			 0xe4503cf3, /// 0xf20
			 0xa0b1925d, /// 0xf24
			 0xcfe6f81c, /// 0xf28
			 0x2e8a609a, /// 0xf2c
			 0xd8665240, /// 0xf30
			 0x107817e1, /// 0xf34
			 0xbce1dbad, /// 0xf38
			 0xc827a7c5, /// 0xf3c
			 0xe749ce05, /// 0xf40
			 0x34b41009, /// 0xf44
			 0x71b2e59f, /// 0xf48
			 0x643a79af, /// 0xf4c
			 0x53d67cdc, /// 0xf50
			 0xa09c6017, /// 0xf54
			 0x652e0646, /// 0xf58
			 0xa91c4ed5, /// 0xf5c
			 0x7af0ae8a, /// 0xf60
			 0xf6ace535, /// 0xf64
			 0xc6593e8c, /// 0xf68
			 0x2e0c7d32, /// 0xf6c
			 0xb9e672e5, /// 0xf70
			 0x11c499bf, /// 0xf74
			 0x1b3ab337, /// 0xf78
			 0x4eda3624, /// 0xf7c
			 0x01686cfd, /// 0xf80
			 0x9cb66863, /// 0xf84
			 0xf643b888, /// 0xf88
			 0xd7dede1f, /// 0xf8c
			 0xce85187d, /// 0xf90
			 0xf2f8fc9e, /// 0xf94
			 0x4edc4002, /// 0xf98
			 0x2f64e22a, /// 0xf9c
			 0x5b4a2dc4, /// 0xfa0
			 0xc6e254f1, /// 0xfa4
			 0xaf263e15, /// 0xfa8
			 0xe3edc498, /// 0xfac
			 0xdd3eb045, /// 0xfb0
			 0x5ad373be, /// 0xfb4
			 0x8557abc6, /// 0xfb8
			 0x394e6bc9, /// 0xfbc
			 0xd8625df1, /// 0xfc0
			 0xfc9e28de, /// 0xfc4
			 0xe850167c, /// 0xfc8
			 0xb70bf26a, /// 0xfcc
			 0x6a8e604a, /// 0xfd0
			 0xb8665ea2, /// 0xfd4
			 0x1b0a3993, /// 0xfd8
			 0x712bb0a0, /// 0xfdc
			 0xe5842a53, /// 0xfe0
			 0xc5331326, /// 0xfe4
			 0xaed1c057, /// 0xfe8
			 0x6bc803be, /// 0xfec
			 0xc339c66c, /// 0xff0
			 0x49ca3908, /// 0xff4
			 0xe1922905, /// 0xff8
			 0x8ef6e65c /// last
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
			 0x00000684,
			 0x00000554,
			 0x00000354,
			 0x00000448,
			 0x00000764,
			 0x000001f8,
			 0x0000040c,
			 0x000000c4,

			 /// vpu register f2
			 0x41b80000,
			 0x41400000,
			 0x41880000,
			 0x41400000,
			 0x41c00000,
			 0x41d00000,
			 0x41c00000,
			 0x41d80000,

			 /// vpu register f3
			 0x41c80000,
			 0x41b00000,
			 0x41000000,
			 0x41000000,
			 0x41a00000,
			 0x41900000,
			 0x40000000,
			 0x40a00000,

			 /// vpu register f4
			 0x41500000,
			 0x40000000,
			 0x41c80000,
			 0x41a80000,
			 0x41d00000,
			 0x41300000,
			 0x41f00000,
			 0x41c80000,

			 /// vpu register f5
			 0x41500000,
			 0x41b80000,
			 0x41b80000,
			 0x41500000,
			 0x41e80000,
			 0x41b00000,
			 0x40800000,
			 0x41a00000,

			 /// vpu register f6
			 0x40400000,
			 0x41a80000,
			 0x41600000,
			 0x41000000,
			 0x40a00000,
			 0x41e00000,
			 0x3f800000,
			 0x41700000,

			 /// vpu register f7
			 0x41d00000,
			 0x40a00000,
			 0x41f00000,
			 0x41800000,
			 0x41b80000,
			 0x41500000,
			 0x41980000,
			 0x41000000,

			 /// vpu register f8
			 0x41700000,
			 0x41980000,
			 0x41b00000,
			 0x40e00000,
			 0x41880000,
			 0x41d00000,
			 0x40a00000,
			 0x41000000,

			 /// vpu register f9
			 0x41a00000,
			 0x41500000,
			 0x3f800000,
			 0x41f00000,
			 0x41c80000,
			 0x41600000,
			 0x41c00000,
			 0x41300000,

			 /// vpu register f10
			 0x40a00000,
			 0x41700000,
			 0x41500000,
			 0x41f80000,
			 0x41980000,
			 0x41f00000,
			 0x41500000,
			 0x41000000,

			 /// vpu register f11
			 0x41980000,
			 0x41700000,
			 0x41980000,
			 0x41700000,
			 0x41d00000,
			 0x41200000,
			 0x40800000,
			 0x41f80000,

			 /// vpu register f12
			 0x41800000,
			 0x41500000,
			 0x41500000,
			 0x41700000,
			 0x40400000,
			 0x41c80000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f13
			 0x41b00000,
			 0x40a00000,
			 0x41400000,
			 0x41e80000,
			 0x41880000,
			 0x40c00000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f14
			 0x41800000,
			 0x41e00000,
			 0x41a80000,
			 0x41f80000,
			 0x41f80000,
			 0x41000000,
			 0x41d00000,
			 0x41200000,

			 /// vpu register f15
			 0x41e80000,
			 0x41c80000,
			 0x41400000,
			 0x41880000,
			 0x41e00000,
			 0x41e00000,
			 0x41c80000,
			 0x41700000,

			 /// vpu register f16
			 0x41c80000,
			 0x40c00000,
			 0x41c00000,
			 0x42000000,
			 0x40e00000,
			 0x41000000,
			 0x40800000,
			 0x41980000,

			 /// vpu register f17
			 0x41600000,
			 0x41a00000,
			 0x41100000,
			 0x40c00000,
			 0x42000000,
			 0x41f00000,
			 0x41600000,
			 0x40e00000,

			 /// vpu register f18
			 0x41e80000,
			 0x41a80000,
			 0x41980000,
			 0x41600000,
			 0x41300000,
			 0x41b00000,
			 0x41b00000,
			 0x41e80000,

			 /// vpu register f19
			 0x41900000,
			 0x41e80000,
			 0x41000000,
			 0x41980000,
			 0x3f800000,
			 0x41880000,
			 0x41a00000,
			 0x41e80000,

			 /// vpu register f20
			 0x41a80000,
			 0x40e00000,
			 0x41f00000,
			 0x41200000,
			 0x40400000,
			 0x41600000,
			 0x41b80000,
			 0x40000000,

			 /// vpu register f21
			 0x41f00000,
			 0x41b00000,
			 0x41000000,
			 0x40400000,
			 0x41880000,
			 0x41400000,
			 0x41e00000,
			 0x40000000,

			 /// vpu register f22
			 0x41600000,
			 0x40a00000,
			 0x41600000,
			 0x41100000,
			 0x41300000,
			 0x41500000,
			 0x41c80000,
			 0x41800000,

			 /// vpu register f23
			 0x41400000,
			 0x41880000,
			 0x41b80000,
			 0x40e00000,
			 0x40400000,
			 0x41400000,
			 0x41b80000,
			 0x3f800000,

			 /// vpu register f24
			 0x41600000,
			 0x41300000,
			 0x41d00000,
			 0x41400000,
			 0x3f800000,
			 0x41d00000,
			 0x40e00000,
			 0x41d80000,

			 /// vpu register f25
			 0x41a80000,
			 0x42000000,
			 0x41b80000,
			 0x41b80000,
			 0x40a00000,
			 0x41800000,
			 0x41a80000,
			 0x41c00000,

			 /// vpu register f26
			 0x41200000,
			 0x41880000,
			 0x40000000,
			 0x41c80000,
			 0x41f80000,
			 0x41800000,
			 0x41b80000,
			 0x41d80000,

			 /// vpu register f27
			 0x40000000,
			 0x40c00000,
			 0x40a00000,
			 0x41c80000,
			 0x40800000,
			 0x41c00000,
			 0x41f80000,
			 0x41a80000,

			 /// vpu register f28
			 0x41d00000,
			 0x41e00000,
			 0x41700000,
			 0x3f800000,
			 0x40e00000,
			 0x41a00000,
			 0x41980000,
			 0x41d00000,

			 /// vpu register f29
			 0x41800000,
			 0x41900000,
			 0x41c00000,
			 0x40400000,
			 0x41c80000,
			 0x41c80000,
			 0x41b80000,
			 0x41800000,

			 /// vpu register f30
			 0x41c00000,
			 0x41d00000,
			 0x40e00000,
			 0x40a00000,
			 0x41e00000,
			 0x41a00000,
			 0x41880000,
			 0x42000000,

			 /// vpu register f31
			 0x41e00000,
			 0x41e00000,
			 0x41b80000,
			 0x41a80000,
			 0x42000000,
			 0x41800000,
			 0x40c00000,
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
		"fsub.s f5, f22, f29, rup\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f19, f1, f2, rdn\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f4, f28, f10, rup\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f18, f22, f26, rdn\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f26, f8, f28, rne\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f3, f24, f17, rdn\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f20, f29, f0, rne\n"                          ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f4, f19, f1, rtz\n"                           ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f18, f29, f7, dyn\n"                          ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f26, f15, f2, rtz\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f25, f21, f24, dyn\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f25, f5, rup\n"                           ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f7, f3, f25, rdn\n"                           ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f19, f21, f25, rup\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f21, f13, f0, rdn\n"                          ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f19, f22, f18, rtz\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f8, f6, dyn\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f4, f26, rdn\n"                           ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f4, f1, f26, rmm\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f1, f17, rmm\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f13, f29, f15, dyn\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f7, f7, f2, rne\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f29, f11, f11, rtz\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f5, f6, f2, rne\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f19, f16, f4, rdn\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f6, f14, f28, rtz\n"                          ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f6, f23, f28, rdn\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f2, f23, f27, rdn\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f18, f4, f0, rmm\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f3, f5, f6, dyn\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f5, f12, f6, rmm\n"                           ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f23, f6, f25, rmm\n"                          ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f19, f24, f0, rtz\n"                          ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f13, f21, f27, rup\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f3, f16, f20, rdn\n"                          ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f15, f10, f11, rup\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f3, f24, f21, dyn\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f14, f24, f30, rup\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f27, f4, rmm\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f11, f19, f7, rup\n"                          ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f4, f0, f15, rmm\n"                           ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f29, f11, f23, rup\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f17, f13, f13, rup\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f29, f22, f9, rdn\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f21, f10, f2, rmm\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f12, f3, f17, rup\n"                          ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f25, f8, f13, rup\n"                          ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f28, f16, f26, rtz\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f12, f19, f16, rmm\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f3, f23, f2, rup\n"                           ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f21, f16, f7, rdn\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f20, f17, f30, rup\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f27, f26, f16, dyn\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f20, f21, dyn\n"                          ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f12, f23, f7, rtz\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f21, f2, f13, rup\n"                          ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f26, f11, f27, rdn\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f27, f21, f29, rup\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f25, f7, f23, rup\n"                          ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f19, f23, f30, rtz\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f29, f2, f14, rmm\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f29, f13, f25, dyn\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f21, f9, f13, rmm\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f25, f14, f11, rmm\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f9, f3, f14, dyn\n"                           ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f10, f7, f14, rmm\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f14, f25, f3, rmm\n"                          ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f12, f18, f7, rdn\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f28, f5, f7, dyn\n"                           ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f21, f17, rmm\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f23, f18, f27, rdn\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f15, f28, f20, rtz\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f25, f27, f28, dyn\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f27, f26, f20, rtz\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f17, f1, f8, rmm\n"                           ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f8, f3, f6, rup\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f2, f3, f0, rdn\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f22, f12, f22, dyn\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f25, f9, f17, rup\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f27, f28, f29, rne\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f5, f4, f21, rne\n"                           ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f17, f6, f22, rne\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f19, f29, f1, rup\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f12, f18, f12, dyn\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f22, f28, f2, dyn\n"                          ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f15, f22, f12, rmm\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f17, f5, f28, rne\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f29, f26, f26, rup\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f12, f27, f3, rne\n"                          ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f9, f16, f1, rne\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f10, f30, f27, rne\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f22, f22, f25, rne\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f26, f15, f4, rtz\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f27, f22, f17, rdn\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f5, f8, f13, rne\n"                           ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f27, f21, f26, rdn\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f25, f26, f16, rmm\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f11, f17, f23, rtz\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.s f20, f29, f28, rmm\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
