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
			 0xce374238, /// 0x0
			 0x15a11cbc, /// 0x4
			 0xb9126d73, /// 0x8
			 0x1ab08b30, /// 0xc
			 0x349501cd, /// 0x10
			 0x2ec361dc, /// 0x14
			 0x851a2ac0, /// 0x18
			 0xc599a9f0, /// 0x1c
			 0x10465664, /// 0x20
			 0x5f587f94, /// 0x24
			 0x0484a898, /// 0x28
			 0x2f6ae571, /// 0x2c
			 0xbb6b7f85, /// 0x30
			 0xaf08d7c7, /// 0x34
			 0xdc761aed, /// 0x38
			 0xa3b8f5a3, /// 0x3c
			 0xc986d40d, /// 0x40
			 0x7b37534f, /// 0x44
			 0xe44e4f44, /// 0x48
			 0x1bb827d1, /// 0x4c
			 0x809209cf, /// 0x50
			 0xc64b2543, /// 0x54
			 0x1eb9525c, /// 0x58
			 0x6a7bc24b, /// 0x5c
			 0x5f3bd3d6, /// 0x60
			 0xece135c3, /// 0x64
			 0x5c598b5f, /// 0x68
			 0x4ae94141, /// 0x6c
			 0x70b11025, /// 0x70
			 0xb3c42b8e, /// 0x74
			 0xa7f2c72d, /// 0x78
			 0x74816a05, /// 0x7c
			 0x6393b5d1, /// 0x80
			 0x81f7d556, /// 0x84
			 0xf07f6b45, /// 0x88
			 0x789c37b2, /// 0x8c
			 0x8519342a, /// 0x90
			 0x34f6accb, /// 0x94
			 0xfc1e0dc9, /// 0x98
			 0x688c96ff, /// 0x9c
			 0xb1b4d5fd, /// 0xa0
			 0xdad74df1, /// 0xa4
			 0x46c14209, /// 0xa8
			 0x478ba6e7, /// 0xac
			 0xa348cedf, /// 0xb0
			 0xed849c96, /// 0xb4
			 0xd4d37924, /// 0xb8
			 0xe5dd2060, /// 0xbc
			 0xf362459e, /// 0xc0
			 0x2e061463, /// 0xc4
			 0xe123c08b, /// 0xc8
			 0x4d98c630, /// 0xcc
			 0x57c81fd4, /// 0xd0
			 0x9ffa402a, /// 0xd4
			 0x2bd2d7f4, /// 0xd8
			 0xc15a6640, /// 0xdc
			 0x40c73492, /// 0xe0
			 0x98bcb67e, /// 0xe4
			 0xd495d54e, /// 0xe8
			 0xa8cc8e19, /// 0xec
			 0xbd9d7fdd, /// 0xf0
			 0x23432eb1, /// 0xf4
			 0x6dc45f0b, /// 0xf8
			 0x3ac97204, /// 0xfc
			 0x20175cba, /// 0x100
			 0xdbaf87d3, /// 0x104
			 0xd8d00e84, /// 0x108
			 0xc5971523, /// 0x10c
			 0x30b782e7, /// 0x110
			 0x607d0abe, /// 0x114
			 0x3d9217bb, /// 0x118
			 0x97dc2af5, /// 0x11c
			 0x83781b4b, /// 0x120
			 0xb2fc4773, /// 0x124
			 0xcd440de8, /// 0x128
			 0xc39207e7, /// 0x12c
			 0x203aa2b0, /// 0x130
			 0x3db31b35, /// 0x134
			 0xcdd1ddf2, /// 0x138
			 0x48b510a5, /// 0x13c
			 0x8a4fec69, /// 0x140
			 0x863cce69, /// 0x144
			 0x828a4d48, /// 0x148
			 0xadee7d1b, /// 0x14c
			 0xff6f7049, /// 0x150
			 0xc0a266c8, /// 0x154
			 0x829f4ff5, /// 0x158
			 0x7709dff7, /// 0x15c
			 0x63c55c60, /// 0x160
			 0x1b7d3727, /// 0x164
			 0x6bb49bfa, /// 0x168
			 0x44afb241, /// 0x16c
			 0xaff71527, /// 0x170
			 0x118be38e, /// 0x174
			 0x3d5e08cf, /// 0x178
			 0x344c5f84, /// 0x17c
			 0x5d9007ce, /// 0x180
			 0x97aba437, /// 0x184
			 0x2d616619, /// 0x188
			 0x14b1b55a, /// 0x18c
			 0xc6623496, /// 0x190
			 0x17717468, /// 0x194
			 0x16ce3dad, /// 0x198
			 0x5084904a, /// 0x19c
			 0x73e98fbc, /// 0x1a0
			 0xb65776e4, /// 0x1a4
			 0x8c9462c0, /// 0x1a8
			 0x5ec3379e, /// 0x1ac
			 0x30b285f1, /// 0x1b0
			 0x1976958f, /// 0x1b4
			 0x0bc035eb, /// 0x1b8
			 0xb7ed9030, /// 0x1bc
			 0x4351c542, /// 0x1c0
			 0xfb3f3bce, /// 0x1c4
			 0x8f096845, /// 0x1c8
			 0xe8085479, /// 0x1cc
			 0x3ad4fd77, /// 0x1d0
			 0x022011b7, /// 0x1d4
			 0x94b355f1, /// 0x1d8
			 0x59a9534c, /// 0x1dc
			 0xfd76f533, /// 0x1e0
			 0x117881c8, /// 0x1e4
			 0xdfbc1217, /// 0x1e8
			 0x08b9209e, /// 0x1ec
			 0xe634e578, /// 0x1f0
			 0x9d8c9779, /// 0x1f4
			 0x5f060568, /// 0x1f8
			 0x9ccc9e47, /// 0x1fc
			 0x88be78f4, /// 0x200
			 0x00ccca16, /// 0x204
			 0x1f980823, /// 0x208
			 0xda93c5c3, /// 0x20c
			 0x8145a6c6, /// 0x210
			 0x78e7eafb, /// 0x214
			 0xfa955dc7, /// 0x218
			 0x27749475, /// 0x21c
			 0xfbfda5a3, /// 0x220
			 0xb0baf343, /// 0x224
			 0xa92c2133, /// 0x228
			 0xc65f4a53, /// 0x22c
			 0x6c42a216, /// 0x230
			 0xdf554700, /// 0x234
			 0xa903e586, /// 0x238
			 0x840448ff, /// 0x23c
			 0xa01d7539, /// 0x240
			 0x00076425, /// 0x244
			 0x796a0c70, /// 0x248
			 0x8638a64d, /// 0x24c
			 0x1dd722cf, /// 0x250
			 0x6718ed39, /// 0x254
			 0x50ef13c5, /// 0x258
			 0x779cc690, /// 0x25c
			 0xd36aba3a, /// 0x260
			 0x1a7a8fda, /// 0x264
			 0x9752ae4f, /// 0x268
			 0x1a6f2a0e, /// 0x26c
			 0xcc40ce2a, /// 0x270
			 0x0a3367cb, /// 0x274
			 0x02c5a43d, /// 0x278
			 0x7d7fe185, /// 0x27c
			 0xc439a382, /// 0x280
			 0x9b06a0a2, /// 0x284
			 0xd996dd68, /// 0x288
			 0x9c4139e5, /// 0x28c
			 0xe1fc2c78, /// 0x290
			 0xa06055cd, /// 0x294
			 0x6a7cb9e7, /// 0x298
			 0x77f8832d, /// 0x29c
			 0xf7ec60c1, /// 0x2a0
			 0x6369e3d9, /// 0x2a4
			 0x1117e449, /// 0x2a8
			 0xc1da6c7d, /// 0x2ac
			 0x86ad7cf4, /// 0x2b0
			 0x0399a43f, /// 0x2b4
			 0x506dff91, /// 0x2b8
			 0xa6038bfc, /// 0x2bc
			 0xc56d6f8c, /// 0x2c0
			 0x79f9631c, /// 0x2c4
			 0x0660eed3, /// 0x2c8
			 0xc36b86bc, /// 0x2cc
			 0xdb0b5e98, /// 0x2d0
			 0xc6433c23, /// 0x2d4
			 0x52055d83, /// 0x2d8
			 0x879940d4, /// 0x2dc
			 0xba06f485, /// 0x2e0
			 0xd7ea6aa4, /// 0x2e4
			 0x2963c263, /// 0x2e8
			 0x9cba5911, /// 0x2ec
			 0x016f07f8, /// 0x2f0
			 0x65804f0f, /// 0x2f4
			 0x97e5c17f, /// 0x2f8
			 0xffa79216, /// 0x2fc
			 0x361692ce, /// 0x300
			 0x3f339494, /// 0x304
			 0x7172f944, /// 0x308
			 0xd60d9a29, /// 0x30c
			 0x4edba715, /// 0x310
			 0x428f9174, /// 0x314
			 0x44af77ba, /// 0x318
			 0x804cfdec, /// 0x31c
			 0xc87a358d, /// 0x320
			 0x36ae196a, /// 0x324
			 0xe113fd16, /// 0x328
			 0xf889de2d, /// 0x32c
			 0xe493646f, /// 0x330
			 0x02ece145, /// 0x334
			 0xff05cff2, /// 0x338
			 0x74843ce4, /// 0x33c
			 0x8e95bc2a, /// 0x340
			 0x433e5736, /// 0x344
			 0x4d131c09, /// 0x348
			 0x5ef44469, /// 0x34c
			 0x65aa89cb, /// 0x350
			 0x475eaa58, /// 0x354
			 0x18b10064, /// 0x358
			 0xe5a40b58, /// 0x35c
			 0xbb5b83f3, /// 0x360
			 0xcdff2249, /// 0x364
			 0xbd91cbd2, /// 0x368
			 0x752c8a6d, /// 0x36c
			 0x1c3ad761, /// 0x370
			 0xe8d7c33b, /// 0x374
			 0x26a83dbe, /// 0x378
			 0x4c0df173, /// 0x37c
			 0xc2b67efc, /// 0x380
			 0xc1cd451e, /// 0x384
			 0x653170a5, /// 0x388
			 0x892f77b0, /// 0x38c
			 0x63b35a03, /// 0x390
			 0x1b9dd4bb, /// 0x394
			 0x47cc26ea, /// 0x398
			 0xc15cc90a, /// 0x39c
			 0x64b0d76a, /// 0x3a0
			 0x41de2342, /// 0x3a4
			 0xfe1e8c99, /// 0x3a8
			 0x41cc76ea, /// 0x3ac
			 0x2ec915c2, /// 0x3b0
			 0xcffcb39e, /// 0x3b4
			 0x804b5304, /// 0x3b8
			 0xeac3ce88, /// 0x3bc
			 0xabb0cda1, /// 0x3c0
			 0x66a3b0bb, /// 0x3c4
			 0x7ef2eab7, /// 0x3c8
			 0xec92828d, /// 0x3cc
			 0x346d4136, /// 0x3d0
			 0xe7391bfe, /// 0x3d4
			 0x19a04ef4, /// 0x3d8
			 0x30c7d58d, /// 0x3dc
			 0xbadc3591, /// 0x3e0
			 0x0eeaf61c, /// 0x3e4
			 0xda2c0329, /// 0x3e8
			 0xa54f949b, /// 0x3ec
			 0x2224be16, /// 0x3f0
			 0xe7b9cbc7, /// 0x3f4
			 0x6c2d0806, /// 0x3f8
			 0xbf4b44b1, /// 0x3fc
			 0x4b59ef7c, /// 0x400
			 0x653d173b, /// 0x404
			 0x5a5c4b0a, /// 0x408
			 0x2b58cb7d, /// 0x40c
			 0x4d8afe32, /// 0x410
			 0xad27a803, /// 0x414
			 0xed1acf0c, /// 0x418
			 0x0c06e386, /// 0x41c
			 0x633747da, /// 0x420
			 0x18376989, /// 0x424
			 0x7f00c501, /// 0x428
			 0x8c3bbf0b, /// 0x42c
			 0xab18bcef, /// 0x430
			 0xc2244197, /// 0x434
			 0xc554f150, /// 0x438
			 0x8ed1ba10, /// 0x43c
			 0xe91d72c3, /// 0x440
			 0x640bf78f, /// 0x444
			 0x056a8411, /// 0x448
			 0x436419bc, /// 0x44c
			 0x05a8d004, /// 0x450
			 0x8a5be0b1, /// 0x454
			 0x2a83cb07, /// 0x458
			 0x917a73df, /// 0x45c
			 0x71bfc16e, /// 0x460
			 0xe9d60c1a, /// 0x464
			 0xd31cc2a3, /// 0x468
			 0x5585ed6f, /// 0x46c
			 0x452129c0, /// 0x470
			 0xc3482806, /// 0x474
			 0x5ae8a1f1, /// 0x478
			 0xb22da069, /// 0x47c
			 0x728a2334, /// 0x480
			 0x5bff0e8a, /// 0x484
			 0x580aa38d, /// 0x488
			 0x7aa3419c, /// 0x48c
			 0x96e18bb2, /// 0x490
			 0x589546ae, /// 0x494
			 0xe8ccef8d, /// 0x498
			 0x51132dc0, /// 0x49c
			 0x177d21af, /// 0x4a0
			 0xec0682ca, /// 0x4a4
			 0xedbace05, /// 0x4a8
			 0x1c456ae8, /// 0x4ac
			 0xb6dad422, /// 0x4b0
			 0x75f50d24, /// 0x4b4
			 0x8dc687b1, /// 0x4b8
			 0xd4a94014, /// 0x4bc
			 0xa6b7f2dc, /// 0x4c0
			 0x8182d5b6, /// 0x4c4
			 0x05b17b50, /// 0x4c8
			 0xa7eeecd2, /// 0x4cc
			 0x6358982f, /// 0x4d0
			 0x6f1a9493, /// 0x4d4
			 0x0191559a, /// 0x4d8
			 0x4e4299da, /// 0x4dc
			 0x4c599536, /// 0x4e0
			 0x40e3d448, /// 0x4e4
			 0xbefce232, /// 0x4e8
			 0xcc8e7808, /// 0x4ec
			 0x24e3f746, /// 0x4f0
			 0xf42899d9, /// 0x4f4
			 0xa36ad878, /// 0x4f8
			 0x29ed92b9, /// 0x4fc
			 0x27031f6c, /// 0x500
			 0x6e1d516a, /// 0x504
			 0xd2c3586a, /// 0x508
			 0x4fbdd28e, /// 0x50c
			 0x81bf0038, /// 0x510
			 0x07287878, /// 0x514
			 0x56659971, /// 0x518
			 0x8fe13f68, /// 0x51c
			 0x9d4d0073, /// 0x520
			 0x49c197da, /// 0x524
			 0xa9cb7b37, /// 0x528
			 0x8658e377, /// 0x52c
			 0xc41f5c5a, /// 0x530
			 0xd0d36ab9, /// 0x534
			 0xa7b61175, /// 0x538
			 0x8625c245, /// 0x53c
			 0xfe47b786, /// 0x540
			 0xaa18845a, /// 0x544
			 0x57b1e46f, /// 0x548
			 0x4c0246a2, /// 0x54c
			 0xc849f7a6, /// 0x550
			 0x98f9cd88, /// 0x554
			 0xa05e02bc, /// 0x558
			 0x711ea97d, /// 0x55c
			 0xc635d9e3, /// 0x560
			 0xc4504dd1, /// 0x564
			 0x640edbcb, /// 0x568
			 0xa41802c4, /// 0x56c
			 0x87f769db, /// 0x570
			 0x89f93b37, /// 0x574
			 0x52c4a8c9, /// 0x578
			 0xc12357bc, /// 0x57c
			 0x5f525851, /// 0x580
			 0x3fe4e521, /// 0x584
			 0xcfe49725, /// 0x588
			 0x61c08997, /// 0x58c
			 0xe5721dc3, /// 0x590
			 0x3e183ea4, /// 0x594
			 0xb476d760, /// 0x598
			 0xc1fe4cb0, /// 0x59c
			 0x97e00eac, /// 0x5a0
			 0xaf3dbdad, /// 0x5a4
			 0xc48a817a, /// 0x5a8
			 0x32edf703, /// 0x5ac
			 0xf93388a3, /// 0x5b0
			 0xaff0b021, /// 0x5b4
			 0x847d5bc4, /// 0x5b8
			 0x78663b83, /// 0x5bc
			 0x2f6b225b, /// 0x5c0
			 0x675be329, /// 0x5c4
			 0x2354b17b, /// 0x5c8
			 0x9452997c, /// 0x5cc
			 0x49aff551, /// 0x5d0
			 0x581fd7cf, /// 0x5d4
			 0x991771de, /// 0x5d8
			 0x14f5276b, /// 0x5dc
			 0x34cb88f2, /// 0x5e0
			 0xe456f15a, /// 0x5e4
			 0x82335943, /// 0x5e8
			 0x86520f1b, /// 0x5ec
			 0xe697e6c7, /// 0x5f0
			 0x02e712f7, /// 0x5f4
			 0xd7986763, /// 0x5f8
			 0x123e803d, /// 0x5fc
			 0x1f8f0f16, /// 0x600
			 0x6ba1422b, /// 0x604
			 0x325ccd69, /// 0x608
			 0x068235cd, /// 0x60c
			 0xf3a92afd, /// 0x610
			 0xfa34ec0e, /// 0x614
			 0xdc73f7cd, /// 0x618
			 0x0136b89a, /// 0x61c
			 0xe768b245, /// 0x620
			 0xf7b52a92, /// 0x624
			 0x4b822247, /// 0x628
			 0xe0eef44e, /// 0x62c
			 0x842fc80d, /// 0x630
			 0x796ed714, /// 0x634
			 0xf27f6367, /// 0x638
			 0x32418ffe, /// 0x63c
			 0x4ef921c3, /// 0x640
			 0x4cecfc90, /// 0x644
			 0x40f4274e, /// 0x648
			 0xa695238f, /// 0x64c
			 0x60bd41fc, /// 0x650
			 0xf04c9402, /// 0x654
			 0xfd971b92, /// 0x658
			 0xb8068bdd, /// 0x65c
			 0x3a840ae2, /// 0x660
			 0x94698ed4, /// 0x664
			 0x10c52927, /// 0x668
			 0x85f68d77, /// 0x66c
			 0xa69cf8aa, /// 0x670
			 0x23b34bb1, /// 0x674
			 0x029ec956, /// 0x678
			 0x4e786f7f, /// 0x67c
			 0xc58ade46, /// 0x680
			 0x53501d59, /// 0x684
			 0xc7cb314e, /// 0x688
			 0x340cf770, /// 0x68c
			 0x076b916c, /// 0x690
			 0xc5d77ba4, /// 0x694
			 0xdde4bf6f, /// 0x698
			 0xfb65cba5, /// 0x69c
			 0x464a842b, /// 0x6a0
			 0x1f29af83, /// 0x6a4
			 0x818858ff, /// 0x6a8
			 0x33f7c268, /// 0x6ac
			 0xb019472d, /// 0x6b0
			 0x28c8a81f, /// 0x6b4
			 0x7d5e21b2, /// 0x6b8
			 0x56c3d87d, /// 0x6bc
			 0xbac730bb, /// 0x6c0
			 0x2b889d34, /// 0x6c4
			 0x0850dc8e, /// 0x6c8
			 0xe4ca2697, /// 0x6cc
			 0x3d74b76d, /// 0x6d0
			 0xebbb04e7, /// 0x6d4
			 0x72c82843, /// 0x6d8
			 0x42cbcb46, /// 0x6dc
			 0xae3a0b02, /// 0x6e0
			 0xd502f23b, /// 0x6e4
			 0x80177467, /// 0x6e8
			 0xd3630abe, /// 0x6ec
			 0x46c3c36b, /// 0x6f0
			 0xd5635134, /// 0x6f4
			 0x2edc2232, /// 0x6f8
			 0xc7026ece, /// 0x6fc
			 0x1c43c061, /// 0x700
			 0xc6c4c30a, /// 0x704
			 0x2f71c208, /// 0x708
			 0xef7b6480, /// 0x70c
			 0x9d10325d, /// 0x710
			 0x55fcc94d, /// 0x714
			 0x2825527e, /// 0x718
			 0x431ccd95, /// 0x71c
			 0x0037a0be, /// 0x720
			 0x1849538b, /// 0x724
			 0xe40f818e, /// 0x728
			 0xbc6b28d1, /// 0x72c
			 0xafee2b6c, /// 0x730
			 0xf8004c74, /// 0x734
			 0xc6ab3067, /// 0x738
			 0x77771c60, /// 0x73c
			 0x53814da6, /// 0x740
			 0xcacd7b4c, /// 0x744
			 0xabdfe06e, /// 0x748
			 0x4a921bb3, /// 0x74c
			 0xec5af78c, /// 0x750
			 0x5ff128ad, /// 0x754
			 0xf20a084a, /// 0x758
			 0x3369ef6d, /// 0x75c
			 0x070abf3e, /// 0x760
			 0xb46d4bea, /// 0x764
			 0xb66c1f74, /// 0x768
			 0xbccbfd02, /// 0x76c
			 0x8ee04e7e, /// 0x770
			 0x724cff3d, /// 0x774
			 0x94e74290, /// 0x778
			 0x329157d6, /// 0x77c
			 0x2e866eed, /// 0x780
			 0x6a17f580, /// 0x784
			 0x909bb627, /// 0x788
			 0xc51e31eb, /// 0x78c
			 0x736d1647, /// 0x790
			 0xeb8210b6, /// 0x794
			 0x60616ffa, /// 0x798
			 0xdf1939eb, /// 0x79c
			 0x933cb973, /// 0x7a0
			 0xe05f44d9, /// 0x7a4
			 0xb03b13b3, /// 0x7a8
			 0x25973bef, /// 0x7ac
			 0xf4db996b, /// 0x7b0
			 0xbf0274d1, /// 0x7b4
			 0x2e9aac07, /// 0x7b8
			 0xf0167c8b, /// 0x7bc
			 0x29f597f3, /// 0x7c0
			 0xc418d5a1, /// 0x7c4
			 0xb3330dbc, /// 0x7c8
			 0x78edb5c6, /// 0x7cc
			 0xb22276c5, /// 0x7d0
			 0xe4df7f23, /// 0x7d4
			 0x1ad343ac, /// 0x7d8
			 0x7b57fdc7, /// 0x7dc
			 0xbc91db34, /// 0x7e0
			 0xe52aaaec, /// 0x7e4
			 0x7c02419e, /// 0x7e8
			 0xfcd0e19b, /// 0x7ec
			 0xe8789592, /// 0x7f0
			 0xc9c8f0c6, /// 0x7f4
			 0xa1380931, /// 0x7f8
			 0xfb3c47a6, /// 0x7fc
			 0xf860e487, /// 0x800
			 0xdb80ea49, /// 0x804
			 0xdcd42c71, /// 0x808
			 0x52376deb, /// 0x80c
			 0x427a5940, /// 0x810
			 0x2af3fd46, /// 0x814
			 0x08411c7f, /// 0x818
			 0xf2ab214c, /// 0x81c
			 0xde84bae1, /// 0x820
			 0x22e7294e, /// 0x824
			 0xfca1a77d, /// 0x828
			 0x9d01607e, /// 0x82c
			 0x6fbdfb0b, /// 0x830
			 0x09335a42, /// 0x834
			 0x2a1d9b86, /// 0x838
			 0x36337913, /// 0x83c
			 0x9aa091d1, /// 0x840
			 0x4f342321, /// 0x844
			 0xc3c3f3a8, /// 0x848
			 0x4d7953c5, /// 0x84c
			 0x49f35bd5, /// 0x850
			 0xb19fa624, /// 0x854
			 0x6aff9c12, /// 0x858
			 0x4e195d48, /// 0x85c
			 0xa55dc34c, /// 0x860
			 0xb0b7b0d8, /// 0x864
			 0xeadae6ca, /// 0x868
			 0x59dbc96c, /// 0x86c
			 0xe574c242, /// 0x870
			 0xd719698d, /// 0x874
			 0x90205874, /// 0x878
			 0x4c2c7ad6, /// 0x87c
			 0x01c5611a, /// 0x880
			 0x0cbc6390, /// 0x884
			 0x6a3fa327, /// 0x888
			 0x6e50f627, /// 0x88c
			 0x07813902, /// 0x890
			 0x2e374f1d, /// 0x894
			 0x564bdeb0, /// 0x898
			 0x9e5c4f6b, /// 0x89c
			 0x5ffb8d0e, /// 0x8a0
			 0x904910c3, /// 0x8a4
			 0x9ebf8942, /// 0x8a8
			 0xdffcd069, /// 0x8ac
			 0xfeb11efc, /// 0x8b0
			 0x31b90d53, /// 0x8b4
			 0x982fa817, /// 0x8b8
			 0x448a175f, /// 0x8bc
			 0xc4fbc2b4, /// 0x8c0
			 0x48c487ad, /// 0x8c4
			 0xdfb278eb, /// 0x8c8
			 0xe9611fed, /// 0x8cc
			 0xb0299a6b, /// 0x8d0
			 0x1a1d604c, /// 0x8d4
			 0x124860c0, /// 0x8d8
			 0x7b1786f6, /// 0x8dc
			 0x6bd844ef, /// 0x8e0
			 0x15687ba7, /// 0x8e4
			 0xd4647879, /// 0x8e8
			 0xca79cba7, /// 0x8ec
			 0x2404eabd, /// 0x8f0
			 0xbec10672, /// 0x8f4
			 0x3f29f548, /// 0x8f8
			 0x5d2ba58b, /// 0x8fc
			 0xd733a248, /// 0x900
			 0x272a2cf9, /// 0x904
			 0x85c3005b, /// 0x908
			 0x482f2016, /// 0x90c
			 0xebc69384, /// 0x910
			 0x3dc86963, /// 0x914
			 0xead032f4, /// 0x918
			 0xbad53cd4, /// 0x91c
			 0xd4643dd6, /// 0x920
			 0x06cd57b4, /// 0x924
			 0x1b4d85da, /// 0x928
			 0x4868dd02, /// 0x92c
			 0x4baa30f3, /// 0x930
			 0x0f2eaad1, /// 0x934
			 0x348f1b93, /// 0x938
			 0x62a1197c, /// 0x93c
			 0xcd4e8e6b, /// 0x940
			 0x0bdf071c, /// 0x944
			 0x0fca67a6, /// 0x948
			 0x0c9a3652, /// 0x94c
			 0x3f83b724, /// 0x950
			 0x0a8a2acf, /// 0x954
			 0xe9ed2554, /// 0x958
			 0x911ba6da, /// 0x95c
			 0xfb298f45, /// 0x960
			 0xf1e3450e, /// 0x964
			 0xa5ce4278, /// 0x968
			 0x85b09a27, /// 0x96c
			 0xc0554d2b, /// 0x970
			 0x515d8820, /// 0x974
			 0x972922bf, /// 0x978
			 0x28150cf0, /// 0x97c
			 0x415a2ac7, /// 0x980
			 0xe77c96ca, /// 0x984
			 0xc0e0b3e4, /// 0x988
			 0x5c3726e1, /// 0x98c
			 0xaa7525cb, /// 0x990
			 0x6694283d, /// 0x994
			 0x751738de, /// 0x998
			 0x079c9bb2, /// 0x99c
			 0xf602206f, /// 0x9a0
			 0xb34f6073, /// 0x9a4
			 0x92eb3ae1, /// 0x9a8
			 0x058db2b5, /// 0x9ac
			 0xc09a3478, /// 0x9b0
			 0x776c0663, /// 0x9b4
			 0xe093650d, /// 0x9b8
			 0x2e5a14e3, /// 0x9bc
			 0xb7e48a29, /// 0x9c0
			 0xc05c4852, /// 0x9c4
			 0xc0b58255, /// 0x9c8
			 0x81061eb8, /// 0x9cc
			 0x32bfacbb, /// 0x9d0
			 0x9647ba5e, /// 0x9d4
			 0x5721447b, /// 0x9d8
			 0x64089dd4, /// 0x9dc
			 0x36c53b68, /// 0x9e0
			 0xcc208c2d, /// 0x9e4
			 0x956b314c, /// 0x9e8
			 0x15abb93a, /// 0x9ec
			 0x1924487a, /// 0x9f0
			 0xf4d2d0cb, /// 0x9f4
			 0xe566e7e2, /// 0x9f8
			 0xecc0e9c7, /// 0x9fc
			 0x3a70cee4, /// 0xa00
			 0x3d65f86f, /// 0xa04
			 0x5bb80f30, /// 0xa08
			 0xa261ef1d, /// 0xa0c
			 0x98cb4f0a, /// 0xa10
			 0x0fb3f38c, /// 0xa14
			 0xa8fe1bde, /// 0xa18
			 0x9395dc2d, /// 0xa1c
			 0xcd45285b, /// 0xa20
			 0xbb2a9c94, /// 0xa24
			 0x3ec0c373, /// 0xa28
			 0x1d542d95, /// 0xa2c
			 0xf1f04cd9, /// 0xa30
			 0x62272b43, /// 0xa34
			 0x2daab858, /// 0xa38
			 0x28fb022b, /// 0xa3c
			 0xfc2b4f56, /// 0xa40
			 0xd564a5cb, /// 0xa44
			 0xb3239780, /// 0xa48
			 0x6144b8e4, /// 0xa4c
			 0x45159f76, /// 0xa50
			 0xb38f2912, /// 0xa54
			 0xaf0fc01e, /// 0xa58
			 0x291420ff, /// 0xa5c
			 0x4076e487, /// 0xa60
			 0x6e47f552, /// 0xa64
			 0x738280c4, /// 0xa68
			 0x0ace7604, /// 0xa6c
			 0x7a7ae822, /// 0xa70
			 0xcb03f9cd, /// 0xa74
			 0xea9e6f38, /// 0xa78
			 0x40e94a6b, /// 0xa7c
			 0xc8646d4b, /// 0xa80
			 0x64d8c7f0, /// 0xa84
			 0x5e93d1bd, /// 0xa88
			 0xc94e2eb4, /// 0xa8c
			 0xe64e8e76, /// 0xa90
			 0x3b331c84, /// 0xa94
			 0xc6a4d60d, /// 0xa98
			 0x57a0d089, /// 0xa9c
			 0x9f35e671, /// 0xaa0
			 0x3874ddbd, /// 0xaa4
			 0xb71fc68c, /// 0xaa8
			 0xa59cdc4e, /// 0xaac
			 0x309d7da8, /// 0xab0
			 0x24b029d1, /// 0xab4
			 0x2cfa1b75, /// 0xab8
			 0x7a735fa0, /// 0xabc
			 0x2afbc8c6, /// 0xac0
			 0x46ce6503, /// 0xac4
			 0xb58fc108, /// 0xac8
			 0x86f5ad5e, /// 0xacc
			 0x48653870, /// 0xad0
			 0xcfa76fc6, /// 0xad4
			 0x937b9265, /// 0xad8
			 0xce06edb7, /// 0xadc
			 0xc58e4cea, /// 0xae0
			 0x736bc0c4, /// 0xae4
			 0x0c498b46, /// 0xae8
			 0x194d02b8, /// 0xaec
			 0x4940042f, /// 0xaf0
			 0xcf6f8c30, /// 0xaf4
			 0xb13f17d3, /// 0xaf8
			 0x14a65873, /// 0xafc
			 0x80673551, /// 0xb00
			 0x7df05516, /// 0xb04
			 0x8e8ee5f6, /// 0xb08
			 0xdca6ad81, /// 0xb0c
			 0xafc01a00, /// 0xb10
			 0xf58e1b03, /// 0xb14
			 0x8ff01d40, /// 0xb18
			 0xd6639671, /// 0xb1c
			 0x690d9561, /// 0xb20
			 0xa34a3459, /// 0xb24
			 0x766a82fa, /// 0xb28
			 0xd349852d, /// 0xb2c
			 0x5d05bd2b, /// 0xb30
			 0x9913dd57, /// 0xb34
			 0x3fda00d0, /// 0xb38
			 0xedec0236, /// 0xb3c
			 0x989aba8b, /// 0xb40
			 0x61a1df7e, /// 0xb44
			 0x197f8204, /// 0xb48
			 0x5d18591e, /// 0xb4c
			 0xd379b0c7, /// 0xb50
			 0x9af2b2e4, /// 0xb54
			 0xcfb96690, /// 0xb58
			 0x31bfd118, /// 0xb5c
			 0xa00263a8, /// 0xb60
			 0x29cee8c9, /// 0xb64
			 0x52f4f728, /// 0xb68
			 0x66df26d3, /// 0xb6c
			 0x9b56940a, /// 0xb70
			 0x1f56086c, /// 0xb74
			 0x4c8c0168, /// 0xb78
			 0xdbbbfc73, /// 0xb7c
			 0xa2d42158, /// 0xb80
			 0x2554b56f, /// 0xb84
			 0xfe1f75fb, /// 0xb88
			 0x03b9d116, /// 0xb8c
			 0x0db58502, /// 0xb90
			 0xc0eb8ba7, /// 0xb94
			 0xe34bb0e6, /// 0xb98
			 0xabad1209, /// 0xb9c
			 0xcdc39aa1, /// 0xba0
			 0x7122103e, /// 0xba4
			 0xb8f3ea54, /// 0xba8
			 0x655fe72d, /// 0xbac
			 0x8e764e25, /// 0xbb0
			 0xf87c1d04, /// 0xbb4
			 0xa27bf8c4, /// 0xbb8
			 0xfa8bae0b, /// 0xbbc
			 0x8a6819b2, /// 0xbc0
			 0x8af89db9, /// 0xbc4
			 0x81395c21, /// 0xbc8
			 0xef8fc352, /// 0xbcc
			 0x4e852d11, /// 0xbd0
			 0x255f9dac, /// 0xbd4
			 0xe577448d, /// 0xbd8
			 0x60cd0ead, /// 0xbdc
			 0xff6b36f9, /// 0xbe0
			 0x9237cde8, /// 0xbe4
			 0xb58197f0, /// 0xbe8
			 0x01a90eea, /// 0xbec
			 0xe282d378, /// 0xbf0
			 0x8f4671fd, /// 0xbf4
			 0x51cb86d7, /// 0xbf8
			 0x7125a6e6, /// 0xbfc
			 0xc367a5cb, /// 0xc00
			 0x8b11ab28, /// 0xc04
			 0x740ada30, /// 0xc08
			 0x9de44d9c, /// 0xc0c
			 0x66c13dc6, /// 0xc10
			 0x01555d7b, /// 0xc14
			 0xcb731e21, /// 0xc18
			 0xf809f2f4, /// 0xc1c
			 0xe90568bc, /// 0xc20
			 0x7724fda3, /// 0xc24
			 0x653942cf, /// 0xc28
			 0x2bd95df0, /// 0xc2c
			 0x0c6a0dca, /// 0xc30
			 0x4e58b809, /// 0xc34
			 0x73c33ff9, /// 0xc38
			 0x2ef7764e, /// 0xc3c
			 0x5ba730c5, /// 0xc40
			 0xd5fed905, /// 0xc44
			 0x950b4403, /// 0xc48
			 0x0c6e9636, /// 0xc4c
			 0x9a7ef733, /// 0xc50
			 0x6dfeb5f6, /// 0xc54
			 0x2c189d1d, /// 0xc58
			 0xcc0bb4a1, /// 0xc5c
			 0xf9420453, /// 0xc60
			 0x7abe1b41, /// 0xc64
			 0xcc7bbf17, /// 0xc68
			 0xd2e49dc6, /// 0xc6c
			 0x1b727a77, /// 0xc70
			 0x8f2d0176, /// 0xc74
			 0x2679916c, /// 0xc78
			 0xba607dda, /// 0xc7c
			 0x6110dd4c, /// 0xc80
			 0x95c1c40f, /// 0xc84
			 0x48b0df99, /// 0xc88
			 0xaab7a924, /// 0xc8c
			 0x7b6ff68c, /// 0xc90
			 0xefb7204f, /// 0xc94
			 0xd0118481, /// 0xc98
			 0x2c22d21d, /// 0xc9c
			 0x693433d6, /// 0xca0
			 0xea801787, /// 0xca4
			 0x12716a87, /// 0xca8
			 0x433653c6, /// 0xcac
			 0x8b0e7225, /// 0xcb0
			 0x6171efad, /// 0xcb4
			 0x6e7a0f1c, /// 0xcb8
			 0x91fc1e9d, /// 0xcbc
			 0x1d6abff7, /// 0xcc0
			 0x3ed19df8, /// 0xcc4
			 0x750e9337, /// 0xcc8
			 0x02f1bedb, /// 0xccc
			 0x605efda8, /// 0xcd0
			 0x61f8011b, /// 0xcd4
			 0xb8697762, /// 0xcd8
			 0xe62e9fe3, /// 0xcdc
			 0xbf2ef636, /// 0xce0
			 0xb789d1f5, /// 0xce4
			 0xfb95f3e2, /// 0xce8
			 0x3908ef0e, /// 0xcec
			 0x6dd8bfd8, /// 0xcf0
			 0xedad69e7, /// 0xcf4
			 0x53b84f9a, /// 0xcf8
			 0xd31a3362, /// 0xcfc
			 0xaa016193, /// 0xd00
			 0x0350a98b, /// 0xd04
			 0x8ff993bd, /// 0xd08
			 0x9fa535ee, /// 0xd0c
			 0xdc81e017, /// 0xd10
			 0x870eed9e, /// 0xd14
			 0xaaa35184, /// 0xd18
			 0xd005ccae, /// 0xd1c
			 0xe112b02d, /// 0xd20
			 0x9d69b161, /// 0xd24
			 0x44f2eb32, /// 0xd28
			 0x5f611b96, /// 0xd2c
			 0x4a56bb5c, /// 0xd30
			 0xcd3ec5b5, /// 0xd34
			 0x0487e88e, /// 0xd38
			 0x2331a233, /// 0xd3c
			 0xa3a8afdc, /// 0xd40
			 0x3ae5fcdf, /// 0xd44
			 0xfe9bd1b7, /// 0xd48
			 0xf4792ad8, /// 0xd4c
			 0x7c9aafa3, /// 0xd50
			 0x9592b8f4, /// 0xd54
			 0x8891743c, /// 0xd58
			 0xb0717499, /// 0xd5c
			 0xb436b4b1, /// 0xd60
			 0xfa03f73e, /// 0xd64
			 0xf1874327, /// 0xd68
			 0xe6007b8d, /// 0xd6c
			 0x2bc3ab9c, /// 0xd70
			 0x98a680f8, /// 0xd74
			 0xd7f44839, /// 0xd78
			 0x2c3f2f6b, /// 0xd7c
			 0x9ac010a8, /// 0xd80
			 0x29c5c2d3, /// 0xd84
			 0xff83c2a3, /// 0xd88
			 0x30f97bf5, /// 0xd8c
			 0x37394db0, /// 0xd90
			 0xa2ab9cfb, /// 0xd94
			 0x4a694859, /// 0xd98
			 0x42409c92, /// 0xd9c
			 0x5bd43a03, /// 0xda0
			 0xf04d1b8b, /// 0xda4
			 0xec31decb, /// 0xda8
			 0x9d34e79c, /// 0xdac
			 0xbd1e94a6, /// 0xdb0
			 0x032454ea, /// 0xdb4
			 0x3ce21c53, /// 0xdb8
			 0x1fa66c15, /// 0xdbc
			 0x566b9997, /// 0xdc0
			 0xb8113d65, /// 0xdc4
			 0xa34cd254, /// 0xdc8
			 0x6925e134, /// 0xdcc
			 0xd4d64165, /// 0xdd0
			 0x0a8789f6, /// 0xdd4
			 0x1f35555a, /// 0xdd8
			 0x62c8d56f, /// 0xddc
			 0x1b9594fb, /// 0xde0
			 0x13466e1c, /// 0xde4
			 0x46ef37c7, /// 0xde8
			 0xfa2a0790, /// 0xdec
			 0xa431c74e, /// 0xdf0
			 0x2ac89936, /// 0xdf4
			 0xc6f6c94e, /// 0xdf8
			 0xd27523fd, /// 0xdfc
			 0x3049e2c0, /// 0xe00
			 0xa2881d68, /// 0xe04
			 0xe7cceef9, /// 0xe08
			 0xae2b1b6d, /// 0xe0c
			 0xde8d3878, /// 0xe10
			 0x4f8b195d, /// 0xe14
			 0x95ad13cc, /// 0xe18
			 0x9b4270d1, /// 0xe1c
			 0x548fa4ca, /// 0xe20
			 0x43cdad27, /// 0xe24
			 0x5e9b2751, /// 0xe28
			 0x39ac1c4e, /// 0xe2c
			 0x536d5b85, /// 0xe30
			 0xc9ef79a3, /// 0xe34
			 0xa071ceac, /// 0xe38
			 0x14c9f021, /// 0xe3c
			 0xe80bc096, /// 0xe40
			 0x9d216a16, /// 0xe44
			 0x2a58c211, /// 0xe48
			 0x3a5817e1, /// 0xe4c
			 0x7aa67a3b, /// 0xe50
			 0xdb044a87, /// 0xe54
			 0x66bbc46c, /// 0xe58
			 0xf6221536, /// 0xe5c
			 0xb4064222, /// 0xe60
			 0x62171e83, /// 0xe64
			 0x55b4a9d5, /// 0xe68
			 0xa8faf412, /// 0xe6c
			 0xe3e56b1f, /// 0xe70
			 0x6f174244, /// 0xe74
			 0xe8238641, /// 0xe78
			 0x6a740274, /// 0xe7c
			 0xda596240, /// 0xe80
			 0x1cea2499, /// 0xe84
			 0xa98e2711, /// 0xe88
			 0x2c59957f, /// 0xe8c
			 0x616a7e75, /// 0xe90
			 0x39f11690, /// 0xe94
			 0x9eed88d1, /// 0xe98
			 0x8ed69ea9, /// 0xe9c
			 0x8f37f370, /// 0xea0
			 0xf46f6c6a, /// 0xea4
			 0x2886e084, /// 0xea8
			 0x29586fca, /// 0xeac
			 0xc7fe9765, /// 0xeb0
			 0x72d9f9af, /// 0xeb4
			 0x6095b415, /// 0xeb8
			 0x823fd980, /// 0xebc
			 0x99ec596e, /// 0xec0
			 0x8cf3aa73, /// 0xec4
			 0xe745b731, /// 0xec8
			 0x7a65ecc2, /// 0xecc
			 0x4d399c7d, /// 0xed0
			 0x440183d0, /// 0xed4
			 0x21d5af20, /// 0xed8
			 0x3ad0ba18, /// 0xedc
			 0xe15040ed, /// 0xee0
			 0xd61ca063, /// 0xee4
			 0x426d65b6, /// 0xee8
			 0x2d16c01e, /// 0xeec
			 0xdf970523, /// 0xef0
			 0x2f669f92, /// 0xef4
			 0x598a3f56, /// 0xef8
			 0xd6c9e005, /// 0xefc
			 0xa1765424, /// 0xf00
			 0x0c20223c, /// 0xf04
			 0x450d5e15, /// 0xf08
			 0x4f33c502, /// 0xf0c
			 0x6b841c1d, /// 0xf10
			 0x539f00a8, /// 0xf14
			 0x9305e10d, /// 0xf18
			 0x9b76eab7, /// 0xf1c
			 0x0cdfbee2, /// 0xf20
			 0xe8489c3b, /// 0xf24
			 0xa2b96f2c, /// 0xf28
			 0x5ea03187, /// 0xf2c
			 0xdae50674, /// 0xf30
			 0xf403aecf, /// 0xf34
			 0x21f98fd6, /// 0xf38
			 0x63a4ff00, /// 0xf3c
			 0xd52bd5d6, /// 0xf40
			 0xec3ccea5, /// 0xf44
			 0x27189b2b, /// 0xf48
			 0x9bde1a32, /// 0xf4c
			 0x708ae09a, /// 0xf50
			 0xc96c775d, /// 0xf54
			 0x062e8702, /// 0xf58
			 0x12a046f8, /// 0xf5c
			 0xdd964bf2, /// 0xf60
			 0xe3c7a50b, /// 0xf64
			 0x5e65f4cd, /// 0xf68
			 0xcfa8b357, /// 0xf6c
			 0x9660506d, /// 0xf70
			 0xde801ad0, /// 0xf74
			 0xa3e061f1, /// 0xf78
			 0xe1e72455, /// 0xf7c
			 0xb82f5eef, /// 0xf80
			 0x8a6200a7, /// 0xf84
			 0xce8d475a, /// 0xf88
			 0x9149c623, /// 0xf8c
			 0xa237db1a, /// 0xf90
			 0x61784051, /// 0xf94
			 0x11b380b9, /// 0xf98
			 0x71751094, /// 0xf9c
			 0xa1741b44, /// 0xfa0
			 0x0f3e0172, /// 0xfa4
			 0xdbd817b5, /// 0xfa8
			 0x894d30fb, /// 0xfac
			 0x199faeeb, /// 0xfb0
			 0x4f9d1f65, /// 0xfb4
			 0x2f3fd15d, /// 0xfb8
			 0x15340979, /// 0xfbc
			 0x690a935a, /// 0xfc0
			 0x3875a038, /// 0xfc4
			 0xf44a6986, /// 0xfc8
			 0x2b87762a, /// 0xfcc
			 0x1c5a696d, /// 0xfd0
			 0xcd8d4893, /// 0xfd4
			 0x4101506d, /// 0xfd8
			 0x6b58f507, /// 0xfdc
			 0x9db605a5, /// 0xfe0
			 0x29556afe, /// 0xfe4
			 0x623772db, /// 0xfe8
			 0x96fd35ec, /// 0xfec
			 0x0443b6f3, /// 0xff0
			 0x1809673b, /// 0xff4
			 0xf29cbc41, /// 0xff8
			 0x7ab51d6c /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0000c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00010
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00018
			 0x00000000,                                                  // zero                                        /// 0001c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00020
			 0x80000000,                                                  // -zero                                       /// 00024
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00030
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0003c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00044
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0004c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0c400000,                                                  // Leading 1s:                                 /// 00054
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00058
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00060
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00070
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00084
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00088
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00094
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b4
			 0x33333333,                                                  // 4 random values                             /// 000b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x7f800000,                                                  // inf                                         /// 000c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f8
			 0x33333333,                                                  // 4 random values                             /// 000fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x0e000007,                                                  // Trailing 1s:                                /// 0010c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00114
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00118
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00120
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0012c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00130
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00134
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00138
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0013c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00140
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00144
			 0xffc00001,                                                  // -signaling NaN                              /// 00148
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0014c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00154
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80011111,                                                  // -9.7958E-41                                 /// 00174
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00178
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0017c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00180
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00190
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00194
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00198
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001f8
			 0x33333333,                                                  // 4 random values                             /// 001fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00200
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00204
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00208
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00210
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00218
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0021c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0c400000,                                                  // Leading 1s:                                 /// 00224
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0022c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x0c400000,                                                  // Leading 1s:                                 /// 00234
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00238
			 0x80011111,                                                  // -9.7958E-41                                 /// 0023c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00240
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00244
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00258
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0025c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00268
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0026c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00270
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00274
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00278
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0027c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00280
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00284
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0028c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00290
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0029c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00300
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00308
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0030c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00310
			 0x80011111,                                                  // -9.7958E-41                                 /// 00314
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0031c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00320
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00324
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0032c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0033c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x80011111,                                                  // -9.7958E-41                                 /// 00348
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00350
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00358
			 0x00011111,                                                  // 9.7958E-41                                  /// 0035c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00360
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00364
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00368
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00370
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0037c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00388
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00398
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 003b4
			 0xbf028f5c,                                                  // -0.51                                       /// 003b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003e4
			 0x7fc00001,                                                  // signaling NaN                               /// 003e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003f8
			 0xbf028f5c,                                                  // -0.51                                       /// 003fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00408
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0040c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00414
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00424
			 0x0c600000,                                                  // Leading 1s:                                 /// 00428
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00430
			 0xff800000,                                                  // -inf                                        /// 00434
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0043c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x80000000,                                                  // -zero                                       /// 00448
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0044c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00454
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0xcb000000,                                                  // -8388608.0                                  /// 0045c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00460
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00470
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00478
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00488
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0048c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00490
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00494
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0049c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004e8
			 0x7fc00001,                                                  // signaling NaN                               /// 004ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f0
			 0x3f028f5c,                                                  // 0.51                                        /// 004f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00500
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x7fc00001,                                                  // signaling NaN                               /// 00508
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00510
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0051c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00520
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0052c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00530
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00534
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0053c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00544
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00548
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0054c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00550
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00560
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00564
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00568
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00570
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00578
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00584
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80000000,                                                  // -zero                                       /// 0058c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00594
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00598
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0xffc00001,                                                  // -signaling NaN                              /// 005c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005cc
			 0xffc00001,                                                  // -signaling NaN                              /// 005d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00600
			 0x0c600000,                                                  // Leading 1s:                                 /// 00604
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00608
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00610
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00614
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00618
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0061c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0062c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00634
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00638
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f800000,                                                  // inf                                         /// 00640
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x00011111,                                                  // 9.7958E-41                                  /// 00648
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0064c
			 0x7fc00001,                                                  // signaling NaN                               /// 00650
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00654
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0065c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00668
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0066c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00670
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00674
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00678
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0067c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c600000,                                                  // Leading 1s:                                 /// 00688
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0068c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00690
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00694
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x55555555,                                                  // 4 random values                             /// 0069c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000000,                                                  // -zero                                       /// 006f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00700
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00708
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00710
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00714
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00718
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0071c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00720
			 0x0e000001,                                                  // Trailing 1s:                                /// 00724
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00730
			 0x0c700000,                                                  // Leading 1s:                                 /// 00734
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00738
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00740
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00750
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00758
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00760
			 0xffc00001,                                                  // -signaling NaN                              /// 00764
			 0x0c780000,                                                  // Leading 1s:                                 /// 00768
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00770
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x4b000000,                                                  // 8388608.0                                   /// 00778
			 0x7fc00001,                                                  // signaling NaN                               /// 0077c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e000007,                                                  // Trailing 1s:                                /// 00784
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00788
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00794
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0079c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x4b000000,                                                  // 8388608.0                                   /// 007ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00800
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00804
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0080c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x7fc00001,                                                  // signaling NaN                               /// 00814
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00818
			 0x0c400000,                                                  // Leading 1s:                                 /// 0081c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00824
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00830
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00838
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00840
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00844
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0084c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00850
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00854
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00858
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0085c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00864
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00868
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00874
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00878
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0087c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00880
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00884
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00888
			 0xff800000,                                                  // -inf                                        /// 0088c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00890
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00894
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00898
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0089c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008a8
			 0x00000000,                                                  // zero                                        /// 008ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 008e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00900
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00904
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00908
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0090c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00910
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00914
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x4b000000,                                                  // 8388608.0                                   /// 0091c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00920
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0092c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00934
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00940
			 0x0c600000,                                                  // Leading 1s:                                 /// 00944
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00948
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0094c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00954
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00960
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00964
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00968
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00970
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00974
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00980
			 0xffc00001,                                                  // -signaling NaN                              /// 00984
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80000000,                                                  // -zero                                       /// 00994
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00998
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0099c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c0
			 0x7f800000,                                                  // inf                                         /// 009c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aa4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ab0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ab4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ab8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ac0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ac8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ad8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00adc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ae0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ae8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00af8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00afc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b00
			 0x7f800000,                                                  // inf                                         /// 00b04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b20
			 0x3f028f5c,                                                  // 0.51                                        /// 00b24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x55555555,                                                  // 4 random values                             /// 00b38
			 0x4b000000,                                                  // 8388608.0                                   /// 00b3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b60
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bbc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bcc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bdc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00be4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000000,                                                  // zero                                        /// 00c20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c30
			 0x33333333,                                                  // 4 random values                             /// 00c34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c48
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c64
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0xffc00001,                                                  // -signaling NaN                              /// 00c78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ca8
			 0x00000000,                                                  // zero                                        /// 00cac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ccc
			 0x00000000,                                                  // zero                                        /// 00cd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cdc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ce0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cf4
			 0x33333333,                                                  // 4 random values                             /// 00cf8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cfc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d24
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d28
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d60
			 0x4b000000,                                                  // 8388608.0                                   /// 00d64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00da0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00db0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dcc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ddc
			 0x7f800000,                                                  // inf                                         /// 00de0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00de8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00df0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00000000,                                                  // zero                                        /// 00e34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00eb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ec0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ecc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ed0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ed4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ed8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00eec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00efc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f04
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f20
			 0xbf028f5c,                                                  // -0.51                                       /// 00f24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f28
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00000000,                                                  // zero                                        /// 00f64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f80
			 0x7f800000,                                                  // inf                                         /// 00f84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f88
			 0x33333333,                                                  // 4 random values                             /// 00f8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f98
			 0x33333333,                                                  // 4 random values                             /// 00f9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00fa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc4
			 0xff800000,                                                  // -inf                                        /// 00fc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fdc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fe0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fe4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fe8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ff0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ff4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80800002 // min norm + 2ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c600000,                                                  // Leading 1s:                                 /// 00000
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0000c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00010
			 0x0e000001,                                                  // Trailing 1s:                                /// 00014
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00018
			 0x0e000001,                                                  // Trailing 1s:                                /// 0001c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00020
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00024
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00028
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00034
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0003c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00040
			 0x3f028f5c,                                                  // 0.51                                        /// 00044
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00048
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0004c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00050
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00054
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00058
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0005c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00060
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00064
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00068
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0006c
			 0x4b000000,                                                  // 8388608.0                                   /// 00070
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00074
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00078
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0007c
			 0xff800000,                                                  // -inf                                        /// 00080
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00084
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00088
			 0x0e000001,                                                  // Trailing 1s:                                /// 0008c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00090
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0c600000,                                                  // Leading 1s:                                 /// 00098
			 0x0c700000,                                                  // Leading 1s:                                 /// 0009c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000b0
			 0x3f028f5c,                                                  // 0.51                                        /// 000b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x55555555,                                                  // 4 random values                             /// 000c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00100
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00104
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00108
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00110
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00118
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0011c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00124
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80000000,                                                  // -zero                                       /// 0012c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00130
			 0x3f028f5c,                                                  // 0.51                                        /// 00134
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0013c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00140
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0014c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00154
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00158
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0015c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00160
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00164
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00168
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00170
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00174
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00178
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0017c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00180
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00184
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00188
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0018c
			 0x7f800000,                                                  // inf                                         /// 00190
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00194
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00198
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a0
			 0x33333333,                                                  // 4 random values                             /// 001a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00200
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00204
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x3f028f5c,                                                  // 0.51                                        /// 00210
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00220
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00224
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00234
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00238
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x33333333,                                                  // 4 random values                             /// 00240
			 0x4b000000,                                                  // 8388608.0                                   /// 00244
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0024c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00258
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0025c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00264
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0026c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00274
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0027c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00280
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00284
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00288
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0028c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00290
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00294
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00298
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 002f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00300
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00304
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0030c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00310
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00314
			 0x0c700000,                                                  // Leading 1s:                                 /// 00318
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0031c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00324
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0032c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00334
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00338
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0033c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00348
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0034c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00350
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0035c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00360
			 0x80000000,                                                  // -zero                                       /// 00364
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0036c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00370
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0xffc00001,                                                  // -signaling NaN                              /// 0037c
			 0x55555555,                                                  // 4 random values                             /// 00380
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x0e000003,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00390
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0039c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 003d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xbf028f5c,                                                  // -0.51                                       /// 003e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00400
			 0x0e000007,                                                  // Trailing 1s:                                /// 00404
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x7fc00001,                                                  // signaling NaN                               /// 00418
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0041c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00424
			 0x7fc00001,                                                  // signaling NaN                               /// 00428
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0042c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00430
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00438
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00440
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00450
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00454
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00458
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00464
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00468
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00470
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00478
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00480
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00488
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00000000,                                                  // zero                                        /// 00490
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00494
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00498
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00500
			 0xff800000,                                                  // -inf                                        /// 00504
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00508
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00510
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00514
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00518
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0051c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00528
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0052c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00534
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00538
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0xffc00001,                                                  // -signaling NaN                              /// 00540
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00544
			 0x3f028f5c,                                                  // 0.51                                        /// 00548
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0054c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00550
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00554
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00000000,                                                  // zero                                        /// 0055c
			 0xcb000000,                                                  // -8388608.0                                  /// 00560
			 0x0c780000,                                                  // Leading 1s:                                 /// 00564
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00568
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0056c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00570
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00578
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0057c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00580
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0e000003,                                                  // Trailing 1s:                                /// 00588
			 0x4b000000,                                                  // 8388608.0                                   /// 0058c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00590
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x00011111,                                                  // 9.7958E-41                                  /// 0059c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x55555555,                                                  // 4 random values                             /// 005b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005bc
			 0x80000000,                                                  // -zero                                       /// 005c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00608
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0060c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00614
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00618
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00620
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00628
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0062c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00630
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0064c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00654
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00658
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0065c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0066c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00670
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00674
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00678
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0067c
			 0xff800000,                                                  // -inf                                        /// 00680
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00688
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00690
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00694
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0069c
			 0x55555555,                                                  // 4 random values                             /// 006a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006ac
			 0xff800000,                                                  // -inf                                        /// 006b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0xffc00001,                                                  // -signaling NaN                              /// 006bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00700
			 0xffc00001,                                                  // -signaling NaN                              /// 00704
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0070c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00710
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00718
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00720
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00728
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0072c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00734
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00738
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0073c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00740
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00744
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0074c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00754
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00758
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x7fc00001,                                                  // signaling NaN                               /// 00760
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0e000003,                                                  // Trailing 1s:                                /// 00768
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0076c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00770
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0077c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00780
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00784
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00788
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00794
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00798
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0079c
			 0x0c600000,                                                  // Leading 1s:                                 /// 007a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x55555555,                                                  // 4 random values                             /// 007b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00800
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00808
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00810
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0081c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00820
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00824
			 0x00011111,                                                  // 9.7958E-41                                  /// 00828
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xff800000,                                                  // -inf                                        /// 00830
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00834
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0083c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00840
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00844
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000000,                                                  // zero                                        /// 00854
			 0x7fc00001,                                                  // signaling NaN                               /// 00858
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0085c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00868
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0086c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00870
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00878
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0087c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00884
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00894
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00898
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0089c
			 0x0e000007,                                                  // Trailing 1s:                                /// 008a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a8
			 0xffc00001,                                                  // -signaling NaN                              /// 008ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x7fc00001,                                                  // signaling NaN                               /// 008dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x4b000000,                                                  // 8388608.0                                   /// 00904
			 0x0e000007,                                                  // Trailing 1s:                                /// 00908
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0090c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00910
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00914
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00918
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00928
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00930
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00934
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00938
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0093c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00940
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00944
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00948
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00954
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00958
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0095c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00960
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00964
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00968
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0096c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00974
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00980
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0098c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00990
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00994
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x55555555,                                                  // 4 random values                             /// 0099c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x3f028f5c,                                                  // 0.51                                        /// 009e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009e4
			 0x7fc00001,                                                  // signaling NaN                               /// 009e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0xcb000000,                                                  // -8388608.0                                  /// 009f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0xff800000,                                                  // -inf                                        /// 00a14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xbf028f5c,                                                  // -0.51                                       /// 00a1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a30
			 0xffc00001,                                                  // -signaling NaN                              /// 00a34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000000,                                                  // -zero                                       /// 00a60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ab4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ac4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00adc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ae0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ae8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00af0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00af4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00afc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x33333333,                                                  // 4 random values                             /// 00b2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b60
			 0x3f028f5c,                                                  // 0.51                                        /// 00b64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b78
			 0x00000000,                                                  // zero                                        /// 00b7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ba0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bc8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bdc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00be0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00be4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bf0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bf4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cbc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ce4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cf4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d98
			 0xff800000,                                                  // -inf                                        /// 00d9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00da4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00db4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dd0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dd8
			 0x55555555,                                                  // 4 random values                             /// 00ddc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00de8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00df0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00df4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xbf028f5c,                                                  // -0.51                                       /// 00e00
			 0xffc00001,                                                  // -signaling NaN                              /// 00e04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e30
			 0x00000000,                                                  // zero                                        /// 00e34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb0
			 0x33333333,                                                  // 4 random values                             /// 00eb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ebc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ed4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00edc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ee8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ef4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00efc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f04
			 0x7fc00001,                                                  // signaling NaN                               /// 00f08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f70
			 0x7f800000,                                                  // inf                                         /// 00f74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f80
			 0x4b000000,                                                  // 8388608.0                                   /// 00f84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fa0
			 0x7f800000,                                                  // inf                                         /// 00fa4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff8
			 0x00800003 // min norm + 3ulp                               // subnormals +ve                              /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xc48c052e, /// 0x0
			 0x3e8d3086, /// 0x4
			 0xff6ee5dd, /// 0x8
			 0x2bf8eec9, /// 0xc
			 0x318b04a7, /// 0x10
			 0x50ef5b16, /// 0x14
			 0xd37487ac, /// 0x18
			 0x2db72ef2, /// 0x1c
			 0x9f09a8c0, /// 0x20
			 0x0bce535c, /// 0x24
			 0xe69848c9, /// 0x28
			 0x43b7447f, /// 0x2c
			 0xee947f2c, /// 0x30
			 0xe5f1cf52, /// 0x34
			 0x3d0ecebf, /// 0x38
			 0xcf73192a, /// 0x3c
			 0xb29a453b, /// 0x40
			 0x35e6d07c, /// 0x44
			 0x5d8953ee, /// 0x48
			 0x9f413dfb, /// 0x4c
			 0x5f9ab367, /// 0x50
			 0x14833908, /// 0x54
			 0x7bef8e98, /// 0x58
			 0xc696e5d2, /// 0x5c
			 0x4c7c5565, /// 0x60
			 0xe03a0e9c, /// 0x64
			 0x76413a18, /// 0x68
			 0x0b1459a2, /// 0x6c
			 0xfc9ed2ec, /// 0x70
			 0x935839ae, /// 0x74
			 0x871c7991, /// 0x78
			 0xad7eff76, /// 0x7c
			 0x105ad455, /// 0x80
			 0xb331f74d, /// 0x84
			 0x9b13c55b, /// 0x88
			 0xfc9fd0f2, /// 0x8c
			 0x64c37990, /// 0x90
			 0x5e2911a1, /// 0x94
			 0x5f2e09a4, /// 0x98
			 0x7887ee87, /// 0x9c
			 0x7613c28b, /// 0xa0
			 0xbdab89d6, /// 0xa4
			 0x1afcdcd9, /// 0xa8
			 0x0ef647cc, /// 0xac
			 0x4cabb158, /// 0xb0
			 0x94fc00df, /// 0xb4
			 0x934de82b, /// 0xb8
			 0x85dfec35, /// 0xbc
			 0xbb465c9c, /// 0xc0
			 0x1bac39e0, /// 0xc4
			 0x399cf88b, /// 0xc8
			 0xb5c35b8f, /// 0xcc
			 0x635e0676, /// 0xd0
			 0x1e3ddc17, /// 0xd4
			 0x7c66fcdc, /// 0xd8
			 0x54c442b6, /// 0xdc
			 0xa7834f2b, /// 0xe0
			 0x51e08890, /// 0xe4
			 0xb34b067e, /// 0xe8
			 0x8928e36c, /// 0xec
			 0x6046f36d, /// 0xf0
			 0x371418ca, /// 0xf4
			 0xe3e09fa5, /// 0xf8
			 0x5dd90ed8, /// 0xfc
			 0x677dcd70, /// 0x100
			 0x7a3f9f04, /// 0x104
			 0x16a80c12, /// 0x108
			 0x2de6bbf5, /// 0x10c
			 0x67b0815a, /// 0x110
			 0x0eec587a, /// 0x114
			 0x43029db6, /// 0x118
			 0x5e42b8f2, /// 0x11c
			 0x3d25f613, /// 0x120
			 0xd7062ca9, /// 0x124
			 0x668c9edf, /// 0x128
			 0x8342c9f7, /// 0x12c
			 0xb9d1cc67, /// 0x130
			 0x480351a6, /// 0x134
			 0x4a549153, /// 0x138
			 0xd0e89594, /// 0x13c
			 0x4afaf3fc, /// 0x140
			 0xe9798db5, /// 0x144
			 0xabe42855, /// 0x148
			 0xcceb3401, /// 0x14c
			 0xce536a8c, /// 0x150
			 0xec185a90, /// 0x154
			 0x79ca8822, /// 0x158
			 0x3c8f252d, /// 0x15c
			 0x38e7cdf3, /// 0x160
			 0x0a283858, /// 0x164
			 0xc8e0389c, /// 0x168
			 0x2f3fb027, /// 0x16c
			 0x77c7e031, /// 0x170
			 0xc100b97c, /// 0x174
			 0x82811c5b, /// 0x178
			 0xc26cccf6, /// 0x17c
			 0x5a345126, /// 0x180
			 0x2c6f010b, /// 0x184
			 0x8cb87141, /// 0x188
			 0xfc5c77b2, /// 0x18c
			 0xcd1e4ec7, /// 0x190
			 0x5522d22d, /// 0x194
			 0x4f7f4ac7, /// 0x198
			 0x3dcaf7f7, /// 0x19c
			 0xb9a0b7a2, /// 0x1a0
			 0x08880c87, /// 0x1a4
			 0x1b5f68a9, /// 0x1a8
			 0xcdabcf8f, /// 0x1ac
			 0x35869fd5, /// 0x1b0
			 0x14d7f8f9, /// 0x1b4
			 0x3ecad4df, /// 0x1b8
			 0xead7c534, /// 0x1bc
			 0x98c31ce2, /// 0x1c0
			 0xfa4bd9d4, /// 0x1c4
			 0x95c83e2e, /// 0x1c8
			 0xdb9e8c4c, /// 0x1cc
			 0x733b24c8, /// 0x1d0
			 0x4f8a1bfb, /// 0x1d4
			 0xff2990fe, /// 0x1d8
			 0x22716a21, /// 0x1dc
			 0xcc2e3c9b, /// 0x1e0
			 0xc6943894, /// 0x1e4
			 0x12b56e99, /// 0x1e8
			 0x9517348b, /// 0x1ec
			 0x26841011, /// 0x1f0
			 0xf2a63095, /// 0x1f4
			 0x83cb4ec5, /// 0x1f8
			 0x67f6685c, /// 0x1fc
			 0xcc0a67be, /// 0x200
			 0x0f4de558, /// 0x204
			 0x78a0d506, /// 0x208
			 0xbd0c73fd, /// 0x20c
			 0x950d24e8, /// 0x210
			 0xfb7791aa, /// 0x214
			 0x8db7b7fa, /// 0x218
			 0x8d60e99f, /// 0x21c
			 0x83ae4f6f, /// 0x220
			 0x88838170, /// 0x224
			 0x8e093ce4, /// 0x228
			 0x43fb476e, /// 0x22c
			 0xf441577c, /// 0x230
			 0x911c5b3f, /// 0x234
			 0x91591d84, /// 0x238
			 0x5d107bb0, /// 0x23c
			 0x365ae51b, /// 0x240
			 0xe673c2f9, /// 0x244
			 0xd6f34331, /// 0x248
			 0x9525eb45, /// 0x24c
			 0x2606b661, /// 0x250
			 0x40500515, /// 0x254
			 0x618b9262, /// 0x258
			 0x776f6e4c, /// 0x25c
			 0xe184594f, /// 0x260
			 0xe87796ae, /// 0x264
			 0x391a5887, /// 0x268
			 0x9972d32b, /// 0x26c
			 0xde323cc6, /// 0x270
			 0xb0ba1ce7, /// 0x274
			 0xf37b7514, /// 0x278
			 0x05abed7e, /// 0x27c
			 0x8710588f, /// 0x280
			 0x4b367138, /// 0x284
			 0x10ae8be3, /// 0x288
			 0x74471b57, /// 0x28c
			 0xc711b43b, /// 0x290
			 0xff33cdcb, /// 0x294
			 0x1945468e, /// 0x298
			 0x1be7a320, /// 0x29c
			 0x80da575f, /// 0x2a0
			 0x5074d454, /// 0x2a4
			 0x47dee435, /// 0x2a8
			 0xf13d2188, /// 0x2ac
			 0x1c8399e9, /// 0x2b0
			 0x09a085bc, /// 0x2b4
			 0x208577f6, /// 0x2b8
			 0x942ff3a2, /// 0x2bc
			 0x5c1848ae, /// 0x2c0
			 0xc209a7a4, /// 0x2c4
			 0xdc4abfe6, /// 0x2c8
			 0xb984f95f, /// 0x2cc
			 0xd9247e13, /// 0x2d0
			 0x311184c8, /// 0x2d4
			 0x29de369d, /// 0x2d8
			 0x25394d7e, /// 0x2dc
			 0x0a00b981, /// 0x2e0
			 0xa6910119, /// 0x2e4
			 0x81fd3246, /// 0x2e8
			 0x28a91a63, /// 0x2ec
			 0xdbbedaef, /// 0x2f0
			 0xa0d34bbb, /// 0x2f4
			 0xf5a6a35d, /// 0x2f8
			 0x5e2ac458, /// 0x2fc
			 0xcbec15cb, /// 0x300
			 0x9ea06cd1, /// 0x304
			 0x58b55c8e, /// 0x308
			 0x96666523, /// 0x30c
			 0x17f40e77, /// 0x310
			 0x438c0f5d, /// 0x314
			 0xf99dbddd, /// 0x318
			 0xcb14cf9b, /// 0x31c
			 0x0c392836, /// 0x320
			 0x3adf22ab, /// 0x324
			 0xce90b782, /// 0x328
			 0x3e921d11, /// 0x32c
			 0xaccc5653, /// 0x330
			 0x914654c8, /// 0x334
			 0xfda0fa8b, /// 0x338
			 0xff693b0c, /// 0x33c
			 0xcfb29cc2, /// 0x340
			 0x20657c49, /// 0x344
			 0x0d7f1008, /// 0x348
			 0x046f88cd, /// 0x34c
			 0xcd6329a9, /// 0x350
			 0x085bc9a3, /// 0x354
			 0xdcfa2f13, /// 0x358
			 0xec749896, /// 0x35c
			 0x1b2923d4, /// 0x360
			 0x832daf81, /// 0x364
			 0x379bd09a, /// 0x368
			 0xb86c5070, /// 0x36c
			 0xb33ee564, /// 0x370
			 0x2e1e3dd7, /// 0x374
			 0x7653c13f, /// 0x378
			 0xbc77724a, /// 0x37c
			 0x4a1c300c, /// 0x380
			 0x2159be20, /// 0x384
			 0xdb35d0c5, /// 0x388
			 0x2e063424, /// 0x38c
			 0x775bfed6, /// 0x390
			 0x4044bbfb, /// 0x394
			 0x2c16f273, /// 0x398
			 0xb6279a17, /// 0x39c
			 0x85437066, /// 0x3a0
			 0x98ecfa4d, /// 0x3a4
			 0x6ea689da, /// 0x3a8
			 0xc702762c, /// 0x3ac
			 0xa7fffb5c, /// 0x3b0
			 0x7b1e9a18, /// 0x3b4
			 0xb8ef6ede, /// 0x3b8
			 0xe51ee9d1, /// 0x3bc
			 0x2e94f607, /// 0x3c0
			 0x64340ded, /// 0x3c4
			 0x77e2713a, /// 0x3c8
			 0xe350c226, /// 0x3cc
			 0x5dacf9b7, /// 0x3d0
			 0x7a21a4d7, /// 0x3d4
			 0xbd232d54, /// 0x3d8
			 0x1c13832c, /// 0x3dc
			 0x8d2e8282, /// 0x3e0
			 0x748c43fa, /// 0x3e4
			 0x4af338e3, /// 0x3e8
			 0xa454ce28, /// 0x3ec
			 0xaede40c5, /// 0x3f0
			 0x5c0074db, /// 0x3f4
			 0x0fba4122, /// 0x3f8
			 0x556a5483, /// 0x3fc
			 0x006175f5, /// 0x400
			 0x051dd74d, /// 0x404
			 0xa8521ae9, /// 0x408
			 0x692b1501, /// 0x40c
			 0x64bb8443, /// 0x410
			 0x868d9504, /// 0x414
			 0x9c7df10c, /// 0x418
			 0x4f0dfce4, /// 0x41c
			 0xb53930e7, /// 0x420
			 0xb8530d8c, /// 0x424
			 0x6d9fd88a, /// 0x428
			 0x5989e82e, /// 0x42c
			 0x0810c8ab, /// 0x430
			 0xf6066033, /// 0x434
			 0x6d35ceb5, /// 0x438
			 0x529d383f, /// 0x43c
			 0x69d8e588, /// 0x440
			 0x731f9d6f, /// 0x444
			 0x03f6b58a, /// 0x448
			 0x507eaeaa, /// 0x44c
			 0x79434098, /// 0x450
			 0x26c53b0e, /// 0x454
			 0xc8dae2b8, /// 0x458
			 0xa39bff45, /// 0x45c
			 0xbaf1622f, /// 0x460
			 0xc4acc81f, /// 0x464
			 0xa46527ca, /// 0x468
			 0x6dd26979, /// 0x46c
			 0xaa0a83a1, /// 0x470
			 0xd4fea19b, /// 0x474
			 0xca6e79d1, /// 0x478
			 0x6334fba7, /// 0x47c
			 0x0758e21f, /// 0x480
			 0x455e7fc5, /// 0x484
			 0x5582cf6d, /// 0x488
			 0x841a0c94, /// 0x48c
			 0x9817b1f9, /// 0x490
			 0x285a0fda, /// 0x494
			 0xab30c68b, /// 0x498
			 0x91c3a587, /// 0x49c
			 0x45a7919a, /// 0x4a0
			 0x81a45dbc, /// 0x4a4
			 0x0160b8a9, /// 0x4a8
			 0x4edf5ef7, /// 0x4ac
			 0xfb40f37e, /// 0x4b0
			 0x63e42d5b, /// 0x4b4
			 0xf315431c, /// 0x4b8
			 0x5433738f, /// 0x4bc
			 0x5e6c8845, /// 0x4c0
			 0x324a68f8, /// 0x4c4
			 0xcf5f423e, /// 0x4c8
			 0x0f0ba151, /// 0x4cc
			 0xd6acf298, /// 0x4d0
			 0x86cd03f2, /// 0x4d4
			 0xc96e70d9, /// 0x4d8
			 0x5f5829e9, /// 0x4dc
			 0x34e76f60, /// 0x4e0
			 0x1237636a, /// 0x4e4
			 0x96504757, /// 0x4e8
			 0xa0b0a63a, /// 0x4ec
			 0x0729df60, /// 0x4f0
			 0xba2df7dc, /// 0x4f4
			 0x8c34db3a, /// 0x4f8
			 0x5c41f997, /// 0x4fc
			 0xda352ded, /// 0x500
			 0x36f2c67f, /// 0x504
			 0x7a412746, /// 0x508
			 0x5ae0b657, /// 0x50c
			 0xe7c35d43, /// 0x510
			 0xb8160fd3, /// 0x514
			 0x7dfd6a95, /// 0x518
			 0x5c7ae885, /// 0x51c
			 0x9bee2d18, /// 0x520
			 0x9a1b5f28, /// 0x524
			 0x3c19f389, /// 0x528
			 0x34d00ccb, /// 0x52c
			 0xf7ff7ad1, /// 0x530
			 0xc9f55d0c, /// 0x534
			 0x96bd355c, /// 0x538
			 0xbc922df3, /// 0x53c
			 0xc0e72e5d, /// 0x540
			 0x7555e64d, /// 0x544
			 0xa1598f6d, /// 0x548
			 0x11a86d0e, /// 0x54c
			 0x2187ccde, /// 0x550
			 0xa7f51cdf, /// 0x554
			 0xb6c0f200, /// 0x558
			 0x01bc333f, /// 0x55c
			 0xe57d8c91, /// 0x560
			 0x023935eb, /// 0x564
			 0x03abe6a5, /// 0x568
			 0x1be08f10, /// 0x56c
			 0x404f10b1, /// 0x570
			 0x656bf0a1, /// 0x574
			 0x89da9a61, /// 0x578
			 0xb4d8e04e, /// 0x57c
			 0x3d73a3a0, /// 0x580
			 0x8bfe1ba3, /// 0x584
			 0x7fd17fd0, /// 0x588
			 0xbd572675, /// 0x58c
			 0xadd67ce1, /// 0x590
			 0x69551f12, /// 0x594
			 0x167f0c62, /// 0x598
			 0x7708f10a, /// 0x59c
			 0x45888c1e, /// 0x5a0
			 0x10891bef, /// 0x5a4
			 0xcc4a4556, /// 0x5a8
			 0xb0036c60, /// 0x5ac
			 0xac73a169, /// 0x5b0
			 0x2c5dae05, /// 0x5b4
			 0xf56937b8, /// 0x5b8
			 0x1238ef64, /// 0x5bc
			 0x84f67a8f, /// 0x5c0
			 0x7c486153, /// 0x5c4
			 0x24d7d477, /// 0x5c8
			 0x4163c662, /// 0x5cc
			 0x761462ab, /// 0x5d0
			 0x8e5ee4e5, /// 0x5d4
			 0x82a4de44, /// 0x5d8
			 0xe115d717, /// 0x5dc
			 0x6876af34, /// 0x5e0
			 0xc9a036af, /// 0x5e4
			 0x0485c7bc, /// 0x5e8
			 0x7fb28b52, /// 0x5ec
			 0xb4cdc233, /// 0x5f0
			 0xe22c63aa, /// 0x5f4
			 0x33f47c35, /// 0x5f8
			 0x464064fa, /// 0x5fc
			 0x62dc716f, /// 0x600
			 0x399aa852, /// 0x604
			 0x35fef3e5, /// 0x608
			 0x79c27af2, /// 0x60c
			 0xdeca2ba0, /// 0x610
			 0x5f831ff4, /// 0x614
			 0x247bc657, /// 0x618
			 0xac92a38d, /// 0x61c
			 0xdbaf211f, /// 0x620
			 0xda7f090e, /// 0x624
			 0x30c32cb1, /// 0x628
			 0x08b151c1, /// 0x62c
			 0xfc0c53d4, /// 0x630
			 0xf6216d3a, /// 0x634
			 0x9eb039b2, /// 0x638
			 0xca5a93a9, /// 0x63c
			 0x7c046228, /// 0x640
			 0x5914bac1, /// 0x644
			 0x38b0ddd0, /// 0x648
			 0xe0f14e31, /// 0x64c
			 0xbb4df8b0, /// 0x650
			 0x03c4db34, /// 0x654
			 0x88d12d4f, /// 0x658
			 0xd1597849, /// 0x65c
			 0xf819177c, /// 0x660
			 0x99fe06b3, /// 0x664
			 0x62188afc, /// 0x668
			 0x64e258e0, /// 0x66c
			 0xc266b3a6, /// 0x670
			 0xb1f95d55, /// 0x674
			 0x5e257f0a, /// 0x678
			 0x5d4a0883, /// 0x67c
			 0x216e49ab, /// 0x680
			 0xda4ef48e, /// 0x684
			 0xa7a8e172, /// 0x688
			 0xe7338f5a, /// 0x68c
			 0xcd772a85, /// 0x690
			 0x3f22f791, /// 0x694
			 0x2112e059, /// 0x698
			 0x926b9c71, /// 0x69c
			 0x030c26cb, /// 0x6a0
			 0x5cea792b, /// 0x6a4
			 0xff9cabdb, /// 0x6a8
			 0x27133d12, /// 0x6ac
			 0x56a2fe1e, /// 0x6b0
			 0xb778cfaf, /// 0x6b4
			 0x8a4ac64d, /// 0x6b8
			 0x73902f1b, /// 0x6bc
			 0x9c92d16c, /// 0x6c0
			 0x3af5a055, /// 0x6c4
			 0xae022fb5, /// 0x6c8
			 0x955a6cd3, /// 0x6cc
			 0xb2a58932, /// 0x6d0
			 0x93cdf469, /// 0x6d4
			 0xa8f470f4, /// 0x6d8
			 0x40adc83e, /// 0x6dc
			 0x58be5acb, /// 0x6e0
			 0xb9cf5ea5, /// 0x6e4
			 0xb86da37e, /// 0x6e8
			 0xd0568153, /// 0x6ec
			 0x481e9e04, /// 0x6f0
			 0x37d65e01, /// 0x6f4
			 0x733a37ff, /// 0x6f8
			 0x30d05587, /// 0x6fc
			 0x03ecbbe0, /// 0x700
			 0x167e4d6f, /// 0x704
			 0x0731b2fa, /// 0x708
			 0x7f955ebe, /// 0x70c
			 0x89b7565c, /// 0x710
			 0xa7262f04, /// 0x714
			 0x79f439ea, /// 0x718
			 0x73e74fad, /// 0x71c
			 0xbefb9d32, /// 0x720
			 0xbf5dbc49, /// 0x724
			 0x4b4b3cce, /// 0x728
			 0xd4094f99, /// 0x72c
			 0x48bff898, /// 0x730
			 0x0e00654b, /// 0x734
			 0xfccbbce7, /// 0x738
			 0x1f692d68, /// 0x73c
			 0xf02cdbc4, /// 0x740
			 0x7aabbb09, /// 0x744
			 0x7f741884, /// 0x748
			 0x935f9db9, /// 0x74c
			 0x362df504, /// 0x750
			 0x03969940, /// 0x754
			 0xc77cd40d, /// 0x758
			 0x3326f97f, /// 0x75c
			 0xe8b8ffa6, /// 0x760
			 0xc9542d2f, /// 0x764
			 0x1ec8dd26, /// 0x768
			 0x2a0cee61, /// 0x76c
			 0x207a53a2, /// 0x770
			 0x32336125, /// 0x774
			 0x6bd1e4ef, /// 0x778
			 0x1cf267ec, /// 0x77c
			 0xaae5c055, /// 0x780
			 0xa359168a, /// 0x784
			 0xa43f46ea, /// 0x788
			 0x34505e3f, /// 0x78c
			 0xea653fb5, /// 0x790
			 0x4c39f3e7, /// 0x794
			 0x407d5a27, /// 0x798
			 0xece7b57e, /// 0x79c
			 0xc81df032, /// 0x7a0
			 0x7e77335e, /// 0x7a4
			 0x28b11674, /// 0x7a8
			 0xaff4de8a, /// 0x7ac
			 0x62861099, /// 0x7b0
			 0xc136e664, /// 0x7b4
			 0xb2788ce6, /// 0x7b8
			 0xaedf4117, /// 0x7bc
			 0x46283e5f, /// 0x7c0
			 0x847d18ac, /// 0x7c4
			 0x3eed54bb, /// 0x7c8
			 0x679f8cd3, /// 0x7cc
			 0xe6503879, /// 0x7d0
			 0x918ea5dd, /// 0x7d4
			 0x71bee1dc, /// 0x7d8
			 0x5e4eab69, /// 0x7dc
			 0xd8f0fc05, /// 0x7e0
			 0xdd2bf76f, /// 0x7e4
			 0xafaba19f, /// 0x7e8
			 0xe238f873, /// 0x7ec
			 0x2e04f049, /// 0x7f0
			 0xccc48370, /// 0x7f4
			 0xb16b7658, /// 0x7f8
			 0x5a516f7d, /// 0x7fc
			 0x05fe9868, /// 0x800
			 0xab48e632, /// 0x804
			 0x91a646c8, /// 0x808
			 0xe062aaf1, /// 0x80c
			 0x07197f31, /// 0x810
			 0x482400a0, /// 0x814
			 0x400d490d, /// 0x818
			 0x61f9b89b, /// 0x81c
			 0x50d10ecb, /// 0x820
			 0x93ed9da5, /// 0x824
			 0xce575923, /// 0x828
			 0x819aeffc, /// 0x82c
			 0x53f94645, /// 0x830
			 0x7b623074, /// 0x834
			 0x2bf83867, /// 0x838
			 0x02178e80, /// 0x83c
			 0xe17cdf83, /// 0x840
			 0x4b89a61e, /// 0x844
			 0xca2daaf9, /// 0x848
			 0x4a8ac5a8, /// 0x84c
			 0x62039484, /// 0x850
			 0x836e8a0f, /// 0x854
			 0x4999103e, /// 0x858
			 0xc65653cf, /// 0x85c
			 0x74d8a068, /// 0x860
			 0x1bd59279, /// 0x864
			 0xe5acc0f6, /// 0x868
			 0x20e1b30d, /// 0x86c
			 0x7c8ec68a, /// 0x870
			 0xe3a87302, /// 0x874
			 0x24655987, /// 0x878
			 0x2f4cbf80, /// 0x87c
			 0x529c36ae, /// 0x880
			 0xb656194e, /// 0x884
			 0xa11b5263, /// 0x888
			 0x1b3846ba, /// 0x88c
			 0x0d23b964, /// 0x890
			 0x05f5d5d2, /// 0x894
			 0x8d4550b7, /// 0x898
			 0xb7cd18a8, /// 0x89c
			 0x465f48d7, /// 0x8a0
			 0xfd995776, /// 0x8a4
			 0x97707a80, /// 0x8a8
			 0x31773671, /// 0x8ac
			 0x6a0d6532, /// 0x8b0
			 0xdf130c4c, /// 0x8b4
			 0x90d2bc49, /// 0x8b8
			 0xfdf456f4, /// 0x8bc
			 0xd472cabc, /// 0x8c0
			 0x5683ebec, /// 0x8c4
			 0x8923d838, /// 0x8c8
			 0x98182acb, /// 0x8cc
			 0xefe17438, /// 0x8d0
			 0x171d457a, /// 0x8d4
			 0x664fbb49, /// 0x8d8
			 0x4385cc4b, /// 0x8dc
			 0xd5c4afbe, /// 0x8e0
			 0xb003153c, /// 0x8e4
			 0x360a86a4, /// 0x8e8
			 0x0b920c03, /// 0x8ec
			 0xe260b802, /// 0x8f0
			 0x0cb0c75c, /// 0x8f4
			 0xf3dbbb65, /// 0x8f8
			 0x84560465, /// 0x8fc
			 0xf550116b, /// 0x900
			 0x59c6ff73, /// 0x904
			 0x740876c1, /// 0x908
			 0x54b84d05, /// 0x90c
			 0x3a415585, /// 0x910
			 0xc358ee2a, /// 0x914
			 0xaffcb81f, /// 0x918
			 0x0e2d6a5a, /// 0x91c
			 0x5417bc9c, /// 0x920
			 0xb5eab344, /// 0x924
			 0x16637957, /// 0x928
			 0x94325706, /// 0x92c
			 0xed4bd829, /// 0x930
			 0x98eab739, /// 0x934
			 0x662ec9de, /// 0x938
			 0x44999824, /// 0x93c
			 0xe8349152, /// 0x940
			 0x93ca8fb9, /// 0x944
			 0x7534dbb2, /// 0x948
			 0x6bf69e04, /// 0x94c
			 0xc52fa275, /// 0x950
			 0x7b80875a, /// 0x954
			 0x246b6797, /// 0x958
			 0x380b3b0a, /// 0x95c
			 0xd5bb399f, /// 0x960
			 0xaed2eefb, /// 0x964
			 0x1a891f9a, /// 0x968
			 0xd1d8d641, /// 0x96c
			 0xe6cbcf33, /// 0x970
			 0x51a43e6b, /// 0x974
			 0xf5b9864b, /// 0x978
			 0x5938b4af, /// 0x97c
			 0x3e8c0c38, /// 0x980
			 0x9f4ae4bf, /// 0x984
			 0x17a68fdb, /// 0x988
			 0x61fe920b, /// 0x98c
			 0x7e6e4d60, /// 0x990
			 0x27fd123d, /// 0x994
			 0xb318d8db, /// 0x998
			 0xba197c70, /// 0x99c
			 0xd10b1c5b, /// 0x9a0
			 0x43a3f3d0, /// 0x9a4
			 0x219e5f57, /// 0x9a8
			 0xb995c2f5, /// 0x9ac
			 0x7c8f211a, /// 0x9b0
			 0x60e8acd7, /// 0x9b4
			 0xc4405f33, /// 0x9b8
			 0x82257eec, /// 0x9bc
			 0x32972815, /// 0x9c0
			 0x227dda7b, /// 0x9c4
			 0x0dd3bf1e, /// 0x9c8
			 0x62af6400, /// 0x9cc
			 0x31ad02b2, /// 0x9d0
			 0x7be945e4, /// 0x9d4
			 0x1c0102d9, /// 0x9d8
			 0x67829a11, /// 0x9dc
			 0x7df3632f, /// 0x9e0
			 0x8b9b4741, /// 0x9e4
			 0x176fa3ca, /// 0x9e8
			 0x28183141, /// 0x9ec
			 0xfe6519b9, /// 0x9f0
			 0x3714b4e2, /// 0x9f4
			 0xeb9c0aba, /// 0x9f8
			 0xdf7176ec, /// 0x9fc
			 0x2b3eea01, /// 0xa00
			 0xd21b048d, /// 0xa04
			 0xd2d6c7ee, /// 0xa08
			 0xba4aee99, /// 0xa0c
			 0x047ac8a6, /// 0xa10
			 0x92827d28, /// 0xa14
			 0x6642752e, /// 0xa18
			 0xc65f90ae, /// 0xa1c
			 0x05952cd5, /// 0xa20
			 0x7f965149, /// 0xa24
			 0x2fafeda3, /// 0xa28
			 0x2076c7eb, /// 0xa2c
			 0xdca40de2, /// 0xa30
			 0xd9c85970, /// 0xa34
			 0x36fad3be, /// 0xa38
			 0x2e281479, /// 0xa3c
			 0x8d8c904d, /// 0xa40
			 0xbb7ea039, /// 0xa44
			 0xc915167d, /// 0xa48
			 0xe0700d3f, /// 0xa4c
			 0x84250dd0, /// 0xa50
			 0x78f395d2, /// 0xa54
			 0x332399d4, /// 0xa58
			 0x951b3c4d, /// 0xa5c
			 0x4afa4cf8, /// 0xa60
			 0x61fd01bb, /// 0xa64
			 0xf27f79dd, /// 0xa68
			 0xfefc5bc9, /// 0xa6c
			 0x079114a8, /// 0xa70
			 0x3bb5f648, /// 0xa74
			 0x5716517a, /// 0xa78
			 0xf335da0f, /// 0xa7c
			 0x6a859a29, /// 0xa80
			 0x51c77625, /// 0xa84
			 0xdab688c7, /// 0xa88
			 0x3d21311c, /// 0xa8c
			 0xc7220d90, /// 0xa90
			 0xab69ab1c, /// 0xa94
			 0xd3325218, /// 0xa98
			 0xcdbc4bd2, /// 0xa9c
			 0xa7ebd708, /// 0xaa0
			 0x371caec3, /// 0xaa4
			 0x37442cf2, /// 0xaa8
			 0x03a1717a, /// 0xaac
			 0x174c196e, /// 0xab0
			 0xf826af3b, /// 0xab4
			 0x4b777407, /// 0xab8
			 0xc06854b8, /// 0xabc
			 0x08d4342a, /// 0xac0
			 0xbdea3674, /// 0xac4
			 0x48171fec, /// 0xac8
			 0x943042ab, /// 0xacc
			 0x10bc7b7a, /// 0xad0
			 0xab76b698, /// 0xad4
			 0xa049bc1b, /// 0xad8
			 0xc838565b, /// 0xadc
			 0xb3272cea, /// 0xae0
			 0x673a9244, /// 0xae4
			 0x6634addd, /// 0xae8
			 0xad05749c, /// 0xaec
			 0x2c9f70f7, /// 0xaf0
			 0xaed9fa9e, /// 0xaf4
			 0xcd80aec4, /// 0xaf8
			 0x49acc951, /// 0xafc
			 0xcfc49443, /// 0xb00
			 0x72162b27, /// 0xb04
			 0xbb927c65, /// 0xb08
			 0x2bcb2cae, /// 0xb0c
			 0x29a1962b, /// 0xb10
			 0xef689e5e, /// 0xb14
			 0x2b67341d, /// 0xb18
			 0x55bb1556, /// 0xb1c
			 0x4e9f1f8c, /// 0xb20
			 0x689c8e26, /// 0xb24
			 0x2b2d4e06, /// 0xb28
			 0x58c8c8ec, /// 0xb2c
			 0xa396a64b, /// 0xb30
			 0x4c6727e3, /// 0xb34
			 0x1fd0e213, /// 0xb38
			 0x5e6e138b, /// 0xb3c
			 0x0a9ca2ef, /// 0xb40
			 0xfe7cd0f8, /// 0xb44
			 0xe1d6d455, /// 0xb48
			 0x5eebb7ac, /// 0xb4c
			 0xf2499075, /// 0xb50
			 0x974165be, /// 0xb54
			 0x4fc13371, /// 0xb58
			 0xb9153a5f, /// 0xb5c
			 0x0e092053, /// 0xb60
			 0x1aab8f6a, /// 0xb64
			 0xdc8a7b62, /// 0xb68
			 0x057bb502, /// 0xb6c
			 0x153b2dbd, /// 0xb70
			 0xe5292bd6, /// 0xb74
			 0x0043ce30, /// 0xb78
			 0x3be78ac6, /// 0xb7c
			 0x03e56b60, /// 0xb80
			 0xbf699e4c, /// 0xb84
			 0x670159c8, /// 0xb88
			 0x738a4d33, /// 0xb8c
			 0x8034ba3b, /// 0xb90
			 0x6b728112, /// 0xb94
			 0x59b97b8b, /// 0xb98
			 0xa16a2677, /// 0xb9c
			 0x4e29d117, /// 0xba0
			 0xe6340acd, /// 0xba4
			 0x1af267ee, /// 0xba8
			 0xbc372c90, /// 0xbac
			 0x92b2d71a, /// 0xbb0
			 0xef9c6dbb, /// 0xbb4
			 0xcfbb7dd2, /// 0xbb8
			 0xa4ba0ddd, /// 0xbbc
			 0x536daba6, /// 0xbc0
			 0x68959879, /// 0xbc4
			 0x35613827, /// 0xbc8
			 0x75663b06, /// 0xbcc
			 0x3dda7123, /// 0xbd0
			 0xf3e76d2c, /// 0xbd4
			 0x144e2700, /// 0xbd8
			 0xab49ffb3, /// 0xbdc
			 0x84038162, /// 0xbe0
			 0x63fe92a8, /// 0xbe4
			 0xd0341dc2, /// 0xbe8
			 0xc5e29427, /// 0xbec
			 0xb66a926e, /// 0xbf0
			 0x387f95fa, /// 0xbf4
			 0xf9f98884, /// 0xbf8
			 0xba82c41f, /// 0xbfc
			 0x5204ed4c, /// 0xc00
			 0xf834f652, /// 0xc04
			 0x42290b8a, /// 0xc08
			 0x6fd761dd, /// 0xc0c
			 0x4be723fd, /// 0xc10
			 0xb8baf3b7, /// 0xc14
			 0x55e7c42a, /// 0xc18
			 0x4c1baabb, /// 0xc1c
			 0x167c9960, /// 0xc20
			 0x69b621bf, /// 0xc24
			 0x23f2b97e, /// 0xc28
			 0xc595a4c4, /// 0xc2c
			 0x94bb789b, /// 0xc30
			 0x9f7d9017, /// 0xc34
			 0x96c1e8d4, /// 0xc38
			 0xcb8b43c9, /// 0xc3c
			 0xef30f25c, /// 0xc40
			 0x470feb6a, /// 0xc44
			 0xa344eea1, /// 0xc48
			 0xb25bc95b, /// 0xc4c
			 0xf43598c8, /// 0xc50
			 0xbfdcd6d5, /// 0xc54
			 0x6e415c92, /// 0xc58
			 0x63c05919, /// 0xc5c
			 0x273c9f2e, /// 0xc60
			 0xffbc45ce, /// 0xc64
			 0x8a1bd2f0, /// 0xc68
			 0x9e81f531, /// 0xc6c
			 0xdffb4419, /// 0xc70
			 0x318c63d9, /// 0xc74
			 0xe3b174ac, /// 0xc78
			 0x4dd158f0, /// 0xc7c
			 0x557bd8de, /// 0xc80
			 0x6d994722, /// 0xc84
			 0x8a659c10, /// 0xc88
			 0xf1693c46, /// 0xc8c
			 0x2a6a1037, /// 0xc90
			 0x25bae856, /// 0xc94
			 0xe6a352af, /// 0xc98
			 0x73a297e3, /// 0xc9c
			 0xb927aa73, /// 0xca0
			 0xab17e168, /// 0xca4
			 0x9a052406, /// 0xca8
			 0x692e628e, /// 0xcac
			 0x288bfa71, /// 0xcb0
			 0x41601ac1, /// 0xcb4
			 0x49c31706, /// 0xcb8
			 0x46867d65, /// 0xcbc
			 0x27695e41, /// 0xcc0
			 0x5a5d717c, /// 0xcc4
			 0xd5f6e0f1, /// 0xcc8
			 0x8756eb20, /// 0xccc
			 0xbd98f44d, /// 0xcd0
			 0xf631ad21, /// 0xcd4
			 0x91ae9015, /// 0xcd8
			 0xf02e4c0c, /// 0xcdc
			 0x696531e5, /// 0xce0
			 0x2c6f83fc, /// 0xce4
			 0x486461f8, /// 0xce8
			 0xa202210e, /// 0xcec
			 0xd1cfe3a7, /// 0xcf0
			 0xb2284d6b, /// 0xcf4
			 0x56085efc, /// 0xcf8
			 0x8420b76b, /// 0xcfc
			 0x3258acf2, /// 0xd00
			 0x9d28ec15, /// 0xd04
			 0x18750d81, /// 0xd08
			 0xdbdbd343, /// 0xd0c
			 0x01d8190c, /// 0xd10
			 0x468d43fd, /// 0xd14
			 0x17e30a12, /// 0xd18
			 0x021ce901, /// 0xd1c
			 0xdfc00eb6, /// 0xd20
			 0xab9882eb, /// 0xd24
			 0x34c55e95, /// 0xd28
			 0xbd777561, /// 0xd2c
			 0x4baeb3a9, /// 0xd30
			 0x30c6b80b, /// 0xd34
			 0x90be8ba0, /// 0xd38
			 0xb9d0c193, /// 0xd3c
			 0x0c023dd7, /// 0xd40
			 0x6edeb54c, /// 0xd44
			 0x40a11395, /// 0xd48
			 0xb8e0acbc, /// 0xd4c
			 0x42f72c92, /// 0xd50
			 0xdbe53933, /// 0xd54
			 0x0928ea17, /// 0xd58
			 0x8e42aaa0, /// 0xd5c
			 0x00a8868b, /// 0xd60
			 0xfe192e63, /// 0xd64
			 0xea06270c, /// 0xd68
			 0x420ce682, /// 0xd6c
			 0xc1757db7, /// 0xd70
			 0x18f129fc, /// 0xd74
			 0x78a61e53, /// 0xd78
			 0x8388ec6d, /// 0xd7c
			 0xcfb75dba, /// 0xd80
			 0x1a49a48e, /// 0xd84
			 0xa96b7c73, /// 0xd88
			 0x0610e6cf, /// 0xd8c
			 0xf10f363b, /// 0xd90
			 0xdd2f8bce, /// 0xd94
			 0x6bb5a951, /// 0xd98
			 0xcede9565, /// 0xd9c
			 0x52a3aa81, /// 0xda0
			 0xcaa9ddc1, /// 0xda4
			 0x0f53eb46, /// 0xda8
			 0x7b766722, /// 0xdac
			 0xf5f4ebba, /// 0xdb0
			 0xe08ea7c2, /// 0xdb4
			 0xdc2614dd, /// 0xdb8
			 0x069705c9, /// 0xdbc
			 0xbd9633df, /// 0xdc0
			 0x7cda7c47, /// 0xdc4
			 0xdc628b66, /// 0xdc8
			 0x49e5f113, /// 0xdcc
			 0x5728e967, /// 0xdd0
			 0xb6b8feb7, /// 0xdd4
			 0xe6408817, /// 0xdd8
			 0xe4b6dd68, /// 0xddc
			 0xb6d39251, /// 0xde0
			 0xb4b6d986, /// 0xde4
			 0x025f3835, /// 0xde8
			 0x1852d4b1, /// 0xdec
			 0x6768b8c4, /// 0xdf0
			 0xc043a2bc, /// 0xdf4
			 0x644ed24a, /// 0xdf8
			 0x25bf93b3, /// 0xdfc
			 0x11b29b63, /// 0xe00
			 0x7ee8a64f, /// 0xe04
			 0xe5c398fe, /// 0xe08
			 0xc3dac4f9, /// 0xe0c
			 0xc8920386, /// 0xe10
			 0xe0c4bff3, /// 0xe14
			 0x43a2d24c, /// 0xe18
			 0x8f940620, /// 0xe1c
			 0xad6db736, /// 0xe20
			 0x1fbef88a, /// 0xe24
			 0xcd387e67, /// 0xe28
			 0x80953185, /// 0xe2c
			 0xd9fbb5e6, /// 0xe30
			 0xb5d8909a, /// 0xe34
			 0xc568b925, /// 0xe38
			 0x35bbd4c0, /// 0xe3c
			 0x0ec5927e, /// 0xe40
			 0x0faa579b, /// 0xe44
			 0xf0100062, /// 0xe48
			 0x1b7a9242, /// 0xe4c
			 0xde066007, /// 0xe50
			 0x9c418988, /// 0xe54
			 0xf700c899, /// 0xe58
			 0x35a04010, /// 0xe5c
			 0x10eace2b, /// 0xe60
			 0x37aff439, /// 0xe64
			 0x0eb51b9b, /// 0xe68
			 0xc66e3759, /// 0xe6c
			 0x5a95260e, /// 0xe70
			 0xf9ba6a26, /// 0xe74
			 0x55d2c115, /// 0xe78
			 0xc568cdce, /// 0xe7c
			 0x0151caf6, /// 0xe80
			 0xf912571a, /// 0xe84
			 0xb42e6422, /// 0xe88
			 0x1de46db1, /// 0xe8c
			 0xd0ece460, /// 0xe90
			 0x19661fd2, /// 0xe94
			 0xb05162bf, /// 0xe98
			 0x707f7457, /// 0xe9c
			 0x2df7493a, /// 0xea0
			 0x505dcb22, /// 0xea4
			 0x9d4b49f4, /// 0xea8
			 0x2b22e4dd, /// 0xeac
			 0x9464c336, /// 0xeb0
			 0x7608b112, /// 0xeb4
			 0x0b591724, /// 0xeb8
			 0xafa92404, /// 0xebc
			 0xa6eecdc2, /// 0xec0
			 0x97eb40ec, /// 0xec4
			 0xb7c611f6, /// 0xec8
			 0xebb435bf, /// 0xecc
			 0x9802870f, /// 0xed0
			 0x662810be, /// 0xed4
			 0x3727309d, /// 0xed8
			 0x8d4ba506, /// 0xedc
			 0xceb6187b, /// 0xee0
			 0xe0857ba1, /// 0xee4
			 0xef45ed4e, /// 0xee8
			 0x0a23e311, /// 0xeec
			 0x3141f31e, /// 0xef0
			 0x0fd5f78d, /// 0xef4
			 0x9e86f055, /// 0xef8
			 0x4a49e924, /// 0xefc
			 0x27ade475, /// 0xf00
			 0xec43b592, /// 0xf04
			 0xea123fb5, /// 0xf08
			 0x7d47ea91, /// 0xf0c
			 0xb305bf18, /// 0xf10
			 0x6468b713, /// 0xf14
			 0x149997b1, /// 0xf18
			 0xf0e5221c, /// 0xf1c
			 0x37fabfe3, /// 0xf20
			 0xebdc75de, /// 0xf24
			 0x4b936e1a, /// 0xf28
			 0x02fc5f65, /// 0xf2c
			 0x0d2edd85, /// 0xf30
			 0xeeb3ee48, /// 0xf34
			 0x4e30727a, /// 0xf38
			 0x6daab69c, /// 0xf3c
			 0x5ca93091, /// 0xf40
			 0x69e4410b, /// 0xf44
			 0xacf8da68, /// 0xf48
			 0xf68383ff, /// 0xf4c
			 0x72446f6d, /// 0xf50
			 0x52984638, /// 0xf54
			 0xcea2c3bb, /// 0xf58
			 0x06f4bd5f, /// 0xf5c
			 0x2463071e, /// 0xf60
			 0x921c8289, /// 0xf64
			 0xb883f71c, /// 0xf68
			 0xbaf43eab, /// 0xf6c
			 0x3038b534, /// 0xf70
			 0x10cfd8c8, /// 0xf74
			 0xc9e5c388, /// 0xf78
			 0x160f59a0, /// 0xf7c
			 0x176b94ee, /// 0xf80
			 0xf078f116, /// 0xf84
			 0x484483a6, /// 0xf88
			 0x22c5dc10, /// 0xf8c
			 0xb28f419a, /// 0xf90
			 0xa6a81913, /// 0xf94
			 0x3ee86f8d, /// 0xf98
			 0x5eb9c8f9, /// 0xf9c
			 0x593cdfb9, /// 0xfa0
			 0xade4bf62, /// 0xfa4
			 0x0c9515fc, /// 0xfa8
			 0x968e1a2f, /// 0xfac
			 0x47ab09be, /// 0xfb0
			 0xa20dd633, /// 0xfb4
			 0xd34883dc, /// 0xfb8
			 0x724fbda1, /// 0xfbc
			 0x85035b1a, /// 0xfc0
			 0x444dfb40, /// 0xfc4
			 0xff0b73a8, /// 0xfc8
			 0xe4153dce, /// 0xfcc
			 0x290dec44, /// 0xfd0
			 0x08d2abba, /// 0xfd4
			 0x9ad0425a, /// 0xfd8
			 0x848015f3, /// 0xfdc
			 0x7f64eb56, /// 0xfe0
			 0x75ef2e42, /// 0xfe4
			 0x08d3f33a, /// 0xfe8
			 0x5e0965de, /// 0xfec
			 0x1fe8181b, /// 0xff0
			 0x92faf762, /// 0xff4
			 0xa7b02475, /// 0xff8
			 0xda02a337 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00004
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00008
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0000c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00014
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00018
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00020
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00024
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00028
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00030
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00034
			 0xffc00001,                                                  // -signaling NaN                              /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0003c
			 0x7f800000,                                                  // inf                                         /// 00040
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00044
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00048
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0004c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00050
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00054
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00058
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0005c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x0e000003,                                                  // Trailing 1s:                                /// 00068
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00070
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00074
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00078
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0007c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00084
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00088
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0008c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00090
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00098
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0009c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a8
			 0x7fc00001,                                                  // signaling NaN                               /// 000ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00100
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00104
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0011c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00120
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00124
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0012c
			 0x55555555,                                                  // 4 random values                             /// 00130
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x0c700000,                                                  // Leading 1s:                                 /// 00138
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0013c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00140
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00144
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00148
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0014c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00154
			 0x0e000001,                                                  // Trailing 1s:                                /// 00158
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0015c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0c700000,                                                  // Leading 1s:                                 /// 00168
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00170
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00178
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0017c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00188
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0018c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00190
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00198
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d8
			 0x3f028f5c,                                                  // 0.51                                        /// 001dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001e8
			 0xcb000000,                                                  // -8388608.0                                  /// 001ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00200
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00204
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0020c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00214
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00218
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0021c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00228
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00238
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0023c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00254
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00258
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0025c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00260
			 0x55555555,                                                  // 4 random values                             /// 00264
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00268
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0026c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00270
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00274
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00278
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00280
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00284
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00288
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00294
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00298
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002b8
			 0xff800000,                                                  // -inf                                        /// 002bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002d8
			 0x55555555,                                                  // 4 random values                             /// 002dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 002e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00300
			 0xbf028f5c,                                                  // -0.51                                       /// 00304
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x7fc00001,                                                  // signaling NaN                               /// 0030c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00310
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xbf028f5c,                                                  // -0.51                                       /// 00320
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00324
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00328
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0032c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00330
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00334
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00340
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00344
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00350
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0e000001,                                                  // Trailing 1s:                                /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0035c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00368
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00374
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00378
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00380
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00384
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0038c
			 0x3f028f5c,                                                  // 0.51                                        /// 00390
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00394
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0039c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0xffc00001,                                                  // -signaling NaN                              /// 003bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00400
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x80000000,                                                  // -zero                                       /// 00420
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00424
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00428
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00430
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00434
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00438
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0c400000,                                                  // Leading 1s:                                 /// 00444
			 0xffc00001,                                                  // -signaling NaN                              /// 00448
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0044c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00460
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00468
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0046c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00478
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0047c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00484
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00488
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0048c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00490
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00494
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00498
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0049c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00500
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00504
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00508
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0050c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00514
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0c780000,                                                  // Leading 1s:                                 /// 0051c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00520
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00524
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0052c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00530
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c600000,                                                  // Leading 1s:                                 /// 00538
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0053c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00540
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00544
			 0x80011111,                                                  // -9.7958E-41                                 /// 00548
			 0xffc00001,                                                  // -signaling NaN                              /// 0054c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00554
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00558
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0055c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00564
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00568
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0056c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00570
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0057c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00588
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0058c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00594
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00598
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005bc
			 0x4b000000,                                                  // 8388608.0                                   /// 005c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005c4
			 0x33333333,                                                  // 4 random values                             /// 005c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0xffc00001,                                                  // -signaling NaN                              /// 005dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e0
			 0xffc00001,                                                  // -signaling NaN                              /// 005e4
			 0x33333333,                                                  // 4 random values                             /// 005e8
			 0x7f800000,                                                  // inf                                         /// 005ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00608
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0060c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00610
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0061c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00620
			 0x33333333,                                                  // 4 random values                             /// 00624
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00628
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0062c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00630
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00634
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00638
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0063c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00640
			 0x0e000003,                                                  // Trailing 1s:                                /// 00644
			 0x0c700000,                                                  // Leading 1s:                                 /// 00648
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0064c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00654
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00658
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0065c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00664
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00668
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0066c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00670
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0067c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00680
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00684
			 0x00011111,                                                  // 9.7958E-41                                  /// 00688
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0068c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00690
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00694
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00698
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0069c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00704
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00708
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0070c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00718
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0071c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00720
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00724
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00728
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0072c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00730
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0073c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00740
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00748
			 0x80011111,                                                  // -9.7958E-41                                 /// 0074c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00758
			 0xff800000,                                                  // -inf                                        /// 0075c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00760
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00764
			 0x0c780000,                                                  // Leading 1s:                                 /// 00768
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00770
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00774
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00778
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00780
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00790
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xcb000000,                                                  // -8388608.0                                  /// 00798
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0079c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007c8
			 0x4b000000,                                                  // 8388608.0                                   /// 007cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e4
			 0x7fc00001,                                                  // signaling NaN                               /// 007e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00800
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00804
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00808
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00810
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00814
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00818
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0081c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00820
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00824
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00828
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0082c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0083c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0c600000,                                                  // Leading 1s:                                 /// 00844
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0084c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00850
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00854
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00858
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0085c
			 0x4b000000,                                                  // 8388608.0                                   /// 00860
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00864
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00868
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00874
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00878
			 0x0c600000,                                                  // Leading 1s:                                 /// 0087c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00880
			 0xff800000,                                                  // -inf                                        /// 00884
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0088c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0089c
			 0xffc00001,                                                  // -signaling NaN                              /// 008a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008b8
			 0xbf028f5c,                                                  // -0.51                                       /// 008bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0xff800000,                                                  // -inf                                        /// 008f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00900
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00904
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00910
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00914
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00924
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00928
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00934
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00950
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00954
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0095c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00960
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00964
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0096c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00978
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00980
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00990
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00994
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00998
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0099c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d0
			 0xbf028f5c,                                                  // -0.51                                       /// 009d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a08
			 0x80000000,                                                  // -zero                                       /// 00a0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a28
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a78
			 0xbf028f5c,                                                  // -0.51                                       /// 00a7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x7f800000,                                                  // inf                                         /// 00a88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0xffc00001,                                                  // -signaling NaN                              /// 00a94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ab0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ac8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00acc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00adc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ae0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ae8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00aec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00afc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00000000,                                                  // zero                                        /// 00b10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ba8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bb8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bbc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bd8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bf0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bf8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0xbf028f5c,                                                  // -0.51                                       /// 00c08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c18
			 0x4b000000,                                                  // 8388608.0                                   /// 00c1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80000000,                                                  // -zero                                       /// 00c48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c74
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ca0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cbc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ccc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ce0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ce4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cf0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cf4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d10
			 0xcb000000,                                                  // -8388608.0                                  /// 00d14
			 0x4b000000,                                                  // 8388608.0                                   /// 00d18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x80000000,                                                  // -zero                                       /// 00d30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00000000,                                                  // zero                                        /// 00d60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d74
			 0xffc00001,                                                  // -signaling NaN                              /// 00d78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d84
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00da0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00da8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00db8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dbc
			 0xffc00001,                                                  // -signaling NaN                              /// 00dc0
			 0x00000000,                                                  // zero                                        /// 00dc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dc8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00de8
			 0xffc00001,                                                  // -signaling NaN                              /// 00dec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00df8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e5c
			 0x55555555,                                                  // 4 random values                             /// 00e60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x33333333,                                                  // 4 random values                             /// 00ea8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ec0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ecc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00edc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ee4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ef4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ef8
			 0x33333333,                                                  // 4 random values                             /// 00efc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f64
			 0x33333333,                                                  // 4 random values                             /// 00f68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0xff800000,                                                  // -inf                                        /// 00f80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80000000,                                                  // -zero                                       /// 00fb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fbc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fc4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fc8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fd0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fe8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ff0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0xff800000                                                  // -inf                                        /// last
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
			 0x000002b4,
			 0x0000014c,
			 0x000004e8,
			 0x000004d4,
			 0x00000408,
			 0x00000148,
			 0x0000032c,
			 0x00000574,

			 /// vpu register f2
			 0x40e00000,
			 0x41c00000,
			 0x41f00000,
			 0x41900000,
			 0x41d00000,
			 0x40e00000,
			 0x41d80000,
			 0x41e80000,

			 /// vpu register f3
			 0x41f00000,
			 0x40000000,
			 0x41c00000,
			 0x41880000,
			 0x41700000,
			 0x41400000,
			 0x40800000,
			 0x41800000,

			 /// vpu register f4
			 0x40a00000,
			 0x41880000,
			 0x41100000,
			 0x40e00000,
			 0x40400000,
			 0x41e00000,
			 0x41500000,
			 0x41400000,

			 /// vpu register f5
			 0x41700000,
			 0x41d80000,
			 0x40c00000,
			 0x41200000,
			 0x41800000,
			 0x41200000,
			 0x42000000,
			 0x41600000,

			 /// vpu register f6
			 0x41f80000,
			 0x41300000,
			 0x40800000,
			 0x42000000,
			 0x40400000,
			 0x40000000,
			 0x41f00000,
			 0x41100000,

			 /// vpu register f7
			 0x41a80000,
			 0x41100000,
			 0x40000000,
			 0x41a80000,
			 0x41700000,
			 0x41200000,
			 0x40800000,
			 0x41a00000,

			 /// vpu register f8
			 0x42000000,
			 0x41b00000,
			 0x41c80000,
			 0x41900000,
			 0x41700000,
			 0x41e80000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f9
			 0x41d80000,
			 0x41000000,
			 0x40e00000,
			 0x40e00000,
			 0x41e80000,
			 0x41a80000,
			 0x40800000,
			 0x41400000,

			 /// vpu register f10
			 0x41100000,
			 0x41c00000,
			 0x40000000,
			 0x41c00000,
			 0x40000000,
			 0x41f80000,
			 0x40a00000,
			 0x41880000,

			 /// vpu register f11
			 0x41c80000,
			 0x41a00000,
			 0x41980000,
			 0x41e00000,
			 0x41e00000,
			 0x41b80000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f12
			 0x41980000,
			 0x41d00000,
			 0x41d80000,
			 0x40800000,
			 0x41400000,
			 0x40800000,
			 0x41f80000,
			 0x40e00000,

			 /// vpu register f13
			 0x41a80000,
			 0x40000000,
			 0x41800000,
			 0x40c00000,
			 0x41880000,
			 0x41000000,
			 0x41f80000,
			 0x41b80000,

			 /// vpu register f14
			 0x41900000,
			 0x41f00000,
			 0x40400000,
			 0x41200000,
			 0x41880000,
			 0x41c00000,
			 0x41900000,
			 0x40e00000,

			 /// vpu register f15
			 0x41980000,
			 0x41f00000,
			 0x41000000,
			 0x40400000,
			 0x41700000,
			 0x41c00000,
			 0x41100000,
			 0x41e80000,

			 /// vpu register f16
			 0x41c00000,
			 0x40400000,
			 0x41900000,
			 0x40400000,
			 0x41a80000,
			 0x41500000,
			 0x41c00000,
			 0x41880000,

			 /// vpu register f17
			 0x41c80000,
			 0x41a80000,
			 0x40a00000,
			 0x40400000,
			 0x41d80000,
			 0x41e00000,
			 0x41400000,
			 0x41a00000,

			 /// vpu register f18
			 0x41600000,
			 0x41600000,
			 0x3f800000,
			 0x40e00000,
			 0x41d00000,
			 0x41880000,
			 0x41600000,
			 0x41c80000,

			 /// vpu register f19
			 0x41800000,
			 0x41900000,
			 0x41800000,
			 0x41100000,
			 0x41f00000,
			 0x40a00000,
			 0x41880000,
			 0x40800000,

			 /// vpu register f20
			 0x41e80000,
			 0x41f80000,
			 0x41f80000,
			 0x40800000,
			 0x41880000,
			 0x41880000,
			 0x41800000,
			 0x41880000,

			 /// vpu register f21
			 0x41980000,
			 0x41900000,
			 0x40000000,
			 0x41c80000,
			 0x41200000,
			 0x41300000,
			 0x40e00000,
			 0x3f800000,

			 /// vpu register f22
			 0x41b80000,
			 0x41a80000,
			 0x41980000,
			 0x41a00000,
			 0x41000000,
			 0x40000000,
			 0x41a80000,
			 0x41500000,

			 /// vpu register f23
			 0x40000000,
			 0x41880000,
			 0x41500000,
			 0x40400000,
			 0x3f800000,
			 0x40c00000,
			 0x41d00000,
			 0x3f800000,

			 /// vpu register f24
			 0x41200000,
			 0x40c00000,
			 0x41c00000,
			 0x3f800000,
			 0x41500000,
			 0x41b00000,
			 0x40800000,
			 0x41a80000,

			 /// vpu register f25
			 0x40a00000,
			 0x3f800000,
			 0x41700000,
			 0x42000000,
			 0x41300000,
			 0x40800000,
			 0x41400000,
			 0x41700000,

			 /// vpu register f26
			 0x41600000,
			 0x41b00000,
			 0x41000000,
			 0x41900000,
			 0x41900000,
			 0x41c00000,
			 0x3f800000,
			 0x40400000,

			 /// vpu register f27
			 0x40400000,
			 0x41b80000,
			 0x41700000,
			 0x41f80000,
			 0x41980000,
			 0x41500000,
			 0x41980000,
			 0x41200000,

			 /// vpu register f28
			 0x41e00000,
			 0x40400000,
			 0x41d00000,
			 0x41700000,
			 0x40000000,
			 0x41900000,
			 0x41500000,
			 0x41a80000,

			 /// vpu register f29
			 0x40400000,
			 0x41500000,
			 0x41700000,
			 0x41b80000,
			 0x41f80000,
			 0x41500000,
			 0x41000000,
			 0x41e80000,

			 /// vpu register f30
			 0x41e00000,
			 0x41e80000,
			 0x41c00000,
			 0x41980000,
			 0x41800000,
			 0x41400000,
			 0x41900000,
			 0x41f80000,

			 /// vpu register f31
			 0x41980000,
			 0x41900000,
			 0x41e80000,
			 0x41700000,
			 0x41c80000,
			 0x3f800000,
			 0x40000000,
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
		"famoaddl.pi f3, f1 (x14)\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f8, f1 (x15)\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f3, f1 (x12)\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f15, f1 (x14)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f20, f1 (x11)\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f19, f1 (x14)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f12, f1 (x13)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f25, f1 (x12)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f29, f1 (x14)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f4, f1 (x15)\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f9, f1 (x14)\n"                          ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f19, f1 (x11)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f11, f1 (x15)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f27, f1 (x15)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f5, f1 (x15)\n"                          ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f10, f1 (x14)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f17, f1 (x13)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f8, f1 (x11)\n"                          ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f16, f1 (x11)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f24, f1 (x11)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f15, f1 (x11)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f25, f1 (x11)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f5, f1 (x14)\n"                          ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f14, f1 (x14)\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f30, f1 (x12)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f22, f1 (x14)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f22, f1 (x13)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f6, f1 (x11)\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f29, f1 (x13)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f10, f1 (x12)\n"                         ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f17, f1 (x12)\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f11, f1 (x14)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f22, f1 (x11)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f28, f1 (x12)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f17, f1 (x13)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f25, f1 (x11)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f21, f1 (x14)\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f3, f1 (x12)\n"                          ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f26, f1 (x14)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f7, f1 (x15)\n"                          ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f23, f1 (x12)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f6, f1 (x11)\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f15, f1 (x13)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f11, f1 (x11)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f23, f1 (x12)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f16, f1 (x14)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f7, f1 (x13)\n"                          ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f5, f1 (x14)\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f22, f1 (x11)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f16, f1 (x12)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f26, f1 (x15)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f20, f1 (x13)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f12, f1 (x12)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f21, f1 (x13)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f23, f1 (x12)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f15, f1 (x14)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f6, f1 (x11)\n"                          ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f29, f1 (x14)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f15, f1 (x13)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f8, f1 (x15)\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f12, f1 (x15)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f5, f1 (x14)\n"                          ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f5, f1 (x12)\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f14, f1 (x15)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f6, f1 (x13)\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f15, f1 (x15)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f22, f1 (x13)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f10, f1 (x11)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f20, f1 (x15)\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f25, f1 (x13)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f3, f1 (x11)\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f14, f1 (x13)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f9, f1 (x12)\n"                          ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f16, f1 (x13)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f2, f1 (x12)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f11, f1 (x11)\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f30, f1 (x14)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f14, f1 (x15)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f20, f1 (x15)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f29, f1 (x11)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f11, f1 (x12)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f16, f1 (x14)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f6, f1 (x12)\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f3, f1 (x15)\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f28, f1 (x11)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f9, f1 (x14)\n"                          ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f5, f1 (x14)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f21, f1 (x13)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f2, f1 (x12)\n"                          ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f17, f1 (x12)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f30, f1 (x12)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f17, f1 (x11)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f23, f1 (x15)\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f18, f1 (x13)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f19, f1 (x15)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f15, f1 (x12)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f18, f1 (x11)\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f14, f1 (x14)\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoaddl.pi f2, f1 (x15)\n"                          ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
