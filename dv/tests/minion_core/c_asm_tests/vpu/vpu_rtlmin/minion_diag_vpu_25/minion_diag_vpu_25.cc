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
			 0x32a63836, /// 0x0
			 0x38c792f6, /// 0x4
			 0xe66f2dc1, /// 0x8
			 0x2b971d2d, /// 0xc
			 0x45f8bfe8, /// 0x10
			 0x558f3579, /// 0x14
			 0x74aba5c8, /// 0x18
			 0x7f7aab14, /// 0x1c
			 0xb0caa206, /// 0x20
			 0x609d956d, /// 0x24
			 0x0bbd1ea5, /// 0x28
			 0x86bce309, /// 0x2c
			 0x309aaf0b, /// 0x30
			 0x00524f9d, /// 0x34
			 0x58c84f3a, /// 0x38
			 0x126e1d81, /// 0x3c
			 0xd9efd6cf, /// 0x40
			 0x9ab93f7b, /// 0x44
			 0x7cde4885, /// 0x48
			 0xa6f95ced, /// 0x4c
			 0x9566eceb, /// 0x50
			 0x3ed759f8, /// 0x54
			 0xd1096220, /// 0x58
			 0xa0b5b769, /// 0x5c
			 0x26545300, /// 0x60
			 0x42327847, /// 0x64
			 0x6170cf02, /// 0x68
			 0xb5558277, /// 0x6c
			 0xa4942249, /// 0x70
			 0x2f653df6, /// 0x74
			 0xd8fd7d32, /// 0x78
			 0x6a8399f8, /// 0x7c
			 0xf471cd94, /// 0x80
			 0xc5e8a8b0, /// 0x84
			 0x2061e541, /// 0x88
			 0xa85ccbe6, /// 0x8c
			 0xd0ff63e0, /// 0x90
			 0x88c55f93, /// 0x94
			 0x8d7f2f68, /// 0x98
			 0xa78ecf53, /// 0x9c
			 0x340e9b8c, /// 0xa0
			 0xc010fb6b, /// 0xa4
			 0x7435f889, /// 0xa8
			 0xcc6a4c7f, /// 0xac
			 0x9104e8e3, /// 0xb0
			 0xc56cc738, /// 0xb4
			 0xb6115cdf, /// 0xb8
			 0x302c751c, /// 0xbc
			 0x88eea745, /// 0xc0
			 0x6434393f, /// 0xc4
			 0xbdfbe8fa, /// 0xc8
			 0xec00f32e, /// 0xcc
			 0xd7ec3db8, /// 0xd0
			 0x9477993f, /// 0xd4
			 0x9f9842b3, /// 0xd8
			 0xf8aad0d1, /// 0xdc
			 0xec5774ae, /// 0xe0
			 0xe421d57c, /// 0xe4
			 0x514dfafb, /// 0xe8
			 0x376a1fb0, /// 0xec
			 0x2ca68fc6, /// 0xf0
			 0xa850d801, /// 0xf4
			 0xe709f251, /// 0xf8
			 0x61f0b0de, /// 0xfc
			 0x601dada1, /// 0x100
			 0x8bf6a6ac, /// 0x104
			 0x06e3b8fa, /// 0x108
			 0xc4046b0b, /// 0x10c
			 0x7ad6997a, /// 0x110
			 0x0afc3ec8, /// 0x114
			 0xc2a96944, /// 0x118
			 0xa0c0e1e1, /// 0x11c
			 0x23b19884, /// 0x120
			 0x742d4d87, /// 0x124
			 0xe34c147a, /// 0x128
			 0x09980240, /// 0x12c
			 0xa29a9423, /// 0x130
			 0xae513871, /// 0x134
			 0x23b62504, /// 0x138
			 0x38133bfb, /// 0x13c
			 0xa4056fea, /// 0x140
			 0x3d717c55, /// 0x144
			 0x83565455, /// 0x148
			 0x2e21ec5e, /// 0x14c
			 0x7c41a543, /// 0x150
			 0x7ec3c4f9, /// 0x154
			 0x1660b568, /// 0x158
			 0x1732fdfd, /// 0x15c
			 0x105770fa, /// 0x160
			 0xc2082148, /// 0x164
			 0x80583086, /// 0x168
			 0x617d195b, /// 0x16c
			 0x8592bcff, /// 0x170
			 0x480264a1, /// 0x174
			 0x1676010d, /// 0x178
			 0x6405b31c, /// 0x17c
			 0xe136c5bd, /// 0x180
			 0xfc223350, /// 0x184
			 0xfcfb18ec, /// 0x188
			 0x0e29ce4b, /// 0x18c
			 0xc45a3a6a, /// 0x190
			 0x7c2b3b6c, /// 0x194
			 0xbc25b1fc, /// 0x198
			 0xec6cf314, /// 0x19c
			 0x2847252c, /// 0x1a0
			 0xbb7cd035, /// 0x1a4
			 0x207e9e22, /// 0x1a8
			 0x03d056a8, /// 0x1ac
			 0x034b705b, /// 0x1b0
			 0x5d1cfa9f, /// 0x1b4
			 0x0229d1fb, /// 0x1b8
			 0x4307275b, /// 0x1bc
			 0x4b91bab5, /// 0x1c0
			 0x86c5e422, /// 0x1c4
			 0x88a7c84e, /// 0x1c8
			 0x04987a0a, /// 0x1cc
			 0x5a990687, /// 0x1d0
			 0x57b93d08, /// 0x1d4
			 0x4b425ddb, /// 0x1d8
			 0xeae1aaeb, /// 0x1dc
			 0x6486bdcf, /// 0x1e0
			 0x5d481683, /// 0x1e4
			 0x0b543c5f, /// 0x1e8
			 0x0ed0e4a1, /// 0x1ec
			 0xf3647d8a, /// 0x1f0
			 0x72fb62b5, /// 0x1f4
			 0xd6437dfc, /// 0x1f8
			 0x7b004d7b, /// 0x1fc
			 0x0fadc7fc, /// 0x200
			 0x9980ad4f, /// 0x204
			 0xa9891d4b, /// 0x208
			 0x36b80f81, /// 0x20c
			 0xf13e8aa5, /// 0x210
			 0xb9d8867b, /// 0x214
			 0x051ccc08, /// 0x218
			 0xa8b3824f, /// 0x21c
			 0x6c133c16, /// 0x220
			 0x27b01b13, /// 0x224
			 0x49e2cf77, /// 0x228
			 0xf7e86473, /// 0x22c
			 0x9f83f0d1, /// 0x230
			 0xc68ff990, /// 0x234
			 0x3e3bcf82, /// 0x238
			 0x9e526eb4, /// 0x23c
			 0xcedbac78, /// 0x240
			 0xae5268c5, /// 0x244
			 0xfaa94e0a, /// 0x248
			 0x50d7b551, /// 0x24c
			 0xeb6cc73e, /// 0x250
			 0x860ca5cb, /// 0x254
			 0xf302c22f, /// 0x258
			 0x4ca454e7, /// 0x25c
			 0x69885004, /// 0x260
			 0x70508aaf, /// 0x264
			 0xdb3e5b9c, /// 0x268
			 0xedeb4668, /// 0x26c
			 0xf7e0c22a, /// 0x270
			 0xa7cbedc0, /// 0x274
			 0xb143deae, /// 0x278
			 0xf6e7e324, /// 0x27c
			 0xa9ed4682, /// 0x280
			 0x45e79d4e, /// 0x284
			 0x99ee5b6e, /// 0x288
			 0x81a0ee82, /// 0x28c
			 0x7169e83f, /// 0x290
			 0xda00593b, /// 0x294
			 0x87e25dab, /// 0x298
			 0x7640b321, /// 0x29c
			 0xefc8981a, /// 0x2a0
			 0xf0242527, /// 0x2a4
			 0x88b19067, /// 0x2a8
			 0x9a4aab87, /// 0x2ac
			 0x58da4383, /// 0x2b0
			 0xe8f3b803, /// 0x2b4
			 0x8bb5f02c, /// 0x2b8
			 0x78a9d35c, /// 0x2bc
			 0x1b217933, /// 0x2c0
			 0x435d7b2d, /// 0x2c4
			 0x859bd840, /// 0x2c8
			 0xbb4a3766, /// 0x2cc
			 0x57f0eea3, /// 0x2d0
			 0xdadba124, /// 0x2d4
			 0x775aa735, /// 0x2d8
			 0xba4ad05e, /// 0x2dc
			 0x397cda24, /// 0x2e0
			 0x9499e774, /// 0x2e4
			 0x8a5192a6, /// 0x2e8
			 0xecada689, /// 0x2ec
			 0x235d1d18, /// 0x2f0
			 0x4934e08d, /// 0x2f4
			 0xd1d4c400, /// 0x2f8
			 0xd4fb9e83, /// 0x2fc
			 0x4cd93aaf, /// 0x300
			 0xf400710e, /// 0x304
			 0xac9ebd37, /// 0x308
			 0xde49cd46, /// 0x30c
			 0xdc5d151b, /// 0x310
			 0xed5358be, /// 0x314
			 0x54599b71, /// 0x318
			 0xa5ca20f5, /// 0x31c
			 0x513da25a, /// 0x320
			 0xbbda8289, /// 0x324
			 0x2e35596c, /// 0x328
			 0x4086ef42, /// 0x32c
			 0xf7a18500, /// 0x330
			 0x0bf5e053, /// 0x334
			 0xd40feb14, /// 0x338
			 0x59ac02e2, /// 0x33c
			 0xf585ab83, /// 0x340
			 0x62be80b0, /// 0x344
			 0x50d4e00f, /// 0x348
			 0xe90121c5, /// 0x34c
			 0x166a2fc9, /// 0x350
			 0xe888a8ee, /// 0x354
			 0x15f82115, /// 0x358
			 0xf456acf3, /// 0x35c
			 0x6a0dbb25, /// 0x360
			 0x587767ca, /// 0x364
			 0x40375b85, /// 0x368
			 0x64b253b3, /// 0x36c
			 0xadf2ee8d, /// 0x370
			 0x20a1472a, /// 0x374
			 0x295199dc, /// 0x378
			 0x67b78f05, /// 0x37c
			 0x524dbab5, /// 0x380
			 0x72f9be45, /// 0x384
			 0xe83d2a48, /// 0x388
			 0x92b6150b, /// 0x38c
			 0x78be2176, /// 0x390
			 0x577d6514, /// 0x394
			 0x78351f93, /// 0x398
			 0xd28a8026, /// 0x39c
			 0x883fb037, /// 0x3a0
			 0x1a0a2fb4, /// 0x3a4
			 0x4d850663, /// 0x3a8
			 0x1955e6f3, /// 0x3ac
			 0xe7c25116, /// 0x3b0
			 0x3bb26012, /// 0x3b4
			 0x88c81ab3, /// 0x3b8
			 0x4b49a5ea, /// 0x3bc
			 0x5b2b7024, /// 0x3c0
			 0xdeb6c1ff, /// 0x3c4
			 0x5b72ebe5, /// 0x3c8
			 0x3212b4cb, /// 0x3cc
			 0xe9302cc5, /// 0x3d0
			 0x501b8f4a, /// 0x3d4
			 0x4267459c, /// 0x3d8
			 0xe5d5bb50, /// 0x3dc
			 0xe85009f7, /// 0x3e0
			 0x4782658c, /// 0x3e4
			 0x6d0cede1, /// 0x3e8
			 0xbbc6ce38, /// 0x3ec
			 0x5db81f89, /// 0x3f0
			 0x78afc98f, /// 0x3f4
			 0x95af62bd, /// 0x3f8
			 0xbed57c41, /// 0x3fc
			 0x63dcec92, /// 0x400
			 0x8e46f8a9, /// 0x404
			 0x6ad3a9bc, /// 0x408
			 0x722c12f3, /// 0x40c
			 0x6b9a1c29, /// 0x410
			 0x4a340564, /// 0x414
			 0x533dfe04, /// 0x418
			 0x464ba5b8, /// 0x41c
			 0xaab4edae, /// 0x420
			 0xca157ff3, /// 0x424
			 0xa8576397, /// 0x428
			 0x12795d6d, /// 0x42c
			 0x558d5a00, /// 0x430
			 0xaa78e43a, /// 0x434
			 0xec6e1659, /// 0x438
			 0xde0c9961, /// 0x43c
			 0x4b1f3999, /// 0x440
			 0xa8bd1b56, /// 0x444
			 0xaaa09fbd, /// 0x448
			 0x9b7d09c5, /// 0x44c
			 0xa02234d5, /// 0x450
			 0x02e2e2c1, /// 0x454
			 0x39a21435, /// 0x458
			 0xaa28b20d, /// 0x45c
			 0xfd56bac9, /// 0x460
			 0xc7f45bc6, /// 0x464
			 0xffd82fb3, /// 0x468
			 0x97a81bc0, /// 0x46c
			 0xe6e0aa3d, /// 0x470
			 0x324d4a64, /// 0x474
			 0x5ac87c47, /// 0x478
			 0x406f44a6, /// 0x47c
			 0xec48ca75, /// 0x480
			 0xbcad8052, /// 0x484
			 0x03395c90, /// 0x488
			 0x20ec65e4, /// 0x48c
			 0x6d3241a2, /// 0x490
			 0x0e0c2644, /// 0x494
			 0x2b0bedfb, /// 0x498
			 0x9564df66, /// 0x49c
			 0xac1730b3, /// 0x4a0
			 0xa8acbb95, /// 0x4a4
			 0xc1b22727, /// 0x4a8
			 0x065ca79b, /// 0x4ac
			 0xfe59a6ac, /// 0x4b0
			 0xd546adb3, /// 0x4b4
			 0xdc7e2e51, /// 0x4b8
			 0x924e86cf, /// 0x4bc
			 0xf5f0be6c, /// 0x4c0
			 0xd9039ec4, /// 0x4c4
			 0xb77cda01, /// 0x4c8
			 0x2b1ef8cf, /// 0x4cc
			 0xba7d60df, /// 0x4d0
			 0x58da60cc, /// 0x4d4
			 0x33c7f06c, /// 0x4d8
			 0x9c118b3a, /// 0x4dc
			 0xaf854bb9, /// 0x4e0
			 0x375c5681, /// 0x4e4
			 0x873fb4bc, /// 0x4e8
			 0x345ebd78, /// 0x4ec
			 0x39f8c488, /// 0x4f0
			 0xbdd077bd, /// 0x4f4
			 0x18782e21, /// 0x4f8
			 0xa6400a0f, /// 0x4fc
			 0x64ef00ea, /// 0x500
			 0x45bacdbb, /// 0x504
			 0x5ecc5f03, /// 0x508
			 0xfd9cf598, /// 0x50c
			 0xc5d70e85, /// 0x510
			 0xbd6ddbb3, /// 0x514
			 0xc1084d9b, /// 0x518
			 0x2d8993b1, /// 0x51c
			 0xa797a7e9, /// 0x520
			 0xcaf2b3fc, /// 0x524
			 0xade22311, /// 0x528
			 0x893fa7db, /// 0x52c
			 0x3cf395fd, /// 0x530
			 0xac2e6272, /// 0x534
			 0xe2de45c5, /// 0x538
			 0xd923cf55, /// 0x53c
			 0x78c673d8, /// 0x540
			 0x497fc7d2, /// 0x544
			 0x426a131b, /// 0x548
			 0xee7f847c, /// 0x54c
			 0x203f755f, /// 0x550
			 0xf867d12b, /// 0x554
			 0xe6da37fa, /// 0x558
			 0x70d27285, /// 0x55c
			 0x0492a65e, /// 0x560
			 0x6c28304d, /// 0x564
			 0x4d35fa70, /// 0x568
			 0x8409d22d, /// 0x56c
			 0x35eaba03, /// 0x570
			 0xea1bdacb, /// 0x574
			 0x51fecfc3, /// 0x578
			 0x9bdb74ac, /// 0x57c
			 0xe08353b4, /// 0x580
			 0x483df01b, /// 0x584
			 0xabb3ec01, /// 0x588
			 0x3ffb2c81, /// 0x58c
			 0x6e56398a, /// 0x590
			 0x4b88118e, /// 0x594
			 0x573f7019, /// 0x598
			 0xa370e069, /// 0x59c
			 0x55bb837d, /// 0x5a0
			 0x79c987ad, /// 0x5a4
			 0x8f477687, /// 0x5a8
			 0x6523d763, /// 0x5ac
			 0xaa4c3528, /// 0x5b0
			 0xf1ac2c76, /// 0x5b4
			 0x76dedd9a, /// 0x5b8
			 0xf3bd9bad, /// 0x5bc
			 0x3871a2a7, /// 0x5c0
			 0xd7e5532f, /// 0x5c4
			 0x0ac21f69, /// 0x5c8
			 0x1c494886, /// 0x5cc
			 0x6292a425, /// 0x5d0
			 0xe412bdea, /// 0x5d4
			 0x27f0facc, /// 0x5d8
			 0x70a85fb6, /// 0x5dc
			 0xacd263c8, /// 0x5e0
			 0x3126c3f5, /// 0x5e4
			 0x099dde60, /// 0x5e8
			 0x7004da3c, /// 0x5ec
			 0x3145550a, /// 0x5f0
			 0x4431deb1, /// 0x5f4
			 0x108e8057, /// 0x5f8
			 0xc62e7e0f, /// 0x5fc
			 0xad71ab2d, /// 0x600
			 0x8069c33e, /// 0x604
			 0x2110590f, /// 0x608
			 0x56fcb97a, /// 0x60c
			 0xc385a2ac, /// 0x610
			 0x0a155b86, /// 0x614
			 0x1978b9a5, /// 0x618
			 0x10f9e62a, /// 0x61c
			 0xde1b477a, /// 0x620
			 0x5c245d9e, /// 0x624
			 0x017f9449, /// 0x628
			 0x0f83ae1f, /// 0x62c
			 0xd71a57a4, /// 0x630
			 0x3909701d, /// 0x634
			 0x22b4e191, /// 0x638
			 0x863dea49, /// 0x63c
			 0xf434a613, /// 0x640
			 0x1b8980d8, /// 0x644
			 0x8d45c7da, /// 0x648
			 0xa4fa19fe, /// 0x64c
			 0xc48f4b36, /// 0x650
			 0x7f9f1413, /// 0x654
			 0x4dc00f12, /// 0x658
			 0x12c7f48b, /// 0x65c
			 0x623c4825, /// 0x660
			 0xe644d031, /// 0x664
			 0xcd421092, /// 0x668
			 0x22c233f4, /// 0x66c
			 0xf76e2e92, /// 0x670
			 0x4543e54c, /// 0x674
			 0x4fb3b6d6, /// 0x678
			 0x86f72cff, /// 0x67c
			 0xbf5e7f06, /// 0x680
			 0xda358ab2, /// 0x684
			 0xf5de5a25, /// 0x688
			 0x796bc04d, /// 0x68c
			 0x013bbc3e, /// 0x690
			 0xd2eda075, /// 0x694
			 0x5bdbccff, /// 0x698
			 0x8522708a, /// 0x69c
			 0xf40678b1, /// 0x6a0
			 0x4547ea4c, /// 0x6a4
			 0xd6a1182c, /// 0x6a8
			 0x5ab4e30e, /// 0x6ac
			 0x9027f452, /// 0x6b0
			 0x26a0df0e, /// 0x6b4
			 0x7d15622a, /// 0x6b8
			 0xaebb4319, /// 0x6bc
			 0x20012fe1, /// 0x6c0
			 0x8a44426a, /// 0x6c4
			 0xd89aaca6, /// 0x6c8
			 0xc0f1b0f3, /// 0x6cc
			 0xac8f096b, /// 0x6d0
			 0xf5b62534, /// 0x6d4
			 0x669fae93, /// 0x6d8
			 0x0841cdf2, /// 0x6dc
			 0xfb4f7939, /// 0x6e0
			 0x33698ed3, /// 0x6e4
			 0x4b6b00c4, /// 0x6e8
			 0x3e7283cc, /// 0x6ec
			 0xb996196d, /// 0x6f0
			 0x5a4cc5fa, /// 0x6f4
			 0x2acc7d3d, /// 0x6f8
			 0xe3741c9f, /// 0x6fc
			 0xc0567721, /// 0x700
			 0x3c9c3bdb, /// 0x704
			 0xbf4d958e, /// 0x708
			 0x45c7e9c0, /// 0x70c
			 0x7b7624c9, /// 0x710
			 0x973e640e, /// 0x714
			 0x60ec5179, /// 0x718
			 0x4160214b, /// 0x71c
			 0xef0e888a, /// 0x720
			 0xbe9f414a, /// 0x724
			 0x1da9752e, /// 0x728
			 0x42c201dc, /// 0x72c
			 0x5a7bb2fe, /// 0x730
			 0x9a22f990, /// 0x734
			 0x9810ddc8, /// 0x738
			 0xcd19c1cd, /// 0x73c
			 0x122b8c6e, /// 0x740
			 0xed1c8b6a, /// 0x744
			 0x23645ff7, /// 0x748
			 0xbdf57125, /// 0x74c
			 0x5922d9df, /// 0x750
			 0x3fa4f301, /// 0x754
			 0xc74e5dfc, /// 0x758
			 0xab01a636, /// 0x75c
			 0x196fa814, /// 0x760
			 0xd4441621, /// 0x764
			 0x086be5ab, /// 0x768
			 0x4fa4230a, /// 0x76c
			 0x6b986c10, /// 0x770
			 0xe1bd62a8, /// 0x774
			 0xff7e6ce2, /// 0x778
			 0xcbdce1b9, /// 0x77c
			 0xe3bb7fce, /// 0x780
			 0x32de1dcd, /// 0x784
			 0x321cdeb7, /// 0x788
			 0x8d09a2de, /// 0x78c
			 0xfbd5fdd2, /// 0x790
			 0x07e908df, /// 0x794
			 0xc3e9beb0, /// 0x798
			 0x8f62b33d, /// 0x79c
			 0x3a43bf03, /// 0x7a0
			 0xae9d746c, /// 0x7a4
			 0xfa4049b0, /// 0x7a8
			 0x98e04a35, /// 0x7ac
			 0x1d0004d9, /// 0x7b0
			 0x912a973c, /// 0x7b4
			 0xe61596de, /// 0x7b8
			 0xf6341220, /// 0x7bc
			 0xb0dc1600, /// 0x7c0
			 0xf368a933, /// 0x7c4
			 0xaf5d2eea, /// 0x7c8
			 0xfe7fbc72, /// 0x7cc
			 0x517de1fc, /// 0x7d0
			 0x67ae7388, /// 0x7d4
			 0x9e738f65, /// 0x7d8
			 0xe6a5813c, /// 0x7dc
			 0x5a4365a3, /// 0x7e0
			 0xa24cfdbd, /// 0x7e4
			 0x744cf82b, /// 0x7e8
			 0xf673e91d, /// 0x7ec
			 0xb7e24530, /// 0x7f0
			 0x29d9a0c4, /// 0x7f4
			 0x3cf1c803, /// 0x7f8
			 0x49900332, /// 0x7fc
			 0x9b93fc1a, /// 0x800
			 0xb1fcbf45, /// 0x804
			 0x1ff567dd, /// 0x808
			 0x6f686b02, /// 0x80c
			 0xaccb186f, /// 0x810
			 0x2e1f5d54, /// 0x814
			 0xcfa78187, /// 0x818
			 0xd69ce185, /// 0x81c
			 0x6c5ae7c3, /// 0x820
			 0x6b63b7d5, /// 0x824
			 0x50888c2c, /// 0x828
			 0xe4c20075, /// 0x82c
			 0x71ce2dd9, /// 0x830
			 0x8fdd5284, /// 0x834
			 0xfbabf626, /// 0x838
			 0x937469a5, /// 0x83c
			 0x1d65a7cd, /// 0x840
			 0xc68e5cf1, /// 0x844
			 0x7096c022, /// 0x848
			 0x8d6ae5a1, /// 0x84c
			 0x083f4b68, /// 0x850
			 0x9bcdfef8, /// 0x854
			 0xf6243d55, /// 0x858
			 0xca5ea098, /// 0x85c
			 0x1a787408, /// 0x860
			 0x70fd7da7, /// 0x864
			 0x957baee7, /// 0x868
			 0x37c37724, /// 0x86c
			 0xb762afed, /// 0x870
			 0xf92726dc, /// 0x874
			 0x25c0d9fc, /// 0x878
			 0x0b81c5aa, /// 0x87c
			 0x3bb0a4a6, /// 0x880
			 0x919c5ad3, /// 0x884
			 0x8ceaf3b6, /// 0x888
			 0x6d101d45, /// 0x88c
			 0x3340c27f, /// 0x890
			 0x62feafc7, /// 0x894
			 0x30f8d917, /// 0x898
			 0xf7ef8a23, /// 0x89c
			 0xa2b6115e, /// 0x8a0
			 0x701c9ed2, /// 0x8a4
			 0x7be78f5a, /// 0x8a8
			 0x4765751e, /// 0x8ac
			 0x5caeef2d, /// 0x8b0
			 0xb09e8b20, /// 0x8b4
			 0x50f2941f, /// 0x8b8
			 0xef273d3e, /// 0x8bc
			 0x2abb6256, /// 0x8c0
			 0x83090ed1, /// 0x8c4
			 0xa0f9dd30, /// 0x8c8
			 0xdbde4600, /// 0x8cc
			 0x66d62317, /// 0x8d0
			 0xdce874f4, /// 0x8d4
			 0x62cc0d22, /// 0x8d8
			 0x3fb2f5b1, /// 0x8dc
			 0xe732fe66, /// 0x8e0
			 0x48f2b1a9, /// 0x8e4
			 0x84214203, /// 0x8e8
			 0xdac8d0a1, /// 0x8ec
			 0xa3bd48f0, /// 0x8f0
			 0x1d7227c1, /// 0x8f4
			 0x8ad67c60, /// 0x8f8
			 0x10613442, /// 0x8fc
			 0xab8d16a0, /// 0x900
			 0x6a891702, /// 0x904
			 0x77e5dba2, /// 0x908
			 0x744f6fde, /// 0x90c
			 0xfd0f5d81, /// 0x910
			 0x28671edb, /// 0x914
			 0x23f2cbdc, /// 0x918
			 0xfb40491a, /// 0x91c
			 0x69ace192, /// 0x920
			 0x7d2f0b08, /// 0x924
			 0x41a1b7e1, /// 0x928
			 0xd44a28f0, /// 0x92c
			 0x9d1b16c0, /// 0x930
			 0xad7de5fe, /// 0x934
			 0xbd122f71, /// 0x938
			 0xa431df18, /// 0x93c
			 0x28b63b2c, /// 0x940
			 0x5dbafda4, /// 0x944
			 0x2e898c07, /// 0x948
			 0xe4d51a84, /// 0x94c
			 0xab45e6ca, /// 0x950
			 0xb8a076cb, /// 0x954
			 0x72d4eb75, /// 0x958
			 0xc710a321, /// 0x95c
			 0x912e0d16, /// 0x960
			 0xdf965ea2, /// 0x964
			 0x97a5c3d0, /// 0x968
			 0x35b8193f, /// 0x96c
			 0x059ed4d7, /// 0x970
			 0xf075b2f7, /// 0x974
			 0x0c329c52, /// 0x978
			 0xd75f5ea6, /// 0x97c
			 0xf5b33642, /// 0x980
			 0xcc861326, /// 0x984
			 0x156ca1a5, /// 0x988
			 0xeb900879, /// 0x98c
			 0x6d3d1f06, /// 0x990
			 0x44fac523, /// 0x994
			 0x00191f99, /// 0x998
			 0x40f9d615, /// 0x99c
			 0x5fd0991a, /// 0x9a0
			 0x21702193, /// 0x9a4
			 0xed01bb54, /// 0x9a8
			 0x3b4898dc, /// 0x9ac
			 0x996851f7, /// 0x9b0
			 0xb23290de, /// 0x9b4
			 0x873b341e, /// 0x9b8
			 0x32f18447, /// 0x9bc
			 0x793fb7b0, /// 0x9c0
			 0xbcf7fa5f, /// 0x9c4
			 0x82d7532c, /// 0x9c8
			 0x915c644a, /// 0x9cc
			 0xf849a623, /// 0x9d0
			 0xa13e03af, /// 0x9d4
			 0x31596043, /// 0x9d8
			 0x3b7d6537, /// 0x9dc
			 0x6b5ebdc3, /// 0x9e0
			 0xd49049b2, /// 0x9e4
			 0xc4e43d3c, /// 0x9e8
			 0xd0a5307b, /// 0x9ec
			 0x58771e4b, /// 0x9f0
			 0x205a8571, /// 0x9f4
			 0x7f25cfaa, /// 0x9f8
			 0x35d42457, /// 0x9fc
			 0x0342505a, /// 0xa00
			 0x5afdd3cc, /// 0xa04
			 0x41268252, /// 0xa08
			 0xf4877dbe, /// 0xa0c
			 0x1ebe0d5e, /// 0xa10
			 0xb5f86864, /// 0xa14
			 0x8f4d6231, /// 0xa18
			 0x54e95a45, /// 0xa1c
			 0xc9a21d58, /// 0xa20
			 0xd95ea70e, /// 0xa24
			 0x09753ab0, /// 0xa28
			 0x6f32c2ee, /// 0xa2c
			 0x260de12b, /// 0xa30
			 0x56899f5a, /// 0xa34
			 0x58e099b8, /// 0xa38
			 0x446201cd, /// 0xa3c
			 0xef97c22d, /// 0xa40
			 0xb7331fe0, /// 0xa44
			 0x7a945e8c, /// 0xa48
			 0xaf91a206, /// 0xa4c
			 0x19a7fead, /// 0xa50
			 0x748c22dc, /// 0xa54
			 0x56e8fb3b, /// 0xa58
			 0x8245b8c7, /// 0xa5c
			 0xac4e86ae, /// 0xa60
			 0x5365402a, /// 0xa64
			 0x7447151f, /// 0xa68
			 0x6d7376df, /// 0xa6c
			 0x8143d3ae, /// 0xa70
			 0xe3fad7a0, /// 0xa74
			 0x0e2e8b3f, /// 0xa78
			 0xa099d739, /// 0xa7c
			 0xcc4f4d5e, /// 0xa80
			 0x8cb8ac7e, /// 0xa84
			 0x0ce8932e, /// 0xa88
			 0x6f7ff5ff, /// 0xa8c
			 0xf7422b9b, /// 0xa90
			 0x7c16ee5e, /// 0xa94
			 0x9f432edb, /// 0xa98
			 0x763ded7d, /// 0xa9c
			 0xbd5319f0, /// 0xaa0
			 0x8a971d2e, /// 0xaa4
			 0x1d28efea, /// 0xaa8
			 0xd839db0e, /// 0xaac
			 0xc18bab42, /// 0xab0
			 0x95fa19a9, /// 0xab4
			 0x7c03d0f4, /// 0xab8
			 0x23b6c02c, /// 0xabc
			 0x6a939685, /// 0xac0
			 0xb2d72c9b, /// 0xac4
			 0x36170f03, /// 0xac8
			 0x78e39614, /// 0xacc
			 0x194f7a50, /// 0xad0
			 0x240cc3fe, /// 0xad4
			 0x710a0dad, /// 0xad8
			 0xe3968872, /// 0xadc
			 0xf9ffcedc, /// 0xae0
			 0x2a083f48, /// 0xae4
			 0xab5e0b6a, /// 0xae8
			 0x65c3b618, /// 0xaec
			 0xab64e4f6, /// 0xaf0
			 0x80b98de4, /// 0xaf4
			 0xa575f61a, /// 0xaf8
			 0xa7161872, /// 0xafc
			 0xcc8c3d19, /// 0xb00
			 0xf8b74eb8, /// 0xb04
			 0x6b7ec7f6, /// 0xb08
			 0x174114bc, /// 0xb0c
			 0x10020b9b, /// 0xb10
			 0xec5e9a2a, /// 0xb14
			 0x19433b55, /// 0xb18
			 0x8b9456fd, /// 0xb1c
			 0x559758c5, /// 0xb20
			 0x91ab95c4, /// 0xb24
			 0x2098c4e9, /// 0xb28
			 0x83b30156, /// 0xb2c
			 0x3c042aee, /// 0xb30
			 0x7e3b8fb3, /// 0xb34
			 0x677936e9, /// 0xb38
			 0x3fecbe61, /// 0xb3c
			 0x7ce87321, /// 0xb40
			 0x1f6ee3a8, /// 0xb44
			 0x21a96c46, /// 0xb48
			 0xdd62b49e, /// 0xb4c
			 0xb92d03dd, /// 0xb50
			 0x17b8fd4d, /// 0xb54
			 0xa04b2cd6, /// 0xb58
			 0xc6c73315, /// 0xb5c
			 0x74e9f463, /// 0xb60
			 0xd36b7843, /// 0xb64
			 0xf776b858, /// 0xb68
			 0x71282101, /// 0xb6c
			 0x140a42b5, /// 0xb70
			 0xa5b31157, /// 0xb74
			 0x1c53704f, /// 0xb78
			 0x73c2cfe6, /// 0xb7c
			 0x3f4ed066, /// 0xb80
			 0x9e0452a9, /// 0xb84
			 0x511fbba4, /// 0xb88
			 0x072afa3f, /// 0xb8c
			 0x5440ee02, /// 0xb90
			 0x83a405bc, /// 0xb94
			 0x63c504b9, /// 0xb98
			 0x13361bf5, /// 0xb9c
			 0x4d96c836, /// 0xba0
			 0x5a3b39c2, /// 0xba4
			 0xb5f7f61b, /// 0xba8
			 0x9e5de984, /// 0xbac
			 0xc1b6853f, /// 0xbb0
			 0x7c4aae21, /// 0xbb4
			 0xb5754ae7, /// 0xbb8
			 0xf070e17f, /// 0xbbc
			 0xf58952f3, /// 0xbc0
			 0x84ee34f1, /// 0xbc4
			 0x4b6d00d4, /// 0xbc8
			 0xf12598a8, /// 0xbcc
			 0x8cdbcb67, /// 0xbd0
			 0xca968a51, /// 0xbd4
			 0xb4e32414, /// 0xbd8
			 0x3bb585ea, /// 0xbdc
			 0x9e85340c, /// 0xbe0
			 0x14533a36, /// 0xbe4
			 0x550f71c1, /// 0xbe8
			 0x2641cab9, /// 0xbec
			 0xe366b652, /// 0xbf0
			 0x2d75cf0f, /// 0xbf4
			 0xe4da4de7, /// 0xbf8
			 0x2b7908b6, /// 0xbfc
			 0xd04df066, /// 0xc00
			 0x1d1e385b, /// 0xc04
			 0xee2923e0, /// 0xc08
			 0xd69279f4, /// 0xc0c
			 0x01921e32, /// 0xc10
			 0x592524b6, /// 0xc14
			 0x498cc92c, /// 0xc18
			 0x65bcf2ae, /// 0xc1c
			 0x470b3371, /// 0xc20
			 0xc860a8e7, /// 0xc24
			 0xf0a6b27d, /// 0xc28
			 0x79f1103f, /// 0xc2c
			 0x284a7208, /// 0xc30
			 0x98335541, /// 0xc34
			 0x39d91473, /// 0xc38
			 0x7e0aa790, /// 0xc3c
			 0x22573554, /// 0xc40
			 0x10c668da, /// 0xc44
			 0x6bc72f73, /// 0xc48
			 0xcb889348, /// 0xc4c
			 0xd6aa1596, /// 0xc50
			 0xc90ef189, /// 0xc54
			 0xf2585f51, /// 0xc58
			 0xd72f5b92, /// 0xc5c
			 0xe7fa591f, /// 0xc60
			 0x36ec7ed9, /// 0xc64
			 0x19d69567, /// 0xc68
			 0x7322c12b, /// 0xc6c
			 0x86231fe8, /// 0xc70
			 0x4808412e, /// 0xc74
			 0xbaede0d7, /// 0xc78
			 0x2dd17d2f, /// 0xc7c
			 0x91f433dd, /// 0xc80
			 0x2304d26f, /// 0xc84
			 0x2e471795, /// 0xc88
			 0x2f6ea880, /// 0xc8c
			 0xa73dd2f7, /// 0xc90
			 0xa4b1ef73, /// 0xc94
			 0xfaca97c6, /// 0xc98
			 0x9f1cce06, /// 0xc9c
			 0xbc6e6e8c, /// 0xca0
			 0xbf22b100, /// 0xca4
			 0x01430bcd, /// 0xca8
			 0x974ddfc5, /// 0xcac
			 0x8c4fb1ff, /// 0xcb0
			 0xb8fde474, /// 0xcb4
			 0x0eed4f63, /// 0xcb8
			 0xa1a3f322, /// 0xcbc
			 0x013c1190, /// 0xcc0
			 0x5900c0ee, /// 0xcc4
			 0x852d51e5, /// 0xcc8
			 0x2a5b2b03, /// 0xccc
			 0x09f7bed6, /// 0xcd0
			 0x63e7e7ca, /// 0xcd4
			 0x9376b587, /// 0xcd8
			 0xe766737f, /// 0xcdc
			 0xd3d14ed8, /// 0xce0
			 0x91ac57c3, /// 0xce4
			 0x66ce55da, /// 0xce8
			 0x150d3859, /// 0xcec
			 0x7aa756f5, /// 0xcf0
			 0xc7c2590e, /// 0xcf4
			 0x39f3ffa1, /// 0xcf8
			 0x94db7327, /// 0xcfc
			 0xc922de8b, /// 0xd00
			 0x64a4d514, /// 0xd04
			 0x5b9de4f3, /// 0xd08
			 0x951392f6, /// 0xd0c
			 0xa3ec2ace, /// 0xd10
			 0xbccea2b3, /// 0xd14
			 0x9d116ca2, /// 0xd18
			 0x436be93c, /// 0xd1c
			 0x492deeb6, /// 0xd20
			 0x8ed8cd35, /// 0xd24
			 0xd8521525, /// 0xd28
			 0xf24c050c, /// 0xd2c
			 0x0051a4b9, /// 0xd30
			 0xa6996044, /// 0xd34
			 0xf16fa398, /// 0xd38
			 0xac63079e, /// 0xd3c
			 0x73a81722, /// 0xd40
			 0xe9c5cbbd, /// 0xd44
			 0x4487324d, /// 0xd48
			 0x1c83c803, /// 0xd4c
			 0x5279bef7, /// 0xd50
			 0x31a14d2b, /// 0xd54
			 0xc82bfd5d, /// 0xd58
			 0x096d90ad, /// 0xd5c
			 0xb9b947c7, /// 0xd60
			 0xf8b20fb6, /// 0xd64
			 0x17cbfff6, /// 0xd68
			 0x3e2a2c8d, /// 0xd6c
			 0xe6b129e0, /// 0xd70
			 0x7716a5f5, /// 0xd74
			 0xac19b697, /// 0xd78
			 0xd702d333, /// 0xd7c
			 0xf31619a9, /// 0xd80
			 0x9594391a, /// 0xd84
			 0x2b233742, /// 0xd88
			 0xec3a0857, /// 0xd8c
			 0xf2d3ffb4, /// 0xd90
			 0x2d5c14b8, /// 0xd94
			 0xaba9d519, /// 0xd98
			 0xd2526b21, /// 0xd9c
			 0x5c781aeb, /// 0xda0
			 0x72ee26f6, /// 0xda4
			 0xb17e32c5, /// 0xda8
			 0x52f716de, /// 0xdac
			 0x2c760d3b, /// 0xdb0
			 0x79d16406, /// 0xdb4
			 0xab6591ae, /// 0xdb8
			 0xf4d3e543, /// 0xdbc
			 0x51a67cee, /// 0xdc0
			 0x9945c72f, /// 0xdc4
			 0x9a229dde, /// 0xdc8
			 0x9a659c9b, /// 0xdcc
			 0x73596218, /// 0xdd0
			 0x92d85593, /// 0xdd4
			 0x20eef1b0, /// 0xdd8
			 0x8821d39c, /// 0xddc
			 0x5a22040d, /// 0xde0
			 0x575d5eca, /// 0xde4
			 0xcd683f53, /// 0xde8
			 0xa9bde3bc, /// 0xdec
			 0x2b0693ff, /// 0xdf0
			 0x4fc643da, /// 0xdf4
			 0x49cca9cd, /// 0xdf8
			 0x6158a11b, /// 0xdfc
			 0xa7b475e2, /// 0xe00
			 0xbb44845c, /// 0xe04
			 0xc8d2279d, /// 0xe08
			 0x2781b24f, /// 0xe0c
			 0x705a9ceb, /// 0xe10
			 0x6eb17f86, /// 0xe14
			 0x9a65ba73, /// 0xe18
			 0x50917792, /// 0xe1c
			 0x5c736ee3, /// 0xe20
			 0x31f73b0b, /// 0xe24
			 0x6241de65, /// 0xe28
			 0x6a70e5c4, /// 0xe2c
			 0xefa6f6d5, /// 0xe30
			 0x00ac290b, /// 0xe34
			 0xbcdff5d0, /// 0xe38
			 0xbf83eb56, /// 0xe3c
			 0x0257b181, /// 0xe40
			 0xedc0e2c0, /// 0xe44
			 0x40b3edd7, /// 0xe48
			 0xb9403340, /// 0xe4c
			 0x910462c1, /// 0xe50
			 0x8dd8dd71, /// 0xe54
			 0xfacab9ef, /// 0xe58
			 0xb5d44796, /// 0xe5c
			 0xdb75ee4a, /// 0xe60
			 0xbf718d3d, /// 0xe64
			 0xcbcbb2da, /// 0xe68
			 0x9f0f5607, /// 0xe6c
			 0x03df6c00, /// 0xe70
			 0x548f9a7c, /// 0xe74
			 0x477b8538, /// 0xe78
			 0xa8179f2a, /// 0xe7c
			 0x3b8f2c88, /// 0xe80
			 0x2d3ae26b, /// 0xe84
			 0x3c933877, /// 0xe88
			 0x6fc14d80, /// 0xe8c
			 0xfc188144, /// 0xe90
			 0x73ebb4af, /// 0xe94
			 0x00109421, /// 0xe98
			 0x30b5cc06, /// 0xe9c
			 0x837f286e, /// 0xea0
			 0x95bca965, /// 0xea4
			 0xf6481684, /// 0xea8
			 0x217d2792, /// 0xeac
			 0xc5a543c8, /// 0xeb0
			 0x3fcbf9b9, /// 0xeb4
			 0x7f131052, /// 0xeb8
			 0x4404a323, /// 0xebc
			 0xe483574e, /// 0xec0
			 0x9dd14bfa, /// 0xec4
			 0x26683d78, /// 0xec8
			 0xc8dede88, /// 0xecc
			 0xd0d517d7, /// 0xed0
			 0xf7f175ee, /// 0xed4
			 0x2765794c, /// 0xed8
			 0xfa93860b, /// 0xedc
			 0x16faf807, /// 0xee0
			 0xa79c74b9, /// 0xee4
			 0xb9ed523a, /// 0xee8
			 0xb3294d47, /// 0xeec
			 0xe9321fb9, /// 0xef0
			 0x1ff0bb61, /// 0xef4
			 0xad2c4305, /// 0xef8
			 0x3f743966, /// 0xefc
			 0x850d1212, /// 0xf00
			 0x37925ad6, /// 0xf04
			 0xebfb4dfe, /// 0xf08
			 0xaae1ae25, /// 0xf0c
			 0xdf38e7ef, /// 0xf10
			 0x70387057, /// 0xf14
			 0xfe6d9491, /// 0xf18
			 0xe88c7255, /// 0xf1c
			 0x3c684b13, /// 0xf20
			 0x68fc39dd, /// 0xf24
			 0x5e7eb917, /// 0xf28
			 0x0d676d11, /// 0xf2c
			 0x76508959, /// 0xf30
			 0xb27a3840, /// 0xf34
			 0xf30aaf01, /// 0xf38
			 0xb2d479b2, /// 0xf3c
			 0x761d55d7, /// 0xf40
			 0xe4734aeb, /// 0xf44
			 0x342ee1d4, /// 0xf48
			 0xfb4c8e40, /// 0xf4c
			 0x16a515cb, /// 0xf50
			 0xfcf4ea1b, /// 0xf54
			 0xa38af94f, /// 0xf58
			 0xf7697c6a, /// 0xf5c
			 0x0c912f0a, /// 0xf60
			 0xe97da9df, /// 0xf64
			 0x8c935446, /// 0xf68
			 0x3a94c66d, /// 0xf6c
			 0x6924d3f6, /// 0xf70
			 0x384537f7, /// 0xf74
			 0x5c8b4d07, /// 0xf78
			 0xfdb47c38, /// 0xf7c
			 0xe838afb1, /// 0xf80
			 0xec1fd2dd, /// 0xf84
			 0x41729f76, /// 0xf88
			 0xd44c0e3c, /// 0xf8c
			 0x34e9a232, /// 0xf90
			 0x45cd3bbb, /// 0xf94
			 0x937033c0, /// 0xf98
			 0x13bdd6ff, /// 0xf9c
			 0x366eed91, /// 0xfa0
			 0x37dc5fa3, /// 0xfa4
			 0xe2680bd2, /// 0xfa8
			 0x3ef43ad2, /// 0xfac
			 0x697e1292, /// 0xfb0
			 0xbe9164ef, /// 0xfb4
			 0x313980ed, /// 0xfb8
			 0x5e317dbb, /// 0xfbc
			 0x38e339bd, /// 0xfc0
			 0xe09c30a4, /// 0xfc4
			 0x73c9e363, /// 0xfc8
			 0x9cf845b8, /// 0xfcc
			 0x3a446f57, /// 0xfd0
			 0x5fcd00fd, /// 0xfd4
			 0x803946e7, /// 0xfd8
			 0x20f44341, /// 0xfdc
			 0x9da5bc3d, /// 0xfe0
			 0x864206da, /// 0xfe4
			 0xf73a4d6e, /// 0xfe8
			 0x2b9d353b, /// 0xfec
			 0x2ff11643, /// 0xff0
			 0x65d1abbf, /// 0xff4
			 0x4465ac03, /// 0xff8
			 0x4624f1e1 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xd2b9c24d, /// 0x0
			 0x4285b73d, /// 0x4
			 0xda9ddc4c, /// 0x8
			 0xb9478960, /// 0xc
			 0x0c6b6206, /// 0x10
			 0x18b41fe7, /// 0x14
			 0xc8eaf8a1, /// 0x18
			 0x454023c0, /// 0x1c
			 0x681378c2, /// 0x20
			 0xc2f50ade, /// 0x24
			 0x73176fa7, /// 0x28
			 0xddf2caa5, /// 0x2c
			 0x6a82b72f, /// 0x30
			 0x2097ba8a, /// 0x34
			 0x7d484a2c, /// 0x38
			 0x4c724c52, /// 0x3c
			 0x46fd48c9, /// 0x40
			 0x95c40b8f, /// 0x44
			 0xdd614ccc, /// 0x48
			 0x115c1a8f, /// 0x4c
			 0x26e8fffc, /// 0x50
			 0x5ac06e8f, /// 0x54
			 0xef3afdd1, /// 0x58
			 0x00d976ea, /// 0x5c
			 0x2736af1b, /// 0x60
			 0x230c9330, /// 0x64
			 0x15276729, /// 0x68
			 0xd2daf456, /// 0x6c
			 0x872f40ad, /// 0x70
			 0x2bd80372, /// 0x74
			 0xaf2ce4ba, /// 0x78
			 0x0400ea4c, /// 0x7c
			 0x1d0b73c4, /// 0x80
			 0x42f7ccb4, /// 0x84
			 0xa941027e, /// 0x88
			 0x7dccc5b6, /// 0x8c
			 0xf55204bc, /// 0x90
			 0x210c6dd5, /// 0x94
			 0x68cb1550, /// 0x98
			 0xb3121372, /// 0x9c
			 0x63ddcf50, /// 0xa0
			 0x7734a3cd, /// 0xa4
			 0xfbaef555, /// 0xa8
			 0x28fb9e8f, /// 0xac
			 0x4aea3d2c, /// 0xb0
			 0x13902ff3, /// 0xb4
			 0xf388d38f, /// 0xb8
			 0xfb8d2f25, /// 0xbc
			 0x89920022, /// 0xc0
			 0x1895da98, /// 0xc4
			 0x84b94738, /// 0xc8
			 0x329f3e35, /// 0xcc
			 0xd1c08491, /// 0xd0
			 0xc0b1f0c8, /// 0xd4
			 0x0b552f72, /// 0xd8
			 0x6bf85d87, /// 0xdc
			 0x34ca05ef, /// 0xe0
			 0x662b41c3, /// 0xe4
			 0x7c3292fc, /// 0xe8
			 0xc807815b, /// 0xec
			 0x94c93bf7, /// 0xf0
			 0x7d0b13f0, /// 0xf4
			 0x865d4bdb, /// 0xf8
			 0x96cd8282, /// 0xfc
			 0xdce0da79, /// 0x100
			 0xacb0f3dd, /// 0x104
			 0xf052ed33, /// 0x108
			 0x84222ed5, /// 0x10c
			 0x2638bddd, /// 0x110
			 0x4fb93121, /// 0x114
			 0xc397644d, /// 0x118
			 0x2fdf86a0, /// 0x11c
			 0x7fed1ebf, /// 0x120
			 0x67b21913, /// 0x124
			 0x6ec8fcd4, /// 0x128
			 0xf2f788b4, /// 0x12c
			 0xc76a2637, /// 0x130
			 0xbd5aba76, /// 0x134
			 0xaa2c6727, /// 0x138
			 0x71ea4630, /// 0x13c
			 0x8de14e40, /// 0x140
			 0x48078367, /// 0x144
			 0xe12fc568, /// 0x148
			 0x05049233, /// 0x14c
			 0xe10cb8b1, /// 0x150
			 0x22f5b6e7, /// 0x154
			 0x2f0e698e, /// 0x158
			 0x95ecd7a8, /// 0x15c
			 0xdc90f102, /// 0x160
			 0x25381f76, /// 0x164
			 0x5e189700, /// 0x168
			 0x3782a0f6, /// 0x16c
			 0x70f3ec05, /// 0x170
			 0xf9cf59a1, /// 0x174
			 0x575e637c, /// 0x178
			 0xe86eeff5, /// 0x17c
			 0xcbb7e128, /// 0x180
			 0x0ebc846a, /// 0x184
			 0x36d12be2, /// 0x188
			 0x80a1b0fc, /// 0x18c
			 0xcad7193f, /// 0x190
			 0x96b5a98f, /// 0x194
			 0x93623813, /// 0x198
			 0xb33d6102, /// 0x19c
			 0xc135948e, /// 0x1a0
			 0x04042c8b, /// 0x1a4
			 0x69fe9fd5, /// 0x1a8
			 0x291816be, /// 0x1ac
			 0x28a73ec6, /// 0x1b0
			 0x4fd7417f, /// 0x1b4
			 0xef43f52e, /// 0x1b8
			 0xa7f25782, /// 0x1bc
			 0xe390b5e6, /// 0x1c0
			 0x0625c66c, /// 0x1c4
			 0xafd0374a, /// 0x1c8
			 0x848b610c, /// 0x1cc
			 0x0fff7da6, /// 0x1d0
			 0x7c447175, /// 0x1d4
			 0x8c315376, /// 0x1d8
			 0xa1214694, /// 0x1dc
			 0xdf4456e2, /// 0x1e0
			 0x601ffd3b, /// 0x1e4
			 0xd32c7a33, /// 0x1e8
			 0x3b33bd39, /// 0x1ec
			 0x8f7bdb75, /// 0x1f0
			 0x6fdc3ec1, /// 0x1f4
			 0xa255ca1a, /// 0x1f8
			 0x6ab3ba49, /// 0x1fc
			 0x93a6ffdc, /// 0x200
			 0x0071c47f, /// 0x204
			 0xbf34a7c6, /// 0x208
			 0xda4d5bac, /// 0x20c
			 0x6303eb04, /// 0x210
			 0x853e306c, /// 0x214
			 0x9d992070, /// 0x218
			 0xdd10b41e, /// 0x21c
			 0x13ee2530, /// 0x220
			 0x26951cc7, /// 0x224
			 0x9c6df8f2, /// 0x228
			 0x2aefaa2d, /// 0x22c
			 0x54c7799d, /// 0x230
			 0xd2fdc59a, /// 0x234
			 0xca15c072, /// 0x238
			 0x3c8da0dc, /// 0x23c
			 0x14d1a15e, /// 0x240
			 0x8b49d542, /// 0x244
			 0xc4fc02f1, /// 0x248
			 0xc02c5c7d, /// 0x24c
			 0xe05d501c, /// 0x250
			 0x3c349d58, /// 0x254
			 0x3183f113, /// 0x258
			 0xc857bdd2, /// 0x25c
			 0xa66686b0, /// 0x260
			 0x32215524, /// 0x264
			 0x8c95d2a6, /// 0x268
			 0xd435a231, /// 0x26c
			 0xe6fd27af, /// 0x270
			 0x4e128cac, /// 0x274
			 0x712ccd33, /// 0x278
			 0x77683943, /// 0x27c
			 0x672d9969, /// 0x280
			 0x94a69408, /// 0x284
			 0x3186da0b, /// 0x288
			 0xdde791dd, /// 0x28c
			 0x2b70abb2, /// 0x290
			 0x8699be95, /// 0x294
			 0x8a1230d9, /// 0x298
			 0x12b66f4e, /// 0x29c
			 0x0c770527, /// 0x2a0
			 0xc90476d8, /// 0x2a4
			 0x2b937d5c, /// 0x2a8
			 0xe98332b6, /// 0x2ac
			 0x6c797088, /// 0x2b0
			 0xbfcd459e, /// 0x2b4
			 0x076faa6f, /// 0x2b8
			 0xe876887e, /// 0x2bc
			 0x17d99b33, /// 0x2c0
			 0xc862140c, /// 0x2c4
			 0xf3a68bfc, /// 0x2c8
			 0xc7f222cd, /// 0x2cc
			 0x67775a3b, /// 0x2d0
			 0x49adf76a, /// 0x2d4
			 0xad462de4, /// 0x2d8
			 0x1b164cb2, /// 0x2dc
			 0xae861e92, /// 0x2e0
			 0xea4404c1, /// 0x2e4
			 0xcc374d8e, /// 0x2e8
			 0x8dea726c, /// 0x2ec
			 0x437ba681, /// 0x2f0
			 0xd65a1c50, /// 0x2f4
			 0x7dddc497, /// 0x2f8
			 0x4cde2619, /// 0x2fc
			 0xeedea01f, /// 0x300
			 0xccfe6177, /// 0x304
			 0x7d9c0acd, /// 0x308
			 0x2c1b2c6b, /// 0x30c
			 0xe15145f0, /// 0x310
			 0x908d9a75, /// 0x314
			 0xee3fc95a, /// 0x318
			 0x1b956aad, /// 0x31c
			 0x6cca710a, /// 0x320
			 0x07267c2b, /// 0x324
			 0xe7913262, /// 0x328
			 0xc05b6a37, /// 0x32c
			 0x73f15a45, /// 0x330
			 0xc70bb2eb, /// 0x334
			 0xb7c9b61e, /// 0x338
			 0xd07f0a02, /// 0x33c
			 0x224a3526, /// 0x340
			 0x5311cdbd, /// 0x344
			 0x50872ef2, /// 0x348
			 0xf36ac95f, /// 0x34c
			 0x07c3a6d7, /// 0x350
			 0xbf975212, /// 0x354
			 0xba33d2a2, /// 0x358
			 0x772d8bfa, /// 0x35c
			 0x4f05eab3, /// 0x360
			 0xfe99a75e, /// 0x364
			 0xb4cd39c7, /// 0x368
			 0x37ef6e49, /// 0x36c
			 0xab29172d, /// 0x370
			 0xab816da4, /// 0x374
			 0x31708c89, /// 0x378
			 0x9ab9cff4, /// 0x37c
			 0xe1099df6, /// 0x380
			 0xb849b663, /// 0x384
			 0x49d654f1, /// 0x388
			 0x682a30f1, /// 0x38c
			 0x25db5ec2, /// 0x390
			 0x261b5f06, /// 0x394
			 0x112f9cc6, /// 0x398
			 0x001c12e4, /// 0x39c
			 0xc78a9f7a, /// 0x3a0
			 0xe682b06d, /// 0x3a4
			 0x1e55a670, /// 0x3a8
			 0xb132bfbb, /// 0x3ac
			 0x128894b9, /// 0x3b0
			 0x378f826b, /// 0x3b4
			 0x23fa58f8, /// 0x3b8
			 0xc88856e9, /// 0x3bc
			 0x9acf0898, /// 0x3c0
			 0x78747939, /// 0x3c4
			 0xd87a0617, /// 0x3c8
			 0xde30305b, /// 0x3cc
			 0xeb9c4522, /// 0x3d0
			 0x02d5e113, /// 0x3d4
			 0xbb5b1391, /// 0x3d8
			 0x8bb7fb0c, /// 0x3dc
			 0x47065a6b, /// 0x3e0
			 0xbf892071, /// 0x3e4
			 0x27a76cdb, /// 0x3e8
			 0xbfc47054, /// 0x3ec
			 0x538176e2, /// 0x3f0
			 0xe5bb487b, /// 0x3f4
			 0x1ccee550, /// 0x3f8
			 0x453c3e2c, /// 0x3fc
			 0x1d07e452, /// 0x400
			 0xf110cdfd, /// 0x404
			 0x47a4631c, /// 0x408
			 0x843f39e2, /// 0x40c
			 0x780f7284, /// 0x410
			 0xd6f3bf8e, /// 0x414
			 0x6e81d448, /// 0x418
			 0x851ab43f, /// 0x41c
			 0x9db3ce2e, /// 0x420
			 0xf72655dc, /// 0x424
			 0x0afb0d75, /// 0x428
			 0x7a8bfd5f, /// 0x42c
			 0xefb684c8, /// 0x430
			 0x96c41b2d, /// 0x434
			 0x95758b19, /// 0x438
			 0x87ee258c, /// 0x43c
			 0x23ae6141, /// 0x440
			 0xe467c21e, /// 0x444
			 0xea9d2245, /// 0x448
			 0x0cd83fc6, /// 0x44c
			 0xf47692b1, /// 0x450
			 0x23b83f76, /// 0x454
			 0x28f39abf, /// 0x458
			 0x96ab7c4c, /// 0x45c
			 0x279f5a04, /// 0x460
			 0x9cc388b4, /// 0x464
			 0xbb1c820b, /// 0x468
			 0xd8b8ed68, /// 0x46c
			 0x98ad5b49, /// 0x470
			 0x1b439434, /// 0x474
			 0xb890c68c, /// 0x478
			 0xd7f05749, /// 0x47c
			 0xa8dd77d0, /// 0x480
			 0xf136c5e7, /// 0x484
			 0x198b69b5, /// 0x488
			 0xc2ca6c19, /// 0x48c
			 0x1400caa2, /// 0x490
			 0xb3ac4e46, /// 0x494
			 0x606a0493, /// 0x498
			 0x71bf1094, /// 0x49c
			 0x164b3af8, /// 0x4a0
			 0x6c159589, /// 0x4a4
			 0xa2f7f7b9, /// 0x4a8
			 0x61dd3403, /// 0x4ac
			 0xabeb6e59, /// 0x4b0
			 0xe79b3953, /// 0x4b4
			 0xb7568116, /// 0x4b8
			 0x70490d3b, /// 0x4bc
			 0xe456c166, /// 0x4c0
			 0x772f2b12, /// 0x4c4
			 0x21439616, /// 0x4c8
			 0x89776ac7, /// 0x4cc
			 0x697a725c, /// 0x4d0
			 0x49cc0e99, /// 0x4d4
			 0x350aea08, /// 0x4d8
			 0xd31aa22b, /// 0x4dc
			 0xe4128502, /// 0x4e0
			 0x4b670616, /// 0x4e4
			 0xa296a8ae, /// 0x4e8
			 0x072ade3e, /// 0x4ec
			 0x5e337ea8, /// 0x4f0
			 0xcee89a6a, /// 0x4f4
			 0xbff56c15, /// 0x4f8
			 0x769949d8, /// 0x4fc
			 0x4cbc6e36, /// 0x500
			 0xf092f764, /// 0x504
			 0x7986ddda, /// 0x508
			 0x0cf2e47f, /// 0x50c
			 0xabc9cdd2, /// 0x510
			 0x82007ea8, /// 0x514
			 0x3e16513e, /// 0x518
			 0x6d51e26c, /// 0x51c
			 0xd1c3b259, /// 0x520
			 0xf2190e11, /// 0x524
			 0xec498620, /// 0x528
			 0x36f0db5b, /// 0x52c
			 0xdd7be06d, /// 0x530
			 0x06f501de, /// 0x534
			 0xc64ad55c, /// 0x538
			 0x51492014, /// 0x53c
			 0x62a4ef2f, /// 0x540
			 0xe4ae3e3c, /// 0x544
			 0xebfe2526, /// 0x548
			 0x163e37d5, /// 0x54c
			 0x1a0d6877, /// 0x550
			 0xdbb5f8d7, /// 0x554
			 0x9b824240, /// 0x558
			 0x3ff19442, /// 0x55c
			 0x49e56134, /// 0x560
			 0x32340d2f, /// 0x564
			 0x4818957b, /// 0x568
			 0x8d173e91, /// 0x56c
			 0xd6d6d7ee, /// 0x570
			 0xee28d4c4, /// 0x574
			 0x0c578df0, /// 0x578
			 0xac323766, /// 0x57c
			 0xb1c99768, /// 0x580
			 0xeb3c2e7e, /// 0x584
			 0xe9b13459, /// 0x588
			 0x74638e8f, /// 0x58c
			 0x2ec0178f, /// 0x590
			 0x6055acd3, /// 0x594
			 0x753a23d4, /// 0x598
			 0x74c22073, /// 0x59c
			 0x9ac1f173, /// 0x5a0
			 0x8071527f, /// 0x5a4
			 0x20f9b66d, /// 0x5a8
			 0x82dd2036, /// 0x5ac
			 0x995f435d, /// 0x5b0
			 0xc325915f, /// 0x5b4
			 0x6b313ce1, /// 0x5b8
			 0xe9dd9132, /// 0x5bc
			 0x246c64f1, /// 0x5c0
			 0x05e359d8, /// 0x5c4
			 0x59b53ed8, /// 0x5c8
			 0x02076534, /// 0x5cc
			 0x7f47377d, /// 0x5d0
			 0xb72046f4, /// 0x5d4
			 0x373bcad2, /// 0x5d8
			 0x9c388e44, /// 0x5dc
			 0xc67792ff, /// 0x5e0
			 0xdedb5a09, /// 0x5e4
			 0xe8d7fc71, /// 0x5e8
			 0xc187f47e, /// 0x5ec
			 0xa39e9144, /// 0x5f0
			 0x6a5897cf, /// 0x5f4
			 0x300af5ba, /// 0x5f8
			 0x41b07e31, /// 0x5fc
			 0x89243d64, /// 0x600
			 0xca620ecf, /// 0x604
			 0x5a4f980a, /// 0x608
			 0xb830e0c9, /// 0x60c
			 0xb8c7f4a4, /// 0x610
			 0x30b7434e, /// 0x614
			 0x3487368a, /// 0x618
			 0x8cc7980d, /// 0x61c
			 0x96aaeb50, /// 0x620
			 0xf253a0cf, /// 0x624
			 0x7326277b, /// 0x628
			 0xfe409110, /// 0x62c
			 0xf913e0e2, /// 0x630
			 0x4beebe01, /// 0x634
			 0xbbd27cf4, /// 0x638
			 0x44f960d5, /// 0x63c
			 0x305093f1, /// 0x640
			 0x5b250be1, /// 0x644
			 0x56ac11cd, /// 0x648
			 0xcc6cf0f9, /// 0x64c
			 0xce1b8943, /// 0x650
			 0x0b7d41b4, /// 0x654
			 0x0362a769, /// 0x658
			 0xf22ca370, /// 0x65c
			 0xb401a961, /// 0x660
			 0x62a19fd4, /// 0x664
			 0x0003c3e7, /// 0x668
			 0xeb6b28e4, /// 0x66c
			 0x7dabe19e, /// 0x670
			 0xd0a4b849, /// 0x674
			 0x984d38bd, /// 0x678
			 0x871f97ce, /// 0x67c
			 0xb7a4e4d9, /// 0x680
			 0xd6c2e54f, /// 0x684
			 0x7dda3cde, /// 0x688
			 0x8ca81fe4, /// 0x68c
			 0x206d5bac, /// 0x690
			 0xab44c46f, /// 0x694
			 0xb5a9922e, /// 0x698
			 0x9ed13923, /// 0x69c
			 0xbf98e8ee, /// 0x6a0
			 0x16ddaeb1, /// 0x6a4
			 0x33e6443c, /// 0x6a8
			 0x40fb5348, /// 0x6ac
			 0xeb561d88, /// 0x6b0
			 0x2f9fdd1f, /// 0x6b4
			 0xd0693777, /// 0x6b8
			 0x61007aac, /// 0x6bc
			 0x1b1fd759, /// 0x6c0
			 0xa44acfea, /// 0x6c4
			 0xfdf5b450, /// 0x6c8
			 0x5c36e851, /// 0x6cc
			 0x5ecd8008, /// 0x6d0
			 0x6717fb61, /// 0x6d4
			 0x2312671a, /// 0x6d8
			 0x27cdf1ca, /// 0x6dc
			 0x0ee545e1, /// 0x6e0
			 0x2c7e278a, /// 0x6e4
			 0x990291c0, /// 0x6e8
			 0x94f42ad6, /// 0x6ec
			 0xfc0432ef, /// 0x6f0
			 0xc4449385, /// 0x6f4
			 0x86153368, /// 0x6f8
			 0x4b23ee81, /// 0x6fc
			 0xf9aa34da, /// 0x700
			 0x17266612, /// 0x704
			 0x53f36ded, /// 0x708
			 0xe87d37dc, /// 0x70c
			 0x93eb691a, /// 0x710
			 0x63b45b76, /// 0x714
			 0xe9552702, /// 0x718
			 0x563f97f3, /// 0x71c
			 0xa32bbf4a, /// 0x720
			 0xedf99b73, /// 0x724
			 0xeecf5ad5, /// 0x728
			 0x48ae6fb5, /// 0x72c
			 0x0bb8c9a4, /// 0x730
			 0x33f29e44, /// 0x734
			 0x62bf78eb, /// 0x738
			 0x5c4c8e28, /// 0x73c
			 0xe72aaaa2, /// 0x740
			 0xed402c06, /// 0x744
			 0xf1872676, /// 0x748
			 0xc0ed3a02, /// 0x74c
			 0x33e476d0, /// 0x750
			 0xb532654f, /// 0x754
			 0x2c0aa181, /// 0x758
			 0x9cdad4f8, /// 0x75c
			 0xda1723eb, /// 0x760
			 0xa8df4bd9, /// 0x764
			 0xb7148b40, /// 0x768
			 0xeb057c9e, /// 0x76c
			 0xcc7beea1, /// 0x770
			 0x649c466a, /// 0x774
			 0xa32896f1, /// 0x778
			 0x9c1aa9eb, /// 0x77c
			 0x640bd7d3, /// 0x780
			 0x38da2abb, /// 0x784
			 0x4c5d3184, /// 0x788
			 0x5c3b4767, /// 0x78c
			 0xc26184f1, /// 0x790
			 0x62d2d34c, /// 0x794
			 0x0723e56c, /// 0x798
			 0x1c186537, /// 0x79c
			 0x8c931e8e, /// 0x7a0
			 0xa992f320, /// 0x7a4
			 0xe23c62e3, /// 0x7a8
			 0xf4606213, /// 0x7ac
			 0x6c23f9af, /// 0x7b0
			 0x697c5820, /// 0x7b4
			 0x5c60aa74, /// 0x7b8
			 0xede8d869, /// 0x7bc
			 0x41eb86ec, /// 0x7c0
			 0x1a174044, /// 0x7c4
			 0x7d443b97, /// 0x7c8
			 0xd0875c27, /// 0x7cc
			 0xd3d6f632, /// 0x7d0
			 0x1e5a8932, /// 0x7d4
			 0x806f7c3a, /// 0x7d8
			 0x3c784032, /// 0x7dc
			 0xc22f970c, /// 0x7e0
			 0x2aeea90a, /// 0x7e4
			 0x51e4329b, /// 0x7e8
			 0x36ad25d3, /// 0x7ec
			 0xe1948350, /// 0x7f0
			 0xcc7f2988, /// 0x7f4
			 0xe653dc49, /// 0x7f8
			 0x971eb32b, /// 0x7fc
			 0xebf88ed3, /// 0x800
			 0xe4bfebe8, /// 0x804
			 0xc37957d4, /// 0x808
			 0xaba7a055, /// 0x80c
			 0x60c3da67, /// 0x810
			 0x997bf241, /// 0x814
			 0x5147142c, /// 0x818
			 0x8b03b2e4, /// 0x81c
			 0xe2c1c673, /// 0x820
			 0x1a59e12e, /// 0x824
			 0x1f64817b, /// 0x828
			 0xd38d82df, /// 0x82c
			 0x0cb4f4bc, /// 0x830
			 0x371f5011, /// 0x834
			 0xdf77ab42, /// 0x838
			 0xa70936be, /// 0x83c
			 0xd824ea4e, /// 0x840
			 0xa535a802, /// 0x844
			 0xb9196f55, /// 0x848
			 0x63d6b182, /// 0x84c
			 0xb39b1fd3, /// 0x850
			 0x7b5afdd5, /// 0x854
			 0xddeb04fb, /// 0x858
			 0x09111850, /// 0x85c
			 0x6bf364dd, /// 0x860
			 0xf7f1d35f, /// 0x864
			 0xd92a37f7, /// 0x868
			 0xd66b8b91, /// 0x86c
			 0x02e73e59, /// 0x870
			 0x768a7a13, /// 0x874
			 0x1941b5d7, /// 0x878
			 0xd822f2f5, /// 0x87c
			 0x9556d62c, /// 0x880
			 0x4e231098, /// 0x884
			 0xab96b147, /// 0x888
			 0x407ef59a, /// 0x88c
			 0xc010cc25, /// 0x890
			 0xaeb6f23b, /// 0x894
			 0x215077c1, /// 0x898
			 0x76083dd4, /// 0x89c
			 0x35b00262, /// 0x8a0
			 0x9cb68865, /// 0x8a4
			 0x073e0a95, /// 0x8a8
			 0x90ae457c, /// 0x8ac
			 0x4a97fe34, /// 0x8b0
			 0x366f33a5, /// 0x8b4
			 0x3ce44cd8, /// 0x8b8
			 0x0e58e1a8, /// 0x8bc
			 0x0778151d, /// 0x8c0
			 0x7cb22c65, /// 0x8c4
			 0x644af3de, /// 0x8c8
			 0x6d9b98e7, /// 0x8cc
			 0x0cf56b02, /// 0x8d0
			 0x2ca81d65, /// 0x8d4
			 0x801692cc, /// 0x8d8
			 0x528c9cfb, /// 0x8dc
			 0x2a46be98, /// 0x8e0
			 0x8d289b55, /// 0x8e4
			 0x17f1f60a, /// 0x8e8
			 0x221414f2, /// 0x8ec
			 0x6f0312aa, /// 0x8f0
			 0x0b3121f5, /// 0x8f4
			 0x22f2200c, /// 0x8f8
			 0x0813c4ff, /// 0x8fc
			 0x7c8567eb, /// 0x900
			 0xfe5c4cbf, /// 0x904
			 0x832bdc38, /// 0x908
			 0xc9b7735e, /// 0x90c
			 0x585a0292, /// 0x910
			 0x799038eb, /// 0x914
			 0x6d279c2f, /// 0x918
			 0x8399a0a0, /// 0x91c
			 0x4c08877e, /// 0x920
			 0x96a3e5ac, /// 0x924
			 0x058a5497, /// 0x928
			 0x481a23ff, /// 0x92c
			 0x47465b70, /// 0x930
			 0x4b834b06, /// 0x934
			 0x78897e88, /// 0x938
			 0x797a7e65, /// 0x93c
			 0xded3ebe1, /// 0x940
			 0xa80ebc00, /// 0x944
			 0xe6872dda, /// 0x948
			 0x6d68d296, /// 0x94c
			 0x82436b0f, /// 0x950
			 0x2e460847, /// 0x954
			 0x882d86d9, /// 0x958
			 0x2a45fef1, /// 0x95c
			 0xbca84aa3, /// 0x960
			 0x79c60b48, /// 0x964
			 0x9c880ad7, /// 0x968
			 0x1c067109, /// 0x96c
			 0xe6bb6661, /// 0x970
			 0x8e77cbae, /// 0x974
			 0xd987a08f, /// 0x978
			 0xbee28c5a, /// 0x97c
			 0x6436892b, /// 0x980
			 0x57ba9772, /// 0x984
			 0xdc074741, /// 0x988
			 0xd03dee82, /// 0x98c
			 0x7c59d1a7, /// 0x990
			 0x10fc1ce5, /// 0x994
			 0x3510e9ad, /// 0x998
			 0x0cc3a971, /// 0x99c
			 0x6ddf7e5c, /// 0x9a0
			 0xaa9a3407, /// 0x9a4
			 0xc4f53e22, /// 0x9a8
			 0xc6ea4c67, /// 0x9ac
			 0xcc59ac6a, /// 0x9b0
			 0xdca876d5, /// 0x9b4
			 0xb0ff95ac, /// 0x9b8
			 0xfbcbe412, /// 0x9bc
			 0x9214458d, /// 0x9c0
			 0xb4a73f2a, /// 0x9c4
			 0xe119c3e3, /// 0x9c8
			 0xdbbf81a9, /// 0x9cc
			 0xabf90d47, /// 0x9d0
			 0xc51fcd6a, /// 0x9d4
			 0xc3936879, /// 0x9d8
			 0x8f73df70, /// 0x9dc
			 0x2e797225, /// 0x9e0
			 0x87a3adf5, /// 0x9e4
			 0x21d02ed0, /// 0x9e8
			 0x1fd1a253, /// 0x9ec
			 0x965bb2ad, /// 0x9f0
			 0x69c19150, /// 0x9f4
			 0x854dc2d6, /// 0x9f8
			 0x1c99dfc8, /// 0x9fc
			 0xa4002a4f, /// 0xa00
			 0x2eba0166, /// 0xa04
			 0xbf87cae7, /// 0xa08
			 0xcd4e9e0b, /// 0xa0c
			 0x5bf10ec6, /// 0xa10
			 0x1cf129b9, /// 0xa14
			 0x0bf5724c, /// 0xa18
			 0x9bc43913, /// 0xa1c
			 0xfa7dd961, /// 0xa20
			 0xe756eb10, /// 0xa24
			 0x5f3af508, /// 0xa28
			 0xda8d0eda, /// 0xa2c
			 0x839f987c, /// 0xa30
			 0x21133125, /// 0xa34
			 0xc0585ac4, /// 0xa38
			 0x56b20813, /// 0xa3c
			 0xdf9eca86, /// 0xa40
			 0xaef73fc5, /// 0xa44
			 0xcef3e5b6, /// 0xa48
			 0x5aaeb9f8, /// 0xa4c
			 0x2b30a406, /// 0xa50
			 0x70a19c60, /// 0xa54
			 0x6ff34808, /// 0xa58
			 0xe2164dd5, /// 0xa5c
			 0xc6c5b9c2, /// 0xa60
			 0x1966ae1a, /// 0xa64
			 0x98423da5, /// 0xa68
			 0x811bc4d0, /// 0xa6c
			 0xe21aa5fd, /// 0xa70
			 0xb568db2a, /// 0xa74
			 0xd63e8213, /// 0xa78
			 0x0af26d5b, /// 0xa7c
			 0x291f3994, /// 0xa80
			 0x54efb6d5, /// 0xa84
			 0xcf3e7556, /// 0xa88
			 0x3c0a69a4, /// 0xa8c
			 0x8a6337da, /// 0xa90
			 0x30a3a314, /// 0xa94
			 0x795f56fb, /// 0xa98
			 0xd3e62fe2, /// 0xa9c
			 0x947c604e, /// 0xaa0
			 0xcb8b678f, /// 0xaa4
			 0xcf22cc33, /// 0xaa8
			 0x848ef562, /// 0xaac
			 0x38972f52, /// 0xab0
			 0x62867541, /// 0xab4
			 0xc2995907, /// 0xab8
			 0x71286268, /// 0xabc
			 0x0322a0ad, /// 0xac0
			 0x7a8a01d7, /// 0xac4
			 0xe448a4fb, /// 0xac8
			 0xe84a332d, /// 0xacc
			 0x5b361f6b, /// 0xad0
			 0xce9d7bb0, /// 0xad4
			 0xa4130419, /// 0xad8
			 0xf90e69a1, /// 0xadc
			 0xd7f0b7d9, /// 0xae0
			 0x35d57dc2, /// 0xae4
			 0xb3f9e005, /// 0xae8
			 0xd7a16f49, /// 0xaec
			 0x48572bd1, /// 0xaf0
			 0x234de191, /// 0xaf4
			 0x8707015a, /// 0xaf8
			 0xc8627997, /// 0xafc
			 0x83a46540, /// 0xb00
			 0x4363096e, /// 0xb04
			 0x39123f79, /// 0xb08
			 0x8db636ea, /// 0xb0c
			 0xb6efac44, /// 0xb10
			 0x83c42409, /// 0xb14
			 0xe405e0d6, /// 0xb18
			 0x3fbd2fe8, /// 0xb1c
			 0xcbe34c8a, /// 0xb20
			 0x0f5c0a85, /// 0xb24
			 0x28b2bfbd, /// 0xb28
			 0xaf1ce975, /// 0xb2c
			 0x7b100c75, /// 0xb30
			 0xdf9c0731, /// 0xb34
			 0x9cac81ab, /// 0xb38
			 0x9c54faae, /// 0xb3c
			 0xd5a156b0, /// 0xb40
			 0x51fba8f5, /// 0xb44
			 0x4174e2bb, /// 0xb48
			 0x68b78485, /// 0xb4c
			 0x50b81756, /// 0xb50
			 0x4f998f79, /// 0xb54
			 0x19747615, /// 0xb58
			 0x540d5376, /// 0xb5c
			 0xce02cfeb, /// 0xb60
			 0x504f5c80, /// 0xb64
			 0xcbc8effc, /// 0xb68
			 0x16bd4cb2, /// 0xb6c
			 0x5634039f, /// 0xb70
			 0x04b619a1, /// 0xb74
			 0x5979b6fd, /// 0xb78
			 0x0c3c65a0, /// 0xb7c
			 0x9a6e4274, /// 0xb80
			 0x7e45f55f, /// 0xb84
			 0x8699e268, /// 0xb88
			 0x9ba14198, /// 0xb8c
			 0xf4f8db9c, /// 0xb90
			 0xcc1214e5, /// 0xb94
			 0x7ebf44a2, /// 0xb98
			 0xc4ee80f5, /// 0xb9c
			 0x176e7e95, /// 0xba0
			 0x5af8816c, /// 0xba4
			 0x9a2a4792, /// 0xba8
			 0x89aea02a, /// 0xbac
			 0x0afae349, /// 0xbb0
			 0xb917d257, /// 0xbb4
			 0x9bfe4672, /// 0xbb8
			 0x8196185a, /// 0xbbc
			 0x396d28ca, /// 0xbc0
			 0xbc8f03b0, /// 0xbc4
			 0x74a02610, /// 0xbc8
			 0x7cca43b1, /// 0xbcc
			 0x2b70ee27, /// 0xbd0
			 0x280af4e1, /// 0xbd4
			 0xfa628c36, /// 0xbd8
			 0x94128adf, /// 0xbdc
			 0x329ec4ca, /// 0xbe0
			 0xbf59132f, /// 0xbe4
			 0x978bac82, /// 0xbe8
			 0xcfbe4407, /// 0xbec
			 0x4cf0d549, /// 0xbf0
			 0xed082577, /// 0xbf4
			 0xe2e2605d, /// 0xbf8
			 0xb17a1f3f, /// 0xbfc
			 0xd6a9e52b, /// 0xc00
			 0x5bdd7359, /// 0xc04
			 0xcbd0b950, /// 0xc08
			 0x51f00f04, /// 0xc0c
			 0x5594b425, /// 0xc10
			 0x8d4a0ee7, /// 0xc14
			 0x3b282914, /// 0xc18
			 0xabca6dfd, /// 0xc1c
			 0x04e39874, /// 0xc20
			 0xaf90f98e, /// 0xc24
			 0xf4d8b571, /// 0xc28
			 0xbbc7e24d, /// 0xc2c
			 0x47d20062, /// 0xc30
			 0xef729780, /// 0xc34
			 0xd237613f, /// 0xc38
			 0xb72230ca, /// 0xc3c
			 0x97e9f3f6, /// 0xc40
			 0x63a4b6f9, /// 0xc44
			 0xbbc5cd64, /// 0xc48
			 0x932430a8, /// 0xc4c
			 0xffee0ea2, /// 0xc50
			 0x94c866f3, /// 0xc54
			 0xfe060cae, /// 0xc58
			 0xc7de8f63, /// 0xc5c
			 0x75800d67, /// 0xc60
			 0x552a3777, /// 0xc64
			 0xe2828e7f, /// 0xc68
			 0x4b6e9723, /// 0xc6c
			 0xd6bc1419, /// 0xc70
			 0xd2c36abe, /// 0xc74
			 0x10696136, /// 0xc78
			 0xb604ce9e, /// 0xc7c
			 0x09028bb5, /// 0xc80
			 0xd0a560ea, /// 0xc84
			 0xd95cb11a, /// 0xc88
			 0x6e756563, /// 0xc8c
			 0x2d39248f, /// 0xc90
			 0x70e325f6, /// 0xc94
			 0x4c26d097, /// 0xc98
			 0x77a12d45, /// 0xc9c
			 0x77260bc4, /// 0xca0
			 0x17781da2, /// 0xca4
			 0x800592a1, /// 0xca8
			 0x4c23b007, /// 0xcac
			 0x54d4fb66, /// 0xcb0
			 0x220cbd52, /// 0xcb4
			 0x1b00ef01, /// 0xcb8
			 0x788f0550, /// 0xcbc
			 0x3dda72dd, /// 0xcc0
			 0xf492d08d, /// 0xcc4
			 0x392a3650, /// 0xcc8
			 0xef851f77, /// 0xccc
			 0x911a1944, /// 0xcd0
			 0x6a2b4de3, /// 0xcd4
			 0x1ef016fd, /// 0xcd8
			 0x1898c05f, /// 0xcdc
			 0x3be97a76, /// 0xce0
			 0x2feb3d39, /// 0xce4
			 0xc40abf35, /// 0xce8
			 0xe5447080, /// 0xcec
			 0xe87b1491, /// 0xcf0
			 0x121d36be, /// 0xcf4
			 0x0d8f44db, /// 0xcf8
			 0x9d3a5196, /// 0xcfc
			 0xc70f8b26, /// 0xd00
			 0xea292445, /// 0xd04
			 0x42afe248, /// 0xd08
			 0x6a6f475c, /// 0xd0c
			 0x849b25fa, /// 0xd10
			 0x2780e146, /// 0xd14
			 0xae43209b, /// 0xd18
			 0xf2c38a96, /// 0xd1c
			 0x42c51e69, /// 0xd20
			 0x976c028e, /// 0xd24
			 0x748bd058, /// 0xd28
			 0x91064a65, /// 0xd2c
			 0x60932c6a, /// 0xd30
			 0x4f374040, /// 0xd34
			 0x814968a4, /// 0xd38
			 0x681a7a99, /// 0xd3c
			 0xa5ba3d28, /// 0xd40
			 0x9237e1d7, /// 0xd44
			 0x32277397, /// 0xd48
			 0xebe206ee, /// 0xd4c
			 0x7d26d29d, /// 0xd50
			 0x1b234cd5, /// 0xd54
			 0xba216fee, /// 0xd58
			 0xfca4f15f, /// 0xd5c
			 0x07aa4038, /// 0xd60
			 0x26c23a1a, /// 0xd64
			 0xa5058560, /// 0xd68
			 0x2168d7c0, /// 0xd6c
			 0x509f34b3, /// 0xd70
			 0x157e6936, /// 0xd74
			 0xfa6e132f, /// 0xd78
			 0xfce8d974, /// 0xd7c
			 0x6ec8009c, /// 0xd80
			 0x6a9a68f2, /// 0xd84
			 0x51f25aeb, /// 0xd88
			 0x5e24a0f0, /// 0xd8c
			 0x40229bd3, /// 0xd90
			 0x1c2429c7, /// 0xd94
			 0x8ad6ff62, /// 0xd98
			 0x08a2882b, /// 0xd9c
			 0xba7d16a0, /// 0xda0
			 0xe0e97dce, /// 0xda4
			 0x8ae590e0, /// 0xda8
			 0xd2d00f42, /// 0xdac
			 0xdffe2652, /// 0xdb0
			 0xffd1b360, /// 0xdb4
			 0x60e0e538, /// 0xdb8
			 0x7fdaf934, /// 0xdbc
			 0x19e5feb1, /// 0xdc0
			 0x14a014db, /// 0xdc4
			 0xdf2aa9d2, /// 0xdc8
			 0xc1231664, /// 0xdcc
			 0xb56975a2, /// 0xdd0
			 0x8eb9e2ba, /// 0xdd4
			 0x9b94b854, /// 0xdd8
			 0xb18cd78b, /// 0xddc
			 0xe081da6c, /// 0xde0
			 0x069c454f, /// 0xde4
			 0x5374a038, /// 0xde8
			 0x5c23736e, /// 0xdec
			 0x197e9e3f, /// 0xdf0
			 0xdb6c6364, /// 0xdf4
			 0x8eda7c1c, /// 0xdf8
			 0x5d9f1117, /// 0xdfc
			 0x7db9f0c5, /// 0xe00
			 0x1a734b49, /// 0xe04
			 0x6dc37a51, /// 0xe08
			 0xa9b1c3c7, /// 0xe0c
			 0x76de6b0c, /// 0xe10
			 0x8a37571d, /// 0xe14
			 0x28d36bc3, /// 0xe18
			 0xa53225b5, /// 0xe1c
			 0xdde0d686, /// 0xe20
			 0x0b54c015, /// 0xe24
			 0x66d7a89f, /// 0xe28
			 0x97570739, /// 0xe2c
			 0xe6c3b290, /// 0xe30
			 0x80eaafac, /// 0xe34
			 0x353af56c, /// 0xe38
			 0x12075abe, /// 0xe3c
			 0x62b552db, /// 0xe40
			 0x674f8099, /// 0xe44
			 0x73115f04, /// 0xe48
			 0x60c14e07, /// 0xe4c
			 0x015b3c6d, /// 0xe50
			 0xb55361ad, /// 0xe54
			 0xd37a2447, /// 0xe58
			 0x3b6f4898, /// 0xe5c
			 0x2bfdc422, /// 0xe60
			 0xfd20fabf, /// 0xe64
			 0x7d6e52f6, /// 0xe68
			 0x31dc5d90, /// 0xe6c
			 0xe4615956, /// 0xe70
			 0x7f339162, /// 0xe74
			 0x977d5dda, /// 0xe78
			 0xb4e28d46, /// 0xe7c
			 0xa6a1ac92, /// 0xe80
			 0x1ffa45a9, /// 0xe84
			 0x2af6d68b, /// 0xe88
			 0xd98518ce, /// 0xe8c
			 0xd05fdae5, /// 0xe90
			 0x41655ea2, /// 0xe94
			 0xab3cf79d, /// 0xe98
			 0x439def09, /// 0xe9c
			 0x0de64986, /// 0xea0
			 0xb55cbaa0, /// 0xea4
			 0x64e6ab45, /// 0xea8
			 0x40c5856b, /// 0xeac
			 0x0f84d1ef, /// 0xeb0
			 0xb6ccd0ce, /// 0xeb4
			 0x9f197782, /// 0xeb8
			 0x58cc7853, /// 0xebc
			 0xf236183c, /// 0xec0
			 0xfa9aa8c0, /// 0xec4
			 0x19ed5598, /// 0xec8
			 0x2a508f1a, /// 0xecc
			 0x6bf8a637, /// 0xed0
			 0x1e77b1af, /// 0xed4
			 0x5472254a, /// 0xed8
			 0x3282dd6a, /// 0xedc
			 0x3bcbd803, /// 0xee0
			 0xe6938212, /// 0xee4
			 0xc9a48954, /// 0xee8
			 0x6e45944d, /// 0xeec
			 0x415fe546, /// 0xef0
			 0x6bbd8950, /// 0xef4
			 0x0ba4899e, /// 0xef8
			 0x465f568b, /// 0xefc
			 0x9424be2c, /// 0xf00
			 0x703400e8, /// 0xf04
			 0x22624ae5, /// 0xf08
			 0x5c50fd6f, /// 0xf0c
			 0xddb44ae2, /// 0xf10
			 0x99b661a8, /// 0xf14
			 0x3028df98, /// 0xf18
			 0x296c36ca, /// 0xf1c
			 0xa46267fc, /// 0xf20
			 0xb1b4cb33, /// 0xf24
			 0x64bda06d, /// 0xf28
			 0xde7dd81d, /// 0xf2c
			 0x3880765f, /// 0xf30
			 0x2e34a5a8, /// 0xf34
			 0x831c13a9, /// 0xf38
			 0x19189bdd, /// 0xf3c
			 0xc74bdab5, /// 0xf40
			 0xedd3008a, /// 0xf44
			 0x40633de1, /// 0xf48
			 0x8f198fb3, /// 0xf4c
			 0x264a92ea, /// 0xf50
			 0x7417b04d, /// 0xf54
			 0x23e1e05c, /// 0xf58
			 0x35f99ae5, /// 0xf5c
			 0x0f21f4c0, /// 0xf60
			 0x1819805a, /// 0xf64
			 0xb4ac5938, /// 0xf68
			 0x0681cd26, /// 0xf6c
			 0x0f992f38, /// 0xf70
			 0xa3155045, /// 0xf74
			 0x8a93c34c, /// 0xf78
			 0x814ae1af, /// 0xf7c
			 0xc8b15de3, /// 0xf80
			 0x5d9c295a, /// 0xf84
			 0xdc06c8e6, /// 0xf88
			 0xdda58cca, /// 0xf8c
			 0xcd1b47b5, /// 0xf90
			 0xd1c8e681, /// 0xf94
			 0xf0fcf555, /// 0xf98
			 0xef9eaf1b, /// 0xf9c
			 0x6596198d, /// 0xfa0
			 0xd5c4050b, /// 0xfa4
			 0x7f74d827, /// 0xfa8
			 0x535cd8f4, /// 0xfac
			 0x63716c25, /// 0xfb0
			 0x76fc6285, /// 0xfb4
			 0xe9aa1990, /// 0xfb8
			 0x5a87713a, /// 0xfbc
			 0x4665c0de, /// 0xfc0
			 0x60821b06, /// 0xfc4
			 0x374eb16b, /// 0xfc8
			 0x5920a954, /// 0xfcc
			 0xdc903b01, /// 0xfd0
			 0x60cf9ae6, /// 0xfd4
			 0xe73c306a, /// 0xfd8
			 0x7c75f6c7, /// 0xfdc
			 0xffaf6973, /// 0xfe0
			 0xa703581e, /// 0xfe4
			 0xb439b3a4, /// 0xfe8
			 0x51d94141, /// 0xfec
			 0x11c53949, /// 0xff0
			 0xf13ef52e, /// 0xff4
			 0x4bd6d95a, /// 0xff8
			 0xe049edb4 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00000
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0000c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00018
			 0x3f028f5c,                                                  // 0.51                                        /// 0001c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00024
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00028
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0002c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00030
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0c780000,                                                  // Leading 1s:                                 /// 00038
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00040
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00044
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00048
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00050
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00054
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00058
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0005c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00060
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00064
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00068
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00074
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00078
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00084
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00088
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00090
			 0x80000000,                                                  // -zero                                       /// 00094
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000a8
			 0x7f800000,                                                  // inf                                         /// 000ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x7fc00001,                                                  // signaling NaN                               /// 000b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x3f028f5c,                                                  // 0.51                                        /// 000dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00104
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00108
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00110
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0011c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00124
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c700000,                                                  // Leading 1s:                                 /// 0012c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00144
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0014c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00150
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00154
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00160
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0016c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00170
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00174
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00178
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0017c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00184
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00188
			 0x55555555,                                                  // 4 random values                             /// 0018c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00198
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0019c
			 0xbf028f5c,                                                  // -0.51                                       /// 001a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80000000,                                                  // -zero                                       /// 001a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x33333333,                                                  // 4 random values                             /// 001b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00200
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00204
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00208
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0020c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00210
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00220
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00240
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00244
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00248
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0024c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00254
			 0x0e000003,                                                  // Trailing 1s:                                /// 00258
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x55555555,                                                  // 4 random values                             /// 00260
			 0x3f028f5c,                                                  // 0.51                                        /// 00264
			 0xcb000000,                                                  // -8388608.0                                  /// 00268
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00270
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00278
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00284
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00288
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0028c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00290
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00294
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002d4
			 0x33333333,                                                  // 4 random values                             /// 002d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00300
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00304
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00308
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00310
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00314
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0031c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00320
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0032c
			 0xffc00001,                                                  // -signaling NaN                              /// 00330
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00334
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00338
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0033c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00340
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00344
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00348
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0034c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00350
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00358
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00364
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00368
			 0xbf028f5c,                                                  // -0.51                                       /// 0036c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00370
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00378
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0037c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00380
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00384
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00388
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0038c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00390
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00394
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00398
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b0
			 0x55555555,                                                  // 4 random values                             /// 003b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f8
			 0x00000000,                                                  // zero                                        /// 003fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00400
			 0x0c700000,                                                  // Leading 1s:                                 /// 00404
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00408
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0040c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00410
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0041c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00420
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00430
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0043c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00440
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0044c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00450
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00454
			 0x0c780000,                                                  // Leading 1s:                                 /// 00458
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0045c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00460
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00468
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00474
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00478
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0047c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00480
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c700000,                                                  // Leading 1s:                                 /// 00488
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0048c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00490
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00494
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00498
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00500
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00504
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0050c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00514
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00518
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0051c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00520
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00528
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00530
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00540
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00548
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0054c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00550
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00558
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00000000,                                                  // zero                                        /// 00560
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00568
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0056c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00578
			 0x3f028f5c,                                                  // 0.51                                        /// 0057c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0xffc00001,                                                  // -signaling NaN                              /// 00584
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0058c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x7f800000,                                                  // inf                                         /// 00594
			 0x0e000003,                                                  // Trailing 1s:                                /// 00598
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005b0
			 0xff800000,                                                  // -inf                                        /// 005b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00604
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00608
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0060c
			 0x00000000,                                                  // zero                                        /// 00610
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0c600000,                                                  // Leading 1s:                                 /// 0061c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00620
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00628
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0062c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00630
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00634
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0xcb000000,                                                  // -8388608.0                                  /// 0063c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00640
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00658
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00664
			 0x33333333,                                                  // 4 random values                             /// 00668
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0066c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00670
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00678
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0067c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00684
			 0x0e000003,                                                  // Trailing 1s:                                /// 00688
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00694
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00698
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0069c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006a4
			 0x4b000000,                                                  // 8388608.0                                   /// 006a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006cc
			 0x7f800000,                                                  // inf                                         /// 006d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x55555555,                                                  // 4 random values                             /// 006e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00700
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00704
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00708
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00710
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00720
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00724
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00728
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00730
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00734
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00740
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00748
			 0x80000000,                                                  // -zero                                       /// 0074c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00750
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00758
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0075c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00764
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0076c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00770
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00774
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00784
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00788
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0078c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xcb000000,                                                  // -8388608.0                                  /// 00794
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00798
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0079c
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x33333333,                                                  // 4 random values                             /// 007d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00800
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00808
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0080c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00814
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00818
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00828
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0082c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00830
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00834
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0083c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00840
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00850
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00854
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0085c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00864
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80000000,                                                  // -zero                                       /// 0086c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00870
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00874
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00878
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0087c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00884
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00888
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0088c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00894
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c700000,                                                  // Leading 1s:                                 /// 0089c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008b4
			 0x80000000,                                                  // -zero                                       /// 008b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008dc
			 0x00000000,                                                  // zero                                        /// 008e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00908
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0090c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00914
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00918
			 0x0e000007,                                                  // Trailing 1s:                                /// 0091c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00924
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00928
			 0xbf028f5c,                                                  // -0.51                                       /// 0092c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00934
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0093c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00944
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00948
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0094c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00954
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00958
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00960
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00964
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00968
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0096c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00974
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0c600000,                                                  // Leading 1s:                                 /// 0097c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00980
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0098c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80011111,                                                  // -9.7958E-41                                 /// 00994
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c0
			 0x80000000,                                                  // -zero                                       /// 009c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009fc
			 0xff800000,                                                  // -inf                                        /// 00a00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000000,                                                  // zero                                        /// 00a2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a78
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ab0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab4
			 0x00000000,                                                  // zero                                        /// 00ab8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00abc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00adc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00af8
			 0xff800000,                                                  // -inf                                        /// 00afc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b68
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b70
			 0xff800000,                                                  // -inf                                        /// 00b74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000000,                                                  // -zero                                       /// 00b7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b90
			 0x4b000000,                                                  // 8388608.0                                   /// 00b94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b98
			 0x55555555,                                                  // 4 random values                             /// 00b9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x7f800000,                                                  // inf                                         /// 00bc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bcc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bdc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00be0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c14
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c58
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0xcb000000,                                                  // -8388608.0                                  /// 00c6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80000000,                                                  // -zero                                       /// 00c9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ca0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ca4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ca8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cbc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ccc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cd8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ce0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cfc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d30
			 0x00000000,                                                  // zero                                        /// 00d34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d38
			 0x3f028f5c,                                                  // 0.51                                        /// 00d3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00da4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00da8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00db0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00db4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00de0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000000,                                                  // -zero                                       /// 00dec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00df0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dfc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e20
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e48
			 0x80000000,                                                  // -zero                                       /// 00e4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e50
			 0x55555555,                                                  // 4 random values                             /// 00e54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e90
			 0x7f800000,                                                  // inf                                         /// 00e94
			 0x4b000000,                                                  // 8388608.0                                   /// 00e98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb0
			 0x00000000,                                                  // zero                                        /// 00eb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eb8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ebc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ec0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ecc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ed0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ed4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ee0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00efc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f00
			 0x55555555,                                                  // 4 random values                             /// 00f04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f54
			 0x33333333,                                                  // 4 random values                             /// 00f58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f7c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0xff800000,                                                  // -inf                                        /// 00fa8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fb0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fbc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fc8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fd4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fdc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fe8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ff0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ff8
			 0x80000040                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x5c88b170, /// 0x0
			 0xc76b244b, /// 0x4
			 0xa889521f, /// 0x8
			 0x17f00e39, /// 0xc
			 0x2784f421, /// 0x10
			 0x4e9263f6, /// 0x14
			 0x34e0f856, /// 0x18
			 0x86e7c084, /// 0x1c
			 0xf92d0031, /// 0x20
			 0xe46f81dc, /// 0x24
			 0xfce44a50, /// 0x28
			 0x165e2cde, /// 0x2c
			 0x9c1bfa10, /// 0x30
			 0x7258cf2c, /// 0x34
			 0x88a65155, /// 0x38
			 0x3ebc8172, /// 0x3c
			 0x75d029c1, /// 0x40
			 0x3ff2772e, /// 0x44
			 0x9e91d0ed, /// 0x48
			 0x71214498, /// 0x4c
			 0x39824967, /// 0x50
			 0x1d5594ff, /// 0x54
			 0x7dc47d0a, /// 0x58
			 0xdba6be24, /// 0x5c
			 0xd4bcf0c7, /// 0x60
			 0xdb36d894, /// 0x64
			 0x188b8868, /// 0x68
			 0xeaaf43e3, /// 0x6c
			 0xbc94e302, /// 0x70
			 0x0e18c4aa, /// 0x74
			 0xbc02df02, /// 0x78
			 0x5876615c, /// 0x7c
			 0xd2cbc130, /// 0x80
			 0x59b0065c, /// 0x84
			 0xf4c99e3d, /// 0x88
			 0xd97cb06a, /// 0x8c
			 0xefe131b3, /// 0x90
			 0x1aa0f17f, /// 0x94
			 0x0b457d12, /// 0x98
			 0x9b395570, /// 0x9c
			 0x01a1ea64, /// 0xa0
			 0x476f5f03, /// 0xa4
			 0x27aa7370, /// 0xa8
			 0x1be06b92, /// 0xac
			 0x1523e5a9, /// 0xb0
			 0x9aaeb995, /// 0xb4
			 0xd906b87c, /// 0xb8
			 0x8a4ee1bf, /// 0xbc
			 0x1ef607f0, /// 0xc0
			 0xc511b2db, /// 0xc4
			 0xd961a69c, /// 0xc8
			 0xc00d392c, /// 0xcc
			 0xce52ff84, /// 0xd0
			 0xcf59a31a, /// 0xd4
			 0x6aba0967, /// 0xd8
			 0x96901560, /// 0xdc
			 0x4bc194e1, /// 0xe0
			 0xe51a7e4f, /// 0xe4
			 0x8a7da07e, /// 0xe8
			 0xd7594435, /// 0xec
			 0x6d889f02, /// 0xf0
			 0xddbf279a, /// 0xf4
			 0x2cf72efc, /// 0xf8
			 0xea9132ef, /// 0xfc
			 0x0295e173, /// 0x100
			 0x907a4900, /// 0x104
			 0x8cac33c5, /// 0x108
			 0x724e35fa, /// 0x10c
			 0x5b1e65d3, /// 0x110
			 0x466f67c8, /// 0x114
			 0xe37edee4, /// 0x118
			 0x9bc45c99, /// 0x11c
			 0x7452bb07, /// 0x120
			 0xd5b32dfb, /// 0x124
			 0x8c551bdc, /// 0x128
			 0x81e406a1, /// 0x12c
			 0xa5361067, /// 0x130
			 0x40694e26, /// 0x134
			 0x2dd09dc1, /// 0x138
			 0x7e311880, /// 0x13c
			 0x5a39f082, /// 0x140
			 0x022c8e45, /// 0x144
			 0xb3230504, /// 0x148
			 0x72f15cb0, /// 0x14c
			 0x36c298f7, /// 0x150
			 0x8bca5256, /// 0x154
			 0xd95ec622, /// 0x158
			 0xfe46ab20, /// 0x15c
			 0x242b8dd5, /// 0x160
			 0xa6381182, /// 0x164
			 0xe1cfab9d, /// 0x168
			 0xb73f87d1, /// 0x16c
			 0x4c97238a, /// 0x170
			 0xe5fd4c44, /// 0x174
			 0x2dfae25b, /// 0x178
			 0xd3eb7cda, /// 0x17c
			 0xa723ab6c, /// 0x180
			 0xec51eb95, /// 0x184
			 0xcb84fd9f, /// 0x188
			 0x25cfec9a, /// 0x18c
			 0x932970cf, /// 0x190
			 0xdba4d2d7, /// 0x194
			 0x8e3c8ba5, /// 0x198
			 0x016b6a29, /// 0x19c
			 0x04b95eb0, /// 0x1a0
			 0x3a9d5ddc, /// 0x1a4
			 0x18464d4d, /// 0x1a8
			 0xb232a744, /// 0x1ac
			 0x40ad101a, /// 0x1b0
			 0x31927d45, /// 0x1b4
			 0x0cedab56, /// 0x1b8
			 0xc1880c7c, /// 0x1bc
			 0xb15eeb58, /// 0x1c0
			 0xfbb3c877, /// 0x1c4
			 0xc2c0e3e1, /// 0x1c8
			 0xda32c40f, /// 0x1cc
			 0xa65a68d5, /// 0x1d0
			 0x8506e5e3, /// 0x1d4
			 0x58abe06a, /// 0x1d8
			 0x4283e111, /// 0x1dc
			 0x4d9f1c12, /// 0x1e0
			 0x4f060cbf, /// 0x1e4
			 0x3b7c8529, /// 0x1e8
			 0x0cae160b, /// 0x1ec
			 0x4e913a49, /// 0x1f0
			 0xbb90e1e6, /// 0x1f4
			 0x73577607, /// 0x1f8
			 0x73bdc14a, /// 0x1fc
			 0xdf9dd0e7, /// 0x200
			 0x6a571f13, /// 0x204
			 0x9ee8e429, /// 0x208
			 0xa91b39f9, /// 0x20c
			 0xd7ecfbf4, /// 0x210
			 0x845a5c52, /// 0x214
			 0xa6e96493, /// 0x218
			 0x1900e7ae, /// 0x21c
			 0x76e73895, /// 0x220
			 0x38648d50, /// 0x224
			 0x00d327cf, /// 0x228
			 0x7acc6014, /// 0x22c
			 0x146a46a4, /// 0x230
			 0xb85f26f1, /// 0x234
			 0xd737be30, /// 0x238
			 0xc60b08ba, /// 0x23c
			 0xe0de3739, /// 0x240
			 0xb3523191, /// 0x244
			 0xd5c522da, /// 0x248
			 0xf1ca0ab1, /// 0x24c
			 0xb7bf5563, /// 0x250
			 0xb8ab4944, /// 0x254
			 0x4a89b3d2, /// 0x258
			 0x1fdf61ef, /// 0x25c
			 0x46568910, /// 0x260
			 0xbb9bb812, /// 0x264
			 0x8aee1866, /// 0x268
			 0xd6fbcbf5, /// 0x26c
			 0x673d8d9f, /// 0x270
			 0x54724173, /// 0x274
			 0x975f125c, /// 0x278
			 0xcc97c185, /// 0x27c
			 0xba35bd25, /// 0x280
			 0xa7a2ffb3, /// 0x284
			 0xde7fcf38, /// 0x288
			 0x5d7f5ee8, /// 0x28c
			 0x5f92e515, /// 0x290
			 0xc0ade80c, /// 0x294
			 0xa3070307, /// 0x298
			 0x7515a50a, /// 0x29c
			 0x61aaac61, /// 0x2a0
			 0x7d403ba1, /// 0x2a4
			 0x21f089e1, /// 0x2a8
			 0x2028ef51, /// 0x2ac
			 0x3e30f725, /// 0x2b0
			 0x07a7de1b, /// 0x2b4
			 0x70588ac4, /// 0x2b8
			 0xef1c195b, /// 0x2bc
			 0xd1814e3f, /// 0x2c0
			 0xe6130f9e, /// 0x2c4
			 0x463c7646, /// 0x2c8
			 0x41a4ce66, /// 0x2cc
			 0x0c3f60b0, /// 0x2d0
			 0xfa4f590b, /// 0x2d4
			 0xb43fdd64, /// 0x2d8
			 0x0e506a5c, /// 0x2dc
			 0xba4557cd, /// 0x2e0
			 0x38997591, /// 0x2e4
			 0x0c51f970, /// 0x2e8
			 0xb6c91ccd, /// 0x2ec
			 0xeb9fe5a6, /// 0x2f0
			 0x183635ed, /// 0x2f4
			 0xc69c18e3, /// 0x2f8
			 0xf97a4dc3, /// 0x2fc
			 0x9816b40e, /// 0x300
			 0xa63c4799, /// 0x304
			 0x9fdf07d6, /// 0x308
			 0xa64b77b3, /// 0x30c
			 0x39bf9d1a, /// 0x310
			 0x913e830c, /// 0x314
			 0x733e36cb, /// 0x318
			 0x7b00a0d2, /// 0x31c
			 0x637ebf50, /// 0x320
			 0xcdaecb18, /// 0x324
			 0x0cb0be98, /// 0x328
			 0x61cdde19, /// 0x32c
			 0xc51d5a9e, /// 0x330
			 0xe6d2d470, /// 0x334
			 0x95671fab, /// 0x338
			 0x2665e331, /// 0x33c
			 0xf9ed41c4, /// 0x340
			 0x00d2168c, /// 0x344
			 0x9c0adda3, /// 0x348
			 0x28dc81f5, /// 0x34c
			 0x41851cd3, /// 0x350
			 0xa71a52c0, /// 0x354
			 0xa4c37cfa, /// 0x358
			 0xbd2678cc, /// 0x35c
			 0xbb82fa91, /// 0x360
			 0xd3e010a9, /// 0x364
			 0x52251e33, /// 0x368
			 0xfbfc2801, /// 0x36c
			 0x51bb18b9, /// 0x370
			 0x480f5bfb, /// 0x374
			 0xaf0a09d1, /// 0x378
			 0x5716ce3f, /// 0x37c
			 0x7db6883e, /// 0x380
			 0x2e156db9, /// 0x384
			 0x2893dd36, /// 0x388
			 0x6f68bd27, /// 0x38c
			 0xece53f43, /// 0x390
			 0xe4c95969, /// 0x394
			 0x217a35a8, /// 0x398
			 0xd6a0e66c, /// 0x39c
			 0x7eb79a8b, /// 0x3a0
			 0x6091a070, /// 0x3a4
			 0x7c20ae06, /// 0x3a8
			 0xc6df82c5, /// 0x3ac
			 0x2f372850, /// 0x3b0
			 0xd9e241db, /// 0x3b4
			 0x3318a992, /// 0x3b8
			 0x9c7508e6, /// 0x3bc
			 0x061f1dd2, /// 0x3c0
			 0xa36c3b2f, /// 0x3c4
			 0x8e214b92, /// 0x3c8
			 0x42eb1326, /// 0x3cc
			 0xe1bf9004, /// 0x3d0
			 0x7f1b8b66, /// 0x3d4
			 0x584e4d67, /// 0x3d8
			 0x90c1a395, /// 0x3dc
			 0xf26d89c8, /// 0x3e0
			 0x6c3b680d, /// 0x3e4
			 0x2e061cb4, /// 0x3e8
			 0x78d81df6, /// 0x3ec
			 0x4307bba8, /// 0x3f0
			 0x05c3bb21, /// 0x3f4
			 0x34395f73, /// 0x3f8
			 0xb0b01427, /// 0x3fc
			 0xb2fca536, /// 0x400
			 0xdab842bb, /// 0x404
			 0x5339a35b, /// 0x408
			 0x345428fa, /// 0x40c
			 0x517d5ce6, /// 0x410
			 0x7ee3bd5c, /// 0x414
			 0x05118f28, /// 0x418
			 0x14000915, /// 0x41c
			 0xcfca9c1d, /// 0x420
			 0x5bce8857, /// 0x424
			 0x1d41db61, /// 0x428
			 0xa2c38152, /// 0x42c
			 0xd266d585, /// 0x430
			 0x4a0dab92, /// 0x434
			 0x3dea02a2, /// 0x438
			 0x31936511, /// 0x43c
			 0x0d1be185, /// 0x440
			 0x155f7c54, /// 0x444
			 0xe75922bb, /// 0x448
			 0xbdf329a8, /// 0x44c
			 0x5be722a2, /// 0x450
			 0x73ab1c24, /// 0x454
			 0x25a9ce79, /// 0x458
			 0xb443dc2a, /// 0x45c
			 0xddf6df71, /// 0x460
			 0x20964bd1, /// 0x464
			 0x31f6dc06, /// 0x468
			 0xff312ce1, /// 0x46c
			 0x897b806e, /// 0x470
			 0x83b2c079, /// 0x474
			 0x272e8157, /// 0x478
			 0x54a20dfe, /// 0x47c
			 0xb67c0d81, /// 0x480
			 0x4fea17ad, /// 0x484
			 0x1765a822, /// 0x488
			 0x1c2e5941, /// 0x48c
			 0x702afb06, /// 0x490
			 0x1cfe9100, /// 0x494
			 0x42aaffa3, /// 0x498
			 0x6c4f430a, /// 0x49c
			 0xa786220b, /// 0x4a0
			 0xbce9ad64, /// 0x4a4
			 0xb83f40a7, /// 0x4a8
			 0x8408c38a, /// 0x4ac
			 0x3b909beb, /// 0x4b0
			 0x24f557e7, /// 0x4b4
			 0xc723b337, /// 0x4b8
			 0x1499fbf5, /// 0x4bc
			 0xc349ba22, /// 0x4c0
			 0xa7e2916c, /// 0x4c4
			 0x80d2a523, /// 0x4c8
			 0x8e3d8637, /// 0x4cc
			 0x1db07e19, /// 0x4d0
			 0x44ae6ad3, /// 0x4d4
			 0xcfdaf55b, /// 0x4d8
			 0x9cd4112d, /// 0x4dc
			 0xe39139e4, /// 0x4e0
			 0x77119874, /// 0x4e4
			 0xda448442, /// 0x4e8
			 0x33eaf670, /// 0x4ec
			 0x917644c4, /// 0x4f0
			 0x94917b19, /// 0x4f4
			 0x868fea90, /// 0x4f8
			 0xae48197a, /// 0x4fc
			 0x936bd6c3, /// 0x500
			 0xab17a623, /// 0x504
			 0x3923dcfb, /// 0x508
			 0xabecb2d0, /// 0x50c
			 0x1f2aeabd, /// 0x510
			 0x4f921288, /// 0x514
			 0x5a142728, /// 0x518
			 0x25dd6d6e, /// 0x51c
			 0x067499af, /// 0x520
			 0x5a1e4991, /// 0x524
			 0x683cd422, /// 0x528
			 0xb637ed6d, /// 0x52c
			 0xf83aa5fb, /// 0x530
			 0x1208bd85, /// 0x534
			 0x74457427, /// 0x538
			 0x677b8e64, /// 0x53c
			 0x456d803e, /// 0x540
			 0x67f49692, /// 0x544
			 0x65a70c2d, /// 0x548
			 0x4abfb7ab, /// 0x54c
			 0x0e6b72f0, /// 0x550
			 0x71b4ade8, /// 0x554
			 0x51ac79a4, /// 0x558
			 0x33387884, /// 0x55c
			 0x9256a8ec, /// 0x560
			 0x7567a404, /// 0x564
			 0x2bad2ce3, /// 0x568
			 0x4d3ca767, /// 0x56c
			 0xd75c90b0, /// 0x570
			 0x47f2ba7f, /// 0x574
			 0x8d2d1846, /// 0x578
			 0x86f6e066, /// 0x57c
			 0x7d0c5406, /// 0x580
			 0xf699041f, /// 0x584
			 0x9ebf1fa5, /// 0x588
			 0x6897263b, /// 0x58c
			 0x2127c3f1, /// 0x590
			 0x384b5f12, /// 0x594
			 0x1b83c90d, /// 0x598
			 0x1096b665, /// 0x59c
			 0xa5a1b353, /// 0x5a0
			 0x6d4d3e3e, /// 0x5a4
			 0x8af2f366, /// 0x5a8
			 0xa060e6b7, /// 0x5ac
			 0xce457022, /// 0x5b0
			 0x5080f110, /// 0x5b4
			 0x2c520162, /// 0x5b8
			 0x5daa3793, /// 0x5bc
			 0x959c9f78, /// 0x5c0
			 0xb7d2a93b, /// 0x5c4
			 0x19ed9b56, /// 0x5c8
			 0x087df45a, /// 0x5cc
			 0x16b5c405, /// 0x5d0
			 0xc508db14, /// 0x5d4
			 0x0fe1c271, /// 0x5d8
			 0x78e3d581, /// 0x5dc
			 0xfe56cd54, /// 0x5e0
			 0x606b6c52, /// 0x5e4
			 0x953c7fc8, /// 0x5e8
			 0x2df3f42c, /// 0x5ec
			 0xae9deba4, /// 0x5f0
			 0x62858de9, /// 0x5f4
			 0xb53dff57, /// 0x5f8
			 0x8c3ce822, /// 0x5fc
			 0xfc218d19, /// 0x600
			 0xa0221e5e, /// 0x604
			 0x45a21a17, /// 0x608
			 0xab93bdd4, /// 0x60c
			 0x3e4c0a5d, /// 0x610
			 0x51981319, /// 0x614
			 0x054c978a, /// 0x618
			 0x1724a27c, /// 0x61c
			 0x57fd8169, /// 0x620
			 0x349884a6, /// 0x624
			 0x998af77c, /// 0x628
			 0x8c8efc08, /// 0x62c
			 0x292ed946, /// 0x630
			 0x6d439064, /// 0x634
			 0xeb44ab32, /// 0x638
			 0xf787aad6, /// 0x63c
			 0x07b7b95f, /// 0x640
			 0xfda48e2b, /// 0x644
			 0x602a841f, /// 0x648
			 0x9d4363f7, /// 0x64c
			 0x8adfd30a, /// 0x650
			 0x37a1bc0d, /// 0x654
			 0x52ec57f0, /// 0x658
			 0xfe1b7e46, /// 0x65c
			 0x4aa5a7a4, /// 0x660
			 0x07c44c8e, /// 0x664
			 0x5561852e, /// 0x668
			 0xac9355bb, /// 0x66c
			 0x5f11170a, /// 0x670
			 0x3ce63d9c, /// 0x674
			 0xbb5edbef, /// 0x678
			 0xd6333330, /// 0x67c
			 0x64c66151, /// 0x680
			 0xfc0ed107, /// 0x684
			 0xf457c57d, /// 0x688
			 0x6be093df, /// 0x68c
			 0x926a258a, /// 0x690
			 0x04d45c89, /// 0x694
			 0xe5c0a481, /// 0x698
			 0x206ec524, /// 0x69c
			 0xf79e090c, /// 0x6a0
			 0x33d93225, /// 0x6a4
			 0xba259917, /// 0x6a8
			 0x6850cb12, /// 0x6ac
			 0xf5d93c38, /// 0x6b0
			 0xfaf5163e, /// 0x6b4
			 0x10951847, /// 0x6b8
			 0x509e97eb, /// 0x6bc
			 0x2405df81, /// 0x6c0
			 0x4b01ae0c, /// 0x6c4
			 0x8a8be707, /// 0x6c8
			 0xfa589c58, /// 0x6cc
			 0x811e373b, /// 0x6d0
			 0x77f50c8d, /// 0x6d4
			 0x5b0c3f19, /// 0x6d8
			 0x4239c8e4, /// 0x6dc
			 0x93c630df, /// 0x6e0
			 0xa46b3fe1, /// 0x6e4
			 0xe17a4c08, /// 0x6e8
			 0xf9cd0a4e, /// 0x6ec
			 0x2d0eb275, /// 0x6f0
			 0xd0959c52, /// 0x6f4
			 0xac193f53, /// 0x6f8
			 0x5ffb59ea, /// 0x6fc
			 0x751c6040, /// 0x700
			 0x53c7cb59, /// 0x704
			 0x1bf9f9a4, /// 0x708
			 0x35c765c5, /// 0x70c
			 0x2bcbd666, /// 0x710
			 0x071ece33, /// 0x714
			 0x8b52fbb8, /// 0x718
			 0x7c9fcf20, /// 0x71c
			 0x93af86eb, /// 0x720
			 0x7f52ffc7, /// 0x724
			 0x4e1cf22c, /// 0x728
			 0xf2e9a050, /// 0x72c
			 0x2b84d592, /// 0x730
			 0x794cdeb6, /// 0x734
			 0xca50f481, /// 0x738
			 0xe472f54b, /// 0x73c
			 0x0929abb6, /// 0x740
			 0xca3276ea, /// 0x744
			 0xd510919a, /// 0x748
			 0xef64d65b, /// 0x74c
			 0xf6dbe0b4, /// 0x750
			 0x8e353154, /// 0x754
			 0x53ad3a96, /// 0x758
			 0x9b2237f3, /// 0x75c
			 0x3027e7ed, /// 0x760
			 0x4c9bd40c, /// 0x764
			 0xe926f3d2, /// 0x768
			 0xa94f90a9, /// 0x76c
			 0x69ccfc96, /// 0x770
			 0x9fd2a416, /// 0x774
			 0x1195a95f, /// 0x778
			 0x947c37c2, /// 0x77c
			 0x2f2dd889, /// 0x780
			 0xa4950ccc, /// 0x784
			 0xdfb728f2, /// 0x788
			 0xcd47d3fc, /// 0x78c
			 0xd4eb3f71, /// 0x790
			 0x7cf63dcb, /// 0x794
			 0x2d4f0d37, /// 0x798
			 0xcd1ce88a, /// 0x79c
			 0x5a83661a, /// 0x7a0
			 0x92bc0503, /// 0x7a4
			 0x0a43d958, /// 0x7a8
			 0x189efc18, /// 0x7ac
			 0x01b85a8b, /// 0x7b0
			 0xeaa01e79, /// 0x7b4
			 0xed1ea1da, /// 0x7b8
			 0xacb94746, /// 0x7bc
			 0x13cc18b1, /// 0x7c0
			 0x5afa62f8, /// 0x7c4
			 0xd3ad8add, /// 0x7c8
			 0x0c748e49, /// 0x7cc
			 0xba7d3f0d, /// 0x7d0
			 0x4c4083fe, /// 0x7d4
			 0x810eec9b, /// 0x7d8
			 0xc4a5ddf1, /// 0x7dc
			 0x91a471a6, /// 0x7e0
			 0x7b3e8dc8, /// 0x7e4
			 0x3feff4e7, /// 0x7e8
			 0xc9777078, /// 0x7ec
			 0x4f72b8cc, /// 0x7f0
			 0x0071b7e0, /// 0x7f4
			 0xf5b466b1, /// 0x7f8
			 0x6c2ebb8c, /// 0x7fc
			 0xd87999f9, /// 0x800
			 0x170811b1, /// 0x804
			 0x2828dd51, /// 0x808
			 0xfa399f5d, /// 0x80c
			 0xea8bf95d, /// 0x810
			 0x7f5e8f78, /// 0x814
			 0x076ce7c6, /// 0x818
			 0xd3cdb042, /// 0x81c
			 0x00827ac0, /// 0x820
			 0x3ea1c91c, /// 0x824
			 0x22386896, /// 0x828
			 0xd617f9f1, /// 0x82c
			 0xc424b19f, /// 0x830
			 0x654319d1, /// 0x834
			 0x4aec665b, /// 0x838
			 0xa315b9d9, /// 0x83c
			 0x5a7c9df3, /// 0x840
			 0x7b9bda66, /// 0x844
			 0x88b1f9af, /// 0x848
			 0x5193b7dd, /// 0x84c
			 0x085a81fb, /// 0x850
			 0xbc5bc189, /// 0x854
			 0xe50b54ea, /// 0x858
			 0x8b55879b, /// 0x85c
			 0x698b7d1f, /// 0x860
			 0x156ffd53, /// 0x864
			 0x171a2cfe, /// 0x868
			 0x1a68390f, /// 0x86c
			 0xd7179b26, /// 0x870
			 0xac3adebb, /// 0x874
			 0xde368d48, /// 0x878
			 0xaa41984b, /// 0x87c
			 0xf4511b44, /// 0x880
			 0xd1820d31, /// 0x884
			 0xe656e77b, /// 0x888
			 0x7818244d, /// 0x88c
			 0xd4455622, /// 0x890
			 0xec9a8863, /// 0x894
			 0x3f1f208c, /// 0x898
			 0x3e2e25d4, /// 0x89c
			 0xb63e2b73, /// 0x8a0
			 0x7fe8c6ae, /// 0x8a4
			 0xf34ea59b, /// 0x8a8
			 0x19c95722, /// 0x8ac
			 0x4ed4653a, /// 0x8b0
			 0x25647178, /// 0x8b4
			 0x584c952f, /// 0x8b8
			 0xce9c0b74, /// 0x8bc
			 0xa6c92dcb, /// 0x8c0
			 0x996e85fd, /// 0x8c4
			 0xbd01186e, /// 0x8c8
			 0x016e8c66, /// 0x8cc
			 0x508dd115, /// 0x8d0
			 0xa7d25861, /// 0x8d4
			 0x161888ff, /// 0x8d8
			 0xd3fc7e96, /// 0x8dc
			 0x6ea72d9d, /// 0x8e0
			 0x22f6738f, /// 0x8e4
			 0xe04fa6b0, /// 0x8e8
			 0x6fe596c0, /// 0x8ec
			 0x84ad195c, /// 0x8f0
			 0x49fa1fe9, /// 0x8f4
			 0x19a3fa77, /// 0x8f8
			 0x03e0650b, /// 0x8fc
			 0x344d0681, /// 0x900
			 0xa18b4b91, /// 0x904
			 0xa47ae6ce, /// 0x908
			 0x8a8659b0, /// 0x90c
			 0x893aba88, /// 0x910
			 0xbc50f726, /// 0x914
			 0x0e5eb567, /// 0x918
			 0x202ed657, /// 0x91c
			 0x2893249d, /// 0x920
			 0x45bc54a0, /// 0x924
			 0xd6ef0a55, /// 0x928
			 0x08e2bf53, /// 0x92c
			 0x1fdb8aff, /// 0x930
			 0x47fd4d61, /// 0x934
			 0xc41b4b4a, /// 0x938
			 0xd81fde07, /// 0x93c
			 0xd01988fa, /// 0x940
			 0xea30758e, /// 0x944
			 0x9267a890, /// 0x948
			 0xf310c4c9, /// 0x94c
			 0x962efd7a, /// 0x950
			 0x0c1fe629, /// 0x954
			 0xc99b51d5, /// 0x958
			 0xebc2924c, /// 0x95c
			 0xd7d06209, /// 0x960
			 0xcb97038f, /// 0x964
			 0xfb16b0ce, /// 0x968
			 0xc4b2cbf2, /// 0x96c
			 0x6f5125ba, /// 0x970
			 0x926563cf, /// 0x974
			 0x4d5dc244, /// 0x978
			 0x70b290dd, /// 0x97c
			 0x9e263f50, /// 0x980
			 0x5615686f, /// 0x984
			 0xb04de2e4, /// 0x988
			 0xde0460e5, /// 0x98c
			 0x400bebec, /// 0x990
			 0x98933d6e, /// 0x994
			 0x7599fa7c, /// 0x998
			 0xecf210dc, /// 0x99c
			 0xf796831f, /// 0x9a0
			 0x5b31988e, /// 0x9a4
			 0x68573f85, /// 0x9a8
			 0xa4442852, /// 0x9ac
			 0xa3796b0c, /// 0x9b0
			 0x1832ea57, /// 0x9b4
			 0x667cd31b, /// 0x9b8
			 0x2a3663d8, /// 0x9bc
			 0xcb89da2d, /// 0x9c0
			 0x65a51243, /// 0x9c4
			 0xdd15368e, /// 0x9c8
			 0x8f4c971a, /// 0x9cc
			 0xcb20f690, /// 0x9d0
			 0x016fc8e6, /// 0x9d4
			 0xd5b865b7, /// 0x9d8
			 0x2080c002, /// 0x9dc
			 0x837a4fe3, /// 0x9e0
			 0xd82c6771, /// 0x9e4
			 0xdf77b4b3, /// 0x9e8
			 0xcc87de69, /// 0x9ec
			 0xe9086d2b, /// 0x9f0
			 0xf516bbc6, /// 0x9f4
			 0x6a446c9a, /// 0x9f8
			 0x15919ffb, /// 0x9fc
			 0x2d7ae373, /// 0xa00
			 0xc95d7fbb, /// 0xa04
			 0x27796eaf, /// 0xa08
			 0x2cdcdb5d, /// 0xa0c
			 0x5566596b, /// 0xa10
			 0xb7679bed, /// 0xa14
			 0xf66da186, /// 0xa18
			 0x35b5e988, /// 0xa1c
			 0xbca5feb0, /// 0xa20
			 0x1f837680, /// 0xa24
			 0xed6fae9b, /// 0xa28
			 0x8c37cc4a, /// 0xa2c
			 0x6a5d9a5c, /// 0xa30
			 0x6d5edc53, /// 0xa34
			 0xcf860ee6, /// 0xa38
			 0x4105bb04, /// 0xa3c
			 0x90f175ce, /// 0xa40
			 0x77a81f66, /// 0xa44
			 0x4809362c, /// 0xa48
			 0x93770cbc, /// 0xa4c
			 0x4dd1b8a2, /// 0xa50
			 0xef412ebd, /// 0xa54
			 0x6cd5d6f2, /// 0xa58
			 0x1de6a08c, /// 0xa5c
			 0x5d0f94c5, /// 0xa60
			 0x19768528, /// 0xa64
			 0xade60b1f, /// 0xa68
			 0x6bd82261, /// 0xa6c
			 0xbe9703ec, /// 0xa70
			 0x3099a064, /// 0xa74
			 0xae9de3e9, /// 0xa78
			 0xa48a22ce, /// 0xa7c
			 0xd8fc8087, /// 0xa80
			 0x72606770, /// 0xa84
			 0x5b3f22f0, /// 0xa88
			 0x7d5279a8, /// 0xa8c
			 0x0e5dbd75, /// 0xa90
			 0x0d08ebe3, /// 0xa94
			 0x003d0dd3, /// 0xa98
			 0xcd2090a5, /// 0xa9c
			 0xf21eddbc, /// 0xaa0
			 0x76b5c6a7, /// 0xaa4
			 0x41889af5, /// 0xaa8
			 0x9c7edc94, /// 0xaac
			 0xf4e5db49, /// 0xab0
			 0xdd68ddd8, /// 0xab4
			 0x59b60bcc, /// 0xab8
			 0x0810d341, /// 0xabc
			 0xe68ebbb1, /// 0xac0
			 0x0b393651, /// 0xac4
			 0xd6a98427, /// 0xac8
			 0x9949b24e, /// 0xacc
			 0xc72f9baf, /// 0xad0
			 0x2392fb72, /// 0xad4
			 0xb6bfba8a, /// 0xad8
			 0xfa310694, /// 0xadc
			 0xdd895abe, /// 0xae0
			 0x91c2adcc, /// 0xae4
			 0xf81b96ac, /// 0xae8
			 0xa720f0e6, /// 0xaec
			 0x00d2460b, /// 0xaf0
			 0xe07cd4d5, /// 0xaf4
			 0x2879c594, /// 0xaf8
			 0xa35e4e32, /// 0xafc
			 0x530e6a20, /// 0xb00
			 0x80499d62, /// 0xb04
			 0x29bf1f15, /// 0xb08
			 0xe504530f, /// 0xb0c
			 0xadc27614, /// 0xb10
			 0xb3eae511, /// 0xb14
			 0x8546eeaf, /// 0xb18
			 0x6bb7fc06, /// 0xb1c
			 0x843c06a8, /// 0xb20
			 0x84648764, /// 0xb24
			 0xe1e48fd3, /// 0xb28
			 0x91e07359, /// 0xb2c
			 0x670785a9, /// 0xb30
			 0x2931895f, /// 0xb34
			 0x93ef979f, /// 0xb38
			 0xc7e0f900, /// 0xb3c
			 0x87890594, /// 0xb40
			 0x82a954b1, /// 0xb44
			 0x354e21ab, /// 0xb48
			 0x1d2d0948, /// 0xb4c
			 0x8c906138, /// 0xb50
			 0xdeb20323, /// 0xb54
			 0xa79fc1a1, /// 0xb58
			 0xd00cf479, /// 0xb5c
			 0x34cd9f14, /// 0xb60
			 0xe3e27e60, /// 0xb64
			 0xe3843a55, /// 0xb68
			 0x56eac0ba, /// 0xb6c
			 0xfcefe7d5, /// 0xb70
			 0x99888ea7, /// 0xb74
			 0x9a2fecc3, /// 0xb78
			 0x81060a2b, /// 0xb7c
			 0x638e155f, /// 0xb80
			 0x919bc610, /// 0xb84
			 0x6245dbbe, /// 0xb88
			 0x4e37af19, /// 0xb8c
			 0x4676131b, /// 0xb90
			 0xe33a6743, /// 0xb94
			 0x7ced2dd3, /// 0xb98
			 0x512f4cc8, /// 0xb9c
			 0x730714ca, /// 0xba0
			 0x378147f4, /// 0xba4
			 0xdbdbb0d1, /// 0xba8
			 0x8e30d00f, /// 0xbac
			 0xccd17c72, /// 0xbb0
			 0x7c212c73, /// 0xbb4
			 0x719b55cd, /// 0xbb8
			 0x12948aed, /// 0xbbc
			 0xe29e71fa, /// 0xbc0
			 0x8774f877, /// 0xbc4
			 0x1159ea27, /// 0xbc8
			 0xb743233b, /// 0xbcc
			 0xf33a0afc, /// 0xbd0
			 0x578e075d, /// 0xbd4
			 0xcf49a3a6, /// 0xbd8
			 0xf318018f, /// 0xbdc
			 0x238b2490, /// 0xbe0
			 0x6a43ceac, /// 0xbe4
			 0x34deb4ea, /// 0xbe8
			 0xeaa86e61, /// 0xbec
			 0x134a9c5c, /// 0xbf0
			 0xcbb93919, /// 0xbf4
			 0x8ceca5a3, /// 0xbf8
			 0xc915eb68, /// 0xbfc
			 0xca8a8754, /// 0xc00
			 0xb4069de3, /// 0xc04
			 0xbf0ab25e, /// 0xc08
			 0x20c810c6, /// 0xc0c
			 0x20ccf8a4, /// 0xc10
			 0xc8768207, /// 0xc14
			 0x18a08b00, /// 0xc18
			 0x2f265d41, /// 0xc1c
			 0xef613e08, /// 0xc20
			 0x53856d9d, /// 0xc24
			 0xe717b481, /// 0xc28
			 0x87c9a5ee, /// 0xc2c
			 0x3c5fa0e7, /// 0xc30
			 0xb23f2154, /// 0xc34
			 0xe25d5d63, /// 0xc38
			 0x815dd5cf, /// 0xc3c
			 0x1abb8744, /// 0xc40
			 0x6a7c278a, /// 0xc44
			 0x19e2baf9, /// 0xc48
			 0x0c82b883, /// 0xc4c
			 0x70885400, /// 0xc50
			 0x9e236609, /// 0xc54
			 0xa6d2954a, /// 0xc58
			 0x90537be6, /// 0xc5c
			 0xd2ca7b42, /// 0xc60
			 0xbb3f3fa9, /// 0xc64
			 0x6275172c, /// 0xc68
			 0x08a0e7ff, /// 0xc6c
			 0x1d7f19d9, /// 0xc70
			 0x91cce543, /// 0xc74
			 0x5387ad1e, /// 0xc78
			 0xd5718571, /// 0xc7c
			 0x39e89eb9, /// 0xc80
			 0x3a3d0e8c, /// 0xc84
			 0x783a3405, /// 0xc88
			 0xf03379c9, /// 0xc8c
			 0xfdc94817, /// 0xc90
			 0x19a2d702, /// 0xc94
			 0xe952df58, /// 0xc98
			 0xe9c6b423, /// 0xc9c
			 0x9a99ee4f, /// 0xca0
			 0x6ef4780f, /// 0xca4
			 0x9af81f81, /// 0xca8
			 0x94a2550e, /// 0xcac
			 0xd46e642e, /// 0xcb0
			 0xe7584cf9, /// 0xcb4
			 0x3d7f4bb3, /// 0xcb8
			 0xd3854d14, /// 0xcbc
			 0x12c77472, /// 0xcc0
			 0x4dc3f5c1, /// 0xcc4
			 0x4203f694, /// 0xcc8
			 0x95fc5663, /// 0xccc
			 0xb90968bd, /// 0xcd0
			 0x5c0b7e4c, /// 0xcd4
			 0x21f5160e, /// 0xcd8
			 0x8e1b7ec3, /// 0xcdc
			 0xd3cec49a, /// 0xce0
			 0x0dbf3f9b, /// 0xce4
			 0x7ca51fe6, /// 0xce8
			 0xe92fb684, /// 0xcec
			 0xb39970e9, /// 0xcf0
			 0xffdad1b8, /// 0xcf4
			 0xadb3ffeb, /// 0xcf8
			 0xf6de78e4, /// 0xcfc
			 0x42fd0a1b, /// 0xd00
			 0x680a7925, /// 0xd04
			 0xccdf0d49, /// 0xd08
			 0x2b9dd020, /// 0xd0c
			 0x14e7be63, /// 0xd10
			 0x1d474f09, /// 0xd14
			 0xda101ee1, /// 0xd18
			 0x553bea87, /// 0xd1c
			 0x406438b8, /// 0xd20
			 0x109f226a, /// 0xd24
			 0x1772fa8b, /// 0xd28
			 0x31168264, /// 0xd2c
			 0x535df424, /// 0xd30
			 0xd33ad360, /// 0xd34
			 0x091fc32f, /// 0xd38
			 0x1f1a4132, /// 0xd3c
			 0x4747e402, /// 0xd40
			 0x5fe9a8db, /// 0xd44
			 0x7696e45d, /// 0xd48
			 0x001deac6, /// 0xd4c
			 0xf98a2bee, /// 0xd50
			 0xb8314481, /// 0xd54
			 0xd171aace, /// 0xd58
			 0x738c6b39, /// 0xd5c
			 0x9c36b4ea, /// 0xd60
			 0xac17b96d, /// 0xd64
			 0xf9bb5bde, /// 0xd68
			 0xaa500d0f, /// 0xd6c
			 0x7937f69c, /// 0xd70
			 0x60a807e6, /// 0xd74
			 0x6936f68d, /// 0xd78
			 0xe8adaeaf, /// 0xd7c
			 0x9c2a8135, /// 0xd80
			 0x79c3abe9, /// 0xd84
			 0x69f90801, /// 0xd88
			 0x165cbe39, /// 0xd8c
			 0x797c0cc6, /// 0xd90
			 0x0e389478, /// 0xd94
			 0x49c0dc19, /// 0xd98
			 0x065443b5, /// 0xd9c
			 0x7d0a8ed9, /// 0xda0
			 0xdc5712c8, /// 0xda4
			 0xdb5d579f, /// 0xda8
			 0xe75f546b, /// 0xdac
			 0x26522880, /// 0xdb0
			 0x2ce12df2, /// 0xdb4
			 0x71c52d8c, /// 0xdb8
			 0xbe8b5a50, /// 0xdbc
			 0x3ac53c24, /// 0xdc0
			 0x44d32eb0, /// 0xdc4
			 0x3c3746de, /// 0xdc8
			 0x8a84facd, /// 0xdcc
			 0xbf44ce7f, /// 0xdd0
			 0xbff068f3, /// 0xdd4
			 0x08b8c274, /// 0xdd8
			 0x9df8142b, /// 0xddc
			 0x87a41dc4, /// 0xde0
			 0xe711888c, /// 0xde4
			 0x253c1d68, /// 0xde8
			 0x82e19f8d, /// 0xdec
			 0x6a2fd97a, /// 0xdf0
			 0x1d29775c, /// 0xdf4
			 0xe48730d0, /// 0xdf8
			 0x975e495a, /// 0xdfc
			 0xf64a3931, /// 0xe00
			 0x92edaa86, /// 0xe04
			 0x1a20d52e, /// 0xe08
			 0xa2fe27f6, /// 0xe0c
			 0xb30a35da, /// 0xe10
			 0x17225c13, /// 0xe14
			 0xb2ffdde4, /// 0xe18
			 0x232c19ca, /// 0xe1c
			 0x33f82bbe, /// 0xe20
			 0x8cc40d18, /// 0xe24
			 0xe9d66fac, /// 0xe28
			 0x5ca3c3ff, /// 0xe2c
			 0xb57ff744, /// 0xe30
			 0xb64a71b7, /// 0xe34
			 0x90e7aa44, /// 0xe38
			 0x87834c37, /// 0xe3c
			 0x7a16126e, /// 0xe40
			 0xe3c473c2, /// 0xe44
			 0x76a1f6db, /// 0xe48
			 0xfdb32084, /// 0xe4c
			 0x376a060c, /// 0xe50
			 0x22638ac8, /// 0xe54
			 0x84073c22, /// 0xe58
			 0x3bcef66c, /// 0xe5c
			 0xa8fc0908, /// 0xe60
			 0x23f446f2, /// 0xe64
			 0x62f9c58b, /// 0xe68
			 0xd2e0d185, /// 0xe6c
			 0xd84d5a11, /// 0xe70
			 0xd6b1e7ac, /// 0xe74
			 0x4f002741, /// 0xe78
			 0x846cfc18, /// 0xe7c
			 0x33336a47, /// 0xe80
			 0xcfa2f86b, /// 0xe84
			 0x425bbefc, /// 0xe88
			 0xf25d350d, /// 0xe8c
			 0xc15948d3, /// 0xe90
			 0x66c8692d, /// 0xe94
			 0x83fee730, /// 0xe98
			 0x71cc84e6, /// 0xe9c
			 0x6a5e9532, /// 0xea0
			 0x207aece8, /// 0xea4
			 0xf7a9fd70, /// 0xea8
			 0xa1d41a07, /// 0xeac
			 0x5f7eb387, /// 0xeb0
			 0xeb358f8d, /// 0xeb4
			 0xe993ffc2, /// 0xeb8
			 0xb0ba7b1f, /// 0xebc
			 0x45f3b493, /// 0xec0
			 0xb8744061, /// 0xec4
			 0x3752ea8b, /// 0xec8
			 0x3e179b64, /// 0xecc
			 0xecdf5adb, /// 0xed0
			 0x81d11a69, /// 0xed4
			 0xe7e08c4f, /// 0xed8
			 0xd756e99d, /// 0xedc
			 0xdb0800d5, /// 0xee0
			 0x7d1ecd14, /// 0xee4
			 0x07c8487b, /// 0xee8
			 0x3fe80d8d, /// 0xeec
			 0xa12f472a, /// 0xef0
			 0xa9c96066, /// 0xef4
			 0xca30f1b4, /// 0xef8
			 0x71ffb161, /// 0xefc
			 0x61116d33, /// 0xf00
			 0xfda28afe, /// 0xf04
			 0xc59522be, /// 0xf08
			 0x35b7d13c, /// 0xf0c
			 0xc2752ba7, /// 0xf10
			 0x99cb8683, /// 0xf14
			 0xa63659c8, /// 0xf18
			 0x398b0358, /// 0xf1c
			 0x638e599e, /// 0xf20
			 0x3f984358, /// 0xf24
			 0xcb268721, /// 0xf28
			 0xe08b17a8, /// 0xf2c
			 0xb0181c00, /// 0xf30
			 0x57248d03, /// 0xf34
			 0xcd77c5da, /// 0xf38
			 0x5fa26d78, /// 0xf3c
			 0x24c3f1ab, /// 0xf40
			 0x9a1c726e, /// 0xf44
			 0xe66a1e94, /// 0xf48
			 0x12b1ee57, /// 0xf4c
			 0xeea22c49, /// 0xf50
			 0xc436eb76, /// 0xf54
			 0xf0ec1fc6, /// 0xf58
			 0xdae3958a, /// 0xf5c
			 0x8ed0a4af, /// 0xf60
			 0x9e649fdd, /// 0xf64
			 0x578d8080, /// 0xf68
			 0xa8ba8b4a, /// 0xf6c
			 0x15cbc3ce, /// 0xf70
			 0x121de748, /// 0xf74
			 0xf86cec34, /// 0xf78
			 0x2bdb0501, /// 0xf7c
			 0xec68248b, /// 0xf80
			 0xb98c42d5, /// 0xf84
			 0xc6f86c73, /// 0xf88
			 0xd3ecdd56, /// 0xf8c
			 0x26fa4464, /// 0xf90
			 0x2e0b5767, /// 0xf94
			 0x9b387ae5, /// 0xf98
			 0xd1e8fdef, /// 0xf9c
			 0x3981a571, /// 0xfa0
			 0xf122ab02, /// 0xfa4
			 0x041e94d9, /// 0xfa8
			 0x62ef1b3f, /// 0xfac
			 0x81b85c2e, /// 0xfb0
			 0xba8bdb29, /// 0xfb4
			 0xc6b43ef9, /// 0xfb8
			 0x9b95ce18, /// 0xfbc
			 0xd5ac2854, /// 0xfc0
			 0x7653122d, /// 0xfc4
			 0x81f7c290, /// 0xfc8
			 0xf39ae941, /// 0xfcc
			 0x5603bf9e, /// 0xfd0
			 0x3fb4abe7, /// 0xfd4
			 0x7c47624e, /// 0xfd8
			 0xa1b67378, /// 0xfdc
			 0xc2e91eaa, /// 0xfe0
			 0x55733f42, /// 0xfe4
			 0x678d240a, /// 0xfe8
			 0x527af9df, /// 0xfec
			 0xe506750c, /// 0xff0
			 0xf5c8caab, /// 0xff4
			 0x4669fa12, /// 0xff8
			 0xe71e1f53 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00008
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0000c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00018
			 0x0c780000,                                                  // Leading 1s:                                 /// 0001c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00020
			 0x4b000000,                                                  // 8388608.0                                   /// 00024
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00028
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00030
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0003c
			 0xbf028f5c,                                                  // -0.51                                       /// 00040
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00044
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00050
			 0x0e000001,                                                  // Trailing 1s:                                /// 00054
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0005c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00060
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00070
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00080
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00084
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00088
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00098
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000a4
			 0x3f028f5c,                                                  // 0.51                                        /// 000a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00104
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0010c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00110
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00118
			 0x7f800000,                                                  // inf                                         /// 0011c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00124
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0012c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00130
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00138
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00140
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00144
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00148
			 0x7fc00001,                                                  // signaling NaN                               /// 0014c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00158
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0015c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00160
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00164
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00170
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00174
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0017c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00180
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00184
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00188
			 0xbf028f5c,                                                  // -0.51                                       /// 0018c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00194
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00198
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0019c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x3f028f5c,                                                  // 0.51                                        /// 001a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0xbf028f5c,                                                  // -0.51                                       /// 001e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00200
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0020c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00210
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00220
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0022c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00230
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00238
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00240
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00244
			 0xff800000,                                                  // -inf                                        /// 00248
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00250
			 0x00011111,                                                  // 9.7958E-41                                  /// 00254
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0025c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00260
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00264
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0026c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00278
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00280
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00284
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00288
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0028c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00294
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00298
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00304
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0030c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0031c
			 0x00000000,                                                  // zero                                        /// 00320
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00324
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0032c
			 0x80000000,                                                  // -zero                                       /// 00330
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00334
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e000003,                                                  // Trailing 1s:                                /// 0033c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00344
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00348
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00354
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00358
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0035c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00360
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0xcb000000,                                                  // -8388608.0                                  /// 00368
			 0xcb000000,                                                  // -8388608.0                                  /// 0036c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00370
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0037c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00380
			 0x7fc00001,                                                  // signaling NaN                               /// 00384
			 0x0e000003,                                                  // Trailing 1s:                                /// 00388
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0038c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00390
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00394
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00398
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x4b000000,                                                  // 8388608.0                                   /// 003d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00400
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00408
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00410
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00414
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00418
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0041c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00420
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00424
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00428
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0042c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00434
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00438
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0043c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00444
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00448
			 0x7f800000,                                                  // inf                                         /// 0044c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xffc00001,                                                  // -signaling NaN                              /// 00454
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00458
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0045c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00468
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0046c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0047c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00480
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x7f800000,                                                  // inf                                         /// 00490
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00011111,                                                  // 9.7958E-41                                  /// 0049c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x4b000000,                                                  // 8388608.0                                   /// 004cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00500
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00000000,                                                  // zero                                        /// 00508
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x0c700000,                                                  // Leading 1s:                                 /// 00514
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00518
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0051c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00524
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00534
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0053c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00544
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0054c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00550
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00560
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00564
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00568
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00570
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00574
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0057c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00588
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0058c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00590
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00600
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00604
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0060c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00610
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00614
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00620
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00624
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00628
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00634
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00638
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0063c
			 0x80000000,                                                  // -zero                                       /// 00640
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00644
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00648
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0064c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00650
			 0x7fc00001,                                                  // signaling NaN                               /// 00654
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00658
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7fc00001,                                                  // signaling NaN                               /// 00664
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00668
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0066c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00670
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00678
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0067c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0068c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00694
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00698
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006cc
			 0xbf028f5c,                                                  // -0.51                                       /// 006d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x7f800000,                                                  // inf                                         /// 006d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f800000,                                                  // inf                                         /// 006e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e8
			 0x3f028f5c,                                                  // 0.51                                        /// 006ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x00000000,                                                  // zero                                        /// 006f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00704
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00708
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0070c
			 0xffc00001,                                                  // -signaling NaN                              /// 00710
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00714
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00718
			 0x0c700000,                                                  // Leading 1s:                                 /// 0071c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00720
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00724
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00728
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0072c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x80000000,                                                  // -zero                                       /// 00734
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00740
			 0x7fc00001,                                                  // signaling NaN                               /// 00744
			 0x00011111,                                                  // 9.7958E-41                                  /// 00748
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0074c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00750
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00758
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x7fc00001,                                                  // signaling NaN                               /// 00760
			 0x4b000000,                                                  // 8388608.0                                   /// 00764
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00768
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0076c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00778
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00780
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00784
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0078c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00794
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00798
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0079c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007e4
			 0x4b000000,                                                  // 8388608.0                                   /// 007e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007f0
			 0x80000000,                                                  // -zero                                       /// 007f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00804
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00808
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0080c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0081c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00820
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00824
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00828
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0082c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00830
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00834
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00838
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0083c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00011111,                                                  // 9.7958E-41                                  /// 00844
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0084c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00850
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x4b000000,                                                  // 8388608.0                                   /// 00858
			 0x00011111,                                                  // 9.7958E-41                                  /// 0085c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00860
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00864
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0086c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00870
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00874
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00878
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00880
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00888
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00894
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0089c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b8
			 0x33333333,                                                  // 4 random values                             /// 008bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f8
			 0x80000000,                                                  // -zero                                       /// 008fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00904
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0090c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00910
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00914
			 0x0e000003,                                                  // Trailing 1s:                                /// 00918
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00920
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80011111,                                                  // -9.7958E-41                                 /// 00928
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00934
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00940
			 0x7f800000,                                                  // inf                                         /// 00944
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0094c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00950
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00954
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00958
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0095c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00960
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00964
			 0x0e000001,                                                  // Trailing 1s:                                /// 00968
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0096c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00970
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00974
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00978
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0097c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00980
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00984
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00988
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00990
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x7f800000,                                                  // inf                                         /// 0099c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00000000,                                                  // zero                                        /// 009b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009d0
			 0x55555555,                                                  // 4 random values                             /// 009d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a38
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a44
			 0x55555555,                                                  // 4 random values                             /// 00a48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a78
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aa8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ab0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00abc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0xff800000,                                                  // -inf                                        /// 00ac4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00acc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ad0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ad4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00adc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ae4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae8
			 0x4b000000,                                                  // 8388608.0                                   /// 00aec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00af0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00afc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b68
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x55555555,                                                  // 4 random values                             /// 00b8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ba4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00bb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00be0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00be8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf4
			 0x7f800000,                                                  // inf                                         /// 00bf8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ccc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cfc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00db8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dc4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00de0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00de8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dec
			 0x55555555,                                                  // 4 random values                             /// 00df0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e28
			 0xff800000,                                                  // -inf                                        /// 00e2c
			 0x7f800000,                                                  // inf                                         /// 00e30
			 0xcb000000,                                                  // -8388608.0                                  /// 00e34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ee0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ef0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00efc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0x55555555,                                                  // 4 random values                             /// 00fc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x4b000000,                                                  // 8388608.0                                   /// 00fe0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff4
			 0xff800000,                                                  // -inf                                        /// 00ff8
			 0x7f7ffffe // max norm - 3ulp                               // max norm +ve                                /// last
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
			 0x00000310,
			 0x000005e8,
			 0x0000067c,
			 0x00000474,
			 0x000005cc,
			 0x00000080,
			 0x00000184,
			 0x000006d4,

			 /// vpu register f2
			 0x41800000,
			 0x40a00000,
			 0x41900000,
			 0x40c00000,
			 0x40800000,
			 0x40800000,
			 0x41600000,
			 0x40e00000,

			 /// vpu register f3
			 0x41a80000,
			 0x41900000,
			 0x41d00000,
			 0x41b00000,
			 0x41b00000,
			 0x41e80000,
			 0x41f00000,
			 0x40c00000,

			 /// vpu register f4
			 0x40a00000,
			 0x41f00000,
			 0x41a00000,
			 0x41d00000,
			 0x41b80000,
			 0x41c00000,
			 0x40e00000,
			 0x41b00000,

			 /// vpu register f5
			 0x41980000,
			 0x41900000,
			 0x41980000,
			 0x41a80000,
			 0x3f800000,
			 0x40400000,
			 0x40000000,
			 0x41880000,

			 /// vpu register f6
			 0x41600000,
			 0x41700000,
			 0x41b00000,
			 0x41600000,
			 0x41980000,
			 0x41b00000,
			 0x41c80000,
			 0x40000000,

			 /// vpu register f7
			 0x41b80000,
			 0x41980000,
			 0x41e80000,
			 0x41d00000,
			 0x41880000,
			 0x41c80000,
			 0x40a00000,
			 0x40800000,

			 /// vpu register f8
			 0x41c80000,
			 0x41000000,
			 0x41900000,
			 0x41200000,
			 0x41600000,
			 0x41200000,
			 0x41f80000,
			 0x41400000,

			 /// vpu register f9
			 0x41300000,
			 0x41980000,
			 0x40c00000,
			 0x40400000,
			 0x40c00000,
			 0x41600000,
			 0x42000000,
			 0x41a00000,

			 /// vpu register f10
			 0x40000000,
			 0x41000000,
			 0x40a00000,
			 0x41880000,
			 0x41e00000,
			 0x41900000,
			 0x3f800000,
			 0x41e80000,

			 /// vpu register f11
			 0x40a00000,
			 0x41700000,
			 0x41100000,
			 0x40e00000,
			 0x42000000,
			 0x41f80000,
			 0x3f800000,
			 0x40000000,

			 /// vpu register f12
			 0x40a00000,
			 0x41f80000,
			 0x40e00000,
			 0x41900000,
			 0x40e00000,
			 0x40800000,
			 0x41b80000,
			 0x41700000,

			 /// vpu register f13
			 0x40800000,
			 0x40400000,
			 0x41c80000,
			 0x41d80000,
			 0x41b80000,
			 0x41f80000,
			 0x40c00000,
			 0x41c00000,

			 /// vpu register f14
			 0x41700000,
			 0x41200000,
			 0x41f80000,
			 0x41100000,
			 0x41b80000,
			 0x41200000,
			 0x41900000,
			 0x41700000,

			 /// vpu register f15
			 0x41900000,
			 0x41400000,
			 0x41600000,
			 0x41b00000,
			 0x41800000,
			 0x41980000,
			 0x41f80000,
			 0x40c00000,

			 /// vpu register f16
			 0x41500000,
			 0x41b00000,
			 0x41900000,
			 0x41e80000,
			 0x40800000,
			 0x41980000,
			 0x41900000,
			 0x42000000,

			 /// vpu register f17
			 0x41c80000,
			 0x40a00000,
			 0x42000000,
			 0x40000000,
			 0x40800000,
			 0x40000000,
			 0x40c00000,
			 0x41500000,

			 /// vpu register f18
			 0x41c00000,
			 0x41d00000,
			 0x42000000,
			 0x41200000,
			 0x41400000,
			 0x40e00000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f19
			 0x41f00000,
			 0x41500000,
			 0x40a00000,
			 0x41600000,
			 0x41f80000,
			 0x41d80000,
			 0x41e80000,
			 0x41f00000,

			 /// vpu register f20
			 0x42000000,
			 0x40000000,
			 0x40800000,
			 0x40400000,
			 0x41600000,
			 0x42000000,
			 0x41c00000,
			 0x41c00000,

			 /// vpu register f21
			 0x41b80000,
			 0x41b00000,
			 0x3f800000,
			 0x41e80000,
			 0x41b00000,
			 0x41880000,
			 0x41980000,
			 0x40000000,

			 /// vpu register f22
			 0x41e80000,
			 0x41e00000,
			 0x41500000,
			 0x42000000,
			 0x41a80000,
			 0x41900000,
			 0x3f800000,
			 0x3f800000,

			 /// vpu register f23
			 0x41e00000,
			 0x41f00000,
			 0x41900000,
			 0x40400000,
			 0x40800000,
			 0x41d80000,
			 0x41400000,
			 0x40e00000,

			 /// vpu register f24
			 0x41000000,
			 0x41c80000,
			 0x41a00000,
			 0x40e00000,
			 0x41200000,
			 0x3f800000,
			 0x41b00000,
			 0x41300000,

			 /// vpu register f25
			 0x41c80000,
			 0x41700000,
			 0x41e80000,
			 0x41b00000,
			 0x41b80000,
			 0x40c00000,
			 0x40400000,
			 0x40c00000,

			 /// vpu register f26
			 0x41600000,
			 0x41700000,
			 0x40a00000,
			 0x41a00000,
			 0x41f80000,
			 0x41a80000,
			 0x42000000,
			 0x41880000,

			 /// vpu register f27
			 0x41000000,
			 0x41980000,
			 0x41500000,
			 0x40000000,
			 0x41300000,
			 0x40400000,
			 0x41400000,
			 0x41980000,

			 /// vpu register f28
			 0x41000000,
			 0x41600000,
			 0x41400000,
			 0x41500000,
			 0x41c80000,
			 0x41e00000,
			 0x41800000,
			 0x41700000,

			 /// vpu register f29
			 0x41900000,
			 0x40a00000,
			 0x41c00000,
			 0x41a00000,
			 0x41b80000,
			 0x41f80000,
			 0x40800000,
			 0x40c00000,

			 /// vpu register f30
			 0x41d80000,
			 0x41c00000,
			 0x40e00000,
			 0x41f00000,
			 0x41f80000,
			 0x41000000,
			 0x41800000,
			 0x40400000,

			 /// vpu register f31
			 0x3f800000,
			 0x41e80000,
			 0x40a00000,
			 0x41100000,
			 0x41000000,
			 0x41f80000,
			 0x41880000,
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
		"fsch.ps f30, f1 (x14)\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f21, f1 (x13)\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f4, f1 (x11)\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f29, f1 (x13)\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f6, f1 (x12)\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f13, f1 (x13)\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f18, f1 (x14)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f26, f1 (x12)\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f16, f1 (x13)\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f3, f1 (x13)\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f17, f1 (x14)\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f5, f1 (x15)\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f23, f1 (x12)\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f28, f1 (x14)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f21, f1 (x13)\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f0, f1 (x11)\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f13, f1 (x12)\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f26, f1 (x14)\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f3, f1 (x15)\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f19, f1 (x12)\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f17, f1 (x15)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f18, f1 (x14)\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f2, f1 (x14)\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f9, f1 (x14)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f6, f1 (x15)\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f27, f1 (x11)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f17, f1 (x11)\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f10, f1 (x13)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f13, f1 (x13)\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f20, f1 (x13)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f12, f1 (x13)\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f6, f1 (x13)\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f30, f1 (x15)\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f2, f1 (x13)\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f9, f1 (x14)\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f3, f1 (x12)\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f18, f1 (x14)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f16, f1 (x12)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f16, f1 (x12)\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f18, f1 (x12)\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f5, f1 (x15)\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f25, f1 (x15)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f28, f1 (x12)\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f1, f1 (x11)\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f27, f1 (x12)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f20, f1 (x12)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f18, f1 (x12)\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f28, f1 (x12)\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f24, f1 (x11)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f13, f1 (x14)\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f3, f1 (x13)\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f9, f1 (x13)\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f14, f1 (x13)\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f2, f1 (x11)\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f28, f1 (x12)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f28, f1 (x15)\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f13, f1 (x14)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f8, f1 (x12)\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f8, f1 (x11)\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f8, f1 (x12)\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f19, f1 (x15)\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f22, f1 (x15)\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f15, f1 (x12)\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f4, f1 (x12)\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f15, f1 (x11)\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f22, f1 (x14)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f24, f1 (x14)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f8, f1 (x11)\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f15, f1 (x15)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f14, f1 (x12)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f10, f1 (x11)\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f12, f1 (x12)\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f24, f1 (x15)\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f16, f1 (x12)\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f25, f1 (x14)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f25, f1 (x14)\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f4, f1 (x13)\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f30, f1 (x12)\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f1, f1 (x12)\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f26, f1 (x11)\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f30, f1 (x14)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f13, f1 (x12)\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f26, f1 (x14)\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f19, f1 (x15)\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f30, f1 (x15)\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f20, f1 (x11)\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f29, f1 (x15)\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f24, f1 (x13)\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f10, f1 (x14)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f23, f1 (x15)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f22, f1 (x15)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f12, f1 (x13)\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f19, f1 (x14)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f12, f1 (x13)\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f15, f1 (x14)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f7, f1 (x11)\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f6, f1 (x13)\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f7, f1 (x13)\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsch.ps f24, f1 (x12)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
