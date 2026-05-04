/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"

volatile uint32_t m_op[256] __attribute__ ((aligned (4096))) = {
	     /// f0 is designated 0 register
	     0x00000000,
	     0x00000000,
	     0x00000000,
	     0x00000000,
	     0x00000000,
	     0x00000000,
	     0x00000000,
	     0x00000000,
	     /// f1 is designated offset for memory ops
	     0x000002cc,
	     0x000002b4,
	     0x000002f4,
	     0x0000008c,
	     0x00000148,
	     0x00000078,
	     0x00000150,
	     0x000001c4,
		 /// f2
		 0x0e001fff, 
		 0xff7fffff, 
		 0x0f7ffffe, 
		 0x3f800001, 
		 0xcb000000, 
		 0x807fffff, 
		 0x0c7e0000, 
		 0x7f7fffff, 
		 /// f3
		 0xCCCCCCCC, 
		 0xff7ffffe, 
		 0x00000001, 
		 0x00000001, 
		 0x7f7ffffe, 
		 0x0c780000, 
		 0x80001000, 
		 0x0c7ffffc, 
		 /// f4
		 0x3f800001, 
		 0x0c7ff000, 
		 0xffbfffff, 
		 0x0c7fffff, 
		 0x0e0001ff, 
		 0x0e0000ff, 
		 0x0e000fff, 
		 0x0e07ffff, 
		 /// f5
		 0x00400000, 
		 0x00000004, 
		 0x80000800, 
		 0xff800000, 
		 0x0e003fff, 
		 0x00000001, 
		 0x00000004, 
		 0xbf800000, 
		 /// f6
		 0x0e007fff, 
		 0x4b8c4b40, 
		 0x7f7fffff, 
		 0x0e000fff, 
		 0xff7ffffe, 
		 0x0c7f8000, 
		 0xff800000, 
		 0x00800001, 
		 /// f7
		 0x7f7ffffe, 
		 0x80000002, 
		 0x80800002, 
		 0x80008000, 
		 0xCCCCCCCC, 
		 0x80000002, 
		 0xff800000, 
		 0x807ffffe, 
		 /// f8
		 0x7f7ffffe, 
		 0x3f800000, 
		 0x0e00003f, 
		 0xbf800000, 
		 0x3f800000, 
		 0x00000100, 
		 0x00000000, 
		 0xffc00000, 
		 /// f9
		 0xcb8c4b40, 
		 0xff800001, 
		 0x7f800000, 
		 0x0c7f0000, 
		 0x0c7fffe0, 
		 0x00200000, 
		 0xffc00000, 
		 0x80800003, 
		 /// f10
		 0x0e001fff, 
		 0x3f800001, 
		 0x00000001, 
		 0xff7ffffe, 
		 0x80800003, 
		 0x0e1fffff, 
		 0x7fc00001, 
		 0x80011111, 
		 /// f11
		 0x00800003, 
		 0x7f7ffffe, 
		 0xffc00001, 
		 0x00000004, 
		 0x80800000, 
		 0x00000001, 
		 0xffbfffff, 
		 0x00400000, 
		 /// f12
		 0x0e0fffff, 
		 0xcb8c4b40, 
		 0x00000004, 
		 0x80008000, 
		 0xffc00000, 
		 0x00000080, 
		 0x80000020, 
		 0xcb000000, 
		 /// f13
		 0x80800001, 
		 0x80000002, 
		 0x7fc00001, 
		 0xbf028f5c, 
		 0xffc00000, 
		 0x7f000000, 
		 0x0e1fffff, 
		 0x0e00007f, 
		 /// f14
		 0x80020000, 
		 0x80800001, 
		 0x0e03ffff, 
		 0x0e001fff, 
		 0x7fc00000, 
		 0x80800002, 
		 0xff7ffffe, 
		 0x80000200, 
		 /// f15
		 0x80000040, 
		 0x80000002, 
		 0x0d000ff0, 
		 0x4b000000, 
		 0x00000004, 
		 0x007fffff, 
		 0x0e01ffff, 
		 0x80200000, 
		 /// f16
		 0x80800001, 
		 0x00ffffff, 
		 0x0c7ffff8, 
		 0x7f800000, 
		 0x00100000, 
		 0x0e03ffff, 
		 0x0c7fffff, 
		 0x0e000003, 
		 /// f17
		 0x00000100, 
		 0x80000001, 
		 0x0e001fff, 
		 0x80000001, 
		 0x3f800000, 
		 0x80800000, 
		 0x0e7fffff, 
		 0xffffffff, 
		 /// f18
		 0x0f7ffffd, 
		 0x0e000fff, 
		 0x3f028f5c, 
		 0x7f800001, 
		 0x0e1fffff, 
		 0x80000020, 
		 0x3f028f5c, 
		 0xff800000, 
		 /// f19
		 0x00800000, 
		 0xcb8c4b40, 
		 0x80000400, 
		 0x00000001, 
		 0xffc00000, 
		 0x80800002, 
		 0x0d000ff0, 
		 0x0c600000, 
		 /// f20
		 0x0c780000, 
		 0x80000001, 
		 0x7fc00000, 
		 0x7fc00000, 
		 0x00000002, 
		 0x80000002, 
		 0x00000000, 
		 0x0e0001ff, 
		 /// f21
		 0x80080000, 
		 0x0c7ffc00, 
		 0x00800002, 
		 0x0e00003f, 
		 0x0f7ffffd, 
		 0xff7fffff, 
		 0x00ffffff, 
		 0x80800000, 
		 /// f22
		 0x00800001, 
		 0x7fc00001, 
		 0xffc00000, 
		 0xbf800001, 
		 0x0c780000, 
		 0x0c7ff000, 
		 0x0c7ffc00, 
		 0x80000000, 
		 /// f23
		 0x80002000, 
		 0x80000010, 
		 0x7f7ffffe, 
		 0x80800000, 
		 0x0e3fffff, 
		 0x0e001fff, 
		 0x7f7ffffe, 
		 0x00800000, 
		 /// f24
		 0x00000004, 
		 0x0c7fff80, 
		 0xbf028f5c, 
		 0x007fffff, 
		 0x4b8c4b40, 
		 0x00000008, 
		 0x0c600000, 
		 0xff7ffffe, 
		 /// f25
		 0x0e07ffff, 
		 0x4b8c4b40, 
		 0x0e03ffff, 
		 0x00000800, 
		 0x80800000, 
		 0x80040000, 
		 0x80800002, 
		 0x7f7fffff, 
		 /// f26
		 0xffc00001, 
		 0x7f7ffffe, 
		 0x0c400000, 
		 0x807fffff, 
		 0x0e000001, 
		 0x00800001, 
		 0x80800002, 
		 0x55555555, 
		 /// f27
		 0x0c7ffffc, 
		 0x00000001, 
		 0x7f7fffff, 
		 0x0c7ff800, 
		 0x00400000, 
		 0x80000080, 
		 0xff7ffffe, 
		 0x7fffffff, 
		 /// f28
		 0x00000002, 
		 0xbf800000, 
		 0x3f800000, 
		 0x80004000, 
		 0x0e03ffff, 
		 0x80000040, 
		 0x8f7ffffc, 
		 0x7f7ffffe, 
		 /// f29
		 0xcb000000, 
		 0x00000400, 
		 0x807ffffe, 
		 0x00000100, 
		 0x0c7c0000, 
		 0x00000200, 
		 0x00000100, 
		 0x00008000, 
		 /// f30
		 0x80080000, 
		 0xff7ffffe, 
		 0x55555555, 
		 0x0c7ff800, 
		 0x0e1fffff, 
		 0x8f7fffff, 
		 0x80002000, 
		 0x0e0003ff, 
		 /// f31
		 0x00000002, 
		 0x00080000, 
		 0x0c700000, 
		 0x0c7ffe00, 
		 0xcb000000, 
		 0x80004000, 
		 0x0e0000ff, 
		 0x3f800000
};

volatile uint32_t m_11[2048] __attribute__ ((aligned (4096))) = {
		 0xe8b18e64, /// 0x0000
		 0x8a83e4de, /// 0x0004
		 0x2999582b, /// 0x0008
		 0x77b1e35a, /// 0x000c
		 0x7952b097, /// 0x0010
		 0x5cda46e8, /// 0x0014
		 0x83e9b101, /// 0x0018
		 0xf838b900, /// 0x001c
		 0xb28abe9e, /// 0x0020
		 0x8af69e7a, /// 0x0024
		 0xad6db386, /// 0x0028
		 0x87cd134e, /// 0x002c
		 0x4293ac85, /// 0x0030
		 0xcdb8ad7c, /// 0x0034
		 0xca0ba3dd, /// 0x0038
		 0xfabc6c60, /// 0x003c
		 0xc95076cb, /// 0x0040
		 0xbb505c7e, /// 0x0044
		 0xd77304ab, /// 0x0048
		 0x0fc5ed17, /// 0x004c
		 0x04e2cefa, /// 0x0050
		 0xada24f66, /// 0x0054
		 0xce029997, /// 0x0058
		 0x48683f2e, /// 0x005c
		 0x011f2448, /// 0x0060
		 0x7b9ec7c9, /// 0x0064
		 0x3064be2a, /// 0x0068
		 0x9255ee93, /// 0x006c
		 0x3e2bb1bf, /// 0x0070
		 0x8c47de6e, /// 0x0074
		 0xce574f78, /// 0x0078
		 0x9abe4db5, /// 0x007c
		 0xda0ea69e, /// 0x0080
		 0x0ea5f9e0, /// 0x0084
		 0xaeda2390, /// 0x0088
		 0x10d5d6a1, /// 0x008c
		 0x84cfe9b0, /// 0x0090
		 0x428daf1e, /// 0x0094
		 0x95485cff, /// 0x0098
		 0x58f403fe, /// 0x009c
		 0xbdccd9c9, /// 0x00a0
		 0xdec5444d, /// 0x00a4
		 0x01f3d90d, /// 0x00a8
		 0x8d6f0d26, /// 0x00ac
		 0x8e329820, /// 0x00b0
		 0xcd8d46f2, /// 0x00b4
		 0xef8db5c1, /// 0x00b8
		 0x3873ab9e, /// 0x00bc
		 0xa2434e20, /// 0x00c0
		 0x99a60743, /// 0x00c4
		 0x459e2e68, /// 0x00c8
		 0x3a9f032b, /// 0x00cc
		 0x123d9aa6, /// 0x00d0
		 0xf0a6395f, /// 0x00d4
		 0x03daf18d, /// 0x00d8
		 0x0e46123f, /// 0x00dc
		 0x93876eeb, /// 0x00e0
		 0xc26c87d3, /// 0x00e4
		 0x8cc70800, /// 0x00e8
		 0x14769978, /// 0x00ec
		 0xa4f281c5, /// 0x00f0
		 0x2de01124, /// 0x00f4
		 0x95289cc1, /// 0x00f8
		 0x8caedc33, /// 0x00fc
		 0x6c7eee8e, /// 0x0100
		 0x1a2b3071, /// 0x0104
		 0xb7158395, /// 0x0108
		 0x3599577b, /// 0x010c
		 0x98fd95b3, /// 0x0110
		 0x23b593c0, /// 0x0114
		 0x7bffbe42, /// 0x0118
		 0xb15021df, /// 0x011c
		 0x120780b7, /// 0x0120
		 0x57ff64fa, /// 0x0124
		 0xa5e4d99a, /// 0x0128
		 0xc4f2204d, /// 0x012c
		 0xd3948bd5, /// 0x0130
		 0xd81c3512, /// 0x0134
		 0x7f2398f8, /// 0x0138
		 0xbae6d18e, /// 0x013c
		 0xd1239301, /// 0x0140
		 0xd38cac7e, /// 0x0144
		 0xcbce6028, /// 0x0148
		 0x2c857aa1, /// 0x014c
		 0x12b91505, /// 0x0150
		 0xba02fb88, /// 0x0154
		 0x0ba16348, /// 0x0158
		 0x9aff5951, /// 0x015c
		 0x49a2c408, /// 0x0160
		 0x185d4d2f, /// 0x0164
		 0xb3b2b711, /// 0x0168
		 0xe6b8b7b3, /// 0x016c
		 0xb99ae717, /// 0x0170
		 0xe44d88d1, /// 0x0174
		 0xd06f79e2, /// 0x0178
		 0x3e77d328, /// 0x017c
		 0xf25394e3, /// 0x0180
		 0x51c25a95, /// 0x0184
		 0xddf3feeb, /// 0x0188
		 0xb57e2e05, /// 0x018c
		 0x9edd9d85, /// 0x0190
		 0xbeb10200, /// 0x0194
		 0x1b3d18d4, /// 0x0198
		 0xf8fbb0f5, /// 0x019c
		 0x94ad989d, /// 0x01a0
		 0x79d7e810, /// 0x01a4
		 0x5007c79d, /// 0x01a8
		 0x6ccd9171, /// 0x01ac
		 0xcc084332, /// 0x01b0
		 0x3a6c7d5c, /// 0x01b4
		 0xbe1f2777, /// 0x01b8
		 0x467402b1, /// 0x01bc
		 0x166c1da6, /// 0x01c0
		 0xcd3b2696, /// 0x01c4
		 0xc5da54e4, /// 0x01c8
		 0x5e513711, /// 0x01cc
		 0x092f2b32, /// 0x01d0
		 0x689e06a8, /// 0x01d4
		 0xa1b4c136, /// 0x01d8
		 0xbc250d55, /// 0x01dc
		 0xbd32380b, /// 0x01e0
		 0x4d3b84c1, /// 0x01e4
		 0x963af849, /// 0x01e8
		 0xd728679a, /// 0x01ec
		 0x0cc8b372, /// 0x01f0
		 0x8c5bec48, /// 0x01f4
		 0x2bdda6b9, /// 0x01f8
		 0x88329e4b, /// 0x01fc
		 0x752326ba, /// 0x0200
		 0x51ad5e44, /// 0x0204
		 0x7835fe24, /// 0x0208
		 0x7e6482f1, /// 0x020c
		 0xb378e351, /// 0x0210
		 0x98686ac3, /// 0x0214
		 0x51f7999e, /// 0x0218
		 0xb9a2f8b6, /// 0x021c
		 0xbc80a7ca, /// 0x0220
		 0x1625e01c, /// 0x0224
		 0x78c235af, /// 0x0228
		 0xcd61761e, /// 0x022c
		 0x1e72245d, /// 0x0230
		 0x78c4e101, /// 0x0234
		 0xadd09a33, /// 0x0238
		 0x65774c9f, /// 0x023c
		 0x07f7e5e2, /// 0x0240
		 0xb2d8a720, /// 0x0244
		 0x81874530, /// 0x0248
		 0x0cddba1b, /// 0x024c
		 0x1b34044b, /// 0x0250
		 0x75f6a475, /// 0x0254
		 0x35d56365, /// 0x0258
		 0xcf285868, /// 0x025c
		 0x1f655248, /// 0x0260
		 0x6a4aa852, /// 0x0264
		 0x859b3c87, /// 0x0268
		 0x0cde35ce, /// 0x026c
		 0x02c75a5c, /// 0x0270
		 0x22601e83, /// 0x0274
		 0x3ca0fb3d, /// 0x0278
		 0x60cb3801, /// 0x027c
		 0x074dc6fa, /// 0x0280
		 0xf07f5048, /// 0x0284
		 0xc79a4bc5, /// 0x0288
		 0x3c1293c8, /// 0x028c
		 0x3ad44991, /// 0x0290
		 0x5fb307ba, /// 0x0294
		 0xc28ec383, /// 0x0298
		 0x05ffb43a, /// 0x029c
		 0x42d7bb77, /// 0x02a0
		 0xf26c7e92, /// 0x02a4
		 0xab9cbafd, /// 0x02a8
		 0xee6fea64, /// 0x02ac
		 0x569224b3, /// 0x02b0
		 0x2b2e4400, /// 0x02b4
		 0x43086762, /// 0x02b8
		 0x14cf9900, /// 0x02bc
		 0x28e734b0, /// 0x02c0
		 0xe3b2d827, /// 0x02c4
		 0x46896605, /// 0x02c8
		 0x58077b8e, /// 0x02cc
		 0x248116c5, /// 0x02d0
		 0xca0c02d9, /// 0x02d4
		 0x9102688a, /// 0x02d8
		 0x3311326a, /// 0x02dc
		 0xbf3c01bd, /// 0x02e0
		 0x4fbc3c57, /// 0x02e4
		 0xc672478e, /// 0x02e8
		 0x28ff1cf2, /// 0x02ec
		 0x9c11726f, /// 0x02f0
		 0xa5c42d88, /// 0x02f4
		 0x1406bd95, /// 0x02f8
		 0xff292ed9, /// 0x02fc
		 0x02365ead, /// 0x0300
		 0xfdbc571b, /// 0x0304
		 0x4f58f500, /// 0x0308
		 0xa87be7ee, /// 0x030c
		 0x2655eefb, /// 0x0310
		 0x0f24fdaf, /// 0x0314
		 0xdae9c23a, /// 0x0318
		 0xb311c029, /// 0x031c
		 0xdcde875d, /// 0x0320
		 0xa0559fc2, /// 0x0324
		 0x38552ed8, /// 0x0328
		 0x2d6e5d70, /// 0x032c
		 0x70775886, /// 0x0330
		 0x6a017c70, /// 0x0334
		 0x3e4f6bb3, /// 0x0338
		 0x2a862801, /// 0x033c
		 0x8e6a665d, /// 0x0340
		 0xf29aef08, /// 0x0344
		 0x4bece4b9, /// 0x0348
		 0x7c98f4c9, /// 0x034c
		 0x00080071, /// 0x0350
		 0x614840d3, /// 0x0354
		 0xa671487a, /// 0x0358
		 0x76f92cfc, /// 0x035c
		 0x96185ef8, /// 0x0360
		 0x68a3b162, /// 0x0364
		 0x36d87b22, /// 0x0368
		 0xf61f5d43, /// 0x036c
		 0x90c1c43e, /// 0x0370
		 0x99a54534, /// 0x0374
		 0x2c116643, /// 0x0378
		 0x58164020, /// 0x037c
		 0x1949db7b, /// 0x0380
		 0x5852373b, /// 0x0384
		 0x89564f99, /// 0x0388
		 0x170434e6, /// 0x038c
		 0x73203499, /// 0x0390
		 0x98986d48, /// 0x0394
		 0xa1c5b421, /// 0x0398
		 0x6c791d8c, /// 0x039c
		 0xfd880211, /// 0x03a0
		 0xf59c7a61, /// 0x03a4
		 0xab167b27, /// 0x03a8
		 0x0ba87e7f, /// 0x03ac
		 0x23cf2ff5, /// 0x03b0
		 0x05eed13e, /// 0x03b4
		 0x62931292, /// 0x03b8
		 0xd35cac74, /// 0x03bc
		 0x34ec4754, /// 0x03c0
		 0xbdf3b72e, /// 0x03c4
		 0x9e0a042d, /// 0x03c8
		 0x09ee787b, /// 0x03cc
		 0xd37d1cd2, /// 0x03d0
		 0xe2c61f6d, /// 0x03d4
		 0xfae0a185, /// 0x03d8
		 0x6fee6e05, /// 0x03dc
		 0xfef23892, /// 0x03e0
		 0x0935d57d, /// 0x03e4
		 0xffaed2d7, /// 0x03e8
		 0xd168165b, /// 0x03ec
		 0x85a5f479, /// 0x03f0
		 0x95113da2, /// 0x03f4
		 0x0883f16e, /// 0x03f8
		 0x422d1290, /// 0x03fc
		 0x79b49554, /// 0x0400
		 0x11dc4a6d, /// 0x0404
		 0x14c0f432, /// 0x0408
		 0x4ff90afb, /// 0x040c
		 0xd34cf00a, /// 0x0410
		 0xee8dd8ac, /// 0x0414
		 0x45a7a789, /// 0x0418
		 0x139d4219, /// 0x041c
		 0xc3d91c00, /// 0x0420
		 0x81393451, /// 0x0424
		 0xa811b409, /// 0x0428
		 0x6f4e2f44, /// 0x042c
		 0xb87aa48b, /// 0x0430
		 0xc5015655, /// 0x0434
		 0x897412ff, /// 0x0438
		 0x29ee80aa, /// 0x043c
		 0x3ce3c14e, /// 0x0440
		 0x3772cc5b, /// 0x0444
		 0x61d3b90e, /// 0x0448
		 0x22296a9f, /// 0x044c
		 0x00f78684, /// 0x0450
		 0xbb5d5f09, /// 0x0454
		 0x9c2ac72e, /// 0x0458
		 0x27b93693, /// 0x045c
		 0xb91d66c0, /// 0x0460
		 0xa0e11ea6, /// 0x0464
		 0x115bc9aa, /// 0x0468
		 0xf6898147, /// 0x046c
		 0x2fe8b5da, /// 0x0470
		 0x35f1b3e4, /// 0x0474
		 0x5b2a1b0a, /// 0x0478
		 0xc3f96b07, /// 0x047c
		 0xcc8f6ca0, /// 0x0480
		 0xf21131bb, /// 0x0484
		 0xcbb3f168, /// 0x0488
		 0x67498483, /// 0x048c
		 0x395bc4e1, /// 0x0490
		 0x120df610, /// 0x0494
		 0x4a45a71a, /// 0x0498
		 0x96a12b64, /// 0x049c
		 0xf0fe8fa1, /// 0x04a0
		 0x27ebdbfc, /// 0x04a4
		 0xefff452c, /// 0x04a8
		 0xb48b118d, /// 0x04ac
		 0xfa53478a, /// 0x04b0
		 0x4de794b6, /// 0x04b4
		 0xf2fc084b, /// 0x04b8
		 0x2d18d3c7, /// 0x04bc
		 0x5327704a, /// 0x04c0
		 0x29263667, /// 0x04c4
		 0xb0c2ad29, /// 0x04c8
		 0xdd684b27, /// 0x04cc
		 0x58d22424, /// 0x04d0
		 0x6f8b8cd7, /// 0x04d4
		 0xca47df5e, /// 0x04d8
		 0x7c89f46a, /// 0x04dc
		 0xbb8809d1, /// 0x04e0
		 0xc6473340, /// 0x04e4
		 0x1afa8e27, /// 0x04e8
		 0x95cecae3, /// 0x04ec
		 0x0ad539fe, /// 0x04f0
		 0x23cc3c08, /// 0x04f4
		 0xc5eb23d2, /// 0x04f8
		 0x7742db26, /// 0x04fc
		 0xc04d191e, /// 0x0500
		 0xe9594cb6, /// 0x0504
		 0x7d4256ec, /// 0x0508
		 0x34d3f298, /// 0x050c
		 0xf899c77c, /// 0x0510
		 0xc1cab6fd, /// 0x0514
		 0xef593776, /// 0x0518
		 0x97be13aa, /// 0x051c
		 0x7e01d17a, /// 0x0520
		 0xc731a153, /// 0x0524
		 0x37cd90f3, /// 0x0528
		 0xb481b2cf, /// 0x052c
		 0x418d7649, /// 0x0530
		 0xd0702e13, /// 0x0534
		 0x34152469, /// 0x0538
		 0x0bdd4d85, /// 0x053c
		 0x1a865a39, /// 0x0540
		 0xeb57fcb9, /// 0x0544
		 0x48fb2363, /// 0x0548
		 0xcf2b4978, /// 0x054c
		 0x8d6eb56a, /// 0x0550
		 0xfa914ee7, /// 0x0554
		 0xe99e32d0, /// 0x0558
		 0xea227404, /// 0x055c
		 0x6ff15822, /// 0x0560
		 0xaf93b78e, /// 0x0564
		 0xced71122, /// 0x0568
		 0x50e2800e, /// 0x056c
		 0xf24b5a0a, /// 0x0570
		 0x50cb1815, /// 0x0574
		 0xb59b1e6c, /// 0x0578
		 0xedd07416, /// 0x057c
		 0x8d37af43, /// 0x0580
		 0xc2363d68, /// 0x0584
		 0x86c59843, /// 0x0588
		 0x41e2bb71, /// 0x058c
		 0xdd4b1ff2, /// 0x0590
		 0xeeb8b73d, /// 0x0594
		 0xdc75f01e, /// 0x0598
		 0xcacc130c, /// 0x059c
		 0x3f008efe, /// 0x05a0
		 0x31cef1e4, /// 0x05a4
		 0x5bbdc328, /// 0x05a8
		 0x045e4d1b, /// 0x05ac
		 0xed93e128, /// 0x05b0
		 0x376a37dd, /// 0x05b4
		 0x542930e3, /// 0x05b8
		 0x00a7cb97, /// 0x05bc
		 0x5ae77e1a, /// 0x05c0
		 0x340d5a90, /// 0x05c4
		 0x5040a732, /// 0x05c8
		 0x7bf1e9c8, /// 0x05cc
		 0x57529611, /// 0x05d0
		 0x2711f6ec, /// 0x05d4
		 0xfb6ad0c7, /// 0x05d8
		 0xf686a283, /// 0x05dc
		 0xf5270e83, /// 0x05e0
		 0xf3e08339, /// 0x05e4
		 0x2bb249dd, /// 0x05e8
		 0x04447f45, /// 0x05ec
		 0x67204cd2, /// 0x05f0
		 0x80ee7aaf, /// 0x05f4
		 0xfd70e821, /// 0x05f8
		 0x243908d9, /// 0x05fc
		 0x6e97cfbc, /// 0x0600
		 0x03a5533b, /// 0x0604
		 0xf16e98e9, /// 0x0608
		 0x25bc8251, /// 0x060c
		 0xd616e281, /// 0x0610
		 0xd924087e, /// 0x0614
		 0x6e30f4b6, /// 0x0618
		 0x203bfabb, /// 0x061c
		 0xfd001cd7, /// 0x0620
		 0x743575d9, /// 0x0624
		 0x169131dd, /// 0x0628
		 0x9da9e9b3, /// 0x062c
		 0xf68fa6a8, /// 0x0630
		 0x94994bd9, /// 0x0634
		 0x47b43d1a, /// 0x0638
		 0xac5b7b01, /// 0x063c
		 0x3b0a216e, /// 0x0640
		 0xfada881f, /// 0x0644
		 0x7f8cf66d, /// 0x0648
		 0x39344540, /// 0x064c
		 0xe93b11cd, /// 0x0650
		 0xcb784669, /// 0x0654
		 0xe95f7772, /// 0x0658
		 0xcbd698a0, /// 0x065c
		 0xcca820f6, /// 0x0660
		 0xba7b840e, /// 0x0664
		 0x87f9c65d, /// 0x0668
		 0xd96173ae, /// 0x066c
		 0xdfd1cd08, /// 0x0670
		 0x09751fcd, /// 0x0674
		 0x4403037c, /// 0x0678
		 0x306a2267, /// 0x067c
		 0xf1166dae, /// 0x0680
		 0xf4b3ac13, /// 0x0684
		 0x8320125d, /// 0x0688
		 0x6233e91f, /// 0x068c
		 0xd19c6d00, /// 0x0690
		 0x8ef8e70b, /// 0x0694
		 0xee782f8c, /// 0x0698
		 0x96f3d38c, /// 0x069c
		 0xd3bae013, /// 0x06a0
		 0x33cca95e, /// 0x06a4
		 0xf331d705, /// 0x06a8
		 0x4f903319, /// 0x06ac
		 0x56f3cdbb, /// 0x06b0
		 0x74417f8a, /// 0x06b4
		 0xbbe54273, /// 0x06b8
		 0x1c2fae74, /// 0x06bc
		 0x5db9245f, /// 0x06c0
		 0x68fd122a, /// 0x06c4
		 0x06e872cc, /// 0x06c8
		 0x7dfcf497, /// 0x06cc
		 0xfb923786, /// 0x06d0
		 0x9f3ceda1, /// 0x06d4
		 0x8aaebc66, /// 0x06d8
		 0x55ed27ac, /// 0x06dc
		 0x27ba7d64, /// 0x06e0
		 0xe2e1bc2e, /// 0x06e4
		 0x05a4e40d, /// 0x06e8
		 0x470636fa, /// 0x06ec
		 0x9892f86e, /// 0x06f0
		 0x949ec74b, /// 0x06f4
		 0xfc1f20f6, /// 0x06f8
		 0x8ecaff28, /// 0x06fc
		 0xf6626e59, /// 0x0700
		 0xd06c106e, /// 0x0704
		 0xc913e442, /// 0x0708
		 0x3a956bc4, /// 0x070c
		 0xab157407, /// 0x0710
		 0xc54a58be, /// 0x0714
		 0x3ab848e5, /// 0x0718
		 0x22529733, /// 0x071c
		 0xd4740aca, /// 0x0720
		 0xbd1ab57b, /// 0x0724
		 0xe15255bb, /// 0x0728
		 0x1a74a3dc, /// 0x072c
		 0xee4f5c75, /// 0x0730
		 0xe7a7b650, /// 0x0734
		 0x4adeaf14, /// 0x0738
		 0x88a85432, /// 0x073c
		 0x5c3f5fb0, /// 0x0740
		 0xfc21aa5c, /// 0x0744
		 0xfc1947c3, /// 0x0748
		 0x0a35d970, /// 0x074c
		 0x133c0c18, /// 0x0750
		 0xd21ae36d, /// 0x0754
		 0xcf142be3, /// 0x0758
		 0x17cbb7f7, /// 0x075c
		 0xa1d6d4d9, /// 0x0760
		 0x322535c4, /// 0x0764
		 0xf5dc4ce1, /// 0x0768
		 0xa3a204ab, /// 0x076c
		 0xbfca8ff3, /// 0x0770
		 0x52837f3f, /// 0x0774
		 0x0a7bc3e9, /// 0x0778
		 0xc73c54fe, /// 0x077c
		 0xd3c97ab9, /// 0x0780
		 0x9556ccf1, /// 0x0784
		 0xc49f077d, /// 0x0788
		 0xacecd91a, /// 0x078c
		 0xb9c74325, /// 0x0790
		 0xa46b5da7, /// 0x0794
		 0xbaf0e510, /// 0x0798
		 0x0c81659a, /// 0x079c
		 0xf512c975, /// 0x07a0
		 0x60f03940, /// 0x07a4
		 0x0bbaaa66, /// 0x07a8
		 0x9ded928a, /// 0x07ac
		 0x2241b29c, /// 0x07b0
		 0x1870c487, /// 0x07b4
		 0xedf87ea1, /// 0x07b8
		 0x6a751b7d, /// 0x07bc
		 0x10468b1b, /// 0x07c0
		 0x787b2a51, /// 0x07c4
		 0x8da17378, /// 0x07c8
		 0xce4dd362, /// 0x07cc
		 0xb6489647, /// 0x07d0
		 0x33d11d2d, /// 0x07d4
		 0x7abe46a1, /// 0x07d8
		 0x33d428c2, /// 0x07dc
		 0xd4fe8247, /// 0x07e0
		 0x41dacd74, /// 0x07e4
		 0x808b142d, /// 0x07e8
		 0xb7a43976, /// 0x07ec
		 0xd76b13fa, /// 0x07f0
		 0x997b4e77, /// 0x07f4
		 0x71dd195e, /// 0x07f8
		 0x946b2daa, /// 0x07fc
		 0xb8f4993e, /// 0x0800
		 0xccfe20cc, /// 0x0804
		 0x14333ccc, /// 0x0808
		 0xe5d4d720, /// 0x080c
		 0x23eeb6f9, /// 0x0810
		 0x575d3d19, /// 0x0814
		 0xb073cdf4, /// 0x0818
		 0xa851ba87, /// 0x081c
		 0xf28da974, /// 0x0820
		 0xe3247501, /// 0x0824
		 0x5faa480f, /// 0x0828
		 0x017e616d, /// 0x082c
		 0x5483dad1, /// 0x0830
		 0x208f2f13, /// 0x0834
		 0xbf35b337, /// 0x0838
		 0x2d02e092, /// 0x083c
		 0x23ad5975, /// 0x0840
		 0x2435d96d, /// 0x0844
		 0x38081699, /// 0x0848
		 0x4da09d72, /// 0x084c
		 0x7a17c4e4, /// 0x0850
		 0x13cec700, /// 0x0854
		 0x3118b76f, /// 0x0858
		 0x1de98310, /// 0x085c
		 0x96370bdb, /// 0x0860
		 0xf3053f12, /// 0x0864
		 0xd866b17f, /// 0x0868
		 0x59502a68, /// 0x086c
		 0xd88f32f0, /// 0x0870
		 0x8b6469b7, /// 0x0874
		 0x4ba53c48, /// 0x0878
		 0x833eb01a, /// 0x087c
		 0xfd85ed4f, /// 0x0880
		 0xb4312172, /// 0x0884
		 0x89e9f8b4, /// 0x0888
		 0x238b66e4, /// 0x088c
		 0xa80293f6, /// 0x0890
		 0x0732401b, /// 0x0894
		 0xf08d79fb, /// 0x0898
		 0x9941ca47, /// 0x089c
		 0x8bc7215d, /// 0x08a0
		 0x5a8d5b39, /// 0x08a4
		 0x3f3211ff, /// 0x08a8
		 0x24f93ade, /// 0x08ac
		 0x81c2c3ed, /// 0x08b0
		 0x0e03c505, /// 0x08b4
		 0x2972c9eb, /// 0x08b8
		 0x45993d2a, /// 0x08bc
		 0xdca88fe4, /// 0x08c0
		 0xa09072c1, /// 0x08c4
		 0x3488b5b4, /// 0x08c8
		 0x98f920fa, /// 0x08cc
		 0xe2fb3dbb, /// 0x08d0
		 0x4d9de594, /// 0x08d4
		 0x92788eb8, /// 0x08d8
		 0xdfd7289c, /// 0x08dc
		 0x148b452c, /// 0x08e0
		 0x9267c18b, /// 0x08e4
		 0xf85c7d68, /// 0x08e8
		 0x3dc4bd0e, /// 0x08ec
		 0x41f29436, /// 0x08f0
		 0x41bfa677, /// 0x08f4
		 0xe2e42126, /// 0x08f8
		 0xe70503dc, /// 0x08fc
		 0xa3df16d7, /// 0x0900
		 0x0aede1cd, /// 0x0904
		 0xb45549b3, /// 0x0908
		 0x987a479a, /// 0x090c
		 0x31c9429f, /// 0x0910
		 0x473ccd1c, /// 0x0914
		 0xf1f842de, /// 0x0918
		 0xc23377fe, /// 0x091c
		 0x31a8e4fd, /// 0x0920
		 0x3f69aaf0, /// 0x0924
		 0xeb2fce22, /// 0x0928
		 0x5c38ba2a, /// 0x092c
		 0x6660c985, /// 0x0930
		 0xbb3be16a, /// 0x0934
		 0xc5e20954, /// 0x0938
		 0xb1ee397b, /// 0x093c
		 0x97901a74, /// 0x0940
		 0x06d1c8d7, /// 0x0944
		 0x66db78bb, /// 0x0948
		 0x21419595, /// 0x094c
		 0xffc6fa9e, /// 0x0950
		 0xda2f721a, /// 0x0954
		 0x049aac1f, /// 0x0958
		 0xd848733d, /// 0x095c
		 0xba4c07d6, /// 0x0960
		 0x779a18be, /// 0x0964
		 0xbfdbd242, /// 0x0968
		 0x82fb04e0, /// 0x096c
		 0x962387c0, /// 0x0970
		 0xdb26516f, /// 0x0974
		 0xdb4c7b90, /// 0x0978
		 0x20db2318, /// 0x097c
		 0x7078fbc6, /// 0x0980
		 0xecf24075, /// 0x0984
		 0x9d6ba87c, /// 0x0988
		 0xccb0cba2, /// 0x098c
		 0x400bc6b9, /// 0x0990
		 0x6bb959cf, /// 0x0994
		 0xc43fd498, /// 0x0998
		 0xaa1392b9, /// 0x099c
		 0x80248da6, /// 0x09a0
		 0xb3f83d55, /// 0x09a4
		 0xf8834935, /// 0x09a8
		 0x0c1ff420, /// 0x09ac
		 0xeb3183c8, /// 0x09b0
		 0x6e06a0c9, /// 0x09b4
		 0x5528b189, /// 0x09b8
		 0xd2c04ef0, /// 0x09bc
		 0xb3471bcf, /// 0x09c0
		 0xd14ec410, /// 0x09c4
		 0x5c60385b, /// 0x09c8
		 0x33d47f73, /// 0x09cc
		 0x51dee0df, /// 0x09d0
		 0x4b15e17f, /// 0x09d4
		 0xea657637, /// 0x09d8
		 0xa7221713, /// 0x09dc
		 0x5b3773bb, /// 0x09e0
		 0x1247dfca, /// 0x09e4
		 0xf77f8d1a, /// 0x09e8
		 0xaa4eb69d, /// 0x09ec
		 0x46262222, /// 0x09f0
		 0x9ab8f32b, /// 0x09f4
		 0xfc748a4c, /// 0x09f8
		 0x20c15e9c, /// 0x09fc
		 0xf4e25b40, /// 0x0a00
		 0xbdb02aa9, /// 0x0a04
		 0xea26ac38, /// 0x0a08
		 0x1b1e29a9, /// 0x0a0c
		 0xc94075fe, /// 0x0a10
		 0x5018056f, /// 0x0a14
		 0xa3dbbd61, /// 0x0a18
		 0x648808dc, /// 0x0a1c
		 0xb5987c7a, /// 0x0a20
		 0xc6a19b4f, /// 0x0a24
		 0xf9eb6c93, /// 0x0a28
		 0xb7d039d6, /// 0x0a2c
		 0xc5117fe1, /// 0x0a30
		 0x39c8b59c, /// 0x0a34
		 0x7f453c57, /// 0x0a38
		 0x56c209d6, /// 0x0a3c
		 0x7e690779, /// 0x0a40
		 0x813fe039, /// 0x0a44
		 0x78bb9e99, /// 0x0a48
		 0x5ac2da0b, /// 0x0a4c
		 0x4cf98763, /// 0x0a50
		 0x5f866997, /// 0x0a54
		 0x46509d04, /// 0x0a58
		 0x4c16f6dd, /// 0x0a5c
		 0xfe98e2d3, /// 0x0a60
		 0xc436a086, /// 0x0a64
		 0x7b77451b, /// 0x0a68
		 0x484ef84a, /// 0x0a6c
		 0xd4d66ae0, /// 0x0a70
		 0xe8f0f6bb, /// 0x0a74
		 0x4960880f, /// 0x0a78
		 0xcd0b66ca, /// 0x0a7c
		 0x27d47da7, /// 0x0a80
		 0x67775d20, /// 0x0a84
		 0x2a0bb8c1, /// 0x0a88
		 0xd5c36ff8, /// 0x0a8c
		 0x6a2640ac, /// 0x0a90
		 0xb0ef6c2d, /// 0x0a94
		 0xd0c33c82, /// 0x0a98
		 0xffb25988, /// 0x0a9c
		 0xd874c935, /// 0x0aa0
		 0x753eecbf, /// 0x0aa4
		 0x35cfff15, /// 0x0aa8
		 0xd420ea36, /// 0x0aac
		 0x8bc68a3b, /// 0x0ab0
		 0x567058e1, /// 0x0ab4
		 0x41691e66, /// 0x0ab8
		 0xccdd1e96, /// 0x0abc
		 0x3e1b1afc, /// 0x0ac0
		 0x0b82d804, /// 0x0ac4
		 0xf73d929f, /// 0x0ac8
		 0x340ee12e, /// 0x0acc
		 0x2276d076, /// 0x0ad0
		 0xc391bbcf, /// 0x0ad4
		 0x3d33eb32, /// 0x0ad8
		 0xced1e2c3, /// 0x0adc
		 0x56ff67ef, /// 0x0ae0
		 0x6c678e4b, /// 0x0ae4
		 0xc15a353f, /// 0x0ae8
		 0xafe5f3fe, /// 0x0aec
		 0xd1b74669, /// 0x0af0
		 0xad601176, /// 0x0af4
		 0x22d9ff9a, /// 0x0af8
		 0xdc3bd8f0, /// 0x0afc
		 0xcacbac03, /// 0x0b00
		 0xdfa7e8fb, /// 0x0b04
		 0xa4a28bd2, /// 0x0b08
		 0x739af347, /// 0x0b0c
		 0xf2d01b51, /// 0x0b10
		 0xe6b0c1b4, /// 0x0b14
		 0x00f3b8db, /// 0x0b18
		 0x08d46dff, /// 0x0b1c
		 0xe40a86eb, /// 0x0b20
		 0x0eab4efa, /// 0x0b24
		 0xed42684e, /// 0x0b28
		 0xf04fb50e, /// 0x0b2c
		 0x410e2817, /// 0x0b30
		 0x7dc6145f, /// 0x0b34
		 0x42add892, /// 0x0b38
		 0x1dcedc56, /// 0x0b3c
		 0x89ca224f, /// 0x0b40
		 0xb25ddada, /// 0x0b44
		 0x3ac5e7ad, /// 0x0b48
		 0x17a2725e, /// 0x0b4c
		 0xf1560c09, /// 0x0b50
		 0xbd25af5a, /// 0x0b54
		 0x13a2a983, /// 0x0b58
		 0xbcc4a0d4, /// 0x0b5c
		 0x896321c5, /// 0x0b60
		 0xf43b3531, /// 0x0b64
		 0x7b25dcdb, /// 0x0b68
		 0x79977057, /// 0x0b6c
		 0xa3a7a425, /// 0x0b70
		 0x80a63d00, /// 0x0b74
		 0xe050527c, /// 0x0b78
		 0x7a47931f, /// 0x0b7c
		 0xf20ff529, /// 0x0b80
		 0xb698a6d4, /// 0x0b84
		 0x9433a1f3, /// 0x0b88
		 0xebb5574a, /// 0x0b8c
		 0xd12581ac, /// 0x0b90
		 0xb90d8aca, /// 0x0b94
		 0xfb6bc4e9, /// 0x0b98
		 0x842ddb53, /// 0x0b9c
		 0xe3dd23ff, /// 0x0ba0
		 0xa3e0d385, /// 0x0ba4
		 0x44db5a75, /// 0x0ba8
		 0x1cb53c3d, /// 0x0bac
		 0x96ce49c4, /// 0x0bb0
		 0xb15e7a86, /// 0x0bb4
		 0x4a877d99, /// 0x0bb8
		 0xef84de1d, /// 0x0bbc
		 0x7f02beaa, /// 0x0bc0
		 0x4727024c, /// 0x0bc4
		 0x5386859a, /// 0x0bc8
		 0xd5a9ed1c, /// 0x0bcc
		 0xcdef3388, /// 0x0bd0
		 0x4985a356, /// 0x0bd4
		 0xfbbc2637, /// 0x0bd8
		 0x5954d329, /// 0x0bdc
		 0x8c8f428a, /// 0x0be0
		 0x631e9dc4, /// 0x0be4
		 0xda0f6729, /// 0x0be8
		 0x38c4f3da, /// 0x0bec
		 0x4aaf38b8, /// 0x0bf0
		 0x4a9139c7, /// 0x0bf4
		 0x4bbea792, /// 0x0bf8
		 0xfdf2880b, /// 0x0bfc
		 0x222b36ef, /// 0x0c00
		 0xbcef535a, /// 0x0c04
		 0xfb9c09a2, /// 0x0c08
		 0xf39c9497, /// 0x0c0c
		 0xeb146f57, /// 0x0c10
		 0xedec1685, /// 0x0c14
		 0x52fbb2cd, /// 0x0c18
		 0xd4ca9263, /// 0x0c1c
		 0x3f2089e5, /// 0x0c20
		 0xd78ec2ac, /// 0x0c24
		 0x23f015ba, /// 0x0c28
		 0xaa69c0dc, /// 0x0c2c
		 0xe329490c, /// 0x0c30
		 0x68f54521, /// 0x0c34
		 0x4a1ec83c, /// 0x0c38
		 0xf2e0de9d, /// 0x0c3c
		 0x535437d6, /// 0x0c40
		 0x172ca1a5, /// 0x0c44
		 0xbb75ece1, /// 0x0c48
		 0xd328fc0f, /// 0x0c4c
		 0x285f1134, /// 0x0c50
		 0x102bf597, /// 0x0c54
		 0x4d2b6748, /// 0x0c58
		 0xe312f7c9, /// 0x0c5c
		 0x0b694fa9, /// 0x0c60
		 0xa7f99b74, /// 0x0c64
		 0x10833846, /// 0x0c68
		 0xb803876f, /// 0x0c6c
		 0x4f582edb, /// 0x0c70
		 0x590220b3, /// 0x0c74
		 0x1958ef78, /// 0x0c78
		 0xed027472, /// 0x0c7c
		 0xd3b69912, /// 0x0c80
		 0xc3640933, /// 0x0c84
		 0x3471e07c, /// 0x0c88
		 0x7d202ca4, /// 0x0c8c
		 0x8a119e7e, /// 0x0c90
		 0x40bc1025, /// 0x0c94
		 0x59622fda, /// 0x0c98
		 0x60414851, /// 0x0c9c
		 0x349339d0, /// 0x0ca0
		 0x15951788, /// 0x0ca4
		 0x8fdcbff8, /// 0x0ca8
		 0xb125eed7, /// 0x0cac
		 0x393c771e, /// 0x0cb0
		 0xb08a0ee9, /// 0x0cb4
		 0x1a9c7abc, /// 0x0cb8
		 0x662f564f, /// 0x0cbc
		 0xc7b13faf, /// 0x0cc0
		 0xce9d4922, /// 0x0cc4
		 0x2a23fc67, /// 0x0cc8
		 0x1e247496, /// 0x0ccc
		 0x3803b38f, /// 0x0cd0
		 0xa10ac640, /// 0x0cd4
		 0xcebe93c0, /// 0x0cd8
		 0xa47adde1, /// 0x0cdc
		 0xc533d4c8, /// 0x0ce0
		 0xab559ce8, /// 0x0ce4
		 0xa063bc6e, /// 0x0ce8
		 0x1d1bb629, /// 0x0cec
		 0x9bdd6735, /// 0x0cf0
		 0x47b28151, /// 0x0cf4
		 0x9bb6dc0a, /// 0x0cf8
		 0xf096d0da, /// 0x0cfc
		 0x3bb8328f, /// 0x0d00
		 0x72b59136, /// 0x0d04
		 0x9b213f9c, /// 0x0d08
		 0x681fd519, /// 0x0d0c
		 0xecf5b54c, /// 0x0d10
		 0x8f18f703, /// 0x0d14
		 0xa5029fff, /// 0x0d18
		 0x60b2de73, /// 0x0d1c
		 0xd9ecdc1c, /// 0x0d20
		 0x2a52b806, /// 0x0d24
		 0xa7750dca, /// 0x0d28
		 0xd536630c, /// 0x0d2c
		 0x76ee7193, /// 0x0d30
		 0xe2941a66, /// 0x0d34
		 0xcc3404f8, /// 0x0d38
		 0x0b87db62, /// 0x0d3c
		 0xe41ef305, /// 0x0d40
		 0x620234e2, /// 0x0d44
		 0x2ae27fe7, /// 0x0d48
		 0x21c7a9f6, /// 0x0d4c
		 0xac50e7f6, /// 0x0d50
		 0xe5594470, /// 0x0d54
		 0x43db866c, /// 0x0d58
		 0xde97149a, /// 0x0d5c
		 0xf2c11734, /// 0x0d60
		 0xc1a79bcd, /// 0x0d64
		 0x83356c34, /// 0x0d68
		 0x32a531ee, /// 0x0d6c
		 0xc7430319, /// 0x0d70
		 0x2757a635, /// 0x0d74
		 0x22662309, /// 0x0d78
		 0x96aed5f9, /// 0x0d7c
		 0x2f78bb08, /// 0x0d80
		 0xc4861b6d, /// 0x0d84
		 0xf1fb7a71, /// 0x0d88
		 0x6f1148b4, /// 0x0d8c
		 0x12e1d9ff, /// 0x0d90
		 0x345af33e, /// 0x0d94
		 0x58b0a4c1, /// 0x0d98
		 0xd17f2886, /// 0x0d9c
		 0x1e669570, /// 0x0da0
		 0x271ad596, /// 0x0da4
		 0xcacc7b45, /// 0x0da8
		 0xa992e1fa, /// 0x0dac
		 0xf7a1734c, /// 0x0db0
		 0xf6d24508, /// 0x0db4
		 0xfdd022bf, /// 0x0db8
		 0x985ccf65, /// 0x0dbc
		 0xf9ca94da, /// 0x0dc0
		 0xe296b685, /// 0x0dc4
		 0x23bcb4c4, /// 0x0dc8
		 0xacbaf465, /// 0x0dcc
		 0xf2075a91, /// 0x0dd0
		 0xfae46148, /// 0x0dd4
		 0x5e622f8d, /// 0x0dd8
		 0xa8644f4c, /// 0x0ddc
		 0xd89e78a4, /// 0x0de0
		 0xea7e64ba, /// 0x0de4
		 0xa2170eb3, /// 0x0de8
		 0xfb00240e, /// 0x0dec
		 0x15a53865, /// 0x0df0
		 0x11fa5355, /// 0x0df4
		 0x7254ce64, /// 0x0df8
		 0x87538139, /// 0x0dfc
		 0x82fe446b, /// 0x0e00
		 0x0d7577bf, /// 0x0e04
		 0x28a775bb, /// 0x0e08
		 0x3c616dd0, /// 0x0e0c
		 0xc85e918b, /// 0x0e10
		 0xea8151a9, /// 0x0e14
		 0x7201439d, /// 0x0e18
		 0x16311e4d, /// 0x0e1c
		 0x9ae9b242, /// 0x0e20
		 0x344c7a38, /// 0x0e24
		 0xbe6941b0, /// 0x0e28
		 0xc72f4b6e, /// 0x0e2c
		 0x4433fda6, /// 0x0e30
		 0xb0990c62, /// 0x0e34
		 0xe2c0fe57, /// 0x0e38
		 0x5bf33aab, /// 0x0e3c
		 0x00b9c6fa, /// 0x0e40
		 0xab127e85, /// 0x0e44
		 0xd9499092, /// 0x0e48
		 0x3c92fc38, /// 0x0e4c
		 0x79e4921a, /// 0x0e50
		 0x6e22129b, /// 0x0e54
		 0x50cde01c, /// 0x0e58
		 0xfd9ea8c4, /// 0x0e5c
		 0x0478e911, /// 0x0e60
		 0x122310f6, /// 0x0e64
		 0x30005347, /// 0x0e68
		 0x6dea3a2f, /// 0x0e6c
		 0x2885f429, /// 0x0e70
		 0x5395e24a, /// 0x0e74
		 0x7edb1c20, /// 0x0e78
		 0xdd9d7b60, /// 0x0e7c
		 0x68ffed1f, /// 0x0e80
		 0x087ab67b, /// 0x0e84
		 0x5c357882, /// 0x0e88
		 0x47b69806, /// 0x0e8c
		 0x159a746b, /// 0x0e90
		 0x1228285e, /// 0x0e94
		 0x6c97ff5c, /// 0x0e98
		 0xb6c26605, /// 0x0e9c
		 0x690c0fbe, /// 0x0ea0
		 0x679f5da6, /// 0x0ea4
		 0xc1737c55, /// 0x0ea8
		 0x3306c162, /// 0x0eac
		 0x7e4479db, /// 0x0eb0
		 0x54ef61a0, /// 0x0eb4
		 0xafb05c6d, /// 0x0eb8
		 0xb8954598, /// 0x0ebc
		 0x97f1b836, /// 0x0ec0
		 0xf4dc17ce, /// 0x0ec4
		 0x6e969b9c, /// 0x0ec8
		 0x6a46942c, /// 0x0ecc
		 0x1dd02d47, /// 0x0ed0
		 0xa9e6b82a, /// 0x0ed4
		 0xe0a066e8, /// 0x0ed8
		 0x2ead9374, /// 0x0edc
		 0x10287f7e, /// 0x0ee0
		 0xa7248f5a, /// 0x0ee4
		 0xde937327, /// 0x0ee8
		 0xb2d33276, /// 0x0eec
		 0x457a88b2, /// 0x0ef0
		 0xf86150c2, /// 0x0ef4
		 0x1f649484, /// 0x0ef8
		 0x676bf2b8, /// 0x0efc
		 0xb1464c85, /// 0x0f00
		 0x7e6a086f, /// 0x0f04
		 0x09fbb5ca, /// 0x0f08
		 0x1c87e87d, /// 0x0f0c
		 0x2036d224, /// 0x0f10
		 0x60580b29, /// 0x0f14
		 0x231e039b, /// 0x0f18
		 0x1cb93e9c, /// 0x0f1c
		 0x9ba0579a, /// 0x0f20
		 0x4e6a5abe, /// 0x0f24
		 0xcab88688, /// 0x0f28
		 0x68439c0c, /// 0x0f2c
		 0x28af2396, /// 0x0f30
		 0x5853c7c2, /// 0x0f34
		 0x41f4afdf, /// 0x0f38
		 0x925613df, /// 0x0f3c
		 0x5055e371, /// 0x0f40
		 0x69adc7c2, /// 0x0f44
		 0xe92aa3b3, /// 0x0f48
		 0xefe7d49a, /// 0x0f4c
		 0x621a88b3, /// 0x0f50
		 0xe94f75e3, /// 0x0f54
		 0x868a838c, /// 0x0f58
		 0x70af8742, /// 0x0f5c
		 0xbb0ac381, /// 0x0f60
		 0x9cb5a536, /// 0x0f64
		 0xc6a548cd, /// 0x0f68
		 0xf4ecf430, /// 0x0f6c
		 0x6746ada4, /// 0x0f70
		 0xfeec9595, /// 0x0f74
		 0xb4a87f2f, /// 0x0f78
		 0xdbe13d57, /// 0x0f7c
		 0xc728dd9d, /// 0x0f80
		 0x2f81b203, /// 0x0f84
		 0xeecde365, /// 0x0f88
		 0x733ebe6d, /// 0x0f8c
		 0xc3973357, /// 0x0f90
		 0x9bfcb4eb, /// 0x0f94
		 0x7bcad35c, /// 0x0f98
		 0x035df9e6, /// 0x0f9c
		 0xd51d349f, /// 0x0fa0
		 0x6ab0af72, /// 0x0fa4
		 0xd3b52805, /// 0x0fa8
		 0x90b86964, /// 0x0fac
		 0x2c4501eb, /// 0x0fb0
		 0xa674344a, /// 0x0fb4
		 0xbe9819c8, /// 0x0fb8
		 0xd4aba76c, /// 0x0fbc
		 0xc00cd1f2, /// 0x0fc0
		 0x9470b557, /// 0x0fc4
		 0x85cf6c0f, /// 0x0fc8
		 0xf62bbde5, /// 0x0fcc
		 0xaeb8bca9, /// 0x0fd0
		 0x71defa06, /// 0x0fd4
		 0xeb54e7e4, /// 0x0fd8
		 0xfcda63ad, /// 0x0fdc
		 0xf42d22f4, /// 0x0fe0
		 0xa3a1e14b, /// 0x0fe4
		 0x9ccc0f40, /// 0x0fe8
		 0xccb807ca, /// 0x0fec
		 0xa44bbb41, /// 0x0ff0
		 0x4fbdbaff, /// 0x0ff4
		 0x1a029262, /// 0x0ff8
		 0x8e0f1344, /// 0x0ffc
		 0x14f2dd22, /// 0x1000
		 0x7783c14e, /// 0x1004
		 0x15c6b3ad, /// 0x1008
		 0xc33f9351, /// 0x100c
		 0xc98a5c5a, /// 0x1010
		 0x3516dcbf, /// 0x1014
		 0x9b5e165e, /// 0x1018
		 0x07d58f95, /// 0x101c
		 0x9714dab6, /// 0x1020
		 0xebc0a2cc, /// 0x1024
		 0xef8a3917, /// 0x1028
		 0xd0c9dd89, /// 0x102c
		 0xe3509fe4, /// 0x1030
		 0x189e8ecd, /// 0x1034
		 0x68125fd0, /// 0x1038
		 0x9c73685e, /// 0x103c
		 0xe24f7eec, /// 0x1040
		 0x2f488599, /// 0x1044
		 0x47cc2539, /// 0x1048
		 0xac798b61, /// 0x104c
		 0x282ec157, /// 0x1050
		 0x746a2e69, /// 0x1054
		 0xe7d6109a, /// 0x1058
		 0x9da20a9c, /// 0x105c
		 0x24e7e6cd, /// 0x1060
		 0xcb52dff2, /// 0x1064
		 0x67f5e59f, /// 0x1068
		 0x565b8ce7, /// 0x106c
		 0xd0668d53, /// 0x1070
		 0x8c9143bc, /// 0x1074
		 0xe3e0d89e, /// 0x1078
		 0x4acba1c1, /// 0x107c
		 0xd15e763d, /// 0x1080
		 0x71b93a95, /// 0x1084
		 0xf302cfd4, /// 0x1088
		 0xc1b50816, /// 0x108c
		 0x5e7a0bac, /// 0x1090
		 0xdf3fc384, /// 0x1094
		 0x89bbc874, /// 0x1098
		 0x9dabc91f, /// 0x109c
		 0x3b00a301, /// 0x10a0
		 0x4c4ea9ce, /// 0x10a4
		 0xff0707e1, /// 0x10a8
		 0x8cbda685, /// 0x10ac
		 0x0bd9f9ea, /// 0x10b0
		 0x103f1147, /// 0x10b4
		 0x6a009717, /// 0x10b8
		 0x53700d4b, /// 0x10bc
		 0xe526b457, /// 0x10c0
		 0x7c492801, /// 0x10c4
		 0x373a7596, /// 0x10c8
		 0xd084ee23, /// 0x10cc
		 0xe143043c, /// 0x10d0
		 0x28255995, /// 0x10d4
		 0xb31b8988, /// 0x10d8
		 0x18a76240, /// 0x10dc
		 0x15f0c32e, /// 0x10e0
		 0x10291a11, /// 0x10e4
		 0x4d2fe1dc, /// 0x10e8
		 0xa93cb520, /// 0x10ec
		 0xb3bb940b, /// 0x10f0
		 0x0a3ebfbf, /// 0x10f4
		 0x37793c9e, /// 0x10f8
		 0x712d75c1, /// 0x10fc
		 0x84efc6e9, /// 0x1100
		 0x9af9ac4a, /// 0x1104
		 0xea7314f7, /// 0x1108
		 0x58f4a702, /// 0x110c
		 0xab79df07, /// 0x1110
		 0xac9ece09, /// 0x1114
		 0x9a4b6823, /// 0x1118
		 0x6424d508, /// 0x111c
		 0xa6456776, /// 0x1120
		 0x12a1bea8, /// 0x1124
		 0x3e2cd3dd, /// 0x1128
		 0x98687f59, /// 0x112c
		 0x3986f104, /// 0x1130
		 0xd9c9a4d4, /// 0x1134
		 0xd7f9ebe0, /// 0x1138
		 0xb1b1f7e6, /// 0x113c
		 0x4bb9f209, /// 0x1140
		 0x45ca9cac, /// 0x1144
		 0xca68fa54, /// 0x1148
		 0x981433de, /// 0x114c
		 0x3f38a461, /// 0x1150
		 0x209baa5d, /// 0x1154
		 0xd41fd33d, /// 0x1158
		 0xcc500720, /// 0x115c
		 0xcdfcb597, /// 0x1160
		 0x2ef2102b, /// 0x1164
		 0xa491075c, /// 0x1168
		 0x0baeee43, /// 0x116c
		 0x9b0c456e, /// 0x1170
		 0x7ba08f61, /// 0x1174
		 0x8eda0911, /// 0x1178
		 0x369769b4, /// 0x117c
		 0x3198fdfb, /// 0x1180
		 0xa1e01fb4, /// 0x1184
		 0x56d7e9bd, /// 0x1188
		 0xa4503c9e, /// 0x118c
		 0x5a2a7853, /// 0x1190
		 0xd16e0187, /// 0x1194
		 0x056ae517, /// 0x1198
		 0x3712b665, /// 0x119c
		 0x9860836d, /// 0x11a0
		 0x22f64126, /// 0x11a4
		 0xd91459a6, /// 0x11a8
		 0x43fb1017, /// 0x11ac
		 0xc9012760, /// 0x11b0
		 0x70ad9f99, /// 0x11b4
		 0xa24bc4ff, /// 0x11b8
		 0xacb40bfc, /// 0x11bc
		 0xcefc9ea6, /// 0x11c0
		 0xb21400dc, /// 0x11c4
		 0xb275eeaa, /// 0x11c8
		 0x63d1959d, /// 0x11cc
		 0x4a4ecae0, /// 0x11d0
		 0x9f4c9533, /// 0x11d4
		 0xe01b0dd7, /// 0x11d8
		 0x4198fe89, /// 0x11dc
		 0x941b6018, /// 0x11e0
		 0x4388907d, /// 0x11e4
		 0xf81fc92c, /// 0x11e8
		 0x611214b4, /// 0x11ec
		 0xdbf95910, /// 0x11f0
		 0x8b7c592b, /// 0x11f4
		 0x380fe95d, /// 0x11f8
		 0x286f73a5, /// 0x11fc
		 0xb2f7a286, /// 0x1200
		 0x9e85c950, /// 0x1204
		 0x9a020a07, /// 0x1208
		 0x1a919645, /// 0x120c
		 0x8223e8ac, /// 0x1210
		 0x0805e4e2, /// 0x1214
		 0x8e16e742, /// 0x1218
		 0x202e0c7a, /// 0x121c
		 0x6faedcca, /// 0x1220
		 0xd0e3508b, /// 0x1224
		 0x8ec42ace, /// 0x1228
		 0x5011526d, /// 0x122c
		 0x6791bffb, /// 0x1230
		 0x37c23b9c, /// 0x1234
		 0xc1ecd9eb, /// 0x1238
		 0x1a3de301, /// 0x123c
		 0x8aaef253, /// 0x1240
		 0x1305ae20, /// 0x1244
		 0xe9042790, /// 0x1248
		 0x9c80667d, /// 0x124c
		 0xb02f6638, /// 0x1250
		 0x99a770a3, /// 0x1254
		 0xbfb42096, /// 0x1258
		 0x48a688f9, /// 0x125c
		 0xe5ed4487, /// 0x1260
		 0xcccc9205, /// 0x1264
		 0x598fe111, /// 0x1268
		 0xb74075a7, /// 0x126c
		 0xa6b35963, /// 0x1270
		 0x34f68eec, /// 0x1274
		 0xb63ae0fa, /// 0x1278
		 0x17e81cee, /// 0x127c
		 0x41821d6d, /// 0x1280
		 0x6806e979, /// 0x1284
		 0xdd881fc4, /// 0x1288
		 0x90c709fb, /// 0x128c
		 0xc039b5b8, /// 0x1290
		 0xc8fa8c41, /// 0x1294
		 0xf5e73f39, /// 0x1298
		 0x8a03089b, /// 0x129c
		 0x1c704ec9, /// 0x12a0
		 0x94447499, /// 0x12a4
		 0x28350d95, /// 0x12a8
		 0x062d2e75, /// 0x12ac
		 0xde0ec6ce, /// 0x12b0
		 0x95a1eeee, /// 0x12b4
		 0xde89c979, /// 0x12b8
		 0x87b91650, /// 0x12bc
		 0x2b1314ad, /// 0x12c0
		 0x918f2ddf, /// 0x12c4
		 0x2f3ce684, /// 0x12c8
		 0x9cf4c714, /// 0x12cc
		 0x5311e3c8, /// 0x12d0
		 0x0cf2ff90, /// 0x12d4
		 0xbd246c60, /// 0x12d8
		 0x96569404, /// 0x12dc
		 0x5a5a14ab, /// 0x12e0
		 0xf38e4284, /// 0x12e4
		 0x2f114b53, /// 0x12e8
		 0x963c4d12, /// 0x12ec
		 0xc28d14cc, /// 0x12f0
		 0x934e8072, /// 0x12f4
		 0xdd809c79, /// 0x12f8
		 0xa7ef8e16, /// 0x12fc
		 0x40adc74c, /// 0x1300
		 0xa579529a, /// 0x1304
		 0x7717748c, /// 0x1308
		 0x7fc27615, /// 0x130c
		 0x4edb6d49, /// 0x1310
		 0x8a6559b8, /// 0x1314
		 0x07ce4a78, /// 0x1318
		 0xc0c4ad99, /// 0x131c
		 0x17dd8edb, /// 0x1320
		 0x13823f8f, /// 0x1324
		 0xb521a37e, /// 0x1328
		 0x86006a3e, /// 0x132c
		 0xe502d117, /// 0x1330
		 0x398a1e42, /// 0x1334
		 0xf5f78b88, /// 0x1338
		 0x697a3812, /// 0x133c
		 0x9bb7ff9d, /// 0x1340
		 0xb675a07e, /// 0x1344
		 0x376f25cf, /// 0x1348
		 0x29cbc765, /// 0x134c
		 0x0dacfad2, /// 0x1350
		 0x6131c5b0, /// 0x1354
		 0xa57c5cc0, /// 0x1358
		 0xef67d17f, /// 0x135c
		 0x37087dc5, /// 0x1360
		 0xe07b233e, /// 0x1364
		 0xbf10d4fc, /// 0x1368
		 0xacb407fb, /// 0x136c
		 0x9fd4f615, /// 0x1370
		 0x32e8dcc0, /// 0x1374
		 0x5bad5a1d, /// 0x1378
		 0xc432ee93, /// 0x137c
		 0xa95c3b6b, /// 0x1380
		 0x392259cb, /// 0x1384
		 0xf9d4373b, /// 0x1388
		 0xce9eb8f5, /// 0x138c
		 0xd7d12143, /// 0x1390
		 0xcd946782, /// 0x1394
		 0xd6f20ae8, /// 0x1398
		 0xaac623a9, /// 0x139c
		 0x594e6cfa, /// 0x13a0
		 0xe81a2cad, /// 0x13a4
		 0xa97ab5fa, /// 0x13a8
		 0x2a20761f, /// 0x13ac
		 0x2a25a508, /// 0x13b0
		 0xaf817e08, /// 0x13b4
		 0x5506c81a, /// 0x13b8
		 0x82b9ca1d, /// 0x13bc
		 0x00795f74, /// 0x13c0
		 0xec588cb0, /// 0x13c4
		 0x9ee9192a, /// 0x13c8
		 0x8f0dd14c, /// 0x13cc
		 0xfc5b4a5e, /// 0x13d0
		 0xa27023ca, /// 0x13d4
		 0x50794468, /// 0x13d8
		 0x4975ee0b, /// 0x13dc
		 0x5e2518c8, /// 0x13e0
		 0xf6f4ed47, /// 0x13e4
		 0xeb7c4657, /// 0x13e8
		 0x6fc05df3, /// 0x13ec
		 0x1ae77af0, /// 0x13f0
		 0xee6061cd, /// 0x13f4
		 0xe99c49ef, /// 0x13f8
		 0xe0d3e6fe, /// 0x13fc
		 0x5c8d4aea, /// 0x1400
		 0x89adbc69, /// 0x1404
		 0x8bb67e89, /// 0x1408
		 0x179f88b7, /// 0x140c
		 0xe2a1488f, /// 0x1410
		 0x94f774d2, /// 0x1414
		 0x23472ef7, /// 0x1418
		 0x8a628066, /// 0x141c
		 0x1989ca84, /// 0x1420
		 0x9dfe044f, /// 0x1424
		 0x71586c0b, /// 0x1428
		 0x47da2ef0, /// 0x142c
		 0x4a6113ed, /// 0x1430
		 0x67769cea, /// 0x1434
		 0x3cfe20d3, /// 0x1438
		 0xa72236a8, /// 0x143c
		 0x5c7f646a, /// 0x1440
		 0x1843bcef, /// 0x1444
		 0x65a2f48f, /// 0x1448
		 0x49795e98, /// 0x144c
		 0x43e2b43e, /// 0x1450
		 0xe31f732d, /// 0x1454
		 0x11e7b9a7, /// 0x1458
		 0x3afe4975, /// 0x145c
		 0x54a30b5c, /// 0x1460
		 0x0d22b2d4, /// 0x1464
		 0xd4170474, /// 0x1468
		 0x14e2c2fd, /// 0x146c
		 0xaefb2ae8, /// 0x1470
		 0x6eddbae9, /// 0x1474
		 0x86ceb225, /// 0x1478
		 0x48f11a71, /// 0x147c
		 0x729be25f, /// 0x1480
		 0x335ae2ee, /// 0x1484
		 0x2bae5304, /// 0x1488
		 0x638133f2, /// 0x148c
		 0xdfdde72c, /// 0x1490
		 0x01cce6ad, /// 0x1494
		 0x0519eaaf, /// 0x1498
		 0xaa4c1d0a, /// 0x149c
		 0x1f79fa40, /// 0x14a0
		 0xd9000e0f, /// 0x14a4
		 0x109e3554, /// 0x14a8
		 0xb733e724, /// 0x14ac
		 0x8ac85c1f, /// 0x14b0
		 0x15f5e820, /// 0x14b4
		 0x381b7e66, /// 0x14b8
		 0x4e44a7ab, /// 0x14bc
		 0x3d577ba9, /// 0x14c0
		 0xeab1bc74, /// 0x14c4
		 0xac41b794, /// 0x14c8
		 0xbbe6082e, /// 0x14cc
		 0x049cad97, /// 0x14d0
		 0xb8382885, /// 0x14d4
		 0xda725412, /// 0x14d8
		 0xe0c29391, /// 0x14dc
		 0xa1792554, /// 0x14e0
		 0xc155610f, /// 0x14e4
		 0x09f2d649, /// 0x14e8
		 0xd664f6ee, /// 0x14ec
		 0xaa12a109, /// 0x14f0
		 0x78fe399a, /// 0x14f4
		 0x5c86b498, /// 0x14f8
		 0xc02aefa5, /// 0x14fc
		 0xece4cea0, /// 0x1500
		 0x317ff6b4, /// 0x1504
		 0x1694c944, /// 0x1508
		 0xb91125cc, /// 0x150c
		 0xda88f059, /// 0x1510
		 0xbc9a9814, /// 0x1514
		 0x47d866fc, /// 0x1518
		 0x094f3458, /// 0x151c
		 0x8f420bfd, /// 0x1520
		 0xc12478dd, /// 0x1524
		 0x71325d58, /// 0x1528
		 0x2c151a4a, /// 0x152c
		 0x7d93e15e, /// 0x1530
		 0x332b91ce, /// 0x1534
		 0xc0d0ff28, /// 0x1538
		 0x76e9d3f5, /// 0x153c
		 0xe0db0220, /// 0x1540
		 0xa133d97e, /// 0x1544
		 0x402afad1, /// 0x1548
		 0x8d18e8fc, /// 0x154c
		 0xa3f8af65, /// 0x1550
		 0x2397f836, /// 0x1554
		 0xb445934d, /// 0x1558
		 0x0f20ff46, /// 0x155c
		 0x221dbceb, /// 0x1560
		 0x05cdb39f, /// 0x1564
		 0xf069e55a, /// 0x1568
		 0x15e05d11, /// 0x156c
		 0x073cb043, /// 0x1570
		 0xb192425b, /// 0x1574
		 0xb8054ed4, /// 0x1578
		 0x273d8d05, /// 0x157c
		 0x7b6114d0, /// 0x1580
		 0x0236b252, /// 0x1584
		 0xbfa3433f, /// 0x1588
		 0x12779595, /// 0x158c
		 0x556f14f6, /// 0x1590
		 0xb5591a0a, /// 0x1594
		 0x346a9e52, /// 0x1598
		 0x0eead215, /// 0x159c
		 0x50cd34ac, /// 0x15a0
		 0x2220be69, /// 0x15a4
		 0x7c160adc, /// 0x15a8
		 0x57c32cee, /// 0x15ac
		 0x4d1c5f82, /// 0x15b0
		 0xef80ada8, /// 0x15b4
		 0xc54dc34a, /// 0x15b8
		 0x21646543, /// 0x15bc
		 0xb92a9f73, /// 0x15c0
		 0x4260f2e0, /// 0x15c4
		 0x41f52fd9, /// 0x15c8
		 0x127e84e0, /// 0x15cc
		 0xfd905e65, /// 0x15d0
		 0x79049d0f, /// 0x15d4
		 0xbf789af7, /// 0x15d8
		 0x58bba9c1, /// 0x15dc
		 0x5c03ab4d, /// 0x15e0
		 0x0fb835d8, /// 0x15e4
		 0x314cc5fc, /// 0x15e8
		 0x5adc7a07, /// 0x15ec
		 0x5783f79a, /// 0x15f0
		 0x857f9eb7, /// 0x15f4
		 0x65df2f6a, /// 0x15f8
		 0x319937ba, /// 0x15fc
		 0x3e761617, /// 0x1600
		 0x7d3125ba, /// 0x1604
		 0xc730d669, /// 0x1608
		 0x23881f5a, /// 0x160c
		 0x5acdc255, /// 0x1610
		 0x0132e247, /// 0x1614
		 0xb2a70a30, /// 0x1618
		 0xdfa4eea5, /// 0x161c
		 0xd15c8844, /// 0x1620
		 0x954fc391, /// 0x1624
		 0x70acd8b6, /// 0x1628
		 0x321650b3, /// 0x162c
		 0x82065b71, /// 0x1630
		 0xe4da1eb4, /// 0x1634
		 0x90aa3972, /// 0x1638
		 0xeb218d8c, /// 0x163c
		 0xb78bde44, /// 0x1640
		 0xb0536195, /// 0x1644
		 0xd37aaca6, /// 0x1648
		 0x58bf0931, /// 0x164c
		 0x86120613, /// 0x1650
		 0x418a55ff, /// 0x1654
		 0x90cc6235, /// 0x1658
		 0xb0d3b28d, /// 0x165c
		 0x9954b16d, /// 0x1660
		 0x2bdc48b2, /// 0x1664
		 0x3f719440, /// 0x1668
		 0x5a19f30f, /// 0x166c
		 0xcf8f5f28, /// 0x1670
		 0x7975dbc2, /// 0x1674
		 0x202aef63, /// 0x1678
		 0xb7969903, /// 0x167c
		 0x8277b7a5, /// 0x1680
		 0x22ed0086, /// 0x1684
		 0xa835f7bc, /// 0x1688
		 0xd4a0bb85, /// 0x168c
		 0x47a211bb, /// 0x1690
		 0xd3c4afbd, /// 0x1694
		 0x05cac96a, /// 0x1698
		 0x04e0d07d, /// 0x169c
		 0xe27ca59a, /// 0x16a0
		 0x3ba6539e, /// 0x16a4
		 0x582f1de8, /// 0x16a8
		 0xe65d587a, /// 0x16ac
		 0xbb671734, /// 0x16b0
		 0xe1f0ed97, /// 0x16b4
		 0xc902ada3, /// 0x16b8
		 0xa00e4910, /// 0x16bc
		 0xbc9b3d5b, /// 0x16c0
		 0xfa484081, /// 0x16c4
		 0xc5ea53d3, /// 0x16c8
		 0xa812ed23, /// 0x16cc
		 0xdfa2f75c, /// 0x16d0
		 0x22d87e73, /// 0x16d4
		 0x2d2ad8c4, /// 0x16d8
		 0xa69fe363, /// 0x16dc
		 0x38ece234, /// 0x16e0
		 0xcd68763c, /// 0x16e4
		 0x71498595, /// 0x16e8
		 0xeb1ed765, /// 0x16ec
		 0xc3af7f43, /// 0x16f0
		 0xbcad0336, /// 0x16f4
		 0x9734cb25, /// 0x16f8
		 0xccb99333, /// 0x16fc
		 0x57d1e9e6, /// 0x1700
		 0x8cbe7297, /// 0x1704
		 0x30749790, /// 0x1708
		 0xb6a9f2b1, /// 0x170c
		 0x2b4f40d9, /// 0x1710
		 0x3f36f0b9, /// 0x1714
		 0xca54263e, /// 0x1718
		 0x144e52cd, /// 0x171c
		 0x4fbee000, /// 0x1720
		 0xe2e4a3e6, /// 0x1724
		 0x23013c18, /// 0x1728
		 0x75b45f00, /// 0x172c
		 0xbd685c0c, /// 0x1730
		 0x1197f921, /// 0x1734
		 0xb04a885b, /// 0x1738
		 0x465e03e9, /// 0x173c
		 0xb1af63fd, /// 0x1740
		 0x925826b1, /// 0x1744
		 0xacc9225d, /// 0x1748
		 0x33c2e869, /// 0x174c
		 0x3cc6017d, /// 0x1750
		 0x31b37835, /// 0x1754
		 0xa5007f3b, /// 0x1758
		 0x9a9a1215, /// 0x175c
		 0xef8f101e, /// 0x1760
		 0x43e7429f, /// 0x1764
		 0xc76ae06f, /// 0x1768
		 0xf09ca482, /// 0x176c
		 0x025a51cf, /// 0x1770
		 0x7fb4a04d, /// 0x1774
		 0x77278d94, /// 0x1778
		 0xf01b9ed2, /// 0x177c
		 0x43143742, /// 0x1780
		 0x038354bf, /// 0x1784
		 0xbc213fa6, /// 0x1788
		 0x464382f7, /// 0x178c
		 0x2f8a7669, /// 0x1790
		 0xf7227f59, /// 0x1794
		 0x98a9ff4a, /// 0x1798
		 0xe615385b, /// 0x179c
		 0x6ae14f35, /// 0x17a0
		 0x9334e142, /// 0x17a4
		 0x7171a1ad, /// 0x17a8
		 0xfdd9e75a, /// 0x17ac
		 0x1e3b81a9, /// 0x17b0
		 0x133b7c58, /// 0x17b4
		 0x38f971c3, /// 0x17b8
		 0x70a42860, /// 0x17bc
		 0x6e9daa66, /// 0x17c0
		 0x9a5eb28e, /// 0x17c4
		 0xcf2d4a7f, /// 0x17c8
		 0x14045107, /// 0x17cc
		 0x7b9aade4, /// 0x17d0
		 0x14b3d5a9, /// 0x17d4
		 0xe9fae3d4, /// 0x17d8
		 0x362db462, /// 0x17dc
		 0x03462169, /// 0x17e0
		 0xeaf27019, /// 0x17e4
		 0xe5ab13a3, /// 0x17e8
		 0x3d245b4e, /// 0x17ec
		 0xe57f1e53, /// 0x17f0
		 0x62fc6f0f, /// 0x17f4
		 0x31d3e978, /// 0x17f8
		 0x5d1ed594, /// 0x17fc
		 0x4cd49d3c, /// 0x1800
		 0x1c560622, /// 0x1804
		 0xcbcef404, /// 0x1808
		 0x132d8b27, /// 0x180c
		 0xd077da98, /// 0x1810
		 0x4652a608, /// 0x1814
		 0x0c013b57, /// 0x1818
		 0xaeb71745, /// 0x181c
		 0x69265fbe, /// 0x1820
		 0xea18d852, /// 0x1824
		 0x7eea27dc, /// 0x1828
		 0x2272b7d4, /// 0x182c
		 0xca48c8ff, /// 0x1830
		 0x4d6f6f99, /// 0x1834
		 0x0695dcff, /// 0x1838
		 0x89415f7b, /// 0x183c
		 0xd3ad9a0b, /// 0x1840
		 0x881352af, /// 0x1844
		 0x8f4b7472, /// 0x1848
		 0x30ae8a7b, /// 0x184c
		 0x3bd4240f, /// 0x1850
		 0xf9ae1759, /// 0x1854
		 0x6853f82b, /// 0x1858
		 0xdee61599, /// 0x185c
		 0x8b8ba43f, /// 0x1860
		 0xbfb02419, /// 0x1864
		 0x4e58a47f, /// 0x1868
		 0x506f6d0c, /// 0x186c
		 0xffdf700e, /// 0x1870
		 0xb083c8d3, /// 0x1874
		 0xfcc17938, /// 0x1878
		 0x098d7faf, /// 0x187c
		 0xf7d11a39, /// 0x1880
		 0x61539c31, /// 0x1884
		 0xbb67f36f, /// 0x1888
		 0x68611a97, /// 0x188c
		 0xbab765ff, /// 0x1890
		 0x73da3483, /// 0x1894
		 0x93c65600, /// 0x1898
		 0xeabae006, /// 0x189c
		 0x8ca0f614, /// 0x18a0
		 0x47c3d357, /// 0x18a4
		 0xcc5a0d70, /// 0x18a8
		 0x896b1304, /// 0x18ac
		 0x548fa4fb, /// 0x18b0
		 0x002d96da, /// 0x18b4
		 0x63513779, /// 0x18b8
		 0xfdb41424, /// 0x18bc
		 0xec2f6393, /// 0x18c0
		 0x14b791ca, /// 0x18c4
		 0xeb13aee5, /// 0x18c8
		 0xe689cf27, /// 0x18cc
		 0x3e8567b3, /// 0x18d0
		 0x487efb8c, /// 0x18d4
		 0xa061fffa, /// 0x18d8
		 0xbb230115, /// 0x18dc
		 0xef265739, /// 0x18e0
		 0x6b8ce1f1, /// 0x18e4
		 0xebda91d8, /// 0x18e8
		 0x43489494, /// 0x18ec
		 0xae83c70d, /// 0x18f0
		 0x23357528, /// 0x18f4
		 0x1787e827, /// 0x18f8
		 0xc4f30222, /// 0x18fc
		 0xf8f1ecb6, /// 0x1900
		 0xe0d5ec7d, /// 0x1904
		 0x67b6599f, /// 0x1908
		 0xd992715d, /// 0x190c
		 0x0dc8b779, /// 0x1910
		 0xaf3da458, /// 0x1914
		 0x1baec584, /// 0x1918
		 0x1a04827e, /// 0x191c
		 0xb19b739e, /// 0x1920
		 0x164d7b09, /// 0x1924
		 0x354be2d2, /// 0x1928
		 0xf0a1bf27, /// 0x192c
		 0xfc6dffbf, /// 0x1930
		 0xd7754b77, /// 0x1934
		 0x73a42f15, /// 0x1938
		 0xbddcb526, /// 0x193c
		 0xf6442cfd, /// 0x1940
		 0x3fcf7c9f, /// 0x1944
		 0x34eeb6b7, /// 0x1948
		 0x70f0ac24, /// 0x194c
		 0xdae99f3a, /// 0x1950
		 0xba15ffdd, /// 0x1954
		 0x0555003f, /// 0x1958
		 0xa2c052b5, /// 0x195c
		 0xc8b3505f, /// 0x1960
		 0x768309d7, /// 0x1964
		 0x148a4c99, /// 0x1968
		 0xfa93bbb6, /// 0x196c
		 0xba9fa508, /// 0x1970
		 0x5cfc4969, /// 0x1974
		 0xa3d8f18e, /// 0x1978
		 0xa0b1b8f6, /// 0x197c
		 0x8deeb5a0, /// 0x1980
		 0x231bd736, /// 0x1984
		 0x8d5d9874, /// 0x1988
		 0x2e66ddc0, /// 0x198c
		 0x11d2bb6e, /// 0x1990
		 0xe82cde2d, /// 0x1994
		 0x16a03a12, /// 0x1998
		 0x4d0e3216, /// 0x199c
		 0x0d6c8313, /// 0x19a0
		 0x0db3a7a5, /// 0x19a4
		 0x39cfca7d, /// 0x19a8
		 0xca51c802, /// 0x19ac
		 0x78b7c7f9, /// 0x19b0
		 0x42b77e64, /// 0x19b4
		 0x2bee7ac4, /// 0x19b8
		 0x4fc34558, /// 0x19bc
		 0x3232c1f3, /// 0x19c0
		 0x397a17c6, /// 0x19c4
		 0xf4b61f1e, /// 0x19c8
		 0x01c3f9ec, /// 0x19cc
		 0x91043adc, /// 0x19d0
		 0xb9598487, /// 0x19d4
		 0x2f8091c2, /// 0x19d8
		 0x8d630d0a, /// 0x19dc
		 0x068a1222, /// 0x19e0
		 0x8860c215, /// 0x19e4
		 0x5e38465f, /// 0x19e8
		 0x6a5d8362, /// 0x19ec
		 0x2e22469c, /// 0x19f0
		 0x467ab42c, /// 0x19f4
		 0xff248deb, /// 0x19f8
		 0xe24c0eac, /// 0x19fc
		 0x81ebaa2c, /// 0x1a00
		 0xc9a8616a, /// 0x1a04
		 0xc6a69b09, /// 0x1a08
		 0x7dc85ec8, /// 0x1a0c
		 0x2ea46586, /// 0x1a10
		 0x741ee299, /// 0x1a14
		 0x4e853b64, /// 0x1a18
		 0x37e1a9f9, /// 0x1a1c
		 0x2890e72e, /// 0x1a20
		 0xfc164c99, /// 0x1a24
		 0x880e68c8, /// 0x1a28
		 0xcb10f60b, /// 0x1a2c
		 0xa1f93118, /// 0x1a30
		 0xb8144e39, /// 0x1a34
		 0x18b983f6, /// 0x1a38
		 0xe20f6d4b, /// 0x1a3c
		 0x95a8efc1, /// 0x1a40
		 0xb95958ca, /// 0x1a44
		 0xbedd8494, /// 0x1a48
		 0xdf2d1e4f, /// 0x1a4c
		 0xca67a593, /// 0x1a50
		 0x63b5161e, /// 0x1a54
		 0xfce6a0b6, /// 0x1a58
		 0x339ba46e, /// 0x1a5c
		 0x9d1cb25d, /// 0x1a60
		 0x59bb62ef, /// 0x1a64
		 0xde1b7fed, /// 0x1a68
		 0xf95a7bc3, /// 0x1a6c
		 0x23dd0bed, /// 0x1a70
		 0x70931e0e, /// 0x1a74
		 0x703f8a60, /// 0x1a78
		 0xe28dc9f8, /// 0x1a7c
		 0xe95630b5, /// 0x1a80
		 0x49d0d900, /// 0x1a84
		 0x77c2caf3, /// 0x1a88
		 0x89bdd894, /// 0x1a8c
		 0x29f10174, /// 0x1a90
		 0x630981a6, /// 0x1a94
		 0xf01dc2c7, /// 0x1a98
		 0x8396dcf6, /// 0x1a9c
		 0x874b81df, /// 0x1aa0
		 0xb244c87b, /// 0x1aa4
		 0x474d1601, /// 0x1aa8
		 0x7c8f049c, /// 0x1aac
		 0x2bd2ea5e, /// 0x1ab0
		 0x03efcaa2, /// 0x1ab4
		 0x86cd6721, /// 0x1ab8
		 0xd5b56440, /// 0x1abc
		 0x8c5763a0, /// 0x1ac0
		 0x9313c426, /// 0x1ac4
		 0x1be216b8, /// 0x1ac8
		 0x93053e59, /// 0x1acc
		 0xca00cbfa, /// 0x1ad0
		 0xcbf36c5c, /// 0x1ad4
		 0x00a7ce60, /// 0x1ad8
		 0x078ee25e, /// 0x1adc
		 0x02f0b847, /// 0x1ae0
		 0xa2f9f059, /// 0x1ae4
		 0xf693c138, /// 0x1ae8
		 0xccfe0963, /// 0x1aec
		 0xd67b29eb, /// 0x1af0
		 0xcde2f926, /// 0x1af4
		 0x1baa1377, /// 0x1af8
		 0xa0e4606b, /// 0x1afc
		 0x3209ef94, /// 0x1b00
		 0xe5d75b6e, /// 0x1b04
		 0x8f63f97c, /// 0x1b08
		 0x02a19508, /// 0x1b0c
		 0x4627e43a, /// 0x1b10
		 0xaa347a3b, /// 0x1b14
		 0x5ce2e66d, /// 0x1b18
		 0x555bfc2c, /// 0x1b1c
		 0x050ff90e, /// 0x1b20
		 0x87ada9b5, /// 0x1b24
		 0x301a9dbf, /// 0x1b28
		 0x8e897e50, /// 0x1b2c
		 0x29c84a9c, /// 0x1b30
		 0xbe69f939, /// 0x1b34
		 0x81c28a48, /// 0x1b38
		 0xb2ebbbbc, /// 0x1b3c
		 0xce3e9df6, /// 0x1b40
		 0x1d05bba7, /// 0x1b44
		 0x0d5e6289, /// 0x1b48
		 0xbae5f2f1, /// 0x1b4c
		 0xa409db24, /// 0x1b50
		 0x0fcd7f25, /// 0x1b54
		 0xa5116325, /// 0x1b58
		 0x18fc4a69, /// 0x1b5c
		 0xc66015fe, /// 0x1b60
		 0x4859f1dd, /// 0x1b64
		 0x25b47264, /// 0x1b68
		 0x4feeba8d, /// 0x1b6c
		 0x4df23d12, /// 0x1b70
		 0xfb1dfac4, /// 0x1b74
		 0x8a217843, /// 0x1b78
		 0xaa4468c2, /// 0x1b7c
		 0x02d87064, /// 0x1b80
		 0x60c06025, /// 0x1b84
		 0xd6064fa5, /// 0x1b88
		 0xa2f48d0a, /// 0x1b8c
		 0x131016fd, /// 0x1b90
		 0xdd706d23, /// 0x1b94
		 0x9e6b3949, /// 0x1b98
		 0x8d5112d2, /// 0x1b9c
		 0xed38c97f, /// 0x1ba0
		 0x3690bb73, /// 0x1ba4
		 0xd53f7659, /// 0x1ba8
		 0x9af26eb8, /// 0x1bac
		 0x3a0808eb, /// 0x1bb0
		 0x9e45c426, /// 0x1bb4
		 0xe6750713, /// 0x1bb8
		 0x732b15ae, /// 0x1bbc
		 0xe028da32, /// 0x1bc0
		 0x1c10bc42, /// 0x1bc4
		 0x1fa993b4, /// 0x1bc8
		 0xcb166ceb, /// 0x1bcc
		 0x5f8788bb, /// 0x1bd0
		 0xcf634360, /// 0x1bd4
		 0x4bf06cb1, /// 0x1bd8
		 0x7dd64c24, /// 0x1bdc
		 0x03104fc2, /// 0x1be0
		 0xed1b8a7b, /// 0x1be4
		 0x5b6eaa5f, /// 0x1be8
		 0x1802e530, /// 0x1bec
		 0xb65aeb17, /// 0x1bf0
		 0xff431236, /// 0x1bf4
		 0xbaa8cd80, /// 0x1bf8
		 0x850aad62, /// 0x1bfc
		 0x4c2b0288, /// 0x1c00
		 0xf7da28be, /// 0x1c04
		 0x84eaf811, /// 0x1c08
		 0x211e8ef5, /// 0x1c0c
		 0x955d9ea6, /// 0x1c10
		 0xde63c18e, /// 0x1c14
		 0x2365033c, /// 0x1c18
		 0xa8fe7957, /// 0x1c1c
		 0x5df14f6e, /// 0x1c20
		 0x6a440696, /// 0x1c24
		 0x5cea5854, /// 0x1c28
		 0xd93ad738, /// 0x1c2c
		 0xa27748e5, /// 0x1c30
		 0x59eac3f8, /// 0x1c34
		 0x42eaaf0e, /// 0x1c38
		 0x1a1c740b, /// 0x1c3c
		 0x66e3e2a4, /// 0x1c40
		 0x66cf5611, /// 0x1c44
		 0x127ce22f, /// 0x1c48
		 0x9a776fb4, /// 0x1c4c
		 0x40be33c8, /// 0x1c50
		 0x9f316288, /// 0x1c54
		 0xe3747bb3, /// 0x1c58
		 0x8be72716, /// 0x1c5c
		 0x7262a01a, /// 0x1c60
		 0x2447ac76, /// 0x1c64
		 0x635dff54, /// 0x1c68
		 0x6f2bde19, /// 0x1c6c
		 0x0e2d53ac, /// 0x1c70
		 0x3732ccc3, /// 0x1c74
		 0x91eaa986, /// 0x1c78
		 0x86d00768, /// 0x1c7c
		 0x0ea18ec2, /// 0x1c80
		 0x814eb694, /// 0x1c84
		 0xf714c8f5, /// 0x1c88
		 0x9c53c4d1, /// 0x1c8c
		 0x5c08a1f2, /// 0x1c90
		 0xbb513648, /// 0x1c94
		 0xdb8d0731, /// 0x1c98
		 0xc5a868fc, /// 0x1c9c
		 0x2cdf2a30, /// 0x1ca0
		 0x303afc2a, /// 0x1ca4
		 0x36e5acab, /// 0x1ca8
		 0x24f72c1e, /// 0x1cac
		 0xadce35b4, /// 0x1cb0
		 0xdda55fcc, /// 0x1cb4
		 0x39f7cc89, /// 0x1cb8
		 0x1b61414e, /// 0x1cbc
		 0x6d635617, /// 0x1cc0
		 0x1eeff1f0, /// 0x1cc4
		 0x4d18202f, /// 0x1cc8
		 0xa12ff7ad, /// 0x1ccc
		 0xe6628d56, /// 0x1cd0
		 0x6ee46776, /// 0x1cd4
		 0x55830271, /// 0x1cd8
		 0x6c0d0f2f, /// 0x1cdc
		 0x5e8fd0b6, /// 0x1ce0
		 0x3a19eaee, /// 0x1ce4
		 0x0be166d9, /// 0x1ce8
		 0xd422fc89, /// 0x1cec
		 0x913a1037, /// 0x1cf0
		 0x5773a68a, /// 0x1cf4
		 0x2aa1499c, /// 0x1cf8
		 0xf8cd53a6, /// 0x1cfc
		 0xd99c742d, /// 0x1d00
		 0x2df89016, /// 0x1d04
		 0x4fd9f40c, /// 0x1d08
		 0xd4208b33, /// 0x1d0c
		 0x59a2fc7c, /// 0x1d10
		 0xe4de652d, /// 0x1d14
		 0xe2e95952, /// 0x1d18
		 0xe0c20a81, /// 0x1d1c
		 0xfc71e948, /// 0x1d20
		 0xd40ef1e9, /// 0x1d24
		 0xc5debae7, /// 0x1d28
		 0x7263ff3b, /// 0x1d2c
		 0x3cd6ed9d, /// 0x1d30
		 0x8054818c, /// 0x1d34
		 0x4908661d, /// 0x1d38
		 0x54e016c1, /// 0x1d3c
		 0xa7331105, /// 0x1d40
		 0x4cfb0a60, /// 0x1d44
		 0xeb5d0b38, /// 0x1d48
		 0xf580be4b, /// 0x1d4c
		 0x270055b0, /// 0x1d50
		 0xcb106be5, /// 0x1d54
		 0xb5e95b69, /// 0x1d58
		 0x3669f788, /// 0x1d5c
		 0x71303fad, /// 0x1d60
		 0xe19b70c3, /// 0x1d64
		 0x01e6e879, /// 0x1d68
		 0x31c383bf, /// 0x1d6c
		 0x55f917be, /// 0x1d70
		 0xebf32a32, /// 0x1d74
		 0xa101a83a, /// 0x1d78
		 0xaa4fd4c3, /// 0x1d7c
		 0x989d74ec, /// 0x1d80
		 0xfbac15fa, /// 0x1d84
		 0xa0b14ef6, /// 0x1d88
		 0xa20444b6, /// 0x1d8c
		 0x68133898, /// 0x1d90
		 0x8a346134, /// 0x1d94
		 0x16bc7ea9, /// 0x1d98
		 0x5c98217b, /// 0x1d9c
		 0x045099f0, /// 0x1da0
		 0xa55dc8fd, /// 0x1da4
		 0xf084fe87, /// 0x1da8
		 0x5be479f4, /// 0x1dac
		 0x9607bda3, /// 0x1db0
		 0x1d511a30, /// 0x1db4
		 0x7529c7d2, /// 0x1db8
		 0x17aaddf2, /// 0x1dbc
		 0x9d1a1b4c, /// 0x1dc0
		 0xdaece4b7, /// 0x1dc4
		 0x4de1d01b, /// 0x1dc8
		 0xe75c0b4a, /// 0x1dcc
		 0xdc4c2ee9, /// 0x1dd0
		 0x8f37393f, /// 0x1dd4
		 0x883fab3e, /// 0x1dd8
		 0xd09ee9a2, /// 0x1ddc
		 0x57a2aa52, /// 0x1de0
		 0x3498bd69, /// 0x1de4
		 0x4a256eaa, /// 0x1de8
		 0xe6ddeb29, /// 0x1dec
		 0x6881ece3, /// 0x1df0
		 0x61ab66a1, /// 0x1df4
		 0x65ccbd7c, /// 0x1df8
		 0x30000eff, /// 0x1dfc
		 0x165584ca, /// 0x1e00
		 0x0c8df9a3, /// 0x1e04
		 0x2712da27, /// 0x1e08
		 0x0a9adb8a, /// 0x1e0c
		 0x5c7f2fc3, /// 0x1e10
		 0x8fccdcda, /// 0x1e14
		 0x27645b9b, /// 0x1e18
		 0x23ae38d4, /// 0x1e1c
		 0x84637166, /// 0x1e20
		 0x9d636e32, /// 0x1e24
		 0x980c3bfa, /// 0x1e28
		 0x9920b6b6, /// 0x1e2c
		 0x4a72fc2a, /// 0x1e30
		 0xde881b4f, /// 0x1e34
		 0x6fc8d7a0, /// 0x1e38
		 0x749b0f77, /// 0x1e3c
		 0x4f4d6cc7, /// 0x1e40
		 0xddd4a930, /// 0x1e44
		 0x4244b0fb, /// 0x1e48
		 0x38300273, /// 0x1e4c
		 0x920493c4, /// 0x1e50
		 0xe2e018fa, /// 0x1e54
		 0x06912797, /// 0x1e58
		 0x533690ab, /// 0x1e5c
		 0x7bd455d4, /// 0x1e60
		 0x03ca3c71, /// 0x1e64
		 0x10713ec1, /// 0x1e68
		 0x3fcd4850, /// 0x1e6c
		 0x60363d5c, /// 0x1e70
		 0xd6239940, /// 0x1e74
		 0x385e10ad, /// 0x1e78
		 0x36dc7fc1, /// 0x1e7c
		 0xb6cb239b, /// 0x1e80
		 0xdaf66c5b, /// 0x1e84
		 0xb8edc56e, /// 0x1e88
		 0x2727d1bc, /// 0x1e8c
		 0x45f61c00, /// 0x1e90
		 0x319d9909, /// 0x1e94
		 0x3284888e, /// 0x1e98
		 0x4140b6d5, /// 0x1e9c
		 0x3e778df7, /// 0x1ea0
		 0xcbbdb199, /// 0x1ea4
		 0x0499eb9b, /// 0x1ea8
		 0x4bc7b3f5, /// 0x1eac
		 0x0a5428cb, /// 0x1eb0
		 0xf7a66d68, /// 0x1eb4
		 0x89b4c1a0, /// 0x1eb8
		 0x2277506f, /// 0x1ebc
		 0xf82ae2f7, /// 0x1ec0
		 0x282d0218, /// 0x1ec4
		 0xcb7e19ff, /// 0x1ec8
		 0x9343cbc0, /// 0x1ecc
		 0xce3738b9, /// 0x1ed0
		 0xf91bc66d, /// 0x1ed4
		 0xba94b919, /// 0x1ed8
		 0xa25823d0, /// 0x1edc
		 0xb189798a, /// 0x1ee0
		 0x86320bdd, /// 0x1ee4
		 0x5fc080b1, /// 0x1ee8
		 0x989da94a, /// 0x1eec
		 0x38fb1117, /// 0x1ef0
		 0x70f796e4, /// 0x1ef4
		 0xe6c6f3e5, /// 0x1ef8
		 0xe77d520e, /// 0x1efc
		 0xd119d399, /// 0x1f00
		 0x60228701, /// 0x1f04
		 0x2121a550, /// 0x1f08
		 0xc902f502, /// 0x1f0c
		 0x779aee8b, /// 0x1f10
		 0x6f642cbe, /// 0x1f14
		 0x9b8dc820, /// 0x1f18
		 0x0f29a0c9, /// 0x1f1c
		 0xee056f37, /// 0x1f20
		 0x01f6d595, /// 0x1f24
		 0xe0fd23da, /// 0x1f28
		 0x5bc44c00, /// 0x1f2c
		 0x4188f302, /// 0x1f30
		 0x3e075234, /// 0x1f34
		 0xfd652fb1, /// 0x1f38
		 0x40a08809, /// 0x1f3c
		 0x11ef35d6, /// 0x1f40
		 0x22e2b87d, /// 0x1f44
		 0xd9ef9592, /// 0x1f48
		 0x8ad23c8b, /// 0x1f4c
		 0xa0b023b7, /// 0x1f50
		 0x8ca251ca, /// 0x1f54
		 0xcd1dfcc4, /// 0x1f58
		 0xc31b329c, /// 0x1f5c
		 0xe1a9fd05, /// 0x1f60
		 0xb214cc04, /// 0x1f64
		 0xe98742f1, /// 0x1f68
		 0x15b8a156, /// 0x1f6c
		 0x2c437d22, /// 0x1f70
		 0xcd9f73e8, /// 0x1f74
		 0xe2cd6142, /// 0x1f78
		 0xb56810e0, /// 0x1f7c
		 0x9a604592, /// 0x1f80
		 0x356a2834, /// 0x1f84
		 0x99b8f6d4, /// 0x1f88
		 0xb8e2d1b0, /// 0x1f8c
		 0xfb1e5054, /// 0x1f90
		 0xeec3c54c, /// 0x1f94
		 0x3f9521b6, /// 0x1f98
		 0x55f36e2f, /// 0x1f9c
		 0x74bcf531, /// 0x1fa0
		 0xb36f628c, /// 0x1fa4
		 0xc1d601eb, /// 0x1fa8
		 0x4e322903, /// 0x1fac
		 0x30be0d85, /// 0x1fb0
		 0xd5b465ef, /// 0x1fb4
		 0xba5491ee, /// 0x1fb8
		 0x746a9e11, /// 0x1fbc
		 0x136464a9, /// 0x1fc0
		 0x9590daea, /// 0x1fc4
		 0xeeade23d, /// 0x1fc8
		 0xce79140c, /// 0x1fcc
		 0x6356a916, /// 0x1fd0
		 0x5a6802e9, /// 0x1fd4
		 0xc6da5254, /// 0x1fd8
		 0xae7e10f7, /// 0x1fdc
		 0x9b02343e, /// 0x1fe0
		 0x6bac1291, /// 0x1fe4
		 0x8471bb12, /// 0x1fe8
		 0xbc590a48, /// 0x1fec
		 0x38ab8a9e, /// 0x1ff0
		 0x1c8de49d, /// 0x1ff4
		 0x636f0e3d, /// 0x1ff8
		 0x865e9361 /// last
};

/// Macros

#define VSNIP_RSV           : : [m_11] "r" (m_11), [m_op] "r" (m_op) : "x3", "x4", "x11", "x12", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"





int main() {

	uint64_t mid, tid;
	
	mid = get_minion_id();
	tid = get_thread_id();

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
			"flw.ps f0,  0 (%[m_op])\n"
			"flw.ps f1,  32 (%[m_op])\n"
			"flw.ps f2,  64 (%[m_op])\n"
			"flw.ps f3,  96 (%[m_op])\n"
			"flw.ps f4,  128 (%[m_op])\n"
			"flw.ps f5,  160 (%[m_op])\n"
			"flw.ps f6,  192 (%[m_op])\n"
			"flw.ps f7,  224 (%[m_op])\n"
			"flw.ps f8,  256 (%[m_op])\n"
			"flw.ps f9,  288 (%[m_op])\n"
			"flw.ps f10,  320 (%[m_op])\n"
			"flw.ps f11,  352 (%[m_op])\n"
			"flw.ps f12,  384 (%[m_op])\n"
			"flw.ps f13,  416 (%[m_op])\n"
			"flw.ps f14,  448 (%[m_op])\n"
			"flw.ps f15,  480 (%[m_op])\n"
			"flw.ps f16,  512 (%[m_op])\n"
			"flw.ps f17,  544 (%[m_op])\n"
			"flw.ps f18,  576 (%[m_op])\n"
			"flw.ps f19,  608 (%[m_op])\n"
			"flw.ps f20,  640 (%[m_op])\n"
			"flw.ps f21,  672 (%[m_op])\n"
			"flw.ps f22,  704 (%[m_op])\n"
			"flw.ps f23,  736 (%[m_op])\n"
			"flw.ps f24,  768 (%[m_op])\n"
			"flw.ps f25,  800 (%[m_op])\n"
			"flw.ps f26,  832 (%[m_op])\n"
			"flw.ps f27,  864 (%[m_op])\n"
			"flw.ps f28,  896 (%[m_op])\n"
			"flw.ps f29,  928 (%[m_op])\n"
			"flw.ps f30,  960 (%[m_op])\n"
			"flw.ps f31,  992 (%[m_op])\n"
			"fence\n"
			VSNIP_RSV
		);
		__asm__ __volatile__ (

		"LBL_VPU_SEQ_START_0:\n"
			"fle.ps f8, f7, f6\n"                                 
			"csrr x5, fcsr\n"
			"fscsr x0\n"

			"li x5, 0xffffffffffffffff\n"
			"mova.m.x x5\n"

			"fmaxu.pi f6, f0, f5\n"                               
			"fnmsub.s f4, f4, f2, f4, rmm\n"                      
			//RTLMIN-5479"frsq.ps f9, f2\n"                                    
			"fmax.ps f3, f1, f2\n"                                
			"maskand m0, m2, m2\n"                                
			"fcvt.s.w f2, x11, rdn\n"                             
			"fltm.pi m1, f2, f2\n"                                
			"fmsub.ps f9, f5, f3, f4, rup\n"                      
			"fmv.x.s x4, f5\n"                                    
			"fsgnjx.ps f2, f4, f2\n"                              
			"fcvt.ps.f16 f5, f7\n"                                
			"fmul.ps f6, f6, f0, dyn\n"                           
			"fmulh.pi f2, f1, f0\n"                               
			"fsetm.pi m1, f7\n"                                   
			"fsq2 f4, 189(x11)\n"                                 
			"fadd.s f4, f2, f0, rmm\n"                            
			"flog.ps f4, f5\n"                                    
			"fsrl.pi f6, f0, f4\n"                                
			"fcvt.w.s x4, f4, dyn\n"                              
			"fcvt.pwu.ps f3, f5, rne\n"                           
			"fcvt.ps.pwu f9, f4, rmm\n"                           
			"fmadd.ps f5, f3, f2, f2, rmm\n"                      
			"fsra.pi f6, f6, f3\n"                                
			"fsub.s f4, f2, f6, rdn\n"                            
			"fclass.ps f7, f0\n"                                  
			"fmv.x.s x4, f2\n"                                    
			"maskxor m2, m1, m2\n"                                
			"fbci.ps f9, 502172\n"                                
			"fmadd.ps f8, f0, f3, f5, rdn\n"                      
			"frcp.ps f8, f0\n"                                    
			"fcvt.wu.s x4, f5, rup\n"                             
			"fminu.pi f7, f4, f6\n"                               
			"fsgnjx.ps f8, f7, f0\n"                              
			"fcvt.s.w f9, x11, dyn\n"                             
			"fand.pi f8, f2, f7\n"                                
			"flt.s x4, f1, f1\n"                                  
			"fcvt.ps.f16 f3, f1\n"                                
			"fnmsub.ps f3, f3, f5, f5, rmm\n"                     
			"fround.ps f9, f5, rdn\n"                             
			"fsat8.pi f9, f7\n"                                   
			"fltm.ps m2, f2, f1\n"                                
			"maskand m2, m0, m1\n"                                
			"fcvt.pw.ps f9, f1, dyn\n"                            
			"flog.ps f6, f0\n"                                    
			"faddi.pi f2, f0, 312\n"                              
			"feq.pi f9, f7, f7\n"                                 
			"flt.ps f5, f5, f4\n"                                 
			"fandi.pi f8, f5, 337\n"                              
			"fclass.ps f3, f0\n"                                  
			"fsatu8.pi f7, f1\n"                                  
			"fmul.pi f3, f0, f6\n"                                
			"fmv.x.s x4, f1\n"                                    
			"fsub.ps f6, f4, f4, rtz\n"                           
			//RTLMIN-5479"frsq.ps f2, f7\n"                                    
			"fminu.pi f3, f5, f6\n"                               
			"fmul.ps f4, f6, f6, rne\n"                           
			"fround.ps f6, f4, dyn\n"                             
			"fsgnj.ps f3, f1, f5\n"                               
			"fcvt.pwu.ps f2, f3, rup\n"                           
			"fcvt.pw.ps f3, f6, rup\n"                            
			"maskor m0, m0, m2\n"                                 
			"fsat8.pi f3, f0\n"                                   
			"fmul.ps f8, f4, f0, dyn\n"                           
			"fmax.s f5, f4, f3\n"                                 
			"fcvt.ps.pw f3, f5, rtz\n"                            
			"fsat8.pi f2, f4\n"                                   
			"fcvt.wu.s x3, f5, rmm\n"                             
			"fbc.ps f7, 1318(x11)\n"                              
			"fswizz.ps f6, f4, 36\n"                              
			"flt.ps f3, f7, f0\n"                                 
			"fsat8.pi f9, f4\n"                                   
			"fltm.pi m2, f6, f2\n"                                
			"fbci.ps f7, 55346\n"                                 
			"flq2 f7, 908(x11)\n"                                 
			"fmsub.ps f3, f6, f6, f7, rtz\n"                      
			"fsat8.pi f2, f0\n"                                   
			"flw f6, -2028(x11)\n"                                
			"fsub.pi f2, f0, f4\n"                                
			"fmv.s.x f8, x11\n"                                   
			"fmin.s f5, f4, f1\n"                                 
			"fsub.pi f2, f5, f4\n"                                
			"flt.ps f6, f2, f0\n"                                 
			"fadd.ps f4, f0, f1, dyn\n"                           
			"fsgnjx.s f2, f2, f7\n"                               
			"fle.s x4, f5, f4\n"                                  
			"fmax.ps f9, f5, f4\n"                                
			//RTLMIN-5479"fsin.ps f8, f4\n"                                    
			"fcvt.s.w f6, x11, rne\n"                             
			"fmulhu.pi f4, f5, f7\n"                              
			"fsat8.pi f8, f1\n"                                   
			"fcvt.wu.s x4, f3, rmm\n"                             
			"flt.s x4, f1, f5\n"                                  
			"fandi.pi f6, f1, 92\n"                               
			"fnmadd.ps f4, f1, f7, f7, dyn\n"                     
			"fbc.ps f7, 113(x11)\n"                               
			"fmulh.pi f4, f2, f7\n"                               
			"fcmovm.ps f4, f4, f5\n"                              
			"fltu.pi f2, f3, f0\n"                                
			"fltm.pi m0, f5, f5\n"                                
			"fmax.pi f7, f6, f0\n"                                
			"fltm.ps m2, f3, f7\n"                                
			"flem.ps m2, f6, f2\n"                                
			"fmvs.x.ps x3, f3, 2\n"                               
			"fnmadd.s f2, f7, f2, f5, rdn\n"                      
			"frcp.ps f3, f0\n"                                    
			"fmsub.ps f9, f5, f2, f0, dyn\n"                      
			"mov.m.x m0, x11, 4\n"                                
			"fmsub.ps f8, f7, f0, f2, rne\n"                      
			"fsub.pi f2, f2, f6\n"                                
			"fcvt.ps.pwu f2, f1, rdn\n"                           
			"fmaxu.pi f3, f1, f1\n"                               
			"fmv.x.s x4, f4\n"                                    
			"frcp.ps f2, f3\n"                                    
			"feq.pi f3, f7, f1\n"                                 
			"ffrc.ps f6, f5\n"                                    
			"fnmadd.ps f8, f6, f4, f1, rmm\n"                     
			"fsgnjn.ps f9, f6, f0\n"                              
			"fsw.ps f5, -1921(x11)\n"                             

			"li x5, 0x506bfdc2eef9168a\n"
			"mova.m.x x5\n"                                       
			"fbcx.ps f8, x11\n"                                   
			"fltm.ps m2, f3, f4\n"                                
			"fbci.ps f3, 215057\n"                                
			"feq.s x3, f1, f6\n"                                  
			"fcvt.pwu.ps f3, f3, rup\n"                           
			"feq.pi f4, f2, f5\n"                                 
			"fnmsub.s f4, f5, f7, f6, rup\n"                      
			"fexp.ps f9, f7\n"                                    
			"fbcx.ps f6, x11\n"                                   

			"li x5, 0xfc419b216c0556\n"
			"mova.m.x x5\n"                                       
			"fandi.pi f9, f1, 256\n"                              
			"fsgnjn.ps f2, f0, f3\n"                              
			"fle.s x4, f6, f6\n"                                  
			"fmaxu.pi f6, f3, f1\n"                               
			"feqm.ps m1, f4, f0\n"                                
			"fcvt.pw.ps f6, f3, rmm\n"                            
			"fltu.pi f9, f2, f4\n"                                
			"fsub.s f4, f2, f3, dyn\n"                            
			"fsetm.pi m0, f0\n"                                   
			"fsgnj.s f9, f3, f5\n"                                
			"fcvt.pw.ps f4, f1, rne\n"                            
			"fsra.pi f9, f5, f3\n"                                
			"fandi.pi f5, f4, 452\n"                              
			"fnmsub.s f6, f4, f4, f3, rdn\n"                      
			"masknot m0, m0\n"                                    
			"fpackreph.pi f6, f1\n"                               
			"fsw f2, 585(x11)\n"                                  
			"fle.ps f6, f5, f5\n"                                 
			"fsq2 f1, 255(x11)\n"                                 
			"fsgnjx.s f2, f4, f3\n"                               
			"maskxor m1, m1, m2\n"                                
			"maskpopc x4, m1\n"                                   
			"maskor m1, m2, m0\n"                                 
			"fsgnjx.ps f2, f3, f3\n"                              
			"fadd.ps f7, f6, f3, rmm\n"                           
			"fsll.pi f7, f5, f6\n"                                
			"fltu.pi f7, f7, f4\n"                                
			"fmulh.pi f8, f7, f7\n"                               
			"flt.pi f8, f4, f0\n"                                 
			"fnmadd.s f5, f7, f4, f1, rne\n"                      
			"fsub.pi f6, f6, f4\n"                                
			"fadd.ps f8, f3, f7, rdn\n"                           
			"fsw f5, -678(x11)\n"                                 
			"fsw f1, -575(x11)\n"                                 
			"feq.s x4, f5, f2\n"                                  
			"fswizz.ps f9, f6, 78\n"                              
			"fltm.pi m1, f2, f7\n"                                
			"flt.s x3, f2, f7\n"                                  
			"fnmsub.ps f8, f5, f5, f5, rup\n"                     
			"fsw.ps f2, -509(x11)\n"                              
			"fsub.pi f6, f6, f7\n"                                
			"fsatu8.pi f9, f3\n"                                  
			"fbci.pi f9, 208261\n"                                
			"fsgnjn.s f2, f5, f0\n"                               
			"fnmsub.ps f7, f4, f0, f1, rne\n"                     
			"fand.pi f9, f2, f4\n"                                
			"fsgnjx.ps f9, f2, f7\n"                              
			"fltm.pi m2, f4, f7\n"                                
			"feq.ps f8, f0, f3\n"                                 

			"li x5, 0x86f499060684dd69\n"
			"mova.m.x x5\n"                                       
			"fsw.ps f5, 897(x11)\n"                               
			"fcvt.ps.f16 f6, f5\n"                                
			"fsra.pi f5, f1, f1\n"                                
			"fmvs.x.ps x4, f0, 5\n"                               
			"fmv.x.s x4, f3\n"                                    
			"masknot m1, m0\n"                                    
			"flt.s x4, f3, f1\n"                                  
			"fsrl.pi f6, f2, f3\n"                                
			"flt.pi f3, f3, f1\n"                                 
			"fmin.pi f8, f3, f7\n"                                
			"faddi.pi f4, f4, 105\n"                              
			"fmsub.s f4, f1, f7, f7, rne\n"                       
			"flt.pi f8, f1, f4\n"                                 
			"masknot m1, m2\n"                                    
			"fmvs.x.ps x4, f2, 5\n"                               
			"fmsub.ps f8, f5, f4, f2, rne\n"                      
			"masknot m0, m2\n"                                    
			"flt.s x4, f4, f4\n"                                  
			"fsat8.pi f3, f0\n"                                   
			"fmin.pi f5, f7, f5\n"                                
			"fbci.pi f2, 363004\n"                                
			"flog.ps f8, f2\n"                                    
			"fsgnjx.s f8, f4, f0\n"                               
			"flog.ps f6, f2\n"                                    
			"maskxor m0, m2, m0\n"                                
			"flq2 f2, -1161(x11)\n"                               
			"fmadd.ps f3, f1, f4, f7, rup\n"                      
			"fadd.pi f5, f0, f1\n"                                
			"flq2 f2, -1303(x11)\n"                               
			"maskand m2, m0, m0\n"                                
			"fround.ps f8, f2, rdn\n"                             
			"fsll.pi f7, f2, f7\n"                                
			"fsrli.pi f4, f5, 1\n"                                
			"faddi.pi f7, f0, 268\n"                              
			"fnmadd.s f5, f5, f0, f7, rne\n"                      
			"fclass.ps f2, f7\n"                                  
			"fle.pi f2, f0, f3\n"                                 
			"fmul.s f6, f5, f5, rdn\n"                            
			"feq.ps f6, f5, f5\n"                                 
			"fnmadd.ps f6, f4, f7, f1, rne\n"                     
			"fsrli.pi f3, f2, 15\n"                               
			"fmul.pi f3, f4, f0\n"                                
			"fmin.pi f4, f4, f7\n"                                
			"fbci.pi f6, 381662\n"                                
			"fmv.x.s x3, f3\n"                                    
			"fcvt.ps.pwu f4, f3, rtz\n"                           
			"fsrl.pi f7, f1, f0\n"                                
			//RTLMIN-5479"frsq.ps f5, f7\n"                                    
			"fsub.pi f4, f7, f4\n"                                
			"faddi.pi f6, f6, 501\n"                              
			"flt.s x3, f1, f4\n"                                  
			"fsrai.pi f8, f3, 3\n"                                
			"fand.pi f4, f3, f3\n"                                
			"fcvt.ps.f16 f3, f0\n"                                
			"fbc.ps f7, 617(x11)\n"                               
			"fcvt.s.w f3, x11, rup\n"                             
			"fsgnj.s f2, f4, f1\n"                                
			"fcvt.ps.pw f8, f0, rdn\n"                            
			"fnmadd.s f2, f5, f7, f3, rmm\n"                      
			"faddi.pi f6, f1, 117\n"                              
			"fmulh.pi f6, f5, f3\n"                               
			"fsub.s f6, f1, f2, rne\n"                            
			"masknot m1, m0\n"                                    
			"fmul.s f8, f5, f5, rdn\n"                            
			"fclass.ps f3, f6\n"                                  
			"fsub.ps f8, f4, f1, dyn\n"                           
			"maskxor m1, m0, m1\n"                                
			"fsq2 f7, 1821(x11)\n"                                
			"flw f3, 1455(x11)\n"                                 
			"flem.ps m2, f5, f4\n"                                
			"fcmovm.ps f4, f0, f0\n"                              
			"fmaxu.pi f5, f2, f3\n"                               
			"fcvt.pwu.ps f8, f0, rtz\n"                           
			"fcvt.s.w f5, x11, rtz\n"                             
			"fsw.ps f5, -1044(x11)\n"                             
			"fsgnj.ps f9, f2, f7\n"                               
			"fsrl.pi f2, f1, f2\n"                                
			"fpackreph.pi f5, f4\n"                               
			"fcvt.f16.ps f7, f7\n"                                
			"fminu.pi f2, f7, f3\n"                               
			"fsra.pi f8, f6, f7\n"                                
			"fnot.pi f6, f4\n"                                    
			"fbci.ps f8, 185628\n"                                
			"fcmov.ps f3, f0, f3, f3\n"                           
			"flt.ps f7, f6, f7\n"                                 
			"flt.s x3, f6, f6\n"                                  
			"fadd.ps f5, f6, f0, rtz\n"                           
			"fsetm.pi m1, f1\n"                                   
			"fsrli.pi f2, f6, 11\n"                               
			"feqm.ps m1, f5, f4\n"                                
			"fadd.pi f3, f6, f6\n"                                
			"fmulhu.pi f9, f7, f4\n"                              
			"fsgnj.s f6, f2, f4\n"                                
			"fsrli.pi f3, f3, 14\n"                               
			"fcvt.ps.pwu f7, f5, rmm\n"                           
			"fltm.pi m1, f2, f2\n"                                
			"fcvt.ps.pw f6, f2, rmm\n"                            
			"faddi.pi f5, f3, 509\n"                              
			"fsgnj.s f5, f6, f3\n"                                
			"fsetm.pi m0, f0\n"                                   
			"fpackreph.pi f6, f0\n"                               
			"fcvt.s.w f8, x11, rmm\n"                             
			"fsw f7, 1118(x11)\n"                                 
			"fpackrepb.pi f3, f1\n"                               
			"masknot m2, m0\n"                                    
			"fslli.pi f8, f7, 5\n"                                
			"feq.s x3, f5, f6\n"                                  
			"flt.pi f8, f0, f6\n"                                 
			"feq.pi f7, f2, f3\n"                                 
			"flt.ps f2, f0, f7\n"                                 
			"fclass.s x4, f5\n"                                   
			"fmvz.x.ps x3, f6, 6\n"                               
			"fnot.pi f3, f7\n"                                    
			"maskxor m0, m1, m0\n"                                
			"fclass.ps f6, f5\n"                                  
			"fpackreph.pi f3, f5\n"                               
			"fsra.pi f4, f5, f7\n"                                
			"fmax.pi f4, f7, f3\n"                                
			"fsgnjx.s f6, f0, f4\n"                               
			"fclass.ps f3, f3\n"                                  
			"fslli.pi f3, f2, 2\n"                                
			"fsgnjn.s f2, f5, f4\n"                               
			"feq.ps f7, f5, f0\n"                                 
			"fsrl.pi f8, f0, f4\n"                                
			"fsgnjn.ps f4, f7, f3\n"                              
			"fmin.ps f5, f2, f6\n"                                
			"fsw.ps f4, 451(x11)\n"                               
			"fnmsub.s f7, f7, f7, f7, rne\n"                      
			"flt.ps f7, f3, f1\n"                                 
			//RTLMIN-5479"frsq.ps f8, f4\n"                                    
			"fadd.ps f3, f4, f6, dyn\n"                           
			"fbci.ps f6, 336454\n"                                
			"faddi.pi f2, f4, 346\n"                              
			"fcmov.ps f3, f2, f4, f6\n"                           
			"fsgnjn.s f4, f4, f7\n"                               
			"fmin.pi f5, f7, f6\n"                                
			"fmaxu.pi f6, f1, f4\n"                               
			"feq.pi f6, f6, f6\n"                                 
			"fadd.pi f8, f2, f1\n"                                
			"fcvt.pwu.ps f6, f1, rmm\n"                           
			"fnmsub.s f8, f7, f1, f3, rmm\n"                      
			"fsra.pi f6, f6, f3\n"                                
			"fcvt.ps.pw f2, f2, rmm\n"                            
			"fandi.pi f2, f2, 471\n"                              
			"fmvs.x.ps x3, f7, 3\n"                               
			"flq2 f7, -719(x11)\n"                                
			"fcvt.s.wu f6, x11, rdn\n"                            
			"fmul.pi f7, f4, f0\n"                                
			"fsgnjx.ps f8, f0, f4\n"                              
			"fmvz.x.ps x4, f5, 1\n"                               
			"fltm.ps m0, f6, f1\n"                                
			"mov.m.x m2, x11, 56\n"                               
			"fcvt.w.s x4, f1, rdn\n"                              
			"flt.s x3, f0, f2\n"                                  
			"fand.pi f9, f6, f3\n"                                
			"masknot m1, m0\n"                                    
			"fbci.ps f6, 510614\n"                                
			"fexp.ps f8, f3\n"                                    
			//RTLMIN-5479"fsin.ps f8, f4\n"                                    
			"fsgnj.ps f6, f5, f0\n"                               
			"fand.pi f5, f1, f5\n"                                
			"mova.x.m x3\n"                                       
			"fltu.pi f3, f6, f0\n"                                
			"fcvt.f16.ps f6, f2\n"                                
			"fbci.ps f4, 476259\n"                                
			"fmv.s.x f2, x11\n"                                   
			"fsgnjx.ps f7, f5, f1\n"                              
			"fsgnj.ps f2, f3, f4\n"                               
			"fmvs.x.ps x4, f7, 5\n"                               
			"fsrli.pi f9, f4, 4\n"                                
			"flem.ps m0, f0, f3\n"                                
			"fminu.pi f2, f5, f2\n"                               
			"fle.s x4, f1, f2\n"                                  
			"fcmov.ps f3, f4, f0, f1\n"                           
			"fnmsub.ps f2, f7, f3, f7, rtz\n"                     
			"fcvt.s.w f8, x11, rne\n"                             

			"li x5, 0x2974f18b698feabe\n"
			"mova.m.x x5\n"                                       
			"fsra.pi f5, f7, f3\n"                                
			"fsra.pi f8, f7, f6\n"                                
			"masknot m2, m2\n"                                    
			"flem.ps m1, f1, f6\n"                                
			"fmadd.s f6, f1, f3, f0, rne\n"                       
			"mova.x.m x3\n"                                       
			"fbci.ps f6, 300693\n"                                
			"fsgnjn.ps f6, f4, f6\n"                              
			"fcmov.ps f6, f3, f3, f3\n"                           
			"mova.x.m x3\n"                                       
			"fpackrepb.pi f5, f4\n"                               
			"flt.pi f5, f5, f4\n"                                 
			"fmsub.s f5, f3, f5, f6, rtz\n"                       
			"fsat8.pi f6, f0\n"                                   
			"feq.s x4, f5, f6\n"                                  
			"fsw f3, 1723(x11)\n"                                 
			"fsw.ps f2, -961(x11)\n"                              
			"fnmadd.ps f5, f3, f7, f4, rup\n"                     
			"maskand m1, m2, m2\n"                                
			"fsgnj.ps f2, f7, f0\n"                               
			"fmv.s.x f7, x11\n"                                   
			"fcvt.pwu.ps f3, f6, rup\n"                           
			"mova.x.m x4\n"                                       
			"fsgnj.s f4, f0, f1\n"                                
			"fcvt.ps.pwu f5, f1, rtz\n"                           
			"fsub.s f4, f1, f5, rne\n"                            
			"fnmadd.ps f8, f2, f1, f5, rne\n"                     
			"fcvt.w.s x4, f7, rmm\n"                              
			"flw.ps f5, 340(x11)\n"                               
			"fsw.ps f0, 1227(x11)\n"                              
			"fround.ps f8, f3, rup\n"                             
			"fcvt.ps.pwu f6, f5, rup\n"                           
			"fltu.pi f3, f2, f4\n"                                
			"feqm.ps m1, f6, f6\n"                                
			"fsrai.pi f9, f4, 9\n"                                
			"fltm.pi m2, f4, f4\n"                                
			"fsgnjx.s f6, f7, f7\n"                               
			"fmax.s f7, f3, f3\n"                                 
			"fadd.pi f8, f3, f0\n"                                
			"fadd.pi f4, f7, f0\n"                                
			"flw.ps f2, 1598(x11)\n"                              
			"maskor m2, m0, m2\n"                                 
			"fexp.ps f3, f2\n"                                    
			"fbci.ps f9, 26730\n"                                 
			"fbcx.ps f2, x11\n"                                   
			"fsgnj.ps f5, f4, f0\n"                               
			"fclass.s x4, f1\n"                                   
			"fround.ps f6, f4, rtz\n"                             
			"for.pi f6, f7, f4\n"                                 
			"fnmadd.ps f4, f5, f4, f1, rtz\n"                     
			"fmul.s f5, f6, f6, rne\n"                            
			"maskpopc x3, m0\n"                                   
			"fsra.pi f2, f4, f2\n"                                
			"fswizz.ps f2, f0, 93\n"                              
			"fcmovm.ps f7, f7, f4\n"                              
			"fclass.s x3, f5\n"                                   
			"fmulhu.pi f4, f1, f2\n"                              
			"fcmov.ps f6, f1, f3, f2\n"                           
			"fsrli.pi f7, f4, 5\n"                                
			"fmul.pi f5, f5, f3\n"                                
			"fcvt.pw.ps f5, f2, rup\n"                            
			"fexp.ps f9, f4\n"                                    
			"maskand m2, m1, m2\n"                                
			"mov.m.x m2, x11, 112\n"                              
			"fcvt.s.wu f3, x11, rmm\n"                            
			"ffrc.ps f2, f6\n"                                    
			"fswizz.ps f9, f4, 105\n"                             
			"fcvt.pw.ps f6, f6, rne\n"                            
			"fmadd.s f7, f2, f4, f2, dyn\n"                       
			"ffrc.ps f8, f7\n"                                    
			"fadd.pi f5, f0, f0\n"                                
			"fmin.pi f9, f3, f6\n"                                
			"fsgnj.ps f5, f3, f4\n"                               
			"frcp.ps f8, f1\n"                                    
			"fsgnjx.s f8, f4, f0\n"                               
			"fnmadd.s f8, f3, f4, f7, dyn\n"                      
			"fsgnjx.s f6, f1, f1\n"                               
			"flt.ps f6, f0, f6\n"                                 
			"fnmadd.s f3, f7, f1, f4, rmm\n"                      
			"fnmadd.ps f6, f4, f5, f0, rmm\n"                     
			"fmaxu.pi f2, f2, f3\n"                               
			"fsll.pi f8, f4, f5\n"                                
			"fsetm.pi m1, f1\n"                                   
			"feq.s x4, f7, f3\n"                                  
			"fcvt.wu.s x3, f7, rmm\n"                             
			"fle.s x4, f0, f7\n"                                  
			"maskand m0, m0, m2\n"                                
			"fsrai.pi f7, f6, 13\n"                               
			"fadd.s f4, f6, f1, rdn\n"                            
			"fadd.ps f6, f6, f4, dyn\n"                           
			"fmul.ps f6, f3, f6, rtz\n"                           
			"fmax.pi f5, f5, f0\n"                                
			"fmaxu.pi f3, f1, f6\n"                               
			"fcvt.s.wu f6, x11, rup\n"                            
			"fminu.pi f6, f0, f1\n"                               
			"mov.m.x m2, x11, 21\n"                               
			"fbci.ps f8, 461780\n"                                
			"fswizz.ps f3, f5, 102\n"                             
			"fnmsub.s f3, f1, f3, f4, rmm\n"                      
			"mova.x.m x3\n"                                       
			"fcmovm.ps f6, f7, f4\n"                              
			//RTLMIN-5479"frsq.ps f8, f3\n"                                    
			"flog.ps f5, f7\n"                                    
			"fmul.ps f9, f7, f4, rmm\n"                           
			"fbci.pi f4, 308771\n"                                
			"fcvt.s.wu f5, x11, rup\n"                            

			"li x5, 0xeae884489d0bd0a1\n"
			"mova.m.x x5\n"                                       
			"fsgnjx.s f5, f4, f2\n"                               
			"fslli.pi f4, f4, 2\n"                                
			//RTLMIN-5479"fsin.ps f3, f3\n"                                    
			"fsrl.pi f5, f6, f6\n"                                
			"fmvz.x.ps x3, f4, 6\n"                               
			"fmulh.pi f6, f4, f4\n"                               
			"fnot.pi f4, f5\n"                                    
			"mov.m.x m0, x11, 108\n"                              
			"feq.ps f4, f5, f2\n"                                 
			"fsub.ps f7, f1, f7, rdn\n"                           
			"for.pi f4, f5, f3\n"                                 
			"fltu.pi f2, f2, f2\n"                                
			"fmin.pi f9, f4, f3\n"                                
			"flem.ps m0, f5, f3\n"                                
			"fmin.pi f8, f7, f4\n"                                
			"fpackrepb.pi f5, f0\n"                               
			"fmin.pi f8, f6, f1\n"                                
			"fle.s x4, f1, f2\n"                                  
			"maskxor m1, m2, m1\n"                                
			"frcp.ps f6, f2\n"                                    
			"fmsub.s f3, f5, f6, f7, rne\n"                       
			"fsgnjn.ps f5, f3, f5\n"                              
			"maskpopcz x4, m2\n"                                  
			"fminu.pi f9, f3, f1\n"                               
			"fsgnjx.s f9, f0, f2\n"                               
			//RTLMIN-5479"frsq.ps f4, f0\n"                                    
			"fsub.ps f5, f0, f0, dyn\n"                           
			"fmulh.pi f6, f5, f1\n"                               
			"fmax.pi f8, f6, f0\n"                                
			"faddi.pi f8, f5, 103\n"                              
			"fsgnjx.ps f5, f2, f2\n"                              
			"fle.s x3, f1, f7\n"                                  
			"fbci.pi f2, 141321\n"                                
			"fcvt.ps.f16 f5, f3\n"                                
			"maskand m2, m2, m0\n"                                
			"mov.m.x m0, x11, 10\n"                               
			//RTLMIN-5479"fsin.ps f6, f2\n"                                    
			"fmul.pi f9, f7, f6\n"                                
			"fcvt.ps.pw f7, f1, rdn\n"                            
			"fcvt.pwu.ps f9, f3, rmm\n"                           
			"fsw f0, -115(x11)\n"                                 
			"fclass.s x3, f6\n"                                   
			"fsrai.pi f8, f3, 13\n"                               
			"maskpopc x3, m1\n"                                   
			"fclass.ps f9, f4\n"                                  
			"fmax.pi f6, f6, f0\n"                                
			"flt.pi f8, f7, f0\n"                                 
			"fsra.pi f3, f1, f3\n"                                
			//RTLMIN-5479"frsq.ps f3, f0\n"                                    
			"fmadd.s f6, f5, f3, f1, rmm\n"                       
			"feq.s x3, f1, f2\n"                                  
			"flq2 f2, 102(x11)\n"                                 
			"fmax.pi f7, f2, f2\n"                                
			"feq.pi f5, f4, f7\n"                                 
			"fexp.ps f8, f6\n"                                    
			"fnmadd.ps f9, f5, f0, f3, rne\n"                     
			"maskxor m1, m1, m2\n"                                
			"fsgnjn.ps f3, f6, f2\n"                              
			"fsw.ps f0, -1609(x11)\n"                             
			"fpackrepb.pi f7, f1\n"                               
			"fmv.s.x f2, x11\n"                                   
			"fandi.pi f2, f1, 479\n"                              
			"fsat8.pi f4, f6\n"                                   
			"fclass.s x4, f2\n"                                   
			"fnmsub.ps f8, f6, f4, f6, rne\n"                     
			"fmsub.s f4, f5, f0, f6, rup\n"                       
			"feq.pi f7, f5, f3\n"                                 
			"fcvt.pw.ps f2, f0, rne\n"                            
			"fmulh.pi f7, f6, f4\n"                               
			"fclass.ps f5, f0\n"                                  
			//RTLMIN-5479"fsin.ps f9, f4\n"                                    
			"fcvt.f16.ps f5, f2\n"                                
			"flt.s x3, f3, f4\n"                                  
			"fround.ps f6, f5, rtz\n"                             
			"fmulh.pi f3, f3, f5\n"                               
			"flt.s x4, f6, f2\n"                                  
			"fcvt.ps.pwu f4, f0, dyn\n"                           

			"li x5, 0x766551a3b8eccf22\n"
			"mova.m.x x5\n"                                       
			"fbcx.ps f5, x11\n"                                   
			"fslli.pi f7, f6, 2\n"                                
			"fmvz.x.ps x4, f2, 5\n"                               
			"fbc.ps f8, -873(x11)\n"                              
			"feq.s x3, f6, f3\n"                                  
			"fmadd.s f7, f5, f3, f6, dyn\n"                       
			"fltm.ps m0, f1, f4\n"                                
			"fcvt.ps.pw f3, f1, rne\n"                            

			"li x5, 0x9a410471dc5afb6e\n"
			"mova.m.x x5\n"                                       
			"flw f9, -1694(x11)\n"                                
			"fadd.s f3, f5, f6, rup\n"                            
			"flt.ps f4, f6, f4\n"                                 
			"fmin.s f5, f1, f6\n"                                 
			"fandi.pi f5, f5, 25\n"                               
			"fpackrepb.pi f9, f7\n"                               
			"fadd.s f5, f0, f6, dyn\n"                            
			"fsgnjx.ps f2, f7, f0\n"                              
			"fle.pi f5, f3, f4\n"                                 
			"maskand m0, m1, m1\n"                                
			"fnmsub.s f8, f3, f6, f5, dyn\n"                      
			"fround.ps f9, f2, rtz\n"                             
			"mova.x.m x4\n"                                       
			"fmin.ps f9, f4, f0\n"                                

			"li x5, 0xfd0d1bf1eefccd61\n"
			"mova.m.x x5\n"                                       
			"fcvt.s.wu f6, x11, rmm\n"                            
			"flt.ps f5, f2, f7\n"                                 
			"fand.pi f7, f3, f0\n"                                
			"fcmovm.ps f5, f5, f2\n"                              
			//RTLMIN-5479"fsin.ps f6, f3\n"                                    
			"fnmsub.s f2, f7, f6, f3, rmm\n"                      
			"fmulhu.pi f7, f4, f5\n"                              
			"fcvt.ps.pw f6, f6, rdn\n"                            
			"fcvt.ps.pw f8, f1, rup\n"                            
			"fpackreph.pi f3, f4\n"                               
			"fltu.pi f4, f4, f6\n"                                
			"fmv.s.x f9, x11\n"                                   
			"fsgnj.s f9, f3, f5\n"                                
			"fadd.pi f3, f2, f1\n"                                
			"fsgnjx.ps f9, f3, f0\n"                              
			"fsgnjn.ps f8, f1, f6\n"                              
			"fmin.pi f2, f1, f1\n"                                
			"fmv.x.s x3, f4\n"                                    
			"feq.pi f2, f3, f0\n"                                 
			"fmsub.ps f8, f7, f6, f6, rmm\n"                      
			"fsgnjx.ps f4, f6, f5\n"                              
			"fmax.s f2, f5, f1\n"                                 
			"fbci.pi f7, 204746\n"                                
			"fbci.pi f7, 404500\n"                                
			"flq2 f3, -819(x11)\n"                                
			"fand.pi f5, f6, f3\n"                                
			"fsrli.pi f3, f7, 10\n"                               
			"fxor.pi f3, f0, f0\n"                                
			"feq.pi f4, f6, f3\n"                                 
			"fxor.pi f6, f4, f6\n"                                
			"fle.s x4, f7, f2\n"                                  
			"fcvt.ps.f16 f4, f3\n"                                
			"fmin.s f6, f6, f1\n"                                 
			"fsll.pi f5, f3, f7\n"                                
			"fexp.ps f8, f5\n"                                    
			"fnmadd.ps f3, f1, f3, f4, rne\n"                     
			"fltm.ps m2, f5, f5\n"                                
			"maskor m1, m0, m1\n"                                 
			"fminu.pi f6, f7, f4\n"                               
			"fmvz.x.ps x3, f7, 0\n"                               
			"fnmadd.s f4, f2, f4, f0, rne\n"                      
			"fmvs.x.ps x4, f1, 7\n"                               
			"fle.s x3, f3, f5\n"                                  
			"ffrc.ps f9, f0\n"                                    
			"fmulhu.pi f5, f2, f4\n"                              
			"fxor.pi f9, f2, f2\n"                                
			"fcvt.pwu.ps f2, f6, rne\n"                           
			"fcvt.pw.ps f6, f0, rtz\n"                            
			"flw.ps f8, 277(x11)\n"                               
			"fmax.ps f9, f3, f0\n"                                
			"fmulh.pi f3, f1, f7\n"                               
			"fmin.ps f5, f0, f3\n"                                
			"fbcx.ps f6, x11\n"                                   
			"fcvt.s.w f8, x11, rmm\n"                             
			"maskpopcz x3, m2\n"                                  
			"fmulh.pi f3, f1, f6\n"                               
			"fsgnjx.s f9, f0, f0\n"                               
			"fadd.pi f9, f1, f4\n"                                
			"fcvt.s.wu f2, x11, rup\n"                            
			"maskor m0, m0, m2\n"                                 
			"feq.s x4, f3, f5\n"                                  
			"fsrli.pi f3, f6, 12\n"                               
			"fpackrepb.pi f9, f2\n"                               
			"fbcx.ps f9, x11\n"                                   
			"fle.ps f5, f6, f4\n"                                 
			"fsub.pi f4, f4, f6\n"                                
			"fbcx.ps f3, x11\n"                                   
			"fnmadd.s f7, f0, f3, f2, rne\n"                      
			"fmax.s f5, f4, f7\n"                                 
			"fandi.pi f5, f0, 149\n"                              
			"maskpopcz x3, m1\n"                                  
			"flt.s x3, f5, f1\n"                                  
			"feq.ps f4, f3, f6\n"                                 
			"masknot m1, m0\n"                                    
			"fsrl.pi f2, f3, f4\n"                                
			"fsatu8.pi f2, f0\n"                                  
			"fltm.ps m0, f4, f1\n"                                
			"fcvt.s.w f9, x11, rup\n"                             
			"fmvz.x.ps x4, f1, 4\n"                               
			"fxor.pi f7, f5, f6\n"                                
			"fmvz.x.ps x3, f3, 0\n"                               
			"fmax.pi f6, f3, f4\n"                                
			"maskpopc x3, m1\n"                                   
			"fsgnjn.ps f2, f3, f2\n"                              
			"fmadd.ps f6, f2, f7, f1, rdn\n"                      
			"fsw.ps f2, -418(x11)\n"                              
			"fmax.ps f7, f3, f7\n"                                
			"fmul.s f9, f7, f0, rdn\n"                            
			"fsgnjx.s f9, f5, f7\n"                               
			"feqm.ps m0, f6, f2\n"                                
			"fround.ps f5, f5, rtz\n"                             
			"fpackrepb.pi f6, f2\n"                               
			"fle.pi f3, f5, f3\n"                                 
			"fpackrepb.pi f2, f6\n"                               
			"fcmovm.ps f5, f6, f4\n"                              
			"ffrc.ps f5, f7\n"                                    
			"mov.m.x m1, x11, 112\n"                              
			"fsgnj.s f8, f0, f4\n"                                
			"fmax.ps f8, f2, f1\n"                                
			"fcvt.pw.ps f9, f5, rdn\n"                            
			"fmax.s f9, f0, f5\n"                                 
			"fmvz.x.ps x4, f1, 2\n"                               
			"fnmadd.s f2, f5, f2, f4, rne\n"                      
			"faddi.pi f7, f4, 443\n"                              
			"fsgnjn.s f4, f1, f0\n"                               
			"fclass.ps f4, f5\n"                                  
			"maskxor m2, m0, m2\n"                                
			"flt.s x4, f3, f6\n"                                  
			"fadd.ps f2, f4, f1, dyn\n"                           
			"fnot.pi f2, f6\n"                                    
			"fsgnjn.ps f6, f1, f5\n"                              
			"fle.ps f3, f4, f3\n"                                 
			"fcvt.pwu.ps f6, f6, rmm\n"                           
			"fmv.s.x f9, x11\n"                                   
			"fexp.ps f7, f0\n"                                    
			"fcvt.s.wu f8, x11, rtz\n"                            
			"fltm.pi m0, f3, f2\n"                                
			"fslli.pi f3, f6, 11\n"                               
			"fsub.pi f2, f0, f5\n"                                
			//RTLMIN-5479"fsin.ps f3, f4\n"                                    
			"fsra.pi f6, f4, f0\n"                                
			"for.pi f5, f6, f3\n"                                 
			"fcvt.ps.pwu f3, f4, rtz\n"                           
			"fswizz.ps f8, f1, 115\n"                             
			"fsgnj.s f2, f7, f3\n"                                
			"fmsub.s f9, f5, f1, f7, rdn\n"                       
			"flt.s x4, f6, f7\n"                                  
			"fsub.pi f6, f0, f7\n"                                
			"masknot m1, m1\n"                                    
			"fmadd.s f2, f0, f7, f0, rne\n"                       
			"fmax.s f8, f5, f6\n"                                 
			"fsatu8.pi f3, f1\n"                                  
			"fcvt.s.wu f8, x11, rmm\n"                            
			"fmul.s f4, f1, f0, rup\n"                            
			"frcp.ps f4, f6\n"                                    
			"fcmovm.ps f3, f3, f2\n"                              
			"fmax.s f9, f7, f1\n"                                 
			"fpackreph.pi f6, f3\n"                               
			"fcvt.s.w f4, x11, rmm\n"                             
			"maskor m2, m1, m0\n"                                 
			"flt.ps f7, f1, f0\n"                                 
			"fsub.pi f4, f5, f1\n"                                
			"fsrli.pi f6, f2, 9\n"                                
			"fandi.pi f8, f0, 356\n"                              
			"fnot.pi f3, f5\n"                                    
			"fle.pi f9, f5, f2\n"                                 
			"fsub.pi f4, f5, f6\n"                                
			"fsgnj.s f2, f2, f4\n"                                
			"fmadd.ps f5, f7, f7, f7, dyn\n"                      
			"maskand m2, m1, m1\n"                                
			"fmin.ps f5, f1, f5\n"                                
			"fsgnj.s f5, f2, f6\n"                                
			"fcvt.s.wu f8, x11, rup\n"                            
			"fsgnj.s f8, f7, f5\n"                                
			"fmax.pi f4, f6, f7\n"                                
			"fle.pi f8, f3, f0\n"                                 
			"fmul.pi f6, f1, f7\n"                                
			"fclass.s x3, f5\n"                                   
			"fsgnjn.ps f4, f2, f7\n"                              
			"fsub.pi f8, f1, f4\n"                                
			"fmulh.pi f7, f5, f0\n"                               
			"fcmov.ps f4, f0, f5, f7\n"                           
			"fnmadd.s f4, f0, f4, f0, rtz\n"                      
			"fsgnjn.ps f5, f1, f0\n"                              
			"mov.m.x m2, x11, 23\n"                               
			"fbc.ps f8, -1625(x11)\n"                             
			"fsgnj.ps f3, f4, f3\n"                               
			"fbc.ps f7, 1007(x11)\n"                              
			"fsub.ps f3, f7, f3, rne\n"                           
			"fcvt.ps.f16 f7, f0\n"                                
			"fsrl.pi f8, f3, f3\n"                                
			"fandi.pi f2, f7, 74\n"                               
			"fcvt.ps.pw f5, f6, rdn\n"                            
			"fmulhu.pi f7, f7, f6\n"                              
			"fand.pi f4, f2, f3\n"                                
			"fnmsub.ps f9, f2, f0, f6, rdn\n"                     
			"fcvt.w.s x4, f7, dyn\n"                              
			"fround.ps f8, f7, rne\n"                             
			"feq.s x4, f4, f3\n"                                  
			"fcvt.s.w f9, x11, dyn\n"                             
			"fslli.pi f9, f7, 11\n"                               
			"ffrc.ps f2, f4\n"                                    
			"fmul.s f7, f1, f7, rmm\n"                            
			"fcvt.pwu.ps f8, f1, rup\n"                           
			"fmsub.ps f4, f1, f2, f7, dyn\n"                      
			"fcvt.ps.pwu f5, f5, rdn\n"                           
			"fltm.pi m1, f6, f0\n"                                
			"feq.pi f8, f4, f2\n"                                 
			"fsetm.pi m0, f5\n"                                   
			"fandi.pi f5, f0, 23\n"                               
			"fsatu8.pi f9, f1\n"                                  
			"ffrc.ps f2, f7\n"                                    
			"maskpopcz x3, m2\n"                                  
			"fmadd.ps f7, f1, f2, f2, rdn\n"                      
			"feq.pi f2, f2, f3\n"                                 
			"fmvz.x.ps x3, f6, 3\n"                               
			"fmv.s.x f4, x11\n"                                   
			"flq2 f6, 1637(x11)\n"                                
			"maskpopc x3, m2\n"                                   
			"fclass.s x4, f2\n"                                   
			"mova.x.m x4\n"                                       
			"fbci.pi f2, 2794\n"                                  
			"fcvt.ps.f16 f6, f4\n"                                
			"fsgnjn.s f5, f7, f2\n"                               
			"fcvt.f16.ps f8, f7\n"                                
			"fmv.s.x f8, x11\n"                                   
			"fmv.s.x f7, x11\n"                                   
			"fsat8.pi f4, f5\n"                                   
			"fnmadd.ps f9, f1, f5, f2, rup\n"                     
			"feq.s x4, f1, f4\n"                                  
			"fclass.ps f5, f6\n"                                  
			"fnot.pi f5, f1\n"                                    
			"feq.pi f3, f3, f4\n"                                 
			"fcvt.s.wu f5, x11, rdn\n"                            
			"fmin.s f4, f6, f6\n"                                 
			"fswizz.ps f5, f1, 121\n"                             
			"fsll.pi f7, f7, f4\n"                                
			"fandi.pi f6, f5, 246\n"                              
			"fmin.s f9, f1, f7\n"                                 
			"flem.ps m1, f2, f0\n"                                
			"fsgnj.s f2, f3, f2\n"                                
			"fmaxu.pi f5, f1, f4\n"                               
			"fmulhu.pi f2, f2, f4\n"                              
			"fminu.pi f5, f1, f6\n"                               
			"ffrc.ps f6, f7\n"                                    
			"fmul.pi f9, f7, f4\n"                                
			"maskand m1, m0, m2\n"                                
			"fnmsub.s f8, f2, f6, f6, dyn\n"                      
			"fsrai.pi f7, f7, 0\n"                                
			"feq.pi f7, f5, f4\n"                                 
			"mova.x.m x3\n"                                       
			"flem.ps m1, f4, f2\n"                                
			"fcvt.wu.s x3, f7, rdn\n"                             
			"fmadd.ps f4, f0, f2, f2, rtz\n"                      
			"fcvt.ps.pwu f6, f4, rup\n"                           
			"fsgnj.ps f8, f1, f0\n"                               
			"fmadd.s f6, f1, f1, f1, rup\n"                       
			"fclass.s x3, f1\n"                                   
			"fsub.ps f6, f6, f3, dyn\n"                           

			"li x5, 0xbb73a513a8feb49\n"
			"mova.m.x x5\n"                                       
			"fsgnjn.s f5, f4, f6\n"                               
			"fcvt.f16.ps f4, f6\n"                                
			"fmul.ps f4, f4, f1, rup\n"                           
			"maskor m2, m1, m1\n"                                 
			"fmax.pi f7, f4, f5\n"                                
			"fmvz.x.ps x3, f7, 7\n"                               
			"mov.m.x m2, x11, 109\n"                              
			"fmax.ps f4, f5, f3\n"                                
			"fmin.s f2, f0, f5\n"                                 
			"fsgnjn.s f2, f2, f2\n"                               
			"fltm.pi m0, f3, f2\n"                                
			"fxor.pi f7, f6, f6\n"                                
			"masknot m0, m0\n"                                    
			"mova.x.m x3\n"                                       
			"fmax.s f2, f0, f3\n"                                 
			"fltm.pi m1, f5, f3\n"                                
			"fmv.x.s x4, f2\n"                                    
			"fsetm.pi m1, f7\n"                                   
			"fmaxu.pi f8, f1, f5\n"                               
			"fnmadd.ps f6, f0, f5, f2, rtz\n"                     
			"fadd.pi f4, f4, f4\n"                                
			"fcvt.ps.pwu f8, f3, dyn\n"                           
			"fmin.ps f7, f6, f6\n"                                
			"fsgnj.ps f3, f6, f0\n"                               
			"fslli.pi f3, f6, 4\n"                                
			"fsgnjx.s f6, f0, f7\n"                               
			"fmul.pi f7, f7, f0\n"                                
			"fnmsub.ps f6, f4, f3, f5, rup\n"                     
			//RTLMIN-5479"frsq.ps f5, f4\n"                                    
			"fsgnj.s f6, f7, f5\n"                                
			"fmax.pi f7, f2, f6\n"                                
			"fsgnj.ps f4, f1, f5\n"                               
			"fsw f0, 1918(x11)\n"                                 
			"maskxor m0, m0, m1\n"                                
			"fclass.s x3, f5\n"                                   
			"fsgnj.ps f6, f0, f5\n"                               
			"fxor.pi f5, f1, f0\n"                                
			"fnmsub.ps f7, f2, f0, f3, rup\n"                     
			"fexp.ps f7, f1\n"                                    
			"fnmsub.ps f9, f5, f4, f1, dyn\n"                     
			"flt.ps f6, f1, f1\n"                                 
			"fround.ps f6, f5, rmm\n"                             
			"fpackrepb.pi f2, f0\n"                               
			"fnmsub.s f3, f3, f3, f1, dyn\n"                      
			"masknot m0, m1\n"                                    
			//RTLMIN-5479"fsin.ps f6, f5\n"                                    
			"fle.ps f2, f2, f6\n"                                 
			"fcvt.wu.s x3, f2, rne\n"                             
			"fsgnjx.s f6, f1, f6\n"                               
			"fcvt.wu.s x3, f1, dyn\n"                             
			"fxor.pi f8, f3, f5\n"                                
			"fcvt.ps.pw f4, f3, rtz\n"                            
			"fnmadd.ps f5, f4, f6, f2, dyn\n"                     
			"fclass.s x3, f0\n"                                   
			//RTLMIN-5479"fsin.ps f2, f4\n"                                    
			"ffrc.ps f2, f4\n"                                    
			"faddi.pi f5, f2, 295\n"                              
			"fle.pi f3, f7, f7\n"                                 
			"fadd.ps f6, f7, f7, rdn\n"                           
			//RTLMIN-5479"fsin.ps f8, f1\n"                                    
			"flt.ps f3, f2, f1\n"                                 
			"fnmsub.ps f2, f4, f1, f7, rup\n"                     
			"fsub.ps f6, f1, f3, rtz\n"                           
			"fnmadd.s f8, f1, f5, f6, rup\n"                      
			"fcvt.ps.pwu f6, f6, rmm\n"                           
			"flt.s x3, f0, f5\n"                                  
			"fslli.pi f4, f3, 3\n"                                
			"fsw.ps f0, 612(x11)\n"                               
			"fltm.ps m2, f0, f1\n"                                
			"fsub.s f5, f5, f1, rmm\n"                            
			"fsatu8.pi f9, f3\n"                                  
			"fle.ps f6, f1, f3\n"                                 
			"fsw f2, 168(x11)\n"                                  

			"li x5, 0x546bd9f99cc26349\n"
			"mova.m.x x5\n"                                       
			"fltm.pi m1, f3, f0\n"                                
			"fadd.pi f6, f1, f2\n"                                
			"maskpopc x4, m0\n"                                   
			"fsub.s f5, f5, f3, rne\n"                            
			"fmulhu.pi f9, f3, f5\n"                              
			"maskxor m0, m1, m0\n"                                
			"fmadd.ps f3, f4, f0, f4, rtz\n"                      
			"fmv.x.s x3, f1\n"                                    
			"fmsub.ps f5, f6, f3, f7, dyn\n"                      
			"fsw.ps f3, 137(x11)\n"                               
			"maskpopc x4, m0\n"                                   
			"fcvt.pw.ps f9, f5, rne\n"                            
			"masknot m0, m2\n"                                    
			"mov.m.x m1, x11, 39\n"                               
			"for.pi f7, f6, f4\n"                                 
			"fmin.s f9, f2, f3\n"                                 
			"fswizz.ps f9, f6, 40\n"                              
			"fsll.pi f4, f6, f2\n"                                
			"fmax.ps f9, f5, f4\n"                                
			"flt.s x3, f0, f4\n"                                  
			"fslli.pi f7, f6, 7\n"                                
			"fmax.pi f6, f7, f2\n"                                
			"fmin.s f8, f6, f0\n"                                 
			"fbci.ps f4, 505822\n"                                
			"fsetm.pi m2, f5\n"                                   
			"fsgnjx.s f3, f2, f6\n"                               
			"ffrc.ps f3, f0\n"                                    
			"fsgnjn.s f9, f5, f5\n"                               
			"flt.s x4, f3, f7\n"                                  
			"fsgnjx.ps f2, f4, f7\n"                              
			"flt.ps f2, f0, f2\n"                                 
			"fnmadd.ps f7, f7, f2, f3, rmm\n"                     
			"fadd.s f9, f7, f4, rup\n"                            
			"fltm.ps m1, f7, f2\n"                                
			"fsat8.pi f4, f2\n"                                   
			"fsub.s f5, f2, f3, rtz\n"                            
			"fcvt.f16.ps f3, f4\n"                                
			"flq2 f6, -1769(x11)\n"                               
			"maskpopc x3, m1\n"                                   
			"fcvt.pwu.ps f4, f3, rdn\n"                           
			"fadd.s f7, f3, f2, rdn\n"                            
			"fbci.ps f6, 150782\n"                                
			"fbci.pi f3, 188360\n"                                
			//RTLMIN-5479"fsin.ps f9, f4\n"                                    
			"fle.pi f4, f4, f4\n"                                 
			"fswizz.ps f2, f6, 36\n"                              
			"fmadd.s f3, f6, f2, f7, rup\n"                       
			"feq.ps f5, f3, f1\n"                                 
			"fcvt.pwu.ps f6, f1, rmm\n"                           
			"fsw f3, -1991(x11)\n"                                
			"fand.pi f9, f2, f7\n"                                
			"fswizz.ps f9, f3, 113\n"                             
			"fcvt.ps.f16 f9, f2\n"                                
			"fround.ps f2, f3, dyn\n"                             
			"flw f5, -1441(x11)\n"                                
			"maskor m0, m1, m2\n"                                 
			"fsat8.pi f6, f3\n"                                   
			"fadd.ps f3, f2, f5, rtz\n"                           
			"maskor m2, m1, m1\n"                                 
			"fsll.pi f8, f1, f7\n"                                
			"fltm.pi m2, f4, f5\n"                                
			"flem.ps m0, f5, f6\n"                                
			"fsgnjx.s f4, f2, f3\n"                               
			"maskand m0, m1, m0\n"                                
			"fmvz.x.ps x4, f2, 6\n"                               
			"flog.ps f7, f7\n"                                    
			"flw.ps f3, 785(x11)\n"                               
			"fswizz.ps f5, f3, 104\n"                             
			"fswizz.ps f9, f5, 56\n"                              
			"mova.x.m x4\n"                                       
			"fpackreph.pi f7, f5\n"                               
			"fsub.ps f5, f6, f1, rdn\n"                           
			"fswizz.ps f4, f5, 85\n"                              
			"fclass.s x4, f0\n"                                   
			"maskand m1, m2, m2\n"                                
			"mova.x.m x3\n"                                       
			"flw.ps f3, -747(x11)\n"                              
			"fpackrepb.pi f4, f3\n"                               
			"fcvt.w.s x3, f3, rup\n"                              
			"fcvt.f16.ps f3, f0\n"                                
			"fmax.ps f6, f2, f7\n"                                
			"fbci.pi f6, 1934\n"                                  
			"fmvz.x.ps x4, f6, 1\n"                               
			"fsub.pi f8, f0, f4\n"                                
			"fle.pi f8, f5, f3\n"                                 
			"fmadd.s f2, f7, f6, f7, rne\n"                       
			"fsgnjx.ps f4, f4, f2\n"                              
			"feq.pi f9, f7, f1\n"                                 
			"flw.ps f6, 1566(x11)\n"                              
			"feq.s x3, f3, f4\n"                                  
			"fle.pi f9, f1, f6\n"                                 
			"fminu.pi f7, f6, f7\n"                               
			"fadd.ps f8, f4, f4, rdn\n"                           
			"fbci.ps f2, 455745\n"                                
			//RTLMIN-5479"fsin.ps f6, f4\n"                                    
			"ffrc.ps f2, f7\n"                                    
			"fnmsub.s f6, f0, f2, f2, rmm\n"                      
			"fmulhu.pi f7, f1, f0\n"                              
			"fadd.s f7, f3, f2, dyn\n"                            
			"fsgnj.ps f8, f4, f1\n"                               
			"faddi.pi f6, f2, 3\n"                                
			"fmsub.ps f9, f1, f2, f4, rdn\n"                      
			"fmul.pi f6, f0, f7\n"                                
			"fsgnjn.ps f8, f3, f7\n"                              
			"fexp.ps f2, f4\n"                                    
			"fsgnj.ps f4, f0, f0\n"                               
			"maskpopcz x4, m1\n"                                  
			"fsrai.pi f5, f1, 12\n"                               
			"fmulh.pi f4, f3, f6\n"                               
			"flw.ps f5, -143(x11)\n"                              
			"fmax.pi f7, f3, f7\n"                                
			"fmulh.pi f4, f5, f2\n"                               
			"frcp.ps f9, f5\n"                                    
			"fxor.pi f3, f4, f2\n"                                
			"fpackreph.pi f4, f0\n"                               
			"fmv.x.s x4, f5\n"                                    
			"fnmadd.s f6, f0, f0, f6, rne\n"                      
			"mov.m.x m2, x11, 107\n"                              
			"feq.s x3, f1, f1\n"                                  
			"fsat8.pi f6, f4\n"                                   
			"fle.pi f5, f4, f5\n"                                 
			"fbci.pi f7, 132599\n"                                
			"for.pi f2, f1, f4\n"                                 
			"fcvt.w.s x4, f7, rne\n"                              
			"fcmovm.ps f5, f6, f5\n"                              
			"fcvt.s.wu f3, x11, rne\n"                            
			"fsgnj.s f7, f1, f1\n"                                
			"masknot m0, m0\n"                                    
			"fle.s x3, f0, f6\n"                                  

			"li x5, 0xedd94c376e1abb1e\n"
			"mova.m.x x5\n"                                       
			"fmvz.x.ps x4, f4, 5\n"                               
			"fle.ps f2, f6, f3\n"                                 
			"fsrai.pi f5, f5, 3\n"                                
			"fsw.ps f0, 1456(x11)\n"                              
			"fround.ps f7, f5, rne\n"                             
			"fmulhu.pi f3, f5, f2\n"                              
			"fmin.s f3, f1, f4\n"                                 
			"faddi.pi f3, f3, 464\n"                              
			//RTLMIN-5479"fsin.ps f2, f3\n"                                    
			"fmulhu.pi f2, f3, f2\n"                              
			"frcp.ps f6, f5\n"                                    
			"masknot m0, m2\n"                                    
			"fsra.pi f6, f2, f7\n"                                
			"fsw f3, -936(x11)\n"                                 
			"fcvt.pw.ps f2, f5, rne\n"                            
			"for.pi f8, f0, f6\n"                                 
			"fsub.ps f2, f5, f7, rdn\n"                           
			"fcvt.wu.s x3, f2, rmm\n"                             
			"fsgnjn.s f3, f7, f0\n"                               
			"flw.ps f2, 739(x11)\n"                               
			"fsgnjx.ps f7, f4, f5\n"                              
			"flt.ps f4, f0, f1\n"                                 
			"fsrl.pi f5, f1, f5\n"                                
			"flt.ps f4, f1, f6\n"                                 
			"ffrc.ps f7, f4\n"                                    
			"fmv.x.s x4, f0\n"                                    
			"maskand m1, m1, m0\n"                                
			"fbci.pi f7, 124627\n"                                
			"fslli.pi f5, f3, 0\n"                                
			"flt.s x3, f0, f3\n"                                  
			"fmul.ps f7, f7, f1, rmm\n"                           
			"fclass.s x3, f1\n"                                   
			"fmin.s f8, f2, f3\n"                                 
			"flem.ps m1, f4, f1\n"                                
			"fmin.s f4, f3, f0\n"                                 
			"fround.ps f9, f3, rtz\n"                             
			"feq.pi f5, f4, f2\n"                                 
			"fcvt.ps.f16 f6, f2\n"                                
			"maskor m2, m1, m1\n"                                 
			"mov.m.x m1, x11, 115\n"                              
			"for.pi f3, f0, f3\n"                                 
			"fandi.pi f3, f0, 359\n"                              
			"fsw.ps f7, -648(x11)\n"                              
			"feqm.ps m0, f7, f3\n"                                
			"fslli.pi f3, f0, 8\n"                                
			"fnmsub.ps f8, f2, f0, f4, dyn\n"                     
			"fbcx.ps f8, x11\n"                                   
			"fmin.pi f2, f0, f5\n"                                
			"flw.ps f7, -46(x11)\n"                               
			"feq.pi f3, f3, f1\n"                                 
			"fadd.ps f3, f5, f0, rne\n"                           
			"fslli.pi f7, f1, 8\n"                                
			"flq2 f8, -388(x11)\n"                                
			"fsatu8.pi f7, f3\n"                                  
			"fmul.s f4, f6, f7, dyn\n"                            
			"fclass.s x4, f2\n"                                   
			"fsgnjn.ps f2, f0, f2\n"                              
			"fsgnj.s f8, f6, f5\n"                                
			"fcvt.w.s x3, f7, rne\n"                              
			"fandi.pi f7, f3, 64\n"                               
			"fexp.ps f7, f3\n"                                    
			"fmax.s f3, f1, f0\n"                                 
			"fmaxu.pi f7, f6, f0\n"                               
			"fcmovm.ps f9, f0, f7\n"                              
			"fsgnjn.s f3, f2, f6\n"                               
			"fcvt.f16.ps f7, f5\n"                                
			"fmvs.x.ps x4, f6, 0\n"                               
			"fxor.pi f3, f7, f5\n"                                
			"flt.pi f4, f6, f7\n"                                 
			"fmsub.s f4, f5, f0, f2, dyn\n"                       
			"fsub.ps f7, f6, f7, dyn\n"                           
			"flq2 f9, 454(x11)\n"                                 
			"fmsub.s f2, f3, f7, f6, rmm\n"                       
			"fmv.s.x f4, x11\n"                                   
			"fsetm.pi m0, f7\n"                                   
			"fcvt.f16.ps f9, f2\n"                                
			"fsgnjn.s f4, f1, f1\n"                               
			"fmsub.ps f2, f4, f7, f2, rtz\n"                      
			"fmvs.x.ps x4, f5, 3\n"                               
			"fcvt.wu.s x3, f6, dyn\n"                             
			"fpackreph.pi f4, f5\n"                               
			"fmsub.s f4, f7, f3, f0, rtz\n"                       
			"fandi.pi f4, f2, 214\n"                              
			"flt.pi f9, f1, f4\n"                                 
			"fltm.pi m2, f7, f1\n"                                
			"flw f4, -628(x11)\n"                                 
			"fadd.s f4, f0, f7, rmm\n"                            
			"fmin.pi f7, f1, f5\n"                                
			"fsrl.pi f8, f4, f5\n"                                
			"fmadd.ps f5, f3, f7, f4, rtz\n"                      
			"fcvt.w.s x4, f3, rmm\n"                              
			"fcmovm.ps f2, f6, f3\n"                              
			"mov.m.x m1, x11, 22\n"                               
			"fltu.pi f2, f5, f3\n"                                
			"fsgnj.ps f6, f4, f7\n"                               
			"fltu.pi f3, f6, f6\n"                                
			"flog.ps f7, f1\n"                                    
			"fslli.pi f8, f6, 3\n"                                
			"fadd.s f4, f3, f4, dyn\n"                            
			"maskor m1, m1, m0\n"                                 
			"fmin.pi f3, f1, f1\n"                                
			"maskpopcz x4, m2\n"                                  
			"for.pi f7, f7, f6\n"                                 
			"fround.ps f7, f2, rdn\n"                             
			"fcvt.pwu.ps f6, f7, rmm\n"                           
			"fcvt.s.wu f3, x11, rmm\n"                            

			"li x5, 0xec745564685a4a29\n"
			"mova.m.x x5\n"                                       
			"flog.ps f3, f3\n"                                    
			"fslli.pi f8, f4, 2\n"                                
			"fmaxu.pi f2, f4, f0\n"                               
			"masknot m1, m0\n"                                    

			"li x5, 0x218053daaf6b57c8\n"
			"mova.m.x x5\n"                                       
			"fcmovm.ps f4, f4, f2\n"                              
			"fsub.pi f8, f7, f4\n"                                
			"flw.ps f7, -21(x11)\n"                               
			"fmaxu.pi f9, f6, f0\n"                               
			"fmax.ps f9, f4, f1\n"                                
			"fsub.pi f3, f7, f6\n"                                
			"fsq2 f1, 1049(x11)\n"                                
			"fmin.ps f4, f6, f6\n"                                
			"fmin.pi f4, f1, f6\n"                                
			"fslli.pi f4, f5, 14\n"                               
			"fand.pi f7, f2, f0\n"                                
			"masknot m0, m1\n"                                    
			"frcp.ps f3, f3\n"                                    
			"fbcx.ps f6, x11\n"                                   
			"fsgnjx.s f2, f5, f0\n"                               
			"flw.ps f8, -616(x11)\n"                              
			"fmulh.pi f9, f2, f0\n"                               
			"fbc.ps f4, 1738(x11)\n"                              
			"fsq2 f7, 847(x11)\n"                                 
			"fle.s x4, f5, f5\n"                                  
			"fnmadd.ps f3, f1, f6, f2, rup\n"                     
			"faddi.pi f8, f0, 329\n"                              
			"fltm.pi m2, f0, f4\n"                                
			//RTLMIN-5479"fsin.ps f3, f7\n"                                    
			"fnmsub.ps f5, f1, f7, f2, rup\n"                     
			"for.pi f3, f4, f1\n"                                 
			"fsq2 f3, -1842(x11)\n"                               
			"fcvt.s.wu f6, x11, rup\n"                            
			"fmax.pi f4, f6, f4\n"                                
			"fpackreph.pi f7, f3\n"                               
			"maskor m1, m2, m1\n"                                 
			"fle.s x4, f1, f3\n"                                  
			"fltu.pi f5, f2, f6\n"                                
			"fsq2 f0, -1936(x11)\n"                               
			"fandi.pi f3, f2, 300\n"                              
			"fsub.ps f7, f6, f3, rdn\n"                           
			"fmv.x.s x3, f7\n"                                    
			"fmsub.s f3, f2, f7, f0, dyn\n"                       
			"fmul.ps f8, f4, f3, rne\n"                           
			"fmul.pi f5, f1, f3\n"                                
			"fmvz.x.ps x3, f7, 5\n"                               
			"fadd.pi f6, f2, f3\n"                                
			"fcvt.s.wu f2, x11, rup\n"                            
			"flem.ps m2, f0, f3\n"                                
			"fsub.ps f6, f4, f4, rne\n"                           
			"fnmsub.ps f4, f5, f6, f2, rmm\n"                     
			"fmax.pi f4, f2, f0\n"                                
			"fltm.ps m0, f0, f2\n"                                
			"fmaxu.pi f6, f7, f1\n"                               
			"fswizz.ps f5, f1, 17\n"                              
			"fle.ps f9, f3, f3\n"                                 
			"fmin.pi f7, f7, f1\n"                                
			"fle.pi f6, f5, f7\n"                                 
			"fmvs.x.ps x3, f4, 5\n"                               
			"fpackreph.pi f7, f5\n"                               
			"fcvt.ps.pwu f2, f1, rtz\n"                           
			"maskor m0, m2, m1\n"                                 
			"fsgnjn.s f2, f4, f1\n"                               
			"mov.m.x m2, x11, 72\n"                               
			"fpackrepb.pi f8, f0\n"                               
			"fcvt.ps.pwu f9, f3, rtz\n"                           
			"fcvt.ps.pwu f3, f6, rup\n"                           
			"fmax.s f7, f0, f6\n"                                 
			"fandi.pi f2, f5, 316\n"                              
			"feq.pi f3, f1, f5\n"                                 
			"fswizz.ps f6, f1, 16\n"                              
			"feq.ps f8, f6, f0\n"                                 
			"fmax.s f6, f6, f6\n"                                 
			"maskxor m0, m0, m0\n"                                
			"fand.pi f7, f3, f1\n"                                

			"li x5, 0x8093aea7d4f6a11b\n"
			"mova.m.x x5\n"                                       
			"fsq2 f6, 1296(x11)\n"                                
			"flw f6, 1564(x11)\n"                                 
			"fmax.s f8, f3, f1\n"                                 
			"fpackreph.pi f4, f5\n"                               
			"for.pi f2, f6, f1\n"                                 
			"mov.m.x m1, x11, 0\n"                                
			"fclass.ps f9, f2\n"                                  
			"maskand m2, m1, m2\n"                                
			"fmul.pi f7, f4, f6\n"                                
			"fmul.s f4, f7, f7, dyn\n"                            
			"fsw.ps f7, -414(x11)\n"                              
			"mova.x.m x4\n"                                       
			"fcmov.ps f3, f5, f1, f3\n"                           
			"fcvt.f16.ps f9, f7\n"                                
			"fcvt.w.s x3, f6, rdn\n"                              
			"fcvt.w.s x4, f5, rmm\n"                              
			"fbci.ps f9, 46163\n"                                 
			"fsw.ps f2, 1942(x11)\n"                              
			"feq.pi f7, f7, f7\n"                                 

			"li x5, 0x6e788d2887507281\n"
			"mova.m.x x5\n"                                       
			"feq.pi f2, f2, f6\n"                                 
			"fsgnjn.s f6, f5, f2\n"                               
			"fandi.pi f6, f0, 397\n"                              
			"fadd.ps f5, f6, f6, rne\n"                           
			"fcvt.pw.ps f2, f4, rmm\n"                            
			"fmv.x.s x4, f5\n"                                    
			"fadd.ps f9, f3, f2, rdn\n"                           
			"fmvs.x.ps x4, f5, 1\n"                               
			"fsq2 f0, -1479(x11)\n"                               
			"fbci.pi f5, 181238\n"                                
			"fbcx.ps f6, x11\n"                                   
			"fsatu8.pi f3, f7\n"                                  
			"fpackreph.pi f9, f2\n"                               
			"fsq2 f7, 1697(x11)\n"                                
			"fclass.s x4, f0\n"                                   
			"fmin.ps f9, f4, f0\n"                                
			"fsq2 f6, 33(x11)\n"                                  
			//RTLMIN-5479"fsin.ps f8, f1\n"                                    
			"fsgnj.s f8, f7, f0\n"                                
			//RTLMIN-5479"fsin.ps f8, f4\n"                                    
			"flw.ps f7, -161(x11)\n"                              
			"feq.ps f7, f4, f6\n"                                 
			"fsatu8.pi f9, f3\n"                                  
			"fmv.s.x f4, x11\n"                                   
			"flem.ps m1, f3, f0\n"                                
			"fmaxu.pi f5, f7, f3\n"                               
			"mov.m.x m2, x11, 115\n"                              
			"feq.ps f5, f0, f6\n"                                 
			"fcvt.ps.pwu f2, f3, rne\n"                           
			"flw f3, -722(x11)\n"                                 
			"fnmadd.s f8, f3, f2, f0, rmm\n"                      
			"mova.x.m x3\n"                                       
			"flog.ps f2, f3\n"                                    
			"fcvt.f16.ps f3, f6\n"                                
			"fsrai.pi f3, f4, 13\n"                               
			"fsgnj.ps f9, f7, f4\n"                               
			"fadd.ps f9, f1, f4, rdn\n"                           
			"for.pi f4, f0, f5\n"                                 
			//RTLMIN-5479"frsq.ps f5, f5\n"                                    
			"fnmsub.ps f6, f2, f2, f0, rmm\n"                     
			"fsetm.pi m2, f5\n"                                   
			"fmv.s.x f6, x11\n"                                   
			"fcvt.ps.pwu f4, f6, rmm\n"                           
			"fmadd.ps f3, f5, f2, f2, rdn\n"                      
			"feqm.ps m0, f1, f7\n"                                
			"fmadd.s f9, f5, f0, f1, rdn\n"                       
			"fandi.pi f5, f7, 11\n"                               
			"fnmadd.s f5, f5, f6, f2, dyn\n"                      
			"fcvt.wu.s x3, f5, rtz\n"                             
			"fnmadd.s f4, f6, f0, f5, rdn\n"                      
			"fmax.s f7, f1, f7\n"                                 
			"fmaxu.pi f2, f3, f0\n"                               
			"fsgnjx.ps f6, f3, f7\n"                              
			"fbci.ps f4, 493818\n"                                
			"fcvt.ps.pw f5, f0, rne\n"                            
			"fcmov.ps f6, f6, f6, f4\n"                           
			"fround.ps f9, f2, rdn\n"                             
			"flt.ps f9, f3, f2\n"                                 
			"fmax.s f6, f5, f1\n"                                 
			"fltu.pi f7, f0, f1\n"                                
			"fcvt.s.w f3, x11, rne\n"                             
			"fsrl.pi f3, f5, f7\n"                                
			"fmadd.s f9, f6, f1, f0, dyn\n"                       
			"flem.ps m0, f3, f5\n"                                
			"faddi.pi f3, f5, 469\n"                              
			"mov.m.x m0, x11, 81\n"                               
			"fclass.ps f7, f5\n"                                  
			"fmulh.pi f8, f6, f7\n"                               
			"fmax.s f2, f2, f3\n"                                 
			"fmadd.s f7, f7, f2, f7, dyn\n"                       
			"fsgnj.s f8, f1, f4\n"                                
			"fsatu8.pi f8, f6\n"                                  
			"flt.ps f2, f1, f0\n"                                 
			"fle.ps f8, f0, f7\n"                                 
			"fmvs.x.ps x3, f5, 3\n"                               
			"feq.s x4, f7, f2\n"                                  
			"fxor.pi f6, f6, f5\n"                                
			"fcvt.ps.pw f9, f6, rdn\n"                            
			"fmadd.s f2, f2, f1, f1, rne\n"                       
			"fsub.ps f7, f0, f2, dyn\n"                           
			"fnmadd.s f6, f2, f6, f1, rne\n"                      
			"fcvt.s.w f3, x11, rup\n"                             
			"fcvt.wu.s x3, f6, rne\n"                             
			"frcp.ps f8, f3\n"                                    
			"feqm.ps m2, f3, f2\n"                                
			"fpackrepb.pi f9, f2\n"                               
			"fcmovm.ps f7, f3, f3\n"                              
			"fmax.s f2, f3, f2\n"                                 
			"fltm.pi m1, f2, f3\n"                                
			"fsub.ps f7, f7, f2, rmm\n"                           
			"frcp.ps f3, f4\n"                                    
			"fminu.pi f6, f3, f4\n"                               
			"fcvt.w.s x4, f6, rdn\n"                              
			"fmadd.ps f3, f4, f6, f0, rmm\n"                      
			"fle.pi f6, f7, f1\n"                                 
			"maskpopcz x4, m2\n"                                  
			"feq.s x3, f3, f7\n"                                  
			"fnmsub.ps f4, f3, f2, f4, rtz\n"                     
			"fle.s x3, f5, f2\n"                                  
			"feq.s x3, f0, f6\n"                                  
			"flog.ps f8, f5\n"                                    
			"maskxor m1, m0, m1\n"                                
			"fxor.pi f7, f0, f6\n"                                
			"fcvt.f16.ps f4, f4\n"                                
			"fcmov.ps f6, f7, f7, f6\n"                           
			"maskor m2, m0, m1\n"                                 
			"fltm.pi m0, f3, f1\n"                                
			"fcmovm.ps f8, f5, f0\n"                              
			"fbci.ps f3, 150770\n"                                
			"fslli.pi f6, f1, 13\n"                               
			"fsq2 f3, 612(x11)\n"                                 
			"fpackreph.pi f4, f7\n"                               
			"masknot m1, m2\n"                                    
			"flt.ps f3, f1, f3\n"                                 
			"fsrai.pi f4, f0, 2\n"                                
			"fround.ps f9, f4, rdn\n"                             
			"fmax.pi f6, f0, f3\n"                                
			"fbci.pi f4, 15239\n"                                 
			"fsgnjx.s f3, f5, f0\n"                               
			"fsgnjn.s f5, f6, f2\n"                               
			"maskand m1, m1, m2\n"                                
			"flt.pi f8, f2, f6\n"                                 
			//RTLMIN-5479"frsq.ps f3, f7\n"                                    
			"maskand m0, m0, m0\n"                                
			"fbci.pi f4, 245877\n"                                
			"fsrai.pi f3, f4, 11\n"                               
			"fsub.s f7, f1, f3, rmm\n"                            
			"fmv.s.x f4, x11\n"                                   
			"fcmov.ps f2, f0, f3, f3\n"                           
			"fmv.x.s x3, f4\n"                                    
			"fcvt.s.w f9, x11, rne\n"                             
			"fsll.pi f9, f7, f1\n"                                
			"fsra.pi f3, f5, f1\n"                                
			"fbci.ps f9, 310917\n"                                
			"feqm.ps m2, f4, f5\n"                                
			"maskpopcz x4, m2\n"                                  
			"masknot m0, m0\n"                                    
			"fsra.pi f5, f6, f1\n"                                
			"flw f5, -1221(x11)\n"                                
			"fltm.pi m2, f1, f5\n"                                
			"fminu.pi f5, f4, f0\n"                               
			"fbcx.ps f5, x11\n"                                   
			"flw f8, 1243(x11)\n"                                 
			"fcmov.ps f5, f3, f2, f5\n"                           
			"fmulh.pi f6, f0, f1\n"                               
			"fsrai.pi f8, f5, 3\n"                                
			//RTLMIN-5479"frsq.ps f2, f5\n"                                    
			"flt.s x3, f6, f0\n"                                  
			"fcvt.ps.f16 f4, f2\n"                                
			"fmul.ps f3, f5, f1, rup\n"                           
			"fle.ps f9, f2, f2\n"                                 
			"fle.ps f4, f0, f3\n"                                 
			"fsatu8.pi f5, f3\n"                                  
			"fandi.pi f5, f3, 228\n"                              
			"fsw f5, -1934(x11)\n"                                
			"fand.pi f4, f2, f3\n"                                
			"fmulhu.pi f9, f1, f4\n"                              
			"fnmsub.ps f7, f4, f0, f7, rmm\n"                     
			"fsll.pi f5, f4, f3\n"                                
			"frcp.ps f6, f0\n"                                    
			"fmvz.x.ps x4, f6, 4\n"                               
			"fround.ps f6, f6, rdn\n"                             
			"fnmadd.ps f3, f2, f5, f6, rtz\n"                     
			"fsetm.pi m2, f5\n"                                   
			"fmin.s f4, f5, f6\n"                                 
			"fcvt.pwu.ps f4, f6, rtz\n"                           
			"ffrc.ps f2, f5\n"                                    
			"fcvt.s.w f9, x11, rne\n"                             

			"li x5, 0x8977cd4b3b0b92ac\n"
			"mova.m.x x5\n"                                       
			"fadd.s f8, f6, f0, rtz\n"                            
			"fsub.pi f9, f7, f2\n"                                
			"fltm.pi m2, f4, f1\n"                                
			//RTLMIN-5479"frsq.ps f2, f7\n"                                    
			"fand.pi f9, f1, f4\n"                                
			"maskand m2, m0, m0\n"                                
			"fsetm.pi m2, f0\n"                                   
			"fcmovm.ps f2, f2, f2\n"                              
			"fcvt.pw.ps f8, f2, dyn\n"                            
			"fsgnjn.ps f8, f5, f1\n"                              
			"fsw f2, 1490(x11)\n"                                 
			"fclass.s x3, f6\n"                                   
			"for.pi f3, f6, f5\n"                                 
			"fsub.ps f7, f1, f5, rtz\n"                           
			"fcvt.wu.s x4, f4, rtz\n"                             
			"fmin.ps f9, f2, f0\n"                                
			"fcvt.f16.ps f9, f0\n"                                
			"fmul.s f6, f1, f6, rdn\n"                            
			"faddi.pi f4, f2, 38\n"                               
			"fnmsub.ps f9, f7, f3, f3, rne\n"                     
			"fmulh.pi f8, f0, f3\n"                               
			"fltm.ps m2, f5, f4\n"                                
			"fle.pi f5, f7, f6\n"                                 
			"fmax.pi f4, f0, f1\n"                                
			"fsrl.pi f3, f6, f4\n"                                
			"fsll.pi f2, f0, f4\n"                                
			"fbci.ps f6, 283421\n"                                
			"flem.ps m2, f5, f4\n"                                
			"fmax.s f7, f4, f1\n"                                 
			"fsub.s f4, f2, f4, rup\n"                            
			"fadd.pi f6, f0, f6\n"                                
			"fltm.ps m0, f6, f5\n"                                
			"fbci.pi f2, 370694\n"                                
			"fcmovm.ps f2, f1, f5\n"                              
			"maskand m1, m0, m2\n"                                
			"fsgnj.s f4, f1, f2\n"                                
			"faddi.pi f5, f5, 380\n"                              
			"fclass.ps f5, f5\n"                                  
			"fsgnjx.ps f8, f7, f0\n"                              
			"fmax.ps f8, f2, f5\n"                                
			"mova.x.m x4\n"                                       
			"fadd.pi f2, f4, f6\n"                                
			"fsatu8.pi f7, f2\n"                                  
			"flt.s x3, f7, f5\n"                                  
			"fmin.pi f6, f3, f7\n"                                
			"fbc.ps f8, -452(x11)\n"                              
			"fmvs.x.ps x3, f4, 6\n"                               
			"feq.pi f9, f6, f0\n"                                 
			"fmadd.s f8, f0, f0, f4, rtz\n"                       
			"fsw f6, 48(x11)\n"                                   
			"fsub.s f5, f4, f7, rmm\n"                            
			"flog.ps f5, f5\n"                                    
			//RTLMIN-5479"frsq.ps f2, f5\n"                                    
			"fsgnj.ps f9, f7, f5\n"                               
			"fmadd.ps f7, f6, f0, f4, rup\n"                      
			"flog.ps f4, f1\n"                                    
			"fsq2 f5, 977(x11)\n"                                 
			"fsrai.pi f9, f6, 10\n"                               
			"flt.ps f5, f1, f5\n"                                 
			"fcvt.pw.ps f7, f2, rup\n"                            
			"fmadd.s f9, f6, f6, f0, dyn\n"                       
			"fxor.pi f9, f5, f2\n"                                
			"fbc.ps f5, -1928(x11)\n"                             
			"feq.pi f2, f4, f3\n"                                 
			"mov.m.x m2, x11, 39\n"                               

			"li x5, 0x2ce15f22d6e2a799\n"
			"mova.m.x x5\n"                                       
			"fswizz.ps f3, f0, 127\n"                             
			"fsgnjn.s f8, f3, f4\n"                               
			"fltm.pi m2, f1, f4\n"                                
			"fltu.pi f9, f5, f3\n"                                
			"fexp.ps f5, f2\n"                                    
			"fcvt.w.s x3, f5, dyn\n"                              
			"fsw f5, -262(x11)\n"                                 
			"fnmsub.s f2, f3, f2, f6, rtz\n"                      
			"fmax.pi f3, f2, f5\n"                                
			"fbci.pi f6, 285410\n"                                
			"fswizz.ps f5, f7, 36\n"                              
			"fadd.pi f7, f5, f3\n"                                
			"maskand m0, m0, m1\n"                                
			"fcmov.ps f3, f1, f6, f3\n"                           
			"fsub.s f3, f5, f3, rne\n"                            
			"fbc.ps f3, -1220(x11)\n"                             
			"fbcx.ps f5, x11\n"                                   
			"maskxor m0, m1, m2\n"                                
			"fcmovm.ps f5, f1, f1\n"                              
			"fsw.ps f5, 1211(x11)\n"                              
			"fbcx.ps f9, x11\n"                                   
			"flem.ps m1, f7, f1\n"                                
			"fcvt.wu.s x3, f2, rup\n"                             
			"maskxor m1, m0, m0\n"                                
			"fmax.pi f3, f3, f7\n"                                
			"flem.ps m0, f3, f1\n"                                
			"fcvt.f16.ps f3, f7\n"                                
			"fsgnj.s f3, f7, f5\n"                                
			"fmv.s.x f7, x11\n"                                   
			"feqm.ps m1, f6, f1\n"                                
			"fpackreph.pi f6, f0\n"                               
			"maskpopcz x4, m0\n"                                  
			"fnmsub.ps f2, f6, f4, f0, rdn\n"                     
			"fclass.ps f7, f1\n"                                  
			"fnmsub.s f7, f1, f6, f4, rdn\n"                      
			"fsub.pi f3, f0, f6\n"                                
			"fcvt.s.w f9, x11, rmm\n"                             
			"fand.pi f6, f1, f2\n"                                
			"fsatu8.pi f6, f2\n"                                  
			"fmv.s.x f7, x11\n"                                   
			"feq.s x3, f5, f4\n"                                  
			"maskpopc x4, m2\n"                                   
			"fsgnj.s f8, f1, f4\n"                                
			"fmulhu.pi f6, f1, f0\n"                              
			"fmin.ps f3, f2, f3\n"                                
			"fandi.pi f6, f1, 365\n"                              
			"frcp.ps f2, f4\n"                                    
			"fminu.pi f7, f5, f0\n"                               
			"fsw.ps f1, -1998(x11)\n"                             
			"fsrli.pi f3, f1, 7\n"                                
			"fltm.pi m1, f7, f2\n"                                
			"fmadd.s f3, f6, f3, f0, rne\n"                       
			"fswizz.ps f7, f0, 53\n"                              
			"maskxor m2, m1, m1\n"                                
			"fandi.pi f6, f2, 470\n"                              
			"fmin.s f4, f7, f7\n"                                 
			"fclass.ps f4, f7\n"                                  
			"fsrli.pi f9, f2, 0\n"                                
			"ffrc.ps f5, f1\n"                                    
			"fle.s x3, f1, f3\n"                                  
			"mov.m.x m1, x11, 125\n"                              
			"fmsub.ps f3, f0, f2, f1, rmm\n"                      
			"fmax.ps f4, f5, f3\n"                                
			"maskpopc x3, m1\n"                                   
			"fsub.s f3, f5, f5, dyn\n"                            
			"fxor.pi f7, f5, f0\n"                                
			"fcvt.pw.ps f9, f5, rne\n"                            
			"fsgnj.ps f4, f6, f1\n"                               
			"fmv.s.x f8, x11\n"                                   
			"maskand m2, m0, m1\n"                                
			"fsub.ps f8, f2, f1, rne\n"                           
			"fslli.pi f2, f1, 2\n"                                
			"fltu.pi f6, f6, f2\n"                                
			"faddi.pi f3, f5, 80\n"                               
			"fmvs.x.ps x4, f4, 7\n"                               
			"frcp.ps f6, f3\n"                                    
			"fcmov.ps f7, f5, f5, f5\n"                           
			"fsra.pi f7, f7, f7\n"                                
			"maskpopc x3, m1\n"                                   
			"fsra.pi f5, f7, f5\n"                                
			"feq.ps f9, f0, f5\n"                                 
			"fle.pi f2, f1, f7\n"                                 
			"fexp.ps f8, f4\n"                                    
			"mova.x.m x3\n"                                       
			"fsub.pi f3, f0, f7\n"                                
			"fsub.pi f8, f6, f1\n"                                
			"fsgnjn.ps f3, f3, f3\n"                              
			"flog.ps f3, f7\n"                                    
			"fltu.pi f9, f2, f5\n"                                
			"fmul.ps f8, f5, f6, rne\n"                           
			"fmulhu.pi f9, f7, f7\n"                              
			"feq.pi f3, f7, f3\n"                                 
			"fcvt.s.wu f3, x11, rne\n"                            
			"fmsub.s f4, f4, f5, f0, rdn\n"                       
			"maskxor m0, m1, m2\n"                                
			"fslli.pi f6, f5, 15\n"                               
			"fsgnj.ps f2, f1, f0\n"                               
			"fsgnj.s f4, f7, f7\n"                                
			"fcvt.ps.pwu f5, f4, rup\n"                           
			"fmin.ps f8, f7, f4\n"                                
			"fmulhu.pi f7, f7, f3\n"                              
			"flq2 f8, -217(x11)\n"                                
			"fbci.pi f6, 265189\n"                                
			"fslli.pi f4, f5, 7\n"                                
			"flem.ps m1, f7, f2\n"                                
			"fsgnjn.ps f5, f6, f3\n"                              
			"fswizz.ps f6, f4, 7\n"                               
			"fcvt.wu.s x4, f2, rdn\n"                             
			//RTLMIN-5479"frsq.ps f6, f4\n"                                    
			"feq.s x4, f2, f3\n"                                  
			"fnot.pi f4, f6\n"                                    
			"fandi.pi f4, f6, 101\n"                              
			"fmvz.x.ps x3, f1, 3\n"                               
			"fmul.s f9, f6, f5, rtz\n"                            
			"fcvt.ps.pw f7, f5, rtz\n"                            
			"fsgnjn.s f4, f4, f5\n"                               
			"fsub.pi f7, f6, f0\n"                                
			"fcmovm.ps f5, f1, f4\n"                              
			"fandi.pi f5, f5, 166\n"                              
			"fsrai.pi f7, f3, 14\n"                               
			"fsetm.pi m0, f5\n"                                   
			"fle.ps f3, f5, f4\n"                                 
			"feq.s x3, f6, f2\n"                                  
			"fmaxu.pi f6, f4, f2\n"                               
			"fmulhu.pi f4, f3, f6\n"                              
			"ffrc.ps f3, f6\n"                                    
			"fsrai.pi f6, f4, 2\n"                                
			"fsgnj.s f2, f1, f4\n"                                
			"fsgnjn.ps f8, f6, f3\n"                              
			"feqm.ps m1, f5, f0\n"                                
			"fadd.pi f3, f1, f2\n"                                
			//RTLMIN-5479"fsin.ps f5, f5\n"                                    
			"fsgnj.s f9, f2, f1\n"                                

			"li x5, 0xe220bbe827d00b91\n"
			"mova.m.x x5\n"                                       
			"flem.ps m1, f2, f3\n"                                
			"fsrli.pi f5, f7, 6\n"                                
			"flw.ps f7, 1185(x11)\n"                              
			"fcmovm.ps f8, f1, f5\n"                              
			"fle.s x3, f0, f5\n"                                  
			"faddi.pi f3, f5, 426\n"                              
			"fsgnjn.ps f2, f3, f6\n"                              
			"fsw.ps f3, -386(x11)\n"                              
			"fsgnjn.ps f5, f4, f2\n"                              
			"fsgnjx.s f4, f3, f4\n"                               
			"fcvt.ps.f16 f7, f5\n"                                
			"fmul.pi f6, f5, f2\n"                                
			"maskand m2, m2, m1\n"                                
			"fcvt.pw.ps f6, f3, rmm\n"                            
			"fcvt.pwu.ps f7, f0, dyn\n"                           
			"fltu.pi f9, f5, f7\n"                                
			"fmin.ps f6, f3, f0\n"                                
			"fsub.ps f7, f7, f0, dyn\n"                           
			"fsq2 f0, -614(x11)\n"                                
			"fsrl.pi f5, f7, f4\n"                                
			//RTLMIN-5479"frsq.ps f4, f5\n"                                    
			"fmin.ps f3, f4, f7\n"                                
			"fmv.x.s x4, f5\n"                                    
			"maskor m1, m2, m0\n"                                 
			"fbc.ps f6, 1190(x11)\n"                              
			"mova.x.m x4\n"                                       
			"feq.s x3, f0, f1\n"                                  
			"fcvt.f16.ps f6, f0\n"                                
			"for.pi f9, f2, f5\n"                                 
			"frcp.ps f5, f6\n"                                    
			"fnmadd.ps f4, f3, f5, f4, rne\n"                     
			"fmax.s f2, f1, f6\n"                                 
			"flem.ps m0, f7, f7\n"                                
			"fxor.pi f7, f7, f5\n"                                
			"flog.ps f9, f3\n"                                    
			"frcp.ps f4, f0\n"                                    
			"fcmovm.ps f3, f4, f0\n"                              
			"flt.ps f4, f4, f2\n"                                 
			"flt.pi f2, f7, f6\n"                                 
			"flt.s x4, f6, f6\n"                                  
			"feq.ps f6, f0, f7\n"                                 
			"fmsub.s f3, f0, f4, f2, rdn\n"                       
			"fmadd.ps f2, f1, f5, f1, dyn\n"                      
			"mova.x.m x4\n"                                       
			"flt.pi f9, f5, f6\n"                                 
			"fadd.pi f2, f5, f2\n"                                
			"feq.ps f5, f5, f3\n"                                 
			"fexp.ps f2, f2\n"                                    
			"fxor.pi f6, f6, f7\n"                                
			"fandi.pi f3, f6, 60\n"                               
			"fmin.pi f6, f4, f5\n"                                
			"fmin.s f6, f2, f1\n"                                 
			//RTLMIN-5479"fsin.ps f9, f5\n"                                    
			"fmulhu.pi f6, f1, f3\n"                              
			"fnmsub.ps f8, f0, f3, f0, rne\n"                     
			"feq.pi f3, f5, f2\n"                                 
			"fcvt.pw.ps f5, f6, rtz\n"                            
			"fminu.pi f3, f1, f2\n"                               
			"fmadd.ps f5, f1, f7, f1, rne\n"                      
			"fsgnj.ps f5, f3, f4\n"                               
			"flw.ps f6, -120(x11)\n"                              
			"fsgnjx.ps f2, f3, f7\n"                              
			"fbci.ps f3, 106394\n"                                
			"fbci.pi f2, 366800\n"                                
			"fandi.pi f2, f6, 378\n"                              
			"fcmovm.ps f6, f5, f1\n"                              
			"fcvt.pw.ps f6, f6, rup\n"                            
			"faddi.pi f5, f2, 381\n"                              
			"fcvt.s.wu f4, x11, rne\n"                            
			"fmul.s f4, f5, f6, rmm\n"                            
			"fsrai.pi f8, f7, 8\n"                                
			"fsub.s f3, f5, f4, rtz\n"                            
			"fsgnjx.ps f9, f3, f7\n"                              
			"fcvt.f16.ps f6, f7\n"                                
			"fsgnjn.ps f7, f5, f2\n"                              
			"fsatu8.pi f8, f4\n"                                  
			"feq.s x4, f0, f0\n"                                  
			"fsrl.pi f9, f0, f4\n"                                
			"fcmovm.ps f5, f3, f0\n"                              
			"fpackreph.pi f8, f3\n"                               
			"fcvt.pw.ps f4, f2, dyn\n"                            
			"fbcx.ps f3, x11\n"                                   
			"fle.s x4, f5, f7\n"                                  
			"fmulh.pi f5, f3, f3\n"                               
			"feq.pi f7, f0, f1\n"                                 
			"fltu.pi f5, f2, f4\n"                                
			"fcvt.ps.pwu f3, f4, rne\n"                           
			"fclass.s x3, f7\n"                                   
			"fltm.ps m0, f5, f1\n"                                
			"flw f9, 591(x11)\n"                                  
			"fsgnjx.ps f8, f6, f0\n"                              
			"fcvt.w.s x3, f6, rmm\n"                              
			"fmulhu.pi f4, f1, f0\n"                              
			"fnot.pi f7, f6\n"                                    
			//RTLMIN-5479"frsq.ps f4, f0\n"                                    
			//RTLMIN-5479"frsq.ps f8, f6\n"                                    
			"fsgnj.ps f7, f1, f3\n"                               
			"maskpopcz x3, m2\n"                                  
			"feq.ps f4, f2, f4\n"                                 
			"fnmadd.s f4, f2, f6, f5, rmm\n"                      
			"fnmsub.s f5, f5, f5, f4, rmm\n"                      
			"fsat8.pi f8, f4\n"                                   
			"fslli.pi f5, f6, 0\n"                                
			"fnmadd.ps f8, f3, f1, f1, rtz\n"                     
			"flem.ps m2, f4, f3\n"                                
			"flq2 f6, -1461(x11)\n"                               
			"fcvt.ps.f16 f8, f7\n"                                
			"flem.ps m1, f4, f0\n"                                
			"fclass.ps f5, f3\n"                                  
			"feq.s x3, f7, f5\n"                                  
			"fmv.s.x f3, x11\n"                                   
			"fnmsub.ps f4, f7, f2, f0, rne\n"                     
			"fle.pi f7, f6, f5\n"                                 
			"fclass.ps f3, f4\n"                                  
			"fmv.x.s x3, f4\n"                                    
			"fmin.s f8, f1, f7\n"                                 
			"fcvt.ps.pwu f9, f7, rdn\n"                           
			"fsq2 f0, -1708(x11)\n"                               
			"flem.ps m1, f0, f3\n"                                
			"fltm.pi m0, f6, f6\n"                                
			"fnmsub.ps f3, f2, f3, f4, rup\n"                     
			"fsatu8.pi f3, f4\n"                                  
			"feq.ps f8, f7, f3\n"                                 
			"fcvt.ps.f16 f7, f0\n"                                
			"fcmov.ps f2, f0, f0, f2\n"                           
			"fnmadd.ps f4, f1, f2, f6, rtz\n"                     
			"fmin.pi f9, f7, f1\n"                                
			"flw f9, 513(x11)\n"                                  
			"flog.ps f4, f0\n"                                    
			"fclass.ps f3, f4\n"                                  
			"fnmsub.s f2, f5, f1, f2, rmm\n"                      
			"fle.ps f8, f2, f0\n"                                 
			"feq.s x4, f2, f3\n"                                  
			"fsw f2, 1030(x11)\n"                                 
			"fsgnjn.s f3, f0, f5\n"                               

			"li x5, 0x7c28fa372d0155c4\n"
			"mova.m.x x5\n"                                       
			"fmul.pi f6, f1, f5\n"                                
			"fsrl.pi f4, f2, f1\n"                                
			"fcvt.pw.ps f2, f6, rdn\n"                            
			"fmax.s f7, f6, f5\n"                                 
			"fmax.pi f3, f3, f5\n"                                
			"fbci.pi f4, 274796\n"                                
			"fadd.s f2, f5, f0, rmm\n"                            
			"fsub.ps f7, f5, f4, rup\n"                           
			"fmadd.s f9, f2, f3, f2, rup\n"                       
			"fcvt.ps.f16 f5, f6\n"                                
			"fmadd.s f7, f2, f2, f1, rne\n"                       
			"fnmsub.s f2, f7, f6, f4, rtz\n"                      
			"fmulhu.pi f4, f5, f2\n"                              
			"fmax.s f2, f4, f3\n"                                 
			"fnmadd.s f5, f3, f1, f4, rtz\n"                      
			"fcvt.ps.f16 f7, f0\n"                                
			"masknot m0, m0\n"                                    
			"fsub.s f2, f0, f6, rtz\n"                            
			"fsll.pi f3, f2, f3\n"                                
			"fbc.ps f3, 1468(x11)\n"                              
			"fle.ps f9, f5, f4\n"                                 
			"fbc.ps f4, 346(x11)\n"                               
			"fnot.pi f2, f3\n"                                    
			"fsll.pi f9, f0, f5\n"                                
			"fcvt.s.w f2, x11, rmm\n"                             
			"fmax.pi f3, f2, f1\n"                                
			"fslli.pi f6, f5, 8\n"                                
			"fmadd.ps f7, f2, f2, f0, rtz\n"                      
			//RTLMIN-5479"fsin.ps f9, f4\n"                                    
			"fbci.pi f9, 207924\n"                                
			"fsgnjx.s f7, f0, f7\n"                               
			"fswizz.ps f6, f4, 11\n"                              
			"fclass.s x3, f2\n"                                   
			"fcvt.ps.pw f2, f6, rup\n"                            
			"fcvt.s.wu f7, x11, rmm\n"                            
			"fcvt.ps.pwu f7, f6, rne\n"                           
			"fmin.ps f8, f3, f4\n"                                
			"feq.ps f7, f0, f3\n"                                 
			"fltm.ps m2, f0, f2\n"                                
			"mov.m.x m1, x11, 123\n"                              
			"feqm.ps m0, f0, f0\n"                                
			"fsq2 f3, 771(x11)\n"                                 
			"fmv.s.x f2, x11\n"                                   
			"fmvs.x.ps x3, f6, 7\n"                               
			"fcvt.s.w f9, x11, rne\n"                             
			"fcvt.w.s x3, f3, rmm\n"                              
			"fmv.s.x f8, x11\n"                                   
			"fmvs.x.ps x3, f2, 1\n"                               
			"fmin.ps f3, f3, f2\n"                                
			"mov.m.x m2, x11, 98\n"                               
			"fmin.pi f7, f2, f1\n"                                
			"fnmsub.ps f6, f1, f1, f3, rne\n"                     
			"flt.ps f7, f7, f0\n"                                 
			"fsat8.pi f8, f7\n"                                   
			"feq.ps f9, f7, f7\n"                                 
			"fsgnjn.ps f3, f2, f5\n"                              
			"fsgnjn.s f9, f0, f2\n"                               
			"fbci.pi f7, 430166\n"                                
			"fmvs.x.ps x3, f5, 1\n"                               
			"fltm.pi m1, f5, f2\n"                                
			"feq.pi f6, f5, f1\n"                                 
			"fadd.ps f9, f1, f1, rup\n"                           
			"fcvt.ps.pwu f3, f1, rmm\n"                           
			"flw.ps f7, 119(x11)\n"                               
			"fle.ps f2, f6, f6\n"                                 
			"fltm.ps m1, f4, f7\n"                                
			"fbcx.ps f7, x11\n"                                   
			"feqm.ps m2, f6, f7\n"                                
			"mova.x.m x3\n"                                       
			"fmv.x.s x3, f6\n"                                    
			"fnmsub.s f6, f4, f7, f5, rdn\n"                      
			"maskxor m1, m2, m0\n"                                
			"fnmadd.ps f2, f5, f2, f7, rmm\n"                     
			"fcvt.wu.s x3, f7, rmm\n"                             
			"for.pi f7, f7, f6\n"                                 
			"fmin.s f2, f6, f0\n"                                 
			"fmax.pi f9, f0, f6\n"                                
			"flw f6, -1847(x11)\n"                                
			"fsgnjx.ps f9, f0, f7\n"                              
			"fsw.ps f3, 227(x11)\n"                               
			"flq2 f4, -1090(x11)\n"                               
			"fsat8.pi f8, f5\n"                                   
			"feq.ps f4, f0, f6\n"                                 
			"fmulhu.pi f5, f2, f0\n"                              
			"fadd.s f6, f6, f5, rne\n"                            
			"flem.ps m2, f4, f4\n"                                
			"fsgnj.s f9, f4, f2\n"                                
			"ffrc.ps f2, f1\n"                                    
			"fswizz.ps f4, f2, 17\n"                              
			"maskor m1, m0, m0\n"                                 
			"fpackrepb.pi f3, f3\n"                               
			"flq2 f2, -463(x11)\n"                                
			"fltm.ps m2, f6, f1\n"                                
			"fnmsub.ps f2, f6, f2, f3, rne\n"                     
			"fswizz.ps f2, f2, 125\n"                             
			"fswizz.ps f9, f1, 32\n"                              
			"fsrai.pi f6, f5, 10\n"                               
			"maskpopcz x3, m2\n"                                  
			"fadd.s f5, f4, f4, rup\n"                            
			"fsetm.pi m2, f7\n"                                   
			"fmulhu.pi f6, f5, f4\n"                              
			"fmsub.s f5, f3, f4, f4, rne\n"                       
			"flw f7, -593(x11)\n"                                 
			"flq2 f5, -1754(x11)\n"                               
			"fsgnjn.ps f2, f7, f5\n"                              
			"fcvt.wu.s x4, f2, dyn\n"                             
			"fsgnjx.s f6, f2, f7\n"                               
			"feqm.ps m1, f4, f5\n"                                
			"fclass.ps f7, f0\n"                                  
			"fcvt.w.s x4, f6, rdn\n"                              
			"fmax.pi f6, f2, f3\n"                                
			"maskxor m0, m2, m2\n"                                
			"fcmovm.ps f7, f0, f1\n"                              
			"fbc.ps f5, -617(x11)\n"                              
			"fsw f4, -478(x11)\n"                                 
			"fle.s x4, f6, f2\n"                                  
			//RTLMIN-5479"fsin.ps f5, f2\n"                                    
			"fmax.s f4, f6, f2\n"                                 
			"maskand m1, m2, m0\n"                                
			"for.pi f8, f5, f5\n"                                 
			"fsrli.pi f6, f2, 6\n"                                
			"flw f6, 933(x11)\n"                                  
			"fround.ps f9, f7, rmm\n"                             
			"fmsub.ps f5, f7, f4, f1, rdn\n"                      
			"fadd.ps f9, f4, f4, dyn\n"                           
			"fmulhu.pi f6, f1, f2\n"                              
			"fnmsub.s f8, f6, f6, f2, rdn\n"                      
			"flog.ps f7, f3\n"                                    
			"fand.pi f6, f3, f3\n"                                
			"fsgnjx.s f3, f0, f1\n"                               
			"maskor m2, m2, m1\n"                                 
			"fmvz.x.ps x3, f4, 1\n"                               
			"fsub.s f5, f5, f6, rmm\n"                            
			"fsub.s f7, f0, f1, rdn\n"                            
			"fbc.ps f4, -1119(x11)\n"                             
			"flq2 f7, 1097(x11)\n"                                
			"fnmsub.s f5, f5, f6, f6, rup\n"                      
			"fmul.pi f4, f0, f7\n"                                
			"fsat8.pi f4, f2\n"                                   
			"fclass.s x4, f2\n"                                   
			"fclass.ps f6, f2\n"                                  
			"fpackreph.pi f9, f5\n"                               
			"fsrl.pi f2, f7, f2\n"                                
			"fnmadd.s f9, f7, f6, f3, dyn\n"                      
			"fadd.ps f2, f2, f4, rtz\n"                           
			"fmulh.pi f3, f1, f0\n"                               
			"fandi.pi f6, f2, 406\n"                              
			"fsrl.pi f5, f0, f3\n"                                
			"for.pi f8, f4, f6\n"                                 
			"fsetm.pi m1, f5\n"                                   
			"fexp.ps f6, f1\n"                                    
			"flt.pi f9, f2, f7\n"                                 
			"fbc.ps f7, 1534(x11)\n"                              
			"fcmov.ps f7, f6, f3, f0\n"                           
			"flt.ps f4, f5, f1\n"                                 
			"fmadd.s f8, f5, f6, f6, rmm\n"                       
			"fltm.ps m0, f3, f2\n"                                
			"fcvt.ps.f16 f8, f6\n"                                
			"fltm.pi m1, f1, f2\n"                                
			"fsetm.pi m1, f3\n"                                   
			"fsw.ps f3, -1632(x11)\n"                             
			"fmin.ps f3, f0, f1\n"                                
			"flq2 f9, -499(x11)\n"                                
			"faddi.pi f3, f5, 501\n"                              
			"fslli.pi f6, f5, 3\n"                                
			"fcmov.ps f7, f3, f3, f5\n"                           
			"fandi.pi f9, f2, 154\n"                              
			"fmsub.ps f4, f3, f5, f4, rdn\n"                      
			"masknot m0, m1\n"                                    
			"fmv.x.s x4, f7\n"                                    
			"fxor.pi f8, f3, f2\n"                                
			"feqm.ps m2, f1, f1\n"                                
			"fsra.pi f7, f3, f2\n"                                
			"fcvt.wu.s x4, f7, rdn\n"                             
			"fsgnj.ps f4, f2, f7\n"                               
			"fand.pi f7, f2, f7\n"                                
			"fmsub.s f8, f4, f4, f3, rmm\n"                       
			"fnmsub.s f8, f2, f2, f7, dyn\n"                      
			"fcvt.ps.f16 f6, f7\n"                                
			"fsgnjn.ps f2, f5, f5\n"                              
			"fpackrepb.pi f9, f5\n"                               
			"fsrai.pi f5, f2, 15\n"                               
			"fbc.ps f6, 2034(x11)\n"                              
			"fsgnjn.ps f9, f1, f1\n"                              
			"fsgnjn.s f8, f1, f0\n"                               
			"fsra.pi f8, f7, f1\n"                                
			"fmaxu.pi f8, f5, f4\n"                               
			"fcvt.wu.s x3, f2, rup\n"                             
			"fcvt.ps.pwu f3, f6, rne\n"                           
			"feq.pi f4, f4, f7\n"                                 
			"ffrc.ps f7, f1\n"                                    
			"fsgnjx.ps f4, f6, f1\n"                              
			"fsetm.pi m2, f0\n"                                   
			"maskpopcz x4, m1\n"                                  
			"fmvs.x.ps x3, f2, 4\n"                               
			"fltu.pi f5, f3, f5\n"                                
			"fmadd.ps f4, f7, f6, f6, rup\n"                      
			"feqm.ps m0, f3, f3\n"                                
			"fsat8.pi f2, f5\n"                                   
			"fnmadd.s f9, f1, f1, f2, rdn\n"                      
			"fcvt.s.wu f5, x11, rne\n"                            
			"fsub.s f3, f3, f3, rmm\n"                            
			"fcvt.pwu.ps f2, f7, rtz\n"                           
			"fminu.pi f4, f0, f0\n"                               
			"fle.ps f7, f5, f1\n"                                 
			"fandi.pi f9, f2, 8\n"                                
			"fcvt.f16.ps f3, f2\n"                                
			"fcvt.ps.f16 f9, f0\n"                                
			"fnot.pi f7, f2\n"                                    
			"feq.s x3, f6, f5\n"                                  
			"fsub.ps f8, f1, f7, rne\n"                           
			"fbci.ps f7, 419250\n"                                
			"fand.pi f6, f7, f3\n"                                
			"fmaxu.pi f9, f1, f2\n"                               
			"flt.s x3, f3, f3\n"                                  
			"fcvt.s.w f6, x11, rup\n"                             
			"maskxor m0, m1, m1\n"                                
			"flem.ps m0, f5, f2\n"                                
			"fclass.s x4, f6\n"                                   
			"fltu.pi f8, f5, f6\n"                                
			"maskand m0, m2, m0\n"                                
			"fclass.s x4, f0\n"                                   
			"feq.pi f4, f6, f4\n"                                 
			"mov.m.x m0, x11, 100\n"                              
			//RTLMIN-5479"fsin.ps f4, f3\n"                                    
			"fnot.pi f3, f0\n"                                    
			"fsgnjx.s f5, f1, f7\n"                               
			"fsub.s f7, f2, f1, rmm\n"                            
			"fsll.pi f2, f4, f5\n"                                
			"fcvt.ps.pwu f8, f4, rdn\n"                           
			"maskpopcz x3, m0\n"                                  
			"maskpopcz x4, m2\n"                                  
			"fmsub.ps f9, f4, f5, f1, dyn\n"                      
			"fltm.pi m0, f6, f6\n"                                
			"fpackrepb.pi f9, f2\n"                               
			"fadd.ps f6, f0, f1, rne\n"                           
			"fswizz.ps f4, f4, 82\n"                              
			"fle.ps f4, f0, f6\n"                                 
			"fle.pi f7, f1, f7\n"                                 
			"fsll.pi f4, f5, f6\n"                                
			"fcvt.ps.f16 f3, f3\n"                                
			"fcmovm.ps f6, f3, f1\n"                              
			"maskand m2, m2, m1\n"                                
			"maskpopcz x3, m2\n"                                  
			"fsub.ps f2, f1, f5, rtz\n"                           
			"fcvt.s.wu f8, x11, rup\n"                            
			"fandi.pi f8, f4, 361\n"                              
			"fmul.s f6, f6, f4, rdn\n"                            
			"fmsub.s f3, f4, f3, f3, rne\n"                       
			"feq.s x4, f0, f4\n"                                  
			"fclass.s x4, f5\n"                                   
			"frcp.ps f3, f0\n"                                    
			"fsrl.pi f7, f2, f7\n"                                
			"feqm.ps m0, f0, f3\n"                                
			"fle.ps f7, f0, f4\n"                                 
			"fmin.pi f8, f5, f6\n"                                
			"flw f8, 1960(x11)\n"                                 
			"fnmadd.ps f5, f5, f6, f3, rne\n"                     
			"fmax.pi f5, f3, f3\n"                                
			"fcmovm.ps f6, f6, f6\n"                              
			"fround.ps f2, f1, rtz\n"                             
			"fmsub.ps f3, f3, f4, f6, rtz\n"                      
			"maskand m1, m2, m2\n"                                
			"fsgnjx.s f2, f0, f4\n"                               
			"flem.ps m2, f7, f2\n"                                
			"fslli.pi f7, f0, 14\n"                               
			"for.pi f4, f0, f3\n"                                 
			"fadd.pi f2, f2, f5\n"                                
			"fadd.pi f2, f6, f2\n"                                
			"fswizz.ps f6, f2, 24\n"                              
			"feq.s x3, f6, f4\n"                                  
			"fcvt.s.wu f7, x11, rdn\n"                            
			"frcp.ps f6, f6\n"                                    
			"fmaxu.pi f7, f0, f5\n"                               
			"fcvt.pwu.ps f5, f6, dyn\n"                           
			"fsatu8.pi f3, f4\n"                                  
			"mova.x.m x3\n"                                       
			"fnmadd.ps f7, f4, f5, f7, rdn\n"                     
			"fandi.pi f7, f7, 349\n"                              
			"maskand m2, m0, m1\n"                                
			"maskand m2, m0, m2\n"                                
			"fcvt.w.s x3, f3, dyn\n"                              
			"fcvt.pwu.ps f4, f3, rtz\n"                           
			"fltm.pi m1, f7, f3\n"                                
			"maskor m2, m1, m1\n"                                 
			"flq2 f9, 848(x11)\n"                                 
			"fsub.pi f6, f4, f0\n"                                
			"fcvt.ps.pw f5, f3, rup\n"                            
			"fcvt.f16.ps f9, f4\n"                                
			"fround.ps f5, f3, rdn\n"                             
			"fltu.pi f4, f2, f1\n"                                
			"fbcx.ps f6, x11\n"                                   
			"fcvt.ps.pwu f5, f1, rne\n"                           
			"maskor m0, m0, m2\n"                                 
			"fcvt.ps.pwu f7, f6, rne\n"                           
			"fsat8.pi f4, f5\n"                                   
			"flt.ps f5, f5, f7\n"                                 
			"fltm.ps m0, f4, f0\n"                                
			"flog.ps f3, f2\n"                                    
			"fcvt.s.w f6, x11, rne\n"                             
			"fnmsub.s f6, f3, f7, f7, rdn\n"                      
			"fclass.ps f4, f4\n"                                  
			"fsub.s f7, f7, f1, rne\n"                            
			"fmulh.pi f9, f3, f5\n"                               
			"fmin.ps f7, f2, f4\n"                                
			"fpackrepb.pi f3, f1\n"                               
			"flog.ps f5, f5\n"                                    
			"fcvt.pw.ps f3, f1, rup\n"                            
			"fsq2 f6, -524(x11)\n"                                
			"flem.ps m1, f4, f4\n"                                
			"maskor m0, m1, m1\n"                                 
			"flt.ps f6, f2, f0\n"                                 
			"fnmadd.ps f4, f2, f6, f2, rtz\n"                     
			"flw.ps f7, -363(x11)\n"                              
			"fcmov.ps f5, f0, f4, f7\n"                           
			"maskpopc x4, m2\n"                                   
			"fcvt.ps.f16 f8, f6\n"                                
			"fsgnjx.s f6, f7, f3\n"                               
			"fmin.ps f3, f4, f1\n"                                
			"flw.ps f4, -1413(x11)\n"                             

			"li x5, 0x529b6a082bafe8fb\n"
			"mova.m.x x5\n"                                       
			"fmv.s.x f7, x11\n"                                   
			"fsrai.pi f8, f1, 15\n"                               
			"fswizz.ps f8, f3, 43\n"                              
			"fltm.pi m1, f3, f6\n"                                
			"fcvt.ps.f16 f9, f5\n"                                
			"fexp.ps f5, f1\n"                                    
			"fand.pi f2, f0, f2\n"                                
			"fcvt.ps.f16 f7, f5\n"                                
			"fcmovm.ps f8, f4, f1\n"                              
			"fexp.ps f4, f4\n"                                    
			"fadd.s f7, f1, f3, rup\n"                            
			"maskand m0, m0, m0\n"                                
			"fcvt.wu.s x4, f1, rmm\n"                             
			"flq2 f4, -1394(x11)\n"                               
			"flq2 f7, 344(x11)\n"                                 
			"fmadd.s f6, f0, f6, f3, dyn\n"                       
			"mov.m.x m2, x11, 21\n"                               
			"fnmadd.ps f7, f6, f3, f3, rup\n"                     
			"fmvz.x.ps x3, f5, 1\n"                               
			"fsgnjn.ps f3, f7, f6\n"                              
			"fexp.ps f8, f4\n"                                    
			//RTLMIN-5479"frsq.ps f2, f5\n"                                    
			"flt.pi f5, f0, f2\n"                                 
			"fmv.s.x f5, x11\n"                                   
			"fsw f3, 569(x11)\n"                                  
			"fnot.pi f8, f4\n"                                    
			"fsgnjx.ps f8, f4, f4\n"                              
			"fsatu8.pi f5, f6\n"                                  
			"mova.x.m x4\n"                                       
			"fltm.ps m0, f3, f7\n"                                
			"fswizz.ps f8, f3, 14\n"                              
			//RTLMIN-5479"frsq.ps f9, f7\n"                                    
			"feqm.ps m1, f3, f0\n"                                

			"li x5, 0xe3a5f8455345e5f2\n"
			"mova.m.x x5\n"                                       
			"fltm.pi m0, f1, f7\n"                                
			"fnmsub.ps f2, f6, f1, f6, rtz\n"                     
			"fxor.pi f2, f4, f3\n"                                
			"fmaxu.pi f8, f0, f3\n"                               
			"fsgnj.s f9, f1, f1\n"                                
			"feqm.ps m0, f1, f7\n"                                
			"fbc.ps f5, 1875(x11)\n"                              
			"fmax.pi f3, f4, f5\n"                                
			"fsat8.pi f2, f7\n"                                   
			"maskor m2, m2, m2\n"                                 
			"maskpopcz x4, m0\n"                                  
			"fmaxu.pi f2, f4, f0\n"                               
			"fsgnjx.ps f5, f2, f6\n"                              
			"fmsub.s f2, f0, f6, f0, rdn\n"                       
			"fnmadd.s f8, f0, f3, f4, rdn\n"                      
			"fand.pi f6, f6, f2\n"                                
			"fle.pi f8, f0, f4\n"                                 
			"fslli.pi f3, f2, 1\n"                                
			"flq2 f9, -1123(x11)\n"                               
			"fminu.pi f8, f4, f5\n"                               
			"fsw f3, 226(x11)\n"                                  
			"fcvt.w.s x3, f0, dyn\n"                              
			"fmadd.s f6, f1, f1, f4, rtz\n"                       
			"fcmov.ps f5, f4, f3, f7\n"                           
			"fround.ps f8, f5, rtz\n"                             
			"fandi.pi f4, f1, 57\n"                               
			"for.pi f3, f6, f2\n"                                 
			"flt.s x3, f4, f0\n"                                  
			"fnot.pi f6, f5\n"                                    
			"fsatu8.pi f5, f2\n"                                  
			"fltm.pi m0, f3, f1\n"                                
			"masknot m1, m1\n"                                    
			"fltu.pi f3, f3, f3\n"                                
			"fmax.ps f9, f5, f7\n"                                
			"fxor.pi f4, f1, f0\n"                                
			"fmadd.ps f8, f2, f5, f6, rup\n"                      
			"fcvt.ps.f16 f9, f2\n"                                
			"fcvt.pwu.ps f8, f5, rne\n"                           
			"fmulh.pi f9, f5, f1\n"                               
			"fbci.pi f6, 23921\n"                                 
			"fpackrepb.pi f5, f2\n"                               
			"fmv.s.x f9, x11\n"                                   
			"fmvs.x.ps x3, f2, 2\n"                               
			"fminu.pi f8, f5, f1\n"                               
			"fadd.ps f5, f1, f2, rdn\n"                           
			"fmadd.ps f7, f4, f2, f3, rne\n"                      
			"fslli.pi f8, f0, 7\n"                                
			"fswizz.ps f3, f4, 92\n"                              
			"fmul.s f3, f2, f3, rup\n"                            
			"feq.s x3, f4, f1\n"                                  
			"maskxor m1, m1, m1\n"                                
			"mov.m.x m0, x11, 5\n"                                
			"fmin.s f2, f0, f2\n"                                 
			"fpackreph.pi f2, f1\n"                               
			"fminu.pi f3, f0, f0\n"                               
			"fcvt.wu.s x3, f0, rdn\n"                             
			"flt.ps f6, f0, f0\n"                                 
			"fcvt.ps.pw f9, f3, rup\n"                            

			"li x5, 0x27c955da339021d0\n"
			"mova.m.x x5\n"                                       
			"fsub.ps f3, f2, f3, dyn\n"                           
			//RTLMIN-5479"frsq.ps f2, f5\n"                                    
			"fsgnjn.ps f8, f1, f7\n"                              
			"fmax.s f3, f0, f4\n"                                 
			"feq.pi f3, f7, f6\n"                                 
			"fmin.s f8, f5, f0\n"                                 
			"maskand m0, m0, m0\n"                                
			"fadd.ps f7, f7, f4, rmm\n"                           
			"fltu.pi f8, f1, f5\n"                                
			"fle.ps f6, f7, f2\n"                                 
			"fmul.s f6, f4, f5, rne\n"                            
			"feq.s x4, f1, f7\n"                                  
			"masknot m0, m0\n"                                    
			"fmsub.s f4, f3, f1, f1, rne\n"                       
			"faddi.pi f3, f6, 27\n"                               
			"fadd.s f5, f7, f7, rmm\n"                            
			"for.pi f7, f6, f1\n"                                 
			"fmul.s f5, f4, f6, rup\n"                            
			"fsgnjx.s f8, f3, f3\n"                               
			"fsgnjx.s f2, f7, f0\n"                               
			"feq.pi f2, f6, f6\n"                                 
			"fsub.pi f5, f2, f3\n"                                
			"fsub.ps f2, f5, f7, rdn\n"                           
			"fmulh.pi f4, f1, f3\n"                               
			"fclass.ps f5, f0\n"                                  
			"flt.s x3, f5, f2\n"                                  
			"frcp.ps f6, f0\n"                                    
			"fcvt.s.w f9, x11, rdn\n"                             
			"fsgnjx.s f5, f1, f7\n"                               
			"fsgnj.ps f9, f5, f1\n"                               
			"fminu.pi f5, f2, f2\n"                               
			"fandi.pi f9, f5, 220\n"                              
			"fsll.pi f4, f5, f4\n"                                

			"li x5, 0x510bfe0dde3abeb6\n"
			"mova.m.x x5\n"                                       
			"fmin.pi f6, f7, f3\n"                                
			"maskpopc x4, m1\n"                                   
			"fltm.ps m0, f1, f4\n"                                
			"fmadd.s f5, f4, f6, f4, rup\n"                       
			"fcvt.ps.pw f7, f5, rmm\n"                            
			"fcvt.pw.ps f5, f4, rdn\n"                            
			"fadd.s f8, f1, f3, rtz\n"                            
			"fsrai.pi f5, f3, 7\n"                                
			"fmaxu.pi f5, f4, f6\n"                               
			"fcvt.pwu.ps f2, f0, dyn\n"                           
			"fpackreph.pi f6, f5\n"                               
			"fsat8.pi f3, f7\n"                                   
			"fsub.pi f4, f6, f7\n"                                
			"fmax.s f2, f4, f2\n"                                 
			"fbcx.ps f6, x11\n"                                   
			"flt.s x4, f1, f5\n"                                  
			"fmsub.s f4, f7, f3, f1, dyn\n"                       
			"flt.pi f2, f0, f1\n"                                 
			"fsgnjn.s f8, f5, f6\n"                               
			"fbci.ps f7, 518027\n"                                
			"feqm.ps m0, f7, f4\n"                                
			"fbcx.ps f6, x11\n"                                   
			"fsatu8.pi f2, f3\n"                                  
			"fmul.s f7, f3, f0, dyn\n"                            
			"fround.ps f5, f6, dyn\n"                             
			"feq.s x4, f3, f2\n"                                  
			"flw f6, -1977(x11)\n"                                
			"fsgnjx.s f9, f7, f6\n"                               
			"fmsub.ps f3, f0, f4, f1, rtz\n"                      
			"fltu.pi f3, f6, f1\n"                                
			"ffrc.ps f3, f1\n"                                    
			"fcvt.wu.s x3, f2, rtz\n"                             
			"fxor.pi f7, f4, f0\n"                                
			"fsgnjn.s f9, f4, f5\n"                               
			"fmsub.s f5, f1, f0, f5, rdn\n"                       
			"fclass.ps f8, f3\n"                                  
			"fsq2 f5, 41(x11)\n"                                  
			"feq.s x3, f5, f3\n"                                  
			"flt.pi f9, f1, f1\n"                                 
			"faddi.pi f4, f1, 352\n"                              
			"fclass.ps f9, f5\n"                                  
			"fpackrepb.pi f2, f0\n"                               
			"fsgnjn.ps f2, f1, f1\n"                              
			"maskand m2, m2, m0\n"                                
			"fcvt.f16.ps f7, f5\n"                                
			"fslli.pi f6, f4, 15\n"                               
			"maskpopcz x3, m2\n"                                  
			"fsgnj.s f3, f5, f1\n"                                
			"fcvt.pwu.ps f8, f4, rne\n"                           
			"fmin.ps f2, f5, f3\n"                                
			"fle.ps f4, f5, f3\n"                                 
			"flt.pi f9, f6, f6\n"                                 
			"fcvt.wu.s x3, f5, rtz\n"                             
			"fsgnjx.s f6, f7, f2\n"                               
			"maskpopc x4, m0\n"                                   
			"fadd.pi f3, f4, f3\n"                                
			"flt.pi f8, f0, f4\n"                                 
			"flt.pi f3, f2, f6\n"                                 
			"fnmadd.ps f8, f7, f5, f3, rdn\n"                     
			"fsub.s f8, f3, f5, rtz\n"                            
			"fexp.ps f6, f2\n"                                    
			"fcvt.ps.pwu f6, f5, rmm\n"                           
			"maskpopcz x3, m1\n"                                  
			"maskxor m1, m0, m1\n"                                
			"fslli.pi f6, f6, 9\n"                                
			"fcvt.s.w f5, x11, rdn\n"                             
			"feq.pi f3, f3, f7\n"                                 
			"fsll.pi f8, f7, f7\n"                                
			"fmul.ps f9, f0, f7, rtz\n"                           
			"mova.x.m x3\n"                                       
			"maskor m1, m2, m2\n"                                 
			"fmadd.s f5, f0, f1, f7, rmm\n"                       
			"feqm.ps m1, f1, f0\n"                                
			"fandi.pi f2, f6, 358\n"                              
			"feq.pi f7, f0, f7\n"                                 
			"fbc.ps f2, -1841(x11)\n"                             
			"mova.x.m x3\n"                                       
			"ffrc.ps f8, f2\n"                                    
			"fmin.ps f2, f6, f3\n"                                
			"fsgnjn.ps f2, f2, f3\n"                              
			"fcvt.s.w f4, x11, rup\n"                             
			"fmax.ps f8, f4, f7\n"                                
			"fadd.ps f5, f1, f2, dyn\n"                           
			"maskpopcz x4, m0\n"                                  
			//RTLMIN-5479"frsq.ps f9, f7\n"                                    
			"fbcx.ps f9, x11\n"                                   
			"masknot m1, m0\n"                                    
			"fcvt.ps.pw f7, f2, rne\n"                            
			"fcvt.ps.pwu f5, f2, rtz\n"                           
			"faddi.pi f2, f0, 170\n"                              
			"fsgnjx.ps f6, f4, f5\n"                              
			"fmvs.x.ps x3, f7, 0\n"                               
			"fnmadd.ps f8, f6, f4, f6, rtz\n"                     
			"fle.pi f4, f4, f3\n"                                 
			"fbci.pi f5, 25573\n"                                 
			"fmadd.s f7, f3, f7, f3, rup\n"                       
			"fsgnjn.s f5, f5, f7\n"                               
			"flq2 f9, 1721(x11)\n"                                
			"fmulh.pi f8, f2, f2\n"                               
			"for.pi f3, f3, f5\n"                                 
			"flem.ps m2, f0, f0\n"                                
			"feqm.ps m1, f4, f0\n"                                
			"fmul.ps f4, f1, f7, rne\n"                           
			"fsgnj.ps f7, f7, f5\n"                               
			"fcvt.w.s x4, f5, dyn\n"                              
			"masknot m1, m1\n"                                    
			"flw f7, -2035(x11)\n"                                
			"fmul.pi f4, f5, f2\n"                                
			"fminu.pi f4, f6, f4\n"                               
			"fnmsub.ps f9, f7, f3, f3, rup\n"                     
			"fltm.ps m2, f1, f3\n"                                
			"fsq2 f7, -1838(x11)\n"                               
			"masknot m0, m2\n"                                    
			"fltm.ps m1, f6, f7\n"                                
			"fcvt.s.w f6, x11, rdn\n"                             
			"fmadd.s f2, f3, f7, f5, rdn\n"                       
			"feq.s x4, f3, f5\n"                                  
			"fmax.pi f5, f1, f1\n"                                
			"fcvt.s.w f6, x11, rtz\n"                             
			"fmulh.pi f8, f6, f7\n"                               
			"fmulh.pi f3, f2, f2\n"                               
			"fminu.pi f6, f3, f0\n"                               
			"fsgnjx.ps f6, f6, f5\n"                              
			"fsq2 f5, -1209(x11)\n"                               
			"fsrli.pi f5, f5, 11\n"                               
			"fminu.pi f2, f4, f0\n"                               
			"flw f2, 611(x11)\n"                                  
			"fexp.ps f7, f5\n"                                    
			"fle.pi f2, f6, f4\n"                                 
			"fltm.ps m0, f0, f3\n"                                
			"fsgnjx.s f8, f0, f5\n"                               
			"fsatu8.pi f7, f1\n"                                  
			"fnmadd.s f9, f5, f6, f3, dyn\n"                      
			"fslli.pi f7, f4, 8\n"                                
			"fsat8.pi f2, f4\n"                                   
			"mov.m.x m1, x11, 89\n"                               
			"fcvt.pw.ps f6, f0, dyn\n"                            
			"fswizz.ps f3, f1, 27\n"                              
			"fmvz.x.ps x3, f7, 4\n"                               
			"fltm.ps m2, f6, f5\n"                                
			"fnmsub.s f9, f0, f3, f7, dyn\n"                      
			//RTLMIN-5479"fsin.ps f6, f7\n"                                    
			"maskpopcz x3, m2\n"                                  
			"masknot m2, m0\n"                                    
			"fsrli.pi f4, f4, 11\n"                               
			"maskpopc x4, m0\n"                                   
			"fmul.ps f4, f1, f4, rtz\n"                           
			"fandi.pi f6, f0, 194\n"                              
			"fnmsub.ps f9, f5, f0, f5, rup\n"                     
			"fslli.pi f5, f6, 14\n"                               
			"fcvt.pw.ps f6, f3, rup\n"                            
			"flem.ps m1, f5, f6\n"                                
			"fadd.s f2, f1, f6, rne\n"                            
			"fxor.pi f2, f0, f3\n"                                
			"fsgnj.ps f8, f4, f0\n"                               
			"mov.m.x m2, x11, 53\n"                               
			"flw f8, -1678(x11)\n"                                
			"flog.ps f5, f3\n"                                    
			"mova.x.m x4\n"                                       
			"fcvt.f16.ps f4, f2\n"                                
			"fclass.ps f8, f5\n"                                  
			"fcvt.ps.f16 f2, f5\n"                                
			"fcmovm.ps f4, f0, f2\n"                              
			"fmadd.ps f7, f7, f6, f4, dyn\n"                      
			"mov.m.x m2, x11, 121\n"                              
			"fle.s x4, f2, f5\n"                                  
			"fcvt.f16.ps f9, f5\n"                                
			//RTLMIN-5479"frsq.ps f9, f0\n"                                    
			"fadd.pi f9, f1, f5\n"                                
			"fbci.pi f6, 16958\n"                                 
			"feq.ps f2, f3, f4\n"                                 
			"fsgnjn.s f5, f6, f5\n"                               
			"masknot m0, m0\n"                                    
			"fmadd.s f9, f5, f7, f0, rdn\n"                       
			"fbc.ps f5, 613(x11)\n"                               
			"fsgnj.ps f4, f2, f0\n"                               
			"maskand m1, m2, m0\n"                                
			"fbci.ps f4, 124244\n"                                
			"fsatu8.pi f9, f0\n"                                  
			"fmadd.s f7, f4, f2, f0, rtz\n"                       
			"fexp.ps f4, f0\n"                                    
			"fsub.pi f8, f5, f0\n"                                
			"fsll.pi f7, f0, f3\n"                                
			"fsgnjn.ps f9, f7, f0\n"                              
			"fmadd.s f3, f6, f3, f2, rtz\n"                       
			"fandi.pi f2, f5, 449\n"                              
			"fmulhu.pi f7, f4, f5\n"                              
			"fmulh.pi f9, f1, f1\n"                               
			"maskxor m0, m1, m2\n"                                
			"fpackreph.pi f6, f1\n"                               
			"feq.s x3, f1, f5\n"                                  
			"fxor.pi f5, f5, f4\n"                                
			"fand.pi f4, f5, f7\n"                                
			"fmin.s f6, f0, f4\n"                                 
			"fmul.s f9, f1, f4, rmm\n"                            
			"fcvt.w.s x4, f7, rne\n"                              
			"fcvt.ps.f16 f8, f1\n"                                
			"flw.ps f5, 1870(x11)\n"                              

			"li x5, 0x2ac109c491e74a06\n"
			"mova.m.x x5\n"                                       
			"fmv.s.x f9, x11\n"                                   
			"fsgnjn.s f6, f2, f2\n"                               
			"fclass.s x4, f7\n"                                   
			"fcvt.pw.ps f6, f2, rup\n"                            
			"fmaxu.pi f8, f1, f7\n"                               
			"fandi.pi f5, f5, 481\n"                              
			"fsra.pi f4, f3, f0\n"                                
			"fminu.pi f2, f3, f3\n"                               
			"fsrli.pi f7, f5, 2\n"                                
			"flw f4, -1528(x11)\n"                                
			"flw f5, 1484(x11)\n"                                 
			"fsll.pi f3, f1, f6\n"                                
			"fadd.pi f4, f0, f1\n"                                
			"fcvt.wu.s x4, f3, rmm\n"                             
			"fpackrepb.pi f8, f7\n"                               
			"fmax.s f6, f1, f0\n"                                 
			"fpackrepb.pi f4, f7\n"                               
			"for.pi f9, f0, f7\n"                                 
			"flq2 f3, 549(x11)\n"                                 
			"fcvt.s.wu f8, x11, rup\n"                            
			"mov.m.x m1, x11, 118\n"                              
			"fadd.pi f4, f2, f7\n"                                
			"fnmadd.ps f3, f3, f0, f6, rup\n"                     
			"ffrc.ps f4, f6\n"                                    
			"fbci.ps f3, 463280\n"                                
			"fmaxu.pi f9, f0, f3\n"                               
			"fltm.pi m0, f7, f6\n"                                
			"fadd.pi f8, f3, f3\n"                                
			"fcvt.s.w f7, x11, rdn\n"                             
			"flt.pi f9, f5, f6\n"                                 
			"fsat8.pi f8, f6\n"                                   
			"fswizz.ps f4, f3, 34\n"                              
			"fcmovm.ps f7, f0, f7\n"                              
			"fsgnjn.s f9, f4, f3\n"                               
			"feq.pi f6, f0, f6\n"                                 
			"fltu.pi f4, f3, f3\n"                                
			"fround.ps f7, f5, dyn\n"                             
			"fmul.pi f3, f1, f1\n"                                
			"feq.ps f3, f5, f2\n"                                 
			"fadd.pi f8, f0, f6\n"                                
			"fcvt.wu.s x4, f4, rdn\n"                             
			"masknot m0, m0\n"                                    
			"fbci.pi f8, 333301\n"                                
			"fsgnjx.s f8, f5, f5\n"                               
			"fround.ps f7, f4, rne\n"                             
			"fbcx.ps f7, x11\n"                                   
			"fmsub.ps f6, f0, f4, f4, rdn\n"                      
			"fsub.ps f8, f3, f0, rtz\n"                           
			"fcvt.ps.pw f6, f7, rmm\n"                            
			"fsub.ps f3, f1, f2, rmm\n"                           
			"fltm.pi m0, f4, f4\n"                                
			"fpackrepb.pi f3, f0\n"                               
			"fround.ps f5, f0, rup\n"                             
			"fmadd.ps f3, f6, f7, f4, rup\n"                      
			"fltm.pi m1, f3, f4\n"                                
			"fcmovm.ps f7, f7, f4\n"                              
			"fsatu8.pi f7, f6\n"                                  
			"fcvt.s.w f6, x11, rne\n"                             
			"fsgnjx.ps f6, f2, f6\n"                              
			"fmaxu.pi f5, f7, f5\n"                               
			"maskor m0, m0, m0\n"                                 
			"flw.ps f5, -221(x11)\n"                              
			"maskpopcz x3, m0\n"                                  
			"fbci.pi f3, 295624\n"                                
			"flq2 f8, -454(x11)\n"                                
			"fadd.s f3, f5, f0, rup\n"                            
			"fmsub.ps f7, f7, f7, f1, rdn\n"                      
			"fsrai.pi f9, f3, 0\n"                                
			"fsetm.pi m2, f1\n"                                   
			"maskpopcz x3, m0\n"                                  
			"fcvt.ps.pwu f8, f3, rne\n"                           
			"fandi.pi f6, f7, 350\n"                              
			"fandi.pi f3, f1, 309\n"                              
			"flt.ps f4, f7, f6\n"                                 
			"fsat8.pi f2, f0\n"                                   
			"fsrli.pi f8, f4, 9\n"                                
			"fle.pi f9, f7, f6\n"                                 
			"fsub.pi f2, f1, f3\n"                                
			"fmulhu.pi f5, f2, f3\n"                              
			"fsgnj.ps f2, f1, f4\n"                               
			"fcvt.s.wu f6, x11, rmm\n"                            
			"flog.ps f2, f7\n"                                    
			"fbci.ps f8, 129745\n"                                
			"fsw.ps f7, -1111(x11)\n"                             
			"fmvz.x.ps x3, f0, 1\n"                               
			"fle.ps f7, f1, f6\n"                                 
			"fmax.pi f3, f2, f3\n"                                
			"fmax.s f5, f2, f5\n"                                 
			"fnmadd.s f7, f4, f0, f5, rtz\n"                      
			"fsub.s f3, f3, f2, rmm\n"                            
			"fmul.s f7, f7, f0, rne\n"                            
			"flog.ps f6, f5\n"                                    
			"fmulh.pi f9, f2, f4\n"                               
			"fbci.ps f9, 239794\n"                                
			"fcvt.ps.pwu f3, f4, rmm\n"                           
			"fmsub.s f9, f2, f3, f7, dyn\n"                       
			"fsrai.pi f4, f0, 8\n"                                
			"fsw.ps f3, -1160(x11)\n"                             
			"fmulhu.pi f3, f6, f0\n"                              
			"fxor.pi f6, f5, f1\n"                                
			"fcvt.ps.pwu f9, f0, rmm\n"                           
			"for.pi f7, f5, f3\n"                                 
			"fle.s x4, f4, f4\n"                                  
			"fnot.pi f4, f0\n"                                    
			"fmax.pi f3, f0, f5\n"                                
			"fsub.pi f5, f4, f3\n"                                
			"fminu.pi f4, f7, f4\n"                               
			"fltm.ps m0, f2, f2\n"                                
			"frcp.ps f6, f2\n"                                    
			"flt.ps f2, f6, f2\n"                                 
			"fcvt.w.s x4, f4, rtz\n"                              
			"mov.m.x m0, x11, 113\n"                              
			"fsub.ps f7, f6, f0, rmm\n"                           
			"fcvt.pw.ps f3, f2, rmm\n"                            
			"fltu.pi f6, f4, f6\n"                                
			"fsrai.pi f5, f5, 1\n"                                
			"fnmsub.s f8, f2, f5, f5, rmm\n"                      
			"fmadd.s f5, f2, f1, f5, rne\n"                       
			"fmaxu.pi f4, f4, f4\n"                               
			"fmv.s.x f8, x11\n"                                   
			"feq.pi f7, f6, f1\n"                                 
			"mov.m.x m2, x11, 55\n"                               
			"fltu.pi f5, f2, f1\n"                                
			"fminu.pi f2, f6, f1\n"                               
			"fcvt.s.wu f9, x11, rtz\n"                            
			"fle.s x3, f7, f0\n"                                  
			"fsrl.pi f4, f2, f2\n"                                
			"flog.ps f7, f6\n"                                    
			"fbcx.ps f9, x11\n"                                   
			"fandi.pi f4, f0, 36\n"                               
			"ffrc.ps f5, f4\n"                                    
			"fsgnjx.ps f3, f3, f4\n"                              
			"fltu.pi f3, f3, f1\n"                                
			"fsetm.pi m0, f5\n"                                   
			"feqm.ps m2, f0, f5\n"                                
			"fbci.ps f7, 422426\n"                                
			"fsgnj.ps f5, f0, f7\n"                               
			"flem.ps m1, f5, f5\n"                                
			"faddi.pi f2, f5, 191\n"                              
			"fnmadd.ps f7, f1, f5, f0, rup\n"                     
			"fclass.ps f7, f4\n"                                  
			"fbci.ps f2, 266048\n"                                
			"flw f9, -1245(x11)\n"                                
			"fsq2 f5, 1909(x11)\n"                                
			"fcvt.ps.f16 f7, f4\n"                                
			"fle.s x4, f2, f2\n"                                  
			"fswizz.ps f4, f1, 91\n"                              
			"fmsub.ps f3, f7, f4, f0, rtz\n"                      
			"fsrli.pi f6, f0, 9\n"                                
			"fminu.pi f5, f6, f7\n"                               
			"fmax.s f2, f4, f1\n"                                 
			"ffrc.ps f2, f1\n"                                    
			"faddi.pi f7, f4, 430\n"                              
			"fmax.pi f9, f6, f3\n"                                
			"faddi.pi f3, f0, 473\n"                              
			"fmsub.s f2, f5, f1, f6, rne\n"                       
			"fbcx.ps f7, x11\n"                                   
			"fpackreph.pi f5, f1\n"                               
			"fltm.ps m0, f2, f2\n"                                
			"fsgnjx.s f6, f5, f6\n"                               
			"fnmsub.ps f8, f0, f2, f1, rtz\n"                     
			"flw f2, 502(x11)\n"                                  
			"fsll.pi f8, f6, f6\n"                                
			"fmax.ps f8, f5, f2\n"                                
			"fmin.ps f3, f3, f1\n"                                
			"flw f3, 833(x11)\n"                                  
			"fsatu8.pi f7, f1\n"                                  
			"fcmovm.ps f5, f5, f5\n"                              
			"flt.ps f6, f2, f2\n"                                 
			"fandi.pi f4, f7, 405\n"                              
			"fandi.pi f7, f4, 242\n"                              
			"fclass.s x3, f7\n"                                   
			"fmulhu.pi f4, f2, f5\n"                              
			"fpackrepb.pi f7, f6\n"                               
			"fpackreph.pi f9, f7\n"                               
			"fround.ps f2, f0, rup\n"                             
			"fmin.s f9, f5, f3\n"                                 
			"fsgnj.s f6, f2, f3\n"                                
			"faddi.pi f2, f2, 283\n"                              
			"fcvt.ps.pwu f4, f6, rne\n"                           
			"fclass.ps f9, f0\n"                                  
			"mova.x.m x4\n"                                       
			"fmul.s f6, f6, f0, rdn\n"                            
			"fadd.ps f8, f3, f0, rup\n"                           
			"fmv.x.s x3, f6\n"                                    
			"fcvt.s.w f6, x11, rdn\n"                             
			"fsub.ps f8, f6, f0, rtz\n"                           
			"fcvt.pwu.ps f8, f0, rmm\n"                           
			"fadd.pi f9, f4, f2\n"                                
			"fsw.ps f5, 140(x11)\n"                               
			"fmsub.s f7, f5, f4, f6, rmm\n"                       
			"fsgnjx.ps f8, f3, f7\n"                              
			"fadd.pi f8, f5, f6\n"                                
			"fxor.pi f9, f3, f5\n"                                
			"fcvt.w.s x3, f2, rmm\n"                              
			"fexp.ps f2, f5\n"                                    
			"fbci.ps f2, 119349\n"                                
			"fnmsub.s f4, f0, f7, f3, rdn\n"                      
			"maskand m0, m1, m0\n"                                
			"frcp.ps f3, f2\n"                                    
			"fmin.ps f2, f1, f2\n"                                
			"fmin.s f2, f1, f2\n"                                 
			"fsat8.pi f6, f0\n"                                   
			"maskxor m0, m1, m2\n"                                
			"fbci.ps f4, 149739\n"                                
			"fcvt.ps.pw f8, f6, dyn\n"                            
			"fxor.pi f5, f0, f7\n"                                
			"maskxor m0, m1, m2\n"                                
			"fmsub.s f7, f5, f3, f7, rne\n"                       
			"fsw f5, 2004(x11)\n"                                 
			"fmul.pi f8, f5, f1\n"                                
			"fcvt.pw.ps f3, f2, rtz\n"                            
			"flem.ps m1, f5, f1\n"                                
			"fxor.pi f2, f3, f6\n"                                
			"maskor m1, m1, m1\n"                                 
			"fcvt.pwu.ps f5, f2, rup\n"                           
			"fmax.pi f7, f7, f4\n"                                
			//RTLMIN-5479"frsq.ps f7, f3\n"                                    
			"flem.ps m0, f3, f1\n"                                
			"fbcx.ps f3, x11\n"                                   
			"fmadd.s f7, f6, f6, f5, rup\n"                       
			"fsll.pi f7, f6, f2\n"                                
			"flw.ps f9, 1044(x11)\n"                              
			"fsgnjn.s f5, f1, f6\n"                               
			"fmvs.x.ps x4, f3, 0\n"                               
			"fmvz.x.ps x4, f2, 4\n"                               
			"fand.pi f9, f1, f3\n"                                
			"fle.pi f9, f7, f7\n"                                 
			"fmadd.ps f4, f5, f3, f1, dyn\n"                      
			"fand.pi f8, f3, f7\n"                                
			"fsgnj.ps f7, f4, f1\n"                               
			"fmulh.pi f8, f4, f7\n"                               
			"fnmsub.s f4, f2, f2, f6, rmm\n"                      
			"feq.pi f3, f5, f7\n"                                 
			"fmsub.ps f5, f5, f0, f0, rdn\n"                      
			"fnmsub.s f5, f7, f2, f4, dyn\n"                      
			"flq2 f7, 1061(x11)\n"                                
			"flt.pi f8, f6, f1\n"                                 
			"fsq2 f0, -1483(x11)\n"                               
			"fle.pi f2, f0, f2\n"                                 
			"masknot m1, m0\n"                                    
			"feq.ps f8, f2, f5\n"                                 
			"fsq2 f4, 1604(x11)\n"                                
			"fcvt.s.w f2, x11, dyn\n"                             
			"fmsub.s f9, f0, f4, f4, rdn\n"                       
			"fsrl.pi f6, f0, f7\n"                                
			"fsetm.pi m1, f6\n"                                   
			"fsat8.pi f5, f4\n"                                   
			"fcvt.w.s x3, f2, rtz\n"                              
			"fandi.pi f9, f0, 427\n"                              
			"fcvt.ps.pwu f5, f2, rne\n"                           
			"maskpopcz x4, m1\n"                                  
			"fclass.ps f3, f4\n"                                  
			//RTLMIN-5479"fsin.ps f7, f3\n"                                    
			"mov.m.x m1, x11, 51\n"                               
			"fmax.s f7, f2, f3\n"                                 
			"fsgnjx.ps f3, f7, f7\n"                              
			"maskpopcz x3, m2\n"                                  
			"fsub.pi f4, f2, f4\n"                                
			//RTLMIN-5479"fsin.ps f8, f5\n"                                    
			"fle.pi f9, f1, f1\n"                                 
			"fsrli.pi f5, f6, 15\n"                               
			"fmvs.x.ps x4, f4, 7\n"                               
			"fmax.pi f7, f1, f7\n"                                
			"fsgnjn.ps f8, f1, f2\n"                              
			"fswizz.ps f3, f5, 40\n"                              
			"feq.ps f6, f7, f4\n"                                 
			"fsll.pi f5, f1, f1\n"                                
			"fsrli.pi f8, f7, 2\n"                                
			"fmul.pi f2, f1, f4\n"                                
			"fcvt.ps.pw f6, f2, rup\n"                            
			"fsub.pi f5, f2, f1\n"                                
			"feqm.ps m0, f2, f2\n"                                
			"fsw f4, -320(x11)\n"                                 
			"fsgnjn.s f9, f1, f7\n"                               
			//RTLMIN-5479"frsq.ps f9, f0\n"                                    
			"fcvt.f16.ps f6, f3\n"                                
			"fmsub.s f4, f6, f7, f5, dyn\n"                       
			"frcp.ps f6, f7\n"                                    
			"fandi.pi f3, f4, 131\n"                              
			"faddi.pi f2, f0, 130\n"                              
			"fcvt.f16.ps f3, f2\n"                                
			"fnmsub.ps f2, f3, f3, f7, rne\n"                     
			"fclass.s x4, f2\n"                                   
			"fmaxu.pi f6, f6, f3\n"                               
			"fmul.ps f7, f4, f2, rup\n"                           
			"flem.ps m2, f5, f4\n"                                
			"flw f3, 583(x11)\n"                                  
			"fsw f5, -1718(x11)\n"                                
			"fcvt.pwu.ps f4, f0, rmm\n"                           
			"fandi.pi f9, f1, 242\n"                              
			"fnmadd.ps f7, f7, f0, f7, rne\n"                     
			"fcvt.s.w f5, x11, dyn\n"                             
			"fmin.ps f9, f4, f1\n"                                
			"fmax.s f2, f7, f3\n"                                 
			"fadd.s f6, f2, f0, rne\n"                            
			"fnmadd.ps f2, f7, f5, f6, dyn\n"                     
			//RTLMIN-5479"frsq.ps f5, f5\n"                                    
			"fadd.ps f8, f7, f6, rdn\n"                           
			"flw f2, -446(x11)\n"                                 
			"fcmov.ps f9, f4, f1, f6\n"                           
			"fle.pi f9, f1, f4\n"                                 
			"fmul.ps f6, f7, f2, rtz\n"                           
			"masknot m1, m2\n"                                    
			"fswizz.ps f5, f4, 107\n"                             
			"fsq2 f0, 920(x11)\n"                                 
			"fexp.ps f3, f2\n"                                    
			"fmvz.x.ps x4, f2, 2\n"                               
			"fsrai.pi f7, f4, 12\n"                               
			"fcvt.ps.f16 f7, f3\n"                                
			"maskpopc x3, m0\n"                                   
			"fle.ps f5, f6, f7\n"                                 
			"masknot m2, m0\n"                                    
			"fsrl.pi f4, f3, f0\n"                                
			"maskor m0, m0, m2\n"                                 
			"fmsub.s f8, f7, f3, f6, rtz\n"                       
			"fcvt.s.w f5, x11, rmm\n"                             
			"fadd.s f5, f0, f6, rmm\n"                            
			"fcmovm.ps f7, f4, f7\n"                              
			"fcvt.f16.ps f2, f3\n"                                
			"fcvt.s.wu f2, x11, rup\n"                            
			"fmul.s f2, f7, f0, dyn\n"                            
			"fxor.pi f4, f3, f2\n"                                
			"faddi.pi f3, f1, 415\n"                              
			"fandi.pi f8, f5, 24\n"                               
			"fminu.pi f3, f1, f5\n"                               
			"fmulhu.pi f9, f5, f3\n"                              
			"fsw.ps f2, -1092(x11)\n"                             
			"fcvt.s.wu f9, x11, rdn\n"                            
			"fcmovm.ps f7, f6, f1\n"                              
			"ffrc.ps f5, f0\n"                                    
			"maskor m2, m0, m0\n"                                 
			"fsub.ps f3, f1, f3, dyn\n"                           
			"fbci.pi f8, 429475\n"                                
			"fcvt.f16.ps f7, f1\n"                                
			"fsgnjx.ps f7, f1, f5\n"                              
			"fmulhu.pi f5, f7, f5\n"                              
			"masknot m2, m0\n"                                    
			"fsub.pi f8, f2, f4\n"                                
			"flog.ps f6, f2\n"                                    
			"fcmov.ps f3, f5, f4, f7\n"                           
			"fsgnjn.ps f4, f6, f4\n"                              
			"fcvt.s.w f4, x11, rne\n"                             
			"fslli.pi f6, f6, 12\n"                               

			"li x5, 0xf197535dbd211de2\n"
			"mova.m.x x5\n"                                       
			"fmin.s f4, f5, f2\n"                                 
			"fxor.pi f9, f5, f5\n"                                
			"fmadd.ps f3, f2, f3, f4, rdn\n"                      
			"fround.ps f2, f2, dyn\n"                             
			"fmin.pi f8, f5, f6\n"                                
			"fmax.ps f8, f2, f2\n"                                
			//RTLMIN-5479"frsq.ps f2, f5\n"                                    
			"fandi.pi f7, f1, 257\n"                              
			"fcvt.pw.ps f2, f6, rdn\n"                            
			"flt.ps f2, f2, f6\n"                                 
			"flt.pi f2, f6, f6\n"                                 
			"fmax.pi f3, f1, f6\n"                                
			"fbcx.ps f6, x11\n"                                   
			"fbcx.ps f7, x11\n"                                   
			"fmv.x.s x3, f1\n"                                    
			"fpackreph.pi f4, f0\n"                               
			"fandi.pi f6, f1, 497\n"                              
			"fmvs.x.ps x3, f1, 7\n"                               
			"fmsub.s f5, f5, f1, f2, rtz\n"                       
			"fcvt.ps.pwu f7, f3, dyn\n"                           
			"fxor.pi f5, f2, f1\n"                                
			"flt.s x3, f0, f6\n"                                  
			"fround.ps f3, f4, rup\n"                             
			"fcvt.s.wu f2, x11, rmm\n"                            
			"fltm.ps m0, f2, f2\n"                                
			"fclass.s x4, f1\n"                                   
			"fsrli.pi f4, f1, 15\n"                               
			"flt.ps f4, f7, f3\n"                                 
			"fsub.ps f6, f6, f2, rmm\n"                           
			"maskor m0, m0, m0\n"                                 
			"fmulh.pi f8, f2, f5\n"                               
			"fcmovm.ps f9, f7, f7\n"                              
			"fle.pi f5, f3, f0\n"                                 
			"flw f4, -816(x11)\n"                                 
			"flq2 f5, -1339(x11)\n"                               
			"faddi.pi f7, f4, 504\n"                              
			"fsat8.pi f2, f7\n"                                   
			"fsw f5, -1751(x11)\n"                                
			"fnmsub.ps f2, f5, f6, f0, dyn\n"                     
			"fmsub.ps f9, f6, f0, f1, rdn\n"                      
			"fbc.ps f7, -1091(x11)\n"                             
			"fandi.pi f2, f3, 23\n"                               
			"fexp.ps f8, f2\n"                                    
			"frcp.ps f9, f5\n"                                    
			"faddi.pi f3, f5, 103\n"                              
			"fnmadd.ps f2, f0, f0, f6, rdn\n"                     
			"fmadd.ps f9, f4, f0, f0, rup\n"                      
			"fmsub.ps f8, f7, f5, f6, rup\n"                      
			"fmsub.s f4, f2, f4, f7, dyn\n"                       
			"fsq2 f4, 569(x11)\n"                                 
			"fsrl.pi f4, f5, f3\n"                                
			"fsgnj.ps f3, f4, f6\n"                               
			"fmsub.s f3, f6, f0, f4, rtz\n"                       
			"flw.ps f2, 617(x11)\n"                               
			"for.pi f6, f0, f6\n"                                 
			"fcvt.wu.s x3, f5, rup\n"                             
			"fsgnjx.s f7, f2, f5\n"                               
			"fclass.s x3, f1\n"                                   
			"fmul.s f3, f0, f6, dyn\n"                            
			"fsw.ps f5, -724(x11)\n"                              

			"li x5, 0xc4413ca33e4cc481\n"
			"mova.m.x x5\n"                                       
			"flt.pi f4, f2, f1\n"                                 
			"for.pi f9, f2, f3\n"                                 
			"fmaxu.pi f5, f0, f4\n"                               
			"fnot.pi f7, f2\n"                                    
			"frcp.ps f3, f7\n"                                    
			"fle.ps f4, f2, f6\n"                                 
			//RTLMIN-5479"frsq.ps f8, f3\n"                                    
			"fmsub.ps f8, f6, f0, f2, rne\n"                      
			"fround.ps f8, f5, rmm\n"                             
			"fltu.pi f6, f2, f7\n"                                
			"fsetm.pi m0, f2\n"                                   
			"flem.ps m1, f0, f4\n"                                
			"fbc.ps f2, 1331(x11)\n"                              
			"fle.ps f6, f5, f5\n"                                 
			"flog.ps f4, f1\n"                                    
			"fbci.ps f9, 335567\n"                                
			"fmadd.ps f2, f3, f1, f0, rmm\n"                      
			"fpackrepb.pi f6, f0\n"                               
			"maskpopcz x3, m1\n"                                  
			"fmvs.x.ps x4, f4, 7\n"                               
			"fmax.s f4, f2, f0\n"                                 
			"fminu.pi f9, f6, f4\n"                               
			"fcvt.s.w f4, x11, rne\n"                             
			"fsgnj.s f3, f0, f4\n"                                
			"fmsub.ps f8, f4, f7, f1, rmm\n"                      
			//RTLMIN-5479"fsin.ps f6, f7\n"                                    
			"fmin.s f9, f1, f0\n"                                 
			"fandi.pi f2, f5, 317\n"                              
			"fsub.ps f9, f0, f3, rtz\n"                           
			"fsgnjn.s f3, f4, f3\n"                               
			"fcvt.ps.pw f2, f7, rup\n"                            
			"mov.m.x m2, x11, 123\n"                              
			"fsw f4, -13(x11)\n"                                  
			"fand.pi f9, f5, f7\n"                                
			"fswizz.ps f8, f6, 17\n"                              
			"fmadd.ps f3, f2, f1, f6, rup\n"                      
			"fadd.s f5, f6, f1, rup\n"                            
			"feqm.ps m0, f6, f2\n"                                
			"for.pi f7, f1, f6\n"                                 
			"masknot m1, m1\n"                                    
			"fxor.pi f3, f2, f6\n"                                
			"fsub.s f8, f2, f4, rmm\n"                            
			"fle.ps f5, f0, f0\n"                                 
			"fround.ps f7, f1, dyn\n"                             
			"fnmadd.s f6, f3, f6, f6, dyn\n"                      
			"fmax.pi f8, f3, f2\n"                                
			"fmaxu.pi f7, f5, f5\n"                               
			"fle.ps f3, f7, f1\n"                                 
			"fmax.s f3, f3, f0\n"                                 
			"fsatu8.pi f5, f4\n"                                  
			"flem.ps m1, f1, f5\n"                                

			"li x5, 0x62ba5ff9af11acb1\n"
			"mova.m.x x5\n"                                       
			"fcvt.ps.pw f9, f5, rtz\n"                            
			"feq.pi f3, f3, f6\n"                                 
			"fsat8.pi f2, f4\n"                                   
			"fmsub.s f2, f5, f6, f3, rup\n"                       
			"fpackrepb.pi f4, f6\n"                               
			"fpackrepb.pi f3, f0\n"                               
			"fmsub.ps f6, f6, f3, f2, rtz\n"                      
			"fmadd.s f4, f3, f2, f1, rne\n"                       
			"fmul.pi f5, f4, f6\n"                                
			"fmvz.x.ps x4, f6, 4\n"                               
			"fbci.ps f5, 331415\n"                                
			//RTLMIN-5479"frsq.ps f3, f5\n"                                    
			"fltu.pi f2, f3, f2\n"                                
			"mova.x.m x3\n"                                       
			"fnmadd.ps f7, f2, f1, f0, rne\n"                     
			"fsra.pi f5, f5, f7\n"                                
			"fcvt.pwu.ps f3, f5, dyn\n"                           
			"fltu.pi f9, f2, f4\n"                                
			"fsub.s f7, f2, f1, rup\n"                            
			"fmulh.pi f9, f7, f2\n"                               
			"fbci.pi f7, 80111\n"                                 
			"fnmsub.ps f7, f3, f7, f3, rtz\n"                     
			"fle.ps f6, f7, f1\n"                                 
			"fmax.pi f6, f4, f5\n"                                
			"fle.pi f5, f6, f5\n"                                 
			"fsetm.pi m2, f7\n"                                   
			"fmadd.ps f5, f7, f2, f4, dyn\n"                      
			"fmul.s f3, f0, f7, rup\n"                            
			"fsgnj.s f4, f4, f5\n"                                
			"fsub.s f4, f5, f6, rmm\n"                            
			"flq2 f4, 1550(x11)\n"                                
			"fmsub.s f3, f2, f1, f3, dyn\n"                       
			"fmsub.ps f3, f2, f4, f7, rmm\n"                      
			"fsw f3, -169(x11)\n"                                 
			"fnmadd.ps f4, f1, f6, f4, rup\n"                     
			"fsq2 f3, 61(x11)\n"                                  
			"fsra.pi f5, f1, f6\n"                                
			"fmax.ps f4, f3, f1\n"                                
			"fand.pi f9, f3, f0\n"                                
			"fsw.ps f6, -812(x11)\n"                              
			"fsgnj.ps f2, f1, f4\n"                               
			"frcp.ps f8, f1\n"                                    
			"fle.ps f5, f3, f2\n"                                 
			"frcp.ps f5, f4\n"                                    
			"fmul.pi f5, f3, f0\n"                                
			"fnmsub.ps f6, f3, f0, f5, rup\n"                     
			"fmadd.ps f8, f6, f4, f2, rtz\n"                      
			"fnmadd.s f7, f6, f3, f2, rne\n"                      
			"fadd.s f4, f1, f4, rmm\n"                            
			"fcvt.ps.pwu f3, f3, rmm\n"                           
			"fmax.ps f3, f2, f4\n"                                
			"maskand m0, m1, m2\n"                                
			"feq.pi f4, f1, f7\n"                                 
			"fmulhu.pi f2, f7, f3\n"                              
			"fmax.ps f8, f3, f3\n"                                
			"fxor.pi f5, f3, f5\n"                                
			"fxor.pi f4, f0, f7\n"                                
			"faddi.pi f8, f5, 442\n"                              
			"flem.ps m2, f4, f5\n"                                
			"fandi.pi f6, f5, 15\n"                               
			"maskpopc x4, m1\n"                                   
			"faddi.pi f8, f2, 462\n"                              
			"fadd.pi f7, f7, f5\n"                                
			"fmax.s f9, f3, f7\n"                                 
			"fcvt.ps.f16 f5, f6\n"                                
			"fsub.ps f8, f2, f1, rmm\n"                           
			"fminu.pi f7, f0, f3\n"                               
			"flw.ps f5, 1728(x11)\n"                              
			"fsgnjx.s f8, f6, f4\n"                               
			"fbcx.ps f2, x11\n"                                   
			"fmaxu.pi f8, f1, f6\n"                               
			"fslli.pi f6, f1, 6\n"                                
			"fsgnjx.s f8, f3, f6\n"                               
			"fcvt.pw.ps f4, f6, rmm\n"                            
			"maskpopcz x4, m1\n"                                  
			"fmaxu.pi f2, f5, f5\n"                               
			"fsetm.pi m1, f7\n"                                   
			"fle.s x3, f3, f0\n"                                  
			"fcmov.ps f7, f5, f3, f6\n"                           
			"fcvt.ps.pwu f5, f1, dyn\n"                           
			"fmulh.pi f4, f4, f5\n"                               
			"fcvt.s.w f4, x11, rtz\n"                             
			"fcvt.w.s x3, f7, rup\n"                              
			"maskand m0, m2, m1\n"                                
			"fltm.pi m2, f4, f5\n"                                
			"fadd.s f2, f6, f0, rmm\n"                            
			"fcvt.ps.pw f9, f0, rdn\n"                            
			"fswizz.ps f2, f0, 24\n"                              
			"fcvt.wu.s x3, f2, rne\n"                             
			"fsgnjx.ps f7, f1, f7\n"                              
			"fsll.pi f7, f7, f6\n"                                
			"fcvt.ps.f16 f5, f2\n"                                
			"fsat8.pi f4, f5\n"                                   
			"fsgnj.s f8, f0, f6\n"                                
			"fcvt.s.wu f3, x11, dyn\n"                            
			"fadd.ps f6, f4, f2, rdn\n"                           
			"fsub.s f8, f6, f3, rup\n"                            
			"fnmadd.s f6, f3, f1, f3, rtz\n"                      
			"ffrc.ps f6, f6\n"                                    
			"fmul.pi f5, f6, f2\n"                                
			"fnmsub.s f8, f6, f5, f5, rne\n"                      
			"fsrl.pi f6, f5, f2\n"                                
			"fminu.pi f2, f3, f1\n"                               
			"masknot m1, m2\n"                                    
			"fmsub.ps f8, f3, f5, f4, dyn\n"                      
			"fsatu8.pi f8, f7\n"                                  
			"fcvt.s.w f2, x11, rne\n"                             
			"fand.pi f9, f1, f0\n"                                
			"fxor.pi f8, f4, f1\n"                                
			"fmv.x.s x4, f5\n"                                    
			"fswizz.ps f5, f3, 116\n"                             
			"fle.ps f5, f6, f6\n"                                 
			"fmsub.ps f4, f0, f3, f1, rne\n"                      
			"fcvt.s.wu f4, x11, rtz\n"                            
			"fswizz.ps f5, f7, 16\n"                              
			"mova.x.m x3\n"                                       
			"fnmsub.ps f4, f0, f4, f0, dyn\n"                     
			"fmulhu.pi f8, f3, f5\n"                              
			"feq.pi f8, f7, f1\n"                                 
			"fltm.ps m0, f7, f0\n"                                
			"flem.ps m1, f0, f0\n"                                
			"fbc.ps f5, -1009(x11)\n"                             
			"fpackreph.pi f8, f0\n"                               
			//RTLMIN-5479"fsin.ps f9, f1\n"                                    
			"fmax.ps f6, f0, f4\n"                                
			"fmaxu.pi f4, f4, f4\n"                               
			"fslli.pi f2, f0, 3\n"                                
			"fcvt.wu.s x3, f2, rne\n"                             
			"fltm.ps m2, f2, f2\n"                                
			"fltm.pi m2, f7, f3\n"                                
			"maskor m1, m0, m0\n"                                 
			"fltm.pi m2, f2, f2\n"                                
			"fcmovm.ps f5, f5, f0\n"                              
			//RTLMIN-5479"fsin.ps f6, f7\n"                                    
			"fsgnjx.ps f5, f0, f5\n"                              
			"fcvt.pwu.ps f8, f7, rdn\n"                           
			"fcvt.pw.ps f5, f0, rtz\n"                            
			"flw f6, 1869(x11)\n"                                 
			"flw f2, 1211(x11)\n"                                 
			"fsgnj.ps f8, f0, f3\n"                               
			"fsgnj.s f6, f7, f7\n"                                
			"fmulhu.pi f4, f5, f7\n"                              
			"maskand m0, m2, m2\n"                                
			"fsw.ps f2, 141(x11)\n"                               
			"fcvt.w.s x4, f6, rne\n"                              
			"fsrai.pi f8, f6, 11\n"                               
			"fcvt.w.s x4, f1, rup\n"                              
			"flt.s x4, f1, f1\n"                                  
			"fsw.ps f2, -392(x11)\n"                              
			"fnot.pi f2, f7\n"                                    
			"fcvt.ps.pw f2, f3, rdn\n"                            
			"flt.s x3, f7, f0\n"                                  
			"fsat8.pi f5, f4\n"                                   
			"fround.ps f8, f4, rne\n"                             
			"fmadd.s f3, f3, f0, f5, rmm\n"                       
			"fadd.pi f2, f3, f2\n"                                
			"fnmadd.s f8, f7, f3, f2, rdn\n"                      
			"fbci.pi f3, 438757\n"                                
			"fslli.pi f6, f6, 12\n"                               
			"feqm.ps m2, f3, f2\n"                                
			"fcvt.f16.ps f4, f2\n"                                
			"fcvt.f16.ps f5, f1\n"                                
			"fcvt.pwu.ps f8, f1, rne\n"                           
			"fpackrepb.pi f6, f2\n"                               
			"fsrli.pi f3, f5, 15\n"                               
			"frcp.ps f6, f2\n"                                    
			"flog.ps f2, f0\n"                                    
			//RTLMIN-5479"fsin.ps f5, f7\n"                                    
			"flem.ps m0, f4, f7\n"                                
			"fcvt.s.wu f3, x11, rne\n"                            
			"fcmovm.ps f7, f2, f7\n"                              
			"fcvt.s.wu f9, x11, rne\n"                            
			"fnmsub.s f5, f1, f5, f1, rne\n"                      
			"fcvt.s.w f5, x11, rne\n"                             
			"fclass.s x3, f1\n"                                   
			"fmvs.x.ps x4, f5, 2\n"                               
			"fcvt.s.wu f8, x11, rne\n"                            
			"fmax.s f5, f3, f0\n"                                 
			"faddi.pi f6, f6, 50\n"                               
			"flw.ps f3, 221(x11)\n"                               
			"fnmadd.ps f5, f7, f7, f7, rmm\n"                     
			"fltm.pi m0, f5, f4\n"                                
			"fmsub.ps f3, f1, f5, f5, rtz\n"                      
			"fsgnjn.ps f4, f2, f0\n"                              

			"li x5, 0x1130dc9f229f7a0f\n"
			"mova.m.x x5\n"                                       
			"fsll.pi f7, f6, f3\n"                                
			"fle.pi f6, f7, f5\n"                                 
			"fpackrepb.pi f5, f5\n"                               
			"flq2 f5, 1867(x11)\n"                                
			"fmax.s f5, f7, f1\n"                                 
			"fmax.pi f2, f0, f6\n"                                
			"fcvt.s.wu f8, x11, rdn\n"                            
			"feq.ps f6, f3, f2\n"                                 
			"fmadd.ps f7, f2, f6, f0, rdn\n"                      
			"feqm.ps m2, f4, f0\n"                                
			"fround.ps f6, f5, rne\n"                             
			"maskpopc x3, m1\n"                                   
			"feq.s x4, f2, f3\n"                                  
			"fsetm.pi m1, f4\n"                                   
			"fle.s x3, f5, f3\n"                                  
			"fandi.pi f8, f1, 181\n"                              
			"fcvt.s.w f4, x11, rtz\n"                             
			"fsll.pi f4, f1, f6\n"                                
			"ffrc.ps f8, f1\n"                                    
			"fminu.pi f9, f6, f0\n"                               
			"fmulh.pi f4, f0, f1\n"                               
			"fpackrepb.pi f5, f4\n"                               
			"fmadd.s f6, f5, f4, f1, rdn\n"                       
			"maskxor m1, m1, m2\n"                                
			"fltm.ps m0, f7, f7\n"                                
			"fmulhu.pi f8, f2, f1\n"                              
			"fcvt.ps.f16 f2, f3\n"                                
			"ffrc.ps f4, f4\n"                                    
			"fsgnjn.ps f6, f6, f2\n"                              
			"fadd.ps f8, f1, f4, rmm\n"                           
			"flt.s x4, f5, f1\n"                                  
			"fmin.s f7, f5, f5\n"                                 
			"fcvt.pwu.ps f7, f5, rtz\n"                           
			"fcvt.pwu.ps f5, f0, rup\n"                           
			"fslli.pi f7, f0, 9\n"                                
			"flw.ps f2, -75(x11)\n"                               
			"fmvz.x.ps x3, f2, 0\n"                               
			"fsgnjn.s f8, f5, f5\n"                               

			"li x5, 0x9696efbdb2e88db4\n"
			"mova.m.x x5\n"                                       
			"fnmadd.s f2, f6, f3, f5, rup\n"                      
			"fmin.s f9, f4, f1\n"                                 
			"fmax.pi f4, f5, f0\n"                                
			"fbcx.ps f3, x11\n"                                   
			"fsgnj.ps f4, f6, f4\n"                               
			"fminu.pi f8, f4, f4\n"                               
			"fnmadd.ps f8, f3, f6, f6, rne\n"                     
			"csrr x5, fcsr\n"
			"fscsr x0\n"
			"fsgnj.s f9, f2, f7\n"                                
			"flw f9, -20(x11)\n"                                  
			"fsgnj.s f9, f4, f7\n"                                
			"fsrli.pi f8, f5, 9\n"                                
			"fmax.s f8, f6, f2\n"                                 
			"fxor.pi f6, f0, f6\n"                                
			"fsrli.pi f8, f4, 10\n"                               
			"flt.s x4, f2, f1\n"                                  
			"fmv.s.x f4, x11\n"                                   
			"fsrli.pi f2, f0, 6\n"                                
			"fsgnjx.s f6, f5, f6\n"                               
			"fcvt.ps.pw f8, f0, dyn\n"                            
			"maskor m0, m2, m2\n"                                 
			"fmax.pi f9, f6, f6\n"                                
			"fadd.s f2, f2, f2, rmm\n"                            
			"fcvt.s.wu f8, x11, dyn\n"                            
			"fmulh.pi f8, f7, f4\n"                               
			"fslli.pi f2, f6, 10\n"                               
			"fcvt.wu.s x3, f4, rne\n"                             
			"fmadd.ps f7, f3, f4, f5, rdn\n"                      
			"fcvt.pw.ps f3, f7, dyn\n"                            
			"fnmsub.ps f2, f4, f4, f0, dyn\n"                     
			"for.pi f2, f5, f2\n"                                 
			"feq.pi f2, f4, f3\n"                                 
			"flem.ps m0, f4, f0\n"                                
			"fsub.pi f3, f4, f1\n"                                
			"frcp.ps f3, f1\n"                                    
			"fsw.ps f4, -993(x11)\n"                              
			"fadd.ps f4, f4, f0, rne\n"                           
			"fmvz.x.ps x3, f1, 2\n"                               
			"flw f6, -91(x11)\n"                                  
			"flt.ps f7, f4, f7\n"                                 
			"feqm.ps m1, f0, f5\n"                                
			"fminu.pi f4, f3, f1\n"                               
			"maskxor m0, m0, m2\n"                                
			"feq.pi f7, f7, f2\n"                                 
			"fcvt.s.w f9, x11, rtz\n"                             
			"fle.ps f2, f1, f7\n"                                 
			"feqm.ps m2, f1, f6\n"                                
			"fmsub.s f5, f4, f5, f6, rne\n"                       
			"fmin.s f4, f3, f0\n"                                 
			"fnmsub.ps f5, f5, f3, f4, rup\n"                     
			"fclass.s x4, f6\n"                                   
			"fcvt.f16.ps f6, f7\n"                                
			"fbc.ps f5, -269(x11)\n"                              
			"fmax.ps f3, f1, f3\n"                                
			"fbc.ps f7, 638(x11)\n"                               
			"fbci.pi f6, 339388\n"                                
			"fcvt.w.s x3, f1, rup\n"                              
			"fltu.pi f4, f0, f5\n"                                
			"fcvt.f16.ps f4, f0\n"                                
			"ffrc.ps f6, f0\n"                                    
			"fpackrepb.pi f7, f6\n"                               
			"fbcx.ps f2, x11\n"                                   
			"fsgnjn.s f5, f5, f5\n"                               
			"ffrc.ps f4, f2\n"                                    
			"feqm.ps m1, f6, f1\n"                                
			"fadd.ps f3, f6, f2, rne\n"                           
			"fsll.pi f9, f1, f1\n"                                
			"fsat8.pi f7, f1\n"                                   
			"fsgnjx.s f3, f6, f5\n"                               
			"ffrc.ps f6, f1\n"                                    
			"fbcx.ps f5, x11\n"                                   
			"fmin.pi f6, f7, f0\n"                                
			"fsub.ps f9, f5, f5, dyn\n"                           
			"fnmadd.s f4, f5, f6, f5, dyn\n"                      
			"fsub.pi f6, f0, f5\n"                                
			"fsub.ps f4, f4, f4, dyn\n"                           
			"csrr x5, fflags\n"
			"fscsr x0\n"
			"fcvt.s.w f9, x11, rdn\n"                             
			"fmin.pi f6, f4, f5\n"                                
			"fadd.s f3, f6, f7, rdn\n"                            
			"maskor m1, m2, m2\n"                                 
			"fxor.pi f5, f6, f5\n"                                
			"fmax.ps f8, f3, f1\n"                                
			"fsra.pi f8, f0, f3\n"                                
			"fsrl.pi f9, f3, f7\n"                                
			"feq.s x3, f2, f2\n"                                  
			"flt.pi f8, f5, f5\n"                                 
			"flw.ps f5, 746(x11)\n"                               
			"fadd.pi f3, f0, f5\n"                                
			"fsgnjx.s f2, f6, f1\n"                               
			"fmulhu.pi f8, f0, f4\n"                              
			//RTLMIN-5479"fsin.ps f3, f3\n"                                    
			"fmv.x.s x4, f5\n"                                    
			"maskpopc x3, m1\n"                                   
			"mova.x.m x3\n"                                       
			"fcvt.ps.pw f4, f6, dyn\n"                            
			"fbc.ps f6, -702(x11)\n"                              
			"fcvt.s.wu f6, x11, dyn\n"                            

			"li x5, 0x6fe4f3ab33fae3fc\n"
			"mova.m.x x5\n"                                       
			"fmsub.s f9, f1, f3, f2, rtz\n"                       

			"li x5, 0xffffffff\n"
			"mova.m.x x5\n"

			"flt.pi f4, f2, f2\n"                                 
			"flem.ps m2, f0, f5\n"                                
			"fcvt.ps.pwu f2, f6, rup\n"                           
			"feq.pi f7, f0, f7\n"                                 
			"flq2 f4, 485(x11)\n"                                 
			"mova.x.m x3\n"                                       
			"fclass.s x4, f3\n"                                   
			"fcvt.f16.ps f7, f3\n"                                
			"fxor.pi f4, f3, f2\n"                                
			"fltm.pi m1, f7, f4\n"                                
			"fadd.pi f8, f4, f3\n"                                
			"fltm.pi m1, f5, f2\n"                                
			"fcvt.wu.s x4, f3, rne\n"                             
			"fsq2 f4, -1620(x11)\n"                               
			"fsll.pi f8, f5, f6\n"                                
			"ffrc.ps f5, f2\n"                                    
			"maskand m2, m0, m0\n"                                
			"flem.ps m0, f7, f7\n"                                
			"fsrl.pi f6, f2, f7\n"                                
			"fand.pi f7, f6, f5\n"                                
			"fcvt.ps.f16 f9, f0\n"                                
			"fmulhu.pi f2, f0, f0\n"                              
			"fpackrepb.pi f6, f6\n"                               
			//RTLMIN-5479"fsin.ps f9, f1\n"                                    
			"fsrl.pi f3, f3, f2\n"                                
			"fcvt.ps.pwu f2, f4, dyn\n"                           
			"fclass.ps f3, f7\n"                                  
			"fle.s x3, f2, f1\n"                                  
			"fandi.pi f9, f3, 464\n"                              
			"fbc.ps f8, -1252(x11)\n"                             
			"mova.x.m x3\n"                                       
			"frcp.ps f4, f6\n"                                    
			"flt.ps f4, f6, f6\n"                                 
			"fmadd.s f6, f3, f7, f3, rtz\n"                       
			"fadd.s f2, f0, f6, rmm\n"                            
			"fle.pi f6, f5, f3\n"                                 
			"fmax.pi f4, f3, f0\n"                                
			"fsgnj.s f9, f1, f7\n"                                
			"masknot m2, m0\n"                                    
			"fmulhu.pi f4, f6, f1\n"                              
			"fand.pi f5, f0, f0\n"                                
			"fcvt.wu.s x3, f2, rtz\n"                             
			"fadd.pi f2, f6, f6\n"                                
			"fsetm.pi m1, f2\n"                                   
			"fmadd.ps f4, f1, f5, f2, rmm\n"                      
			"fle.s x4, f3, f2\n"                                  
			"fmvs.x.ps x4, f1, 5\n"                               
			"fclass.s x4, f4\n"                                   
			"fpackrepb.pi f2, f2\n"                               
			"fnot.pi f6, f4\n"                                    
			"flw f9, -127(x11)\n"                                 
			"fle.ps f2, f5, f1\n"                                 
			"fbc.ps f9, 169(x11)\n"                               
			"fltu.pi f2, f1, f1\n"                                
			"fsgnjn.s f9, f5, f4\n"                               
			"faddi.pi f6, f4, 423\n"                              
			"fsgnjn.ps f6, f1, f7\n"                              
			"fmul.ps f5, f1, f3, rmm\n"                           
			"fadd.s f4, f0, f0, rdn\n"                            
			"fbci.pi f9, 294803\n"                                
			"fsub.ps f2, f7, f4, rtz\n"                           
			"fxor.pi f7, f2, f0\n"                                

			"li x5, 0x42f34ae4fc3af8da\n"
			"mova.m.x x5\n"                                       
			"feqm.ps m0, f1, f4\n"                                
			"feqm.ps m1, f6, f5\n"                                
			"flt.ps f2, f4, f6\n"                                 
			"fexp.ps f2, f3\n"                                    
			"fsw.ps f4, -497(x11)\n"                              
			"fadd.s f9, f3, f1, dyn\n"                            
			"fmul.s f7, f5, f2, rup\n"                            
			"fmin.pi f3, f7, f0\n"                                
			"fsrli.pi f2, f2, 14\n"                               
			"maskand m2, m2, m1\n"                                
			"fclass.ps f2, f1\n"                                  
			"maskxor m1, m1, m2\n"                                
			"fmv.s.x f5, x11\n"                                   
			"fmin.pi f4, f0, f7\n"                                
			"maskpopc x3, m1\n"                                   
			"feqm.ps m1, f0, f2\n"                                
			"fbci.ps f6, 15745\n"                                 
			"fpackreph.pi f7, f5\n"                               
			"fsgnjn.ps f4, f1, f0\n"                              
			"fpackreph.pi f6, f4\n"                               
			"fle.ps f3, f2, f4\n"                                 
			"flw.ps f9, 1042(x11)\n"                              
			"fmsub.ps f9, f3, f2, f4, rne\n"                      
			"fmvs.x.ps x3, f2, 3\n"                               
			"fmsub.ps f3, f1, f6, f5, rne\n"                      
			"fsw f6, 1711(x11)\n"                                 
			"flem.ps m1, f1, f4\n"                                
			"feq.s x3, f7, f6\n"                                  
			"flog.ps f3, f2\n"                                    
			"fmulh.pi f7, f0, f0\n"                               
			"fand.pi f9, f0, f1\n"                                
			"fsub.ps f6, f3, f1, rtz\n"                           
			"csrr x5, frm\n"
			"fscsr x0\n"
			"fltu.pi f8, f7, f1\n"                                
			"fsw f0, 1786(x11)\n"                                 
			"fsub.ps f9, f1, f7, rup\n"                           
			"fminu.pi f5, f6, f0\n"                               
			"mov.m.x m2, x11, 79\n"                               
			"maskpopc x4, m1\n"                                   
			"fexp.ps f8, f5\n"                                    
			"flog.ps f7, f3\n"                                    
			"fmin.ps f7, f6, f6\n"                                
			"fltu.pi f4, f7, f3\n"                                
			"fexp.ps f6, f0\n"                                    

			"li x5, 0xffffffff\n"
			"mova.m.x x5\n"

			"fsgnj.ps f8, f0, f3\n"                               
			"fmax.ps f8, f3, f5\n"                                
			"fclass.s x3, f2\n"                                   
			"fmv.s.x f3, x11\n"                                   
			"fltu.pi f2, f0, f6\n"                                
			"fsatu8.pi f5, f7\n"                                  

			"li x5, 0x675a9e9ba072eda2\n"
			"mova.m.x x5\n"

			"fclass.s x3, f1\n"                                   
			//RTLMIN-5479"frsq.ps f9, f5\n"                                    
			"fsrli.pi f7, f2, 13\n"                               
			"fsgnjn.ps f5, f1, f5\n"                              
			"fsw.ps f4, 539(x11)\n"                               
			"fbci.pi f9, 244016\n"                                
			"fmsub.s f6, f4, f1, f7, dyn\n"                       
			"masknot m1, m2\n"                                    
			"fle.s x3, f1, f2\n"                                  
			"fsgnjn.ps f6, f0, f4\n"                              
			"fsrli.pi f4, f0, 1\n"                                
			"fclass.s x4, f3\n"                                   
			"fmax.ps f7, f3, f2\n"                                
			"fxor.pi f6, f7, f6\n"                                
			"fslli.pi f3, f3, 11\n"                               
			"maskand m0, m2, m2\n"                                
			"fswizz.ps f8, f4, 89\n"                              
			"fsrl.pi f2, f4, f4\n"                                
			"fsgnjn.s f5, f1, f7\n"                               
			"fadd.pi f7, f4, f1\n"                                
			"fmax.s f5, f7, f7\n"                                 
			"fsll.pi f7, f1, f4\n"                                
			"fmul.ps f2, f7, f1, rtz\n"                           
			"fmulhu.pi f4, f2, f0\n"                              
			"fclass.ps f6, f4\n"                                  
			"faddi.pi f3, f3, 269\n"                              
			"fmvz.x.ps x3, f0, 6\n"                               
			"fsub.s f4, f5, f5, rne\n"                            
			"fmulhu.pi f3, f6, f5\n"                              
			"fmax.pi f7, f6, f3\n"                                
			"fcvt.ps.pwu f2, f5, rdn\n"                           
			"fsub.ps f5, f6, f5, rtz\n"                           
			"fpackreph.pi f9, f3\n"                               
			"fsub.ps f7, f3, f2, rdn\n"                           
			"fcvt.pw.ps f6, f1, rmm\n"                            
			"fclass.s x3, f2\n"                                   
			"feq.ps f3, f7, f3\n"                                 
			"feq.s x4, f2, f1\n"                                  
			"fround.ps f8, f6, rdn\n"                             
			"fmin.pi f5, f5, f5\n"                                
			"fmv.x.s x4, f4\n"                                    
			//RTLMIN-5479"fsin.ps f5, f7\n"                                    
			"fmsub.ps f6, f3, f4, f6, rne\n"                      
			"fcvt.pw.ps f2, f7, rmm\n"                            
			"fmvs.x.ps x3, f3, 6\n"                               
			"flw.ps f6, 1127(x11)\n"                              
			"flw.ps f9, 1610(x11)\n"                              
			"masknot m2, m0\n"                                    
			"flq2 f3, -1873(x11)\n"                               
			"flt.s x4, f1, f6\n"                                  
			"fcvt.ps.pwu f5, f4, rne\n"                           
			"fbcx.ps f2, x11\n"                                   
			"feq.s x3, f3, f0\n"                                  

		"LBL_VPU_SEQ_START_1:\n"
			"fxor.pi f4, f7, f3\n"                                
			"csrr x5, fcsr\n"
			"fscsr x0\n"

			"li x5, 0xffffffffffffffff\n"
			"mova.m.x x5\n"

			"fmax.pi f8, f5, f3\n"                                
			"fmul.pi f2, f0, f4\n"                                
			"fclass.ps f9, f2\n"                                  
			"fltu.pi f8, f0, f4\n"                                
			"fcvt.ps.pw f7, f2, dyn\n"                            
			"fcmov.ps f7, f5, f4, f2\n"                           
			"fsll.pi f7, f4, f3\n"                                
			"fcvt.ps.pwu f2, f0, rmm\n"                           
			"flem.ps m1, f7, f5\n"                                
			"for.pi f9, f1, f4\n"                                 
			"fltm.pi m1, f0, f1\n"                                
			"fmvs.x.ps x4, f2, 1\n"                               
			"fcvt.ps.pw f8, f6, dyn\n"                            
			"flt.pi f3, f5, f7\n"                                 
			"fsub.s f5, f0, f3, rup\n"                            
			"fnmsub.s f2, f1, f0, f0, rup\n"                      
			"fcvt.f16.ps f4, f3\n"                                
			"fcvt.f16.ps f5, f5\n"                                
			"fandi.pi f4, f2, 300\n"                              
			"fsq2 f4, -1075(x11)\n"                               
			"fmul.s f6, f6, f0, rdn\n"                            
			"fcmov.ps f6, f0, f2, f4\n"                           
			"faddi.pi f3, f2, 472\n"                              
			"fcvt.ps.pw f6, f3, rne\n"                            
			"fxor.pi f8, f6, f7\n"                                
			"fltu.pi f5, f7, f1\n"                                
			"for.pi f8, f4, f2\n"                                 
			"fmax.pi f8, f1, f4\n"                                
			"fxor.pi f3, f2, f7\n"                                
			"fltm.ps m2, f3, f2\n"                                
			"fmin.pi f6, f6, f7\n"                                
			"fmax.ps f8, f0, f2\n"                                
			"feq.ps f4, f0, f2\n"                                 
			"fsgnjn.s f2, f0, f4\n"                               
			"fcvt.f16.ps f6, f6\n"                                
			"fand.pi f8, f5, f5\n"                                
			"fmax.ps f3, f0, f2\n"                                
			"ffrc.ps f5, f2\n"                                    
			"fslli.pi f8, f5, 1\n"                                
			"fbci.pi f8, 332142\n"                                
			"fcmov.ps f5, f6, f4, f0\n"                           
			"feq.ps f2, f5, f6\n"                                 
			"fnmadd.ps f5, f2, f4, f5, rmm\n"                     
			"maskand m1, m2, m1\n"                                
			"fmin.ps f2, f0, f5\n"                                
			"fcvt.wu.s x3, f5, rup\n"                             
			"fmulhu.pi f9, f7, f6\n"                              
			"fsgnjx.s f6, f3, f5\n"                               
			"fmv.s.x f5, x11\n"                                   
			"fmvz.x.ps x3, f1, 5\n"                               
			"fnmadd.ps f5, f3, f4, f0, rne\n"                     
			"fle.s x4, f7, f1\n"                                  
			"fmax.ps f3, f4, f7\n"                                
			"feq.ps f9, f3, f4\n"                                 
			"feq.s x3, f3, f2\n"                                  
			"fcmovm.ps f2, f6, f2\n"                              
			"fadd.pi f5, f2, f0\n"                                
			"fmv.s.x f2, x11\n"                                   
			"fsra.pi f8, f7, f5\n"                                
			"maskor m0, m2, m2\n"                                 
			"fadd.ps f7, f1, f6, rup\n"                           
			"flt.ps f5, f1, f1\n"                                 
			"fsll.pi f9, f5, f1\n"                                
			"fsrli.pi f3, f7, 12\n"                               
			"fsra.pi f4, f3, f7\n"                                
			"fmul.ps f7, f7, f2, dyn\n"                           
			"fsgnjn.ps f3, f2, f5\n"                              
			"fltm.pi m1, f3, f7\n"                                
			"flt.ps f4, f0, f1\n"                                 
			"fnot.pi f6, f0\n"                                    
			"flw.ps f9, -1408(x11)\n"                             
			"fcvt.pwu.ps f3, f1, dyn\n"                           
			"fsgnjn.ps f3, f3, f1\n"                              
			"fmulhu.pi f9, f3, f5\n"                              
			"fsatu8.pi f8, f5\n"                                  
			"fsub.ps f9, f6, f4, rne\n"                           
			"flq2 f7, 1480(x11)\n"                                
			"fsgnjx.ps f7, f3, f1\n"                              
			"flt.s x3, f3, f6\n"                                  
			"fsrai.pi f9, f1, 5\n"                                
			"fltu.pi f2, f0, f7\n"                                
			"fbci.ps f4, 157454\n"                                
			"fmvz.x.ps x4, f4, 3\n"                               
			"fsll.pi f8, f3, f4\n"                                
			"fpackreph.pi f3, f2\n"                               
			"fcvt.ps.pw f3, f5, rup\n"                            
			"fadd.pi f4, f4, f5\n"                                
			"fmul.s f4, f7, f6, rdn\n"                            
			"fmul.pi f2, f2, f6\n"                                
			"fcvt.pwu.ps f2, f3, rtz\n"                           
			"fltm.pi m2, f7, f4\n"                                
			"flt.ps f5, f3, f2\n"                                 
			"fcvt.wu.s x4, f6, rdn\n"                             
			"fpackrepb.pi f7, f1\n"                               
			"fsrl.pi f3, f1, f2\n"                                
			"fsgnjx.s f8, f0, f5\n"                               
			"fsrai.pi f3, f2, 1\n"                                
			"fle.pi f8, f2, f5\n"                                 
			"fcmovm.ps f8, f5, f2\n"                              
			"fnot.pi f2, f7\n"                                    
			"fmin.s f9, f4, f0\n"                                 
			"fsgnj.s f6, f5, f3\n"                                
			"mova.x.m x3\n"                                       
			"fcmov.ps f6, f1, f6, f7\n"                           

			"li x5, 0x2796d72b8003544d\n"
			"mova.m.x x5\n"                                       
			"fxor.pi f7, f7, f5\n"                                
			"flem.ps m2, f6, f0\n"                                
			"maskpopcz x3, m1\n"                                  
			"fpackreph.pi f5, f0\n"                               
			"flog.ps f9, f2\n"                                    
			"fsrl.pi f5, f0, f0\n"                                
			"for.pi f2, f6, f1\n"                                 
			"maskand m2, m2, m1\n"                                
			"ffrc.ps f7, f2\n"                                    
			"fnot.pi f6, f5\n"                                    
			"ffrc.ps f2, f4\n"                                    
			"fsrai.pi f7, f7, 4\n"                                
			"maskpopcz x3, m2\n"                                  
			"feq.s x4, f1, f4\n"                                  
			"fmvz.x.ps x3, f5, 0\n"                               
			"fmax.pi f7, f0, f1\n"                                
			"fmvz.x.ps x4, f7, 2\n"                               
			"fround.ps f3, f5, rup\n"                             
			"mov.m.x m2, x11, 86\n"                               
			"faddi.pi f5, f1, 479\n"                              
			"fcvt.f16.ps f9, f6\n"                                
			"fslli.pi f9, f2, 11\n"                               
			"fslli.pi f4, f7, 8\n"                                
			"fltu.pi f8, f2, f7\n"                                
			"fcvt.f16.ps f9, f0\n"                                
			"fltm.ps m0, f3, f0\n"                                
			"fmulh.pi f7, f3, f6\n"                               
			"fcvt.wu.s x3, f2, rdn\n"                             
			"fsub.pi f3, f7, f0\n"                                
			"fmul.ps f5, f3, f6, rne\n"                           
			"maskand m1, m2, m1\n"                                
			"fsgnj.s f5, f5, f1\n"                                
			"fsrai.pi f6, f0, 7\n"                                
			"masknot m1, m1\n"                                    
			"flq2 f3, 1537(x11)\n"                                
			"faddi.pi f5, f5, 77\n"                               
			"fle.ps f2, f3, f7\n"                                 
			"fmulhu.pi f4, f0, f7\n"                              
			"fmax.s f8, f5, f0\n"                                 
			"fltm.ps m0, f7, f2\n"                                
			"fcvt.s.wu f3, x11, rtz\n"                            
			"fmsub.s f9, f5, f2, f1, dyn\n"                       
			"maskpopcz x4, m0\n"                                  
			"frcp.ps f8, f4\n"                                    
			"fsgnj.ps f6, f2, f7\n"                               
			"fbcx.ps f2, x11\n"                                   
			"fsub.pi f8, f5, f3\n"                                
			"fsub.ps f5, f0, f5, rup\n"                           
			"fbci.pi f9, 466864\n"                                
			//RTLMIN-5479"fsin.ps f7, f2\n"                                    
			"flt.s x3, f2, f5\n"                                  
			"for.pi f9, f0, f6\n"                                 
			"fslli.pi f7, f5, 8\n"                                
			"fclass.ps f3, f3\n"                                  
			"fsw f2, 1916(x11)\n"                                 
			"fsat8.pi f9, f1\n"                                   
			"fsetm.pi m2, f5\n"                                   
			"fmsub.ps f4, f4, f4, f1, rmm\n"                      
			"fmin.pi f6, f6, f3\n"                                
			"fbcx.ps f5, x11\n"                                   
			"fnmadd.ps f2, f6, f6, f3, rmm\n"                     
			"fclass.ps f5, f1\n"                                  
			"fpackrepb.pi f3, f4\n"                               
			"mov.m.x m2, x11, 93\n"                               
			"fnmsub.ps f5, f2, f4, f2, rup\n"                     
			"fcmovm.ps f7, f1, f1\n"                              
			"fsub.pi f6, f5, f4\n"                                
			"fsw.ps f3, 755(x11)\n"                               
			"fnmadd.ps f8, f3, f6, f5, rtz\n"                     
			"fcvt.ps.pw f4, f7, rtz\n"                            
			"fadd.s f6, f4, f7, rup\n"                            
			"fmax.ps f7, f7, f2\n"                                
			"fsatu8.pi f2, f5\n"                                  
			"masknot m1, m0\n"                                    
			"flem.ps m2, f0, f3\n"                                
			"fsw.ps f6, 888(x11)\n"                               
			"fandi.pi f7, f7, 497\n"                              
			"fltu.pi f3, f4, f4\n"                                
			"maskand m1, m0, m2\n"                                
			"fsgnj.ps f9, f4, f7\n"                               
			"fnmsub.s f5, f3, f6, f6, rmm\n"                      
			"fnot.pi f8, f3\n"                                    
			"fxor.pi f4, f5, f2\n"                                
			"mova.x.m x3\n"                                       
			"frcp.ps f6, f5\n"                                    
			"fnmsub.s f3, f4, f4, f0, rtz\n"                      
			"fround.ps f5, f3, rdn\n"                             
			"fsetm.pi m2, f6\n"                                   
			"fsrai.pi f4, f1, 0\n"                                
			"flt.ps f8, f5, f5\n"                                 
			"fnot.pi f7, f7\n"                                    
			"flt.pi f8, f4, f7\n"                                 
			"fround.ps f5, f2, rdn\n"                             
			"maskor m1, m0, m1\n"                                 
			"fsra.pi f8, f7, f3\n"                                
			"fadd.s f9, f4, f2, rtz\n"                            
			"fandi.pi f3, f0, 500\n"                              
			"fcvt.ps.pw f5, f5, rmm\n"                            
			"for.pi f4, f1, f0\n"                                 
			"fbci.pi f8, 157758\n"                                
			"fcvt.w.s x3, f7, rtz\n"                              
			"fcvt.ps.pw f7, f2, rmm\n"                            
			"feqm.ps m1, f4, f2\n"                                
			"fadd.ps f4, f0, f6, rne\n"                           
			"fmul.ps f8, f7, f4, rtz\n"                           
			"fmin.pi f9, f7, f2\n"                                
			"fltm.pi m1, f4, f6\n"                                
			"fsrl.pi f6, f7, f0\n"                                
			"fadd.pi f8, f3, f2\n"                                
			"fmv.s.x f7, x11\n"                                   

			"li x5, 0x6bcb5d4585eeb3f\n"
			"mova.m.x x5\n"                                       
			"fadd.ps f4, f0, f4, dyn\n"                           
			"fsat8.pi f8, f0\n"                                   
			"fsrli.pi f6, f4, 13\n"                               
			"fmadd.s f5, f6, f5, f1, rup\n"                       
			"fadd.s f7, f3, f0, rne\n"                            
			"frcp.ps f2, f1\n"                                    
			"fltm.ps m0, f2, f0\n"                                
			"fpackrepb.pi f3, f0\n"                               
			"fmaxu.pi f4, f7, f5\n"                               
			"fmv.x.s x3, f1\n"                                    
			"fcmov.ps f6, f0, f1, f5\n"                           
			"flt.pi f9, f7, f4\n"                                 
			"fsrai.pi f6, f3, 8\n"                                
			"frcp.ps f6, f2\n"                                    
			"fltm.pi m1, f1, f6\n"                                
			"fadd.s f3, f6, f0, rup\n"                            
			"fclass.s x4, f6\n"                                   
			"fslli.pi f4, f3, 7\n"                                
			"feq.ps f7, f5, f2\n"                                 
			"fltu.pi f9, f3, f0\n"                                
			"fcvt.pwu.ps f2, f5, rup\n"                           
			"fsrli.pi f8, f1, 10\n"                               
			"fsgnjx.ps f7, f5, f0\n"                              
			"flq2 f4, -857(x11)\n"                                
			"flw f2, -437(x11)\n"                                 
			"fsll.pi f9, f6, f1\n"                                
			"fsub.ps f6, f4, f3, dyn\n"                           
			"fcvt.pwu.ps f7, f6, rne\n"                           
			"fnmsub.ps f8, f5, f0, f0, rmm\n"                     
			"fbci.ps f9, 14434\n"                                 
			"fpackrepb.pi f6, f3\n"                               
			"fsll.pi f9, f7, f1\n"                                
			"fcvt.w.s x4, f3, rmm\n"                              
			"fmadd.s f4, f5, f6, f3, rtz\n"                       
			"mov.m.x m0, x11, 104\n"                              
			"flq2 f7, -1326(x11)\n"                               
			"fmv.x.s x3, f5\n"                                    
			"flem.ps m0, f1, f3\n"                                
			"ffrc.ps f6, f5\n"                                    
			"fminu.pi f5, f5, f7\n"                               
			"fadd.s f6, f3, f5, dyn\n"                            
			"fsetm.pi m0, f5\n"                                   
			"fcmov.ps f3, f7, f4, f1\n"                           
			"fmv.s.x f8, x11\n"                                   
			"fsatu8.pi f5, f4\n"                                  
			"fmul.pi f4, f7, f3\n"                                
			"fsw f3, -42(x11)\n"                                  
			"fsub.s f6, f3, f2, rne\n"                            
			"fminu.pi f3, f2, f4\n"                               
			"fbc.ps f7, 598(x11)\n"                               
			"fsetm.pi m1, f0\n"                                   
			"feq.ps f8, f0, f3\n"                                 
			"fsll.pi f2, f1, f1\n"                                
			"fandi.pi f6, f2, 437\n"                              
			//RTLMIN-5479"frsq.ps f7, f2\n"                                    
			"fandi.pi f2, f1, 119\n"                              
			"fpackreph.pi f2, f1\n"                               
			"fsat8.pi f3, f1\n"                                   

			"li x5, 0x19f1ad0b76fecdca\n"
			"mova.m.x x5\n"                                       
			"fsgnjn.ps f4, f7, f6\n"                              
			"fpackreph.pi f6, f2\n"                               
			"fcvt.pwu.ps f9, f5, rne\n"                           
			"fle.ps f9, f4, f4\n"                                 
			"fmax.pi f2, f3, f5\n"                                
			"fsll.pi f4, f3, f5\n"                                
			"fmadd.ps f2, f7, f6, f0, rtz\n"                      
			"fmax.pi f8, f6, f4\n"                                
			"fmul.s f9, f7, f2, rmm\n"                            
			"fle.pi f3, f4, f3\n"                                 
			"fpackrepb.pi f5, f6\n"                               
			//RTLMIN-5479"frsq.ps f9, f7\n"                                    
			"fcvt.pw.ps f3, f6, dyn\n"                            
			//RTLMIN-5479"fsin.ps f4, f6\n"                                    
			"fmv.x.s x4, f0\n"                                    
			"feq.ps f4, f4, f5\n"                                 
			"fsgnjx.s f5, f4, f5\n"                               
			"flw.ps f5, -1083(x11)\n"                             
			"fpackrepb.pi f3, f1\n"                               
			"fsat8.pi f9, f6\n"                                   
			"for.pi f8, f7, f4\n"                                 
			"fmvs.x.ps x3, f1, 3\n"                               
			"fmin.s f7, f1, f0\n"                                 
			"fsgnjn.s f3, f1, f4\n"                               
			"fcvt.s.w f2, x11, dyn\n"                             
			"fmin.s f6, f7, f6\n"                                 
			"fmsub.s f5, f6, f6, f1, rmm\n"                       
			"fmin.pi f9, f0, f4\n"                                
			"fadd.pi f5, f4, f6\n"                                
			"fnmsub.ps f5, f1, f2, f3, rne\n"                     
			"fbci.pi f8, 419580\n"                                
			"fltm.ps m2, f6, f1\n"                                
			"fclass.ps f9, f4\n"                                  
			"fcvt.s.w f2, x11, rne\n"                             
			"maskpopcz x3, m1\n"                                  
			"fcvt.ps.pwu f5, f6, rdn\n"                           
			"flt.ps f2, f1, f0\n"                                 
			"fcvt.ps.f16 f5, f3\n"                                
			"fle.pi f5, f4, f3\n"                                 
			"fle.ps f7, f0, f7\n"                                 
			"fcvt.wu.s x4, f6, dyn\n"                             
			"mov.m.x m1, x11, 93\n"                               
			"for.pi f2, f2, f0\n"                                 
			"flw.ps f8, -1918(x11)\n"                             
			"fsgnjx.ps f9, f4, f2\n"                              
			"fsgnj.s f4, f7, f1\n"                                
			"fmax.pi f4, f1, f6\n"                                
			"fsub.s f9, f2, f4, rtz\n"                            
			"fsub.s f4, f5, f3, rdn\n"                            
			"fmul.s f9, f7, f3, rtz\n"                            
			"fsrli.pi f7, f7, 7\n"                                
			"feqm.ps m1, f2, f5\n"                                
			"fsrli.pi f5, f2, 13\n"                               
			"flem.ps m1, f2, f2\n"                                
			"fle.s x4, f2, f6\n"                                  
			"fsatu8.pi f6, f5\n"                                  
			"fmax.s f5, f6, f2\n"                                 
			"fsgnjn.ps f7, f4, f6\n"                              
			"flog.ps f3, f2\n"                                    
			"fsgnjx.s f5, f2, f0\n"                               
			"mova.x.m x3\n"                                       
			"fsrli.pi f2, f6, 7\n"                                
			"fmsub.ps f2, f4, f0, f4, rtz\n"                      
			"fsgnjn.ps f8, f2, f6\n"                              
			"fmaxu.pi f5, f7, f0\n"                               
			"fcvt.s.wu f3, x11, rdn\n"                            
			"fadd.pi f2, f3, f3\n"                                
			"fsgnj.ps f6, f0, f3\n"                               
			"fmul.s f3, f3, f1, rmm\n"                            
			"fltm.ps m2, f4, f2\n"                                
			"maskand m0, m2, m0\n"                                
			"fnot.pi f8, f2\n"                                    
			"fandi.pi f3, f1, 205\n"                              
			"fnmadd.ps f6, f2, f6, f6, rmm\n"                     
			"fmulhu.pi f8, f1, f1\n"                              
			//RTLMIN-5479"frsq.ps f5, f3\n"                                    
			"fsrai.pi f2, f7, 8\n"                                
			"fsub.ps f2, f5, f0, dyn\n"                           
			"fexp.ps f4, f5\n"                                    
			"flw f2, -339(x11)\n"                                 
			"fnot.pi f7, f5\n"                                    
			"fmsub.ps f7, f2, f7, f0, rmm\n"                      
			"fmaxu.pi f9, f0, f6\n"                               
			"fcvt.f16.ps f7, f2\n"                                
			"fmv.s.x f2, x11\n"                                   
			//RTLMIN-5479"frsq.ps f4, f2\n"                                    
			"fnmadd.ps f5, f5, f3, f3, rdn\n"                     
			"mov.m.x m0, x11, 69\n"                               
			"mova.x.m x3\n"                                       
			"fmv.s.x f3, x11\n"                                   
			"fmin.pi f5, f7, f1\n"                                
			"flq2 f3, 159(x11)\n"                                 
			"feqm.ps m2, f1, f5\n"                                
			"fsub.s f3, f3, f4, rne\n"                            
			"fbci.ps f9, 103987\n"                                
			"fmv.s.x f4, x11\n"                                   
			"fswizz.ps f8, f2, 105\n"                             
			"fslli.pi f4, f0, 7\n"                                
			"fsra.pi f3, f0, f5\n"                                
			"fmadd.ps f7, f6, f4, f2, rne\n"                      
			"feq.pi f3, f6, f6\n"                                 
			"fbci.pi f3, 82812\n"                                 
			"fmul.s f7, f1, f7, rtz\n"                            
			"fbcx.ps f4, x11\n"                                   
			"fle.ps f2, f3, f3\n"                                 
			"fround.ps f5, f0, rtz\n"                             
			"fsat8.pi f5, f1\n"                                   
			"fnmadd.ps f6, f4, f3, f3, dyn\n"                     
			"fsw f2, 971(x11)\n"                                  
			"fmvs.x.ps x3, f4, 4\n"                               
			"fltm.ps m0, f2, f1\n"                                
			"fmin.ps f2, f0, f7\n"                                
			"fsetm.pi m1, f3\n"                                   
			"fmul.ps f5, f6, f3, rtz\n"                           
			"ffrc.ps f3, f6\n"                                    
			"fsw f3, -1049(x11)\n"                                
			"fxor.pi f2, f1, f5\n"                                
			"fsll.pi f4, f7, f7\n"                                
			"fsrl.pi f8, f2, f4\n"                                
			"fslli.pi f4, f3, 13\n"                               
			"flw.ps f9, -566(x11)\n"                              
			"fslli.pi f7, f4, 6\n"                                
			"fsat8.pi f8, f2\n"                                   
			"faddi.pi f4, f7, 392\n"                              
			"flem.ps m0, f7, f6\n"                                
			"fmadd.s f8, f6, f0, f4, dyn\n"                       
			"fcvt.s.w f8, x11, dyn\n"                             
			"fsrli.pi f8, f2, 2\n"                                
			"fmin.s f8, f0, f1\n"                                 
			"fsw f0, 1455(x11)\n"                                 
			"fnmadd.ps f7, f1, f2, f7, dyn\n"                     
			"fmadd.s f3, f1, f3, f6, rdn\n"                       
			"fsgnjn.ps f7, f6, f2\n"                              
			"fcvt.f16.ps f6, f6\n"                                
			"fsub.pi f8, f1, f4\n"                                
			"fmv.x.s x4, f6\n"                                    
			"fmul.s f3, f1, f0, rmm\n"                            
			"fle.pi f2, f3, f7\n"                                 
			"fswizz.ps f8, f4, 73\n"                              
			"fsrai.pi f4, f6, 10\n"                               
			"fcvt.ps.pw f7, f2, rtz\n"                            
			"fcmov.ps f6, f1, f2, f4\n"                           
			"fpackreph.pi f8, f1\n"                               
			"fmaxu.pi f4, f0, f6\n"                               
			"fmadd.s f4, f4, f7, f1, rdn\n"                       
			"fcmovm.ps f6, f2, f1\n"                              
			"fmvs.x.ps x3, f7, 7\n"                               
			"flq2 f6, -4(x11)\n"                                  
			"fnmsub.ps f5, f2, f2, f4, rup\n"                     
			"masknot m0, m2\n"                                    
			"fcvt.w.s x4, f2, rmm\n"                              
			"fcvt.wu.s x3, f7, rmm\n"                             
			"fxor.pi f3, f0, f5\n"                                
			"maskor m1, m0, m2\n"                                 
			"flt.ps f7, f7, f3\n"                                 
			"fsgnj.ps f7, f3, f0\n"                               
			"feq.s x4, f0, f7\n"                                  
			"fand.pi f2, f3, f6\n"                                
			"fsgnj.s f7, f3, f2\n"                                
			"fmv.x.s x4, f5\n"                                    
			"masknot m0, m1\n"                                    
			"feqm.ps m2, f4, f7\n"                                

			"li x5, 0xf9d134286f705a5c\n"
			"mova.m.x x5\n"                                       
			"fsgnj.s f7, f0, f5\n"                                
			"flw.ps f6, -376(x11)\n"                              
			"fpackreph.pi f5, f2\n"                               
			"fcvt.ps.pw f2, f5, rdn\n"                            
			"mov.m.x m0, x11, 99\n"                               
			"fbcx.ps f6, x11\n"                                   
			"fand.pi f2, f6, f6\n"                                
			"fmul.ps f3, f0, f4, dyn\n"                           
			"fnmadd.s f2, f2, f4, f2, rtz\n"                      
			"fltm.pi m1, f7, f1\n"                                
			"fltu.pi f6, f6, f2\n"                                
			"fbc.ps f6, 370(x11)\n"                               
			"fsub.ps f6, f3, f7, rmm\n"                           
			"flq2 f8, 896(x11)\n"                                 
			"fcvt.pwu.ps f4, f5, dyn\n"                           
			"fcvt.ps.pw f5, f7, rtz\n"                            
			"fswizz.ps f5, f7, 57\n"                              
			"fmvs.x.ps x4, f0, 7\n"                               
			"fcmovm.ps f6, f3, f4\n"                              
			"fltu.pi f4, f6, f1\n"                                
			"flw.ps f2, -684(x11)\n"                              
			"fsq2 f4, 647(x11)\n"                                 
			"fmax.pi f7, f0, f1\n"                                
			"fadd.s f2, f0, f5, rdn\n"                            
			"fsgnjx.ps f8, f0, f4\n"                              
			"fcmovm.ps f7, f7, f4\n"                              
			"fbcx.ps f5, x11\n"                                   
			"flt.ps f9, f7, f7\n"                                 
			"fcvt.pwu.ps f2, f5, rtz\n"                           
			"fsgnjx.s f6, f4, f3\n"                               
			"fmsub.ps f4, f5, f0, f6, rtz\n"                      
			"feq.pi f6, f3, f1\n"                                 
			"fcvt.pw.ps f3, f2, rup\n"                            
			"fmulhu.pi f7, f6, f4\n"                              
			"fmsub.s f8, f7, f6, f1, rne\n"                       
			"fcmovm.ps f3, f0, f5\n"                              
			"fadd.s f2, f3, f2, rdn\n"                            
			"fltm.pi m0, f1, f2\n"                                
			"flt.s x3, f0, f6\n"                                  
			"fsw f4, -1064(x11)\n"                                
			"fsgnjn.s f6, f1, f2\n"                               
			"faddi.pi f2, f1, 197\n"                              
			"fmaxu.pi f4, f4, f0\n"                               
			"fcvt.s.wu f7, x11, rdn\n"                            
			"fnot.pi f2, f4\n"                                    
			"fswizz.ps f2, f3, 43\n"                              
			"fsgnj.ps f7, f5, f3\n"                               
			"fmvs.x.ps x4, f2, 5\n"                               
			"fslli.pi f3, f5, 9\n"                                
			"fle.ps f3, f5, f7\n"                                 
			"flw.ps f5, 961(x11)\n"                               
			"fsra.pi f4, f3, f6\n"                                
			"fmvs.x.ps x3, f2, 3\n"                               
			"frcp.ps f7, f1\n"                                    
			"fbci.pi f9, 226056\n"                                
			"fsetm.pi m2, f6\n"                                   
			"fmaxu.pi f5, f3, f2\n"                               
			"flw.ps f3, -16(x11)\n"                               
			"fcvt.ps.f16 f4, f2\n"                                
			"fsw f5, 58(x11)\n"                                   
			"fsra.pi f6, f7, f2\n"                                
			"mov.m.x m0, x11, 120\n"                              
			"fround.ps f6, f7, dyn\n"                             

			"li x5, 0xacadc191e27c497c\n"
			"mova.m.x x5\n"                                       
			"flw.ps f6, 219(x11)\n"                               
			"fminu.pi f7, f6, f3\n"                               
			"fnmadd.s f5, f0, f3, f2, rdn\n"                      
			"flt.pi f6, f2, f3\n"                                 
			"fround.ps f8, f1, dyn\n"                             
			"fclass.s x4, f1\n"                                   
			"fcvt.s.w f9, x11, rmm\n"                             
			"fsgnjn.s f8, f3, f0\n"                               
			"fslli.pi f3, f0, 9\n"                                
			"fxor.pi f2, f1, f6\n"                                
			"fcmovm.ps f8, f7, f0\n"                              
			"fmulh.pi f7, f3, f3\n"                               
			"fsub.s f2, f2, f6, rne\n"                            
			"fmul.ps f3, f3, f2, rup\n"                           
			"fexp.ps f5, f7\n"                                    
			"feq.ps f6, f4, f1\n"                                 
			"fcvt.s.wu f3, x11, rup\n"                            
			"fmvz.x.ps x3, f6, 1\n"                               
			"fnmsub.ps f3, f1, f5, f7, dyn\n"                     
			"fnot.pi f4, f2\n"                                    
			"fbcx.ps f8, x11\n"                                   
			"flt.ps f4, f4, f7\n"                                 
			"fcvt.ps.pw f6, f2, rne\n"                            
			"fsw.ps f4, 174(x11)\n"                               
			"fnmadd.s f9, f7, f3, f2, rup\n"                      
			"flt.s x3, f7, f0\n"                                  
			"fbc.ps f8, 1878(x11)\n"                              
			"fnmadd.s f5, f2, f6, f2, dyn\n"                      
			"fmul.s f9, f0, f1, rdn\n"                            
			"flw f7, -1156(x11)\n"                                
			//RTLMIN-5479"fsin.ps f7, f2\n"                                    
			"fsgnj.ps f5, f7, f2\n"                               
			"fxor.pi f6, f5, f7\n"                                
			"fmax.ps f4, f5, f7\n"                                
			"fcvt.ps.pwu f7, f3, rne\n"                           
			"fadd.ps f9, f5, f6, rtz\n"                           
			"fmax.s f5, f4, f6\n"                                 
			"fnmsub.ps f3, f6, f6, f0, rmm\n"                     
			"maskpopc x3, m1\n"                                   
			"fsgnj.s f4, f0, f7\n"                                
			"fcvt.ps.f16 f5, f0\n"                                
			//RTLMIN-5479"fsin.ps f7, f1\n"                                    
			"maskand m2, m1, m1\n"                                
			"fsrli.pi f7, f6, 14\n"                               
			"fpackrepb.pi f7, f1\n"                               
			"fnmsub.ps f3, f4, f4, f5, rne\n"                     
			"fle.s x3, f1, f3\n"                                  
			"ffrc.ps f7, f2\n"                                    
			"fsw.ps f5, -1165(x11)\n"                             
			"feqm.ps m0, f1, f6\n"                                
			"fbc.ps f5, -397(x11)\n"                              
			"fnmsub.ps f3, f4, f5, f4, rne\n"                     
			"fsub.s f2, f6, f3, rdn\n"                            
			"ffrc.ps f2, f0\n"                                    
			"fmin.ps f7, f0, f7\n"                                
			"fnmadd.ps f2, f6, f5, f5, rup\n"                     
			"frcp.ps f3, f7\n"                                    
			"fcvt.pw.ps f6, f6, rtz\n"                            
			"fandi.pi f4, f4, 345\n"                              
			//RTLMIN-5479"fsin.ps f9, f4\n"                                    
			"fslli.pi f4, f4, 7\n"                                
			//RTLMIN-5479"fsin.ps f8, f1\n"                                    
			"maskor m1, m1, m2\n"                                 
			"flt.s x4, f5, f0\n"                                  
			"fmulh.pi f2, f5, f5\n"                               
			"fltm.ps m1, f4, f5\n"                                
			"fsgnjn.s f7, f2, f1\n"                               
			"fmadd.ps f8, f6, f4, f3, rtz\n"                      
			"fadd.s f7, f3, f0, rdn\n"                            
			"fltm.pi m1, f7, f5\n"                                
			"fandi.pi f4, f0, 140\n"                              
			"fsrl.pi f7, f5, f4\n"                                
			"fltm.ps m0, f5, f5\n"                                
			"fcvt.w.s x4, f7, dyn\n"                              
			"fnot.pi f2, f3\n"                                    
			"for.pi f5, f1, f5\n"                                 
			"fmul.s f7, f1, f0, rtz\n"                            
			"fmsub.s f7, f7, f6, f1, dyn\n"                       
			"fadd.pi f9, f0, f4\n"                                
			"fsw f0, -171(x11)\n"                                 
			"fminu.pi f5, f6, f5\n"                               
			"fsat8.pi f7, f6\n"                                   
			"fltm.ps m2, f2, f1\n"                                
			"fadd.pi f5, f3, f1\n"                                
			"fmax.ps f2, f3, f2\n"                                
			"fadd.pi f5, f1, f6\n"                                
			"fle.ps f8, f3, f4\n"                                 
			"fsgnj.ps f8, f2, f2\n"                               
			"fnmadd.ps f8, f4, f3, f3, rdn\n"                     
			"fpackrepb.pi f6, f6\n"                               
			"fnot.pi f4, f4\n"                                    
			"fsetm.pi m0, f6\n"                                   
			"for.pi f6, f1, f2\n"                                 
			//RTLMIN-5479"frsq.ps f7, f0\n"                                    
			"fmax.s f9, f2, f3\n"                                 
			"fcvt.s.w f3, x11, rdn\n"                             
			"fnmadd.ps f5, f0, f6, f7, rup\n"                     
			//RTLMIN-5479"fsin.ps f3, f0\n"                                    
			"fsgnjn.ps f4, f4, f6\n"                              
			"fmax.s f4, f4, f5\n"                                 
			"fpackrepb.pi f6, f7\n"                               
			"feqm.ps m0, f6, f7\n"                                
			"flw.ps f2, 1406(x11)\n"                              
			//RTLMIN-5479"fsin.ps f3, f0\n"                                    
			"fsatu8.pi f4, f5\n"                                  
			"fmul.ps f3, f6, f6, rdn\n"                           
			"faddi.pi f4, f4, 74\n"                               
			"ffrc.ps f3, f4\n"                                    
			"fsw.ps f7, -728(x11)\n"                              
			"fmax.ps f8, f5, f5\n"                                
			"fle.s x3, f7, f4\n"                                  
			"fmul.s f8, f1, f2, rdn\n"                            
			"fnmsub.ps f8, f2, f5, f2, rne\n"                     
			"fle.pi f5, f6, f4\n"                                 

			"li x5, 0x91d01e5d4d9c97fb\n"
			"mova.m.x x5\n"                                       
			"fnmadd.ps f4, f7, f7, f0, rmm\n"                     
			"feq.s x4, f2, f4\n"                                  
			"fandi.pi f4, f1, 294\n"                              
			"maskpopcz x3, m2\n"                                  
			"fmsub.ps f3, f1, f2, f6, rmm\n"                      
			"fmulhu.pi f3, f3, f6\n"                              
			//RTLMIN-5479"frsq.ps f7, f6\n"                                    
			"fsub.ps f5, f3, f5, rtz\n"                           
			"fsrli.pi f9, f4, 14\n"                               
			"fnmadd.s f2, f4, f2, f5, rne\n"                      
			"fmin.ps f8, f7, f4\n"                                
			"fsq2 f3, 1043(x11)\n"                                
			"fsw.ps f1, 341(x11)\n"                               
			"fltu.pi f6, f3, f6\n"                                
			"fcmovm.ps f5, f4, f4\n"                              
			"fsgnjn.s f7, f4, f7\n"                               
			"fmadd.ps f6, f3, f3, f5, rup\n"                      
			"flt.s x4, f6, f0\n"                                  
			"fmsub.ps f6, f3, f6, f5, rdn\n"                      
			"fcvt.s.wu f7, x11, rdn\n"                            
			"fmv.x.s x3, f2\n"                                    
			"fmv.s.x f3, x11\n"                                   
			"feq.s x3, f6, f5\n"                                  
			"fpackreph.pi f3, f2\n"                               
			"fminu.pi f7, f7, f2\n"                               
			"fcvt.ps.f16 f8, f2\n"                                
			"fmul.s f4, f3, f6, dyn\n"                            
			"fsw f4, -744(x11)\n"                                 
			"fcvt.pw.ps f9, f6, dyn\n"                            
			"fmsub.s f5, f0, f0, f5, rne\n"                       
			"flw f2, 410(x11)\n"                                  
			"fclass.s x4, f2\n"                                   
			"fmin.s f8, f5, f1\n"                                 
			"fnmsub.s f9, f0, f5, f4, rtz\n"                      
			"fnot.pi f9, f6\n"                                    
			"fxor.pi f2, f0, f6\n"                                
			"fadd.s f4, f6, f2, rne\n"                            
			"fnmsub.s f8, f3, f2, f2, rmm\n"                      
			"maskpopcz x3, m2\n"                                  
			"fsetm.pi m0, f5\n"                                   
			"for.pi f7, f5, f6\n"                                 
			"fsll.pi f4, f2, f2\n"                                
			"flw f4, 1578(x11)\n"                                 
			"fmax.s f8, f4, f0\n"                                 
			"fmsub.ps f9, f2, f5, f0, rne\n"                      
			"fadd.pi f5, f4, f7\n"                                
			"fswizz.ps f5, f3, 29\n"                              
			"fmul.s f7, f7, f4, rne\n"                            
			"fsub.ps f8, f1, f4, rtz\n"                           
			"fsgnj.ps f4, f5, f5\n"                               
			"fle.pi f2, f1, f6\n"                                 
			"fbcx.ps f7, x11\n"                                   
			"maskpopc x4, m0\n"                                   
			"maskand m1, m1, m2\n"                                
			"fmulhu.pi f8, f2, f2\n"                              
			"fcvt.ps.pw f6, f6, rmm\n"                            
			"fcvt.pw.ps f6, f2, dyn\n"                            
			"fsgnjn.s f2, f4, f5\n"                               

			"li x5, 0xd8086e05955cee8f\n"
			"mova.m.x x5\n"                                       
			"fmin.pi f2, f3, f3\n"                                
			"fcvt.pwu.ps f7, f1, rup\n"                           
			"fcvt.ps.pw f9, f6, rmm\n"                            
			"fmv.s.x f2, x11\n"                                   
			"ffrc.ps f3, f1\n"                                    
			"fnmadd.s f4, f0, f2, f7, rdn\n"                      
			"fcvt.w.s x3, f2, rtz\n"                              
			"masknot m2, m1\n"                                    
			"flq2 f9, 48(x11)\n"                                  
			"fmvs.x.ps x4, f2, 3\n"                               
			"fadd.ps f8, f1, f2, rup\n"                           
			"feq.pi f3, f7, f2\n"                                 
			"maskor m2, m1, m0\n"                                 
			"feq.ps f4, f7, f1\n"                                 
			"fsgnjx.s f8, f3, f3\n"                               
			"fmv.s.x f4, x11\n"                                   
			"fcvt.ps.pwu f7, f2, rtz\n"                           
			"fmvs.x.ps x3, f2, 7\n"                               
			"flt.pi f4, f1, f1\n"                                 
			"fcvt.pw.ps f9, f1, rup\n"                            
			"fmul.s f4, f1, f3, rtz\n"                            
			"fle.pi f6, f2, f4\n"                                 
			"fsq2 f4, -1080(x11)\n"                               
			"flt.s x3, f4, f4\n"                                  
			"fcvt.pwu.ps f2, f5, rup\n"                           
			"feq.s x4, f4, f0\n"                                  
			"fswizz.ps f3, f2, 30\n"                              
			"fxor.pi f2, f5, f5\n"                                

			"li x5, 0x119fda6f2efa69b7\n"
			"mova.m.x x5\n"                                       
			"fsrli.pi f4, f7, 3\n"                                
			"fclass.s x3, f4\n"                                   
			"fsetm.pi m2, f0\n"                                   
			"fsgnjn.ps f4, f1, f4\n"                              
			"fbcx.ps f7, x11\n"                                   
			"fmsub.s f7, f5, f6, f5, rup\n"                       
			"fsub.s f4, f6, f1, dyn\n"                            
			"fsub.pi f6, f2, f6\n"                                
			"fmsub.s f6, f5, f4, f3, rmm\n"                       
			"fsgnjn.s f2, f4, f3\n"                               
			"fsrl.pi f8, f5, f6\n"                                
			"fsgnjn.s f2, f0, f3\n"                               
			"fcvt.wu.s x3, f4, rne\n"                             
			"fbci.pi f5, 423454\n"                                
			"maskpopcz x4, m1\n"                                  
			"fcvt.wu.s x4, f5, dyn\n"                             
			"fsgnj.s f6, f5, f4\n"                                
			"maskpopcz x3, m2\n"                                  
			"fsll.pi f8, f4, f1\n"                                
			"fsgnjn.ps f3, f2, f2\n"                              
			"for.pi f5, f4, f6\n"                                 
			"fsw.ps f7, 1705(x11)\n"                              
			"fclass.s x4, f3\n"                                   
			"maskpopc x3, m2\n"                                   
			"mov.m.x m1, x11, 30\n"                               
			"fnmadd.s f9, f5, f5, f6, rmm\n"                      
			"fsetm.pi m0, f3\n"                                   
			"mov.m.x m2, x11, 98\n"                               
			"fbc.ps f6, 551(x11)\n"                               
			"fle.ps f8, f5, f0\n"                                 
			"flt.pi f5, f7, f2\n"                                 
			"flt.ps f8, f4, f2\n"                                 
			"fsrai.pi f4, f4, 9\n"                                
			"fle.pi f6, f0, f6\n"                                 
			"fminu.pi f6, f5, f5\n"                               
			"for.pi f6, f3, f7\n"                                 
			"maskpopc x4, m2\n"                                   
			"fsub.pi f4, f2, f2\n"                                
			"fsatu8.pi f8, f6\n"                                  
			"flw.ps f3, 1708(x11)\n"                              
			"fbcx.ps f6, x11\n"                                   
			"fmv.x.s x4, f2\n"                                    
			"fcvt.s.w f8, x11, rdn\n"                             
			//RTLMIN-5479"frsq.ps f7, f1\n"                                    
			"fsrai.pi f5, f0, 1\n"                                
			//RTLMIN-5479"frsq.ps f4, f2\n"                                    
			"fmsub.ps f4, f2, f6, f6, rne\n"                      
			"fand.pi f2, f4, f4\n"                                
			"flog.ps f6, f3\n"                                    
			"fswizz.ps f5, f2, 108\n"                             
			"fsll.pi f4, f5, f1\n"                                
			"fmax.ps f6, f6, f7\n"                                
			"fmin.s f7, f5, f3\n"                                 
			"fsw.ps f0, 1486(x11)\n"                              
			"flw.ps f2, -108(x11)\n"                              
			"fsgnjx.s f9, f2, f2\n"                               
			"fnot.pi f3, f5\n"                                    
			"maskpopc x3, m2\n"                                   
			"fmulhu.pi f6, f4, f3\n"                              

			"li x5, 0x760a329c6b9c41f5\n"
			"mova.m.x x5\n"                                       
			"fcvt.ps.pwu f8, f7, rtz\n"                           
			"fsetm.pi m0, f4\n"                                   
			"fbci.ps f3, 257391\n"                                
			"fsrl.pi f8, f4, f7\n"                                
			"fle.ps f2, f4, f7\n"                                 
			"fcvt.s.wu f2, x11, dyn\n"                            
			"fexp.ps f7, f3\n"                                    
			"fle.ps f2, f4, f1\n"                                 
			"fadd.pi f6, f3, f5\n"                                
			"fclass.s x3, f4\n"                                   
			"fltm.ps m1, f4, f3\n"                                
			"fsll.pi f3, f5, f6\n"                                
			"fsub.ps f9, f6, f2, rdn\n"                           
			"ffrc.ps f2, f3\n"                                    
			"fsrai.pi f4, f4, 13\n"                               
			//RTLMIN-5479"frsq.ps f3, f2\n"                                    
			//RTLMIN-5479"fsin.ps f4, f4\n"                                    
			"frcp.ps f2, f7\n"                                    
			"flw.ps f9, 1536(x11)\n"                              
			"fsgnj.ps f9, f0, f7\n"                               
			"flw.ps f2, -1528(x11)\n"                             
			"fmul.s f7, f5, f5, rtz\n"                            
			"maskxor m2, m0, m1\n"                                
			"fnot.pi f2, f7\n"                                    
			"fmv.s.x f4, x11\n"                                   
			"fmul.s f8, f7, f7, rne\n"                            
			"flt.pi f6, f1, f6\n"                                 
			"fbci.pi f6, 207067\n"                                
			"mova.x.m x4\n"                                       
			"frcp.ps f5, f2\n"                                    
			"fcvt.ps.pwu f4, f6, rup\n"                           
			"maskpopc x4, m2\n"                                   
			"flw.ps f7, -721(x11)\n"                              
			"fltm.ps m2, f5, f3\n"                                
			"maskor m2, m1, m0\n"                                 
			//RTLMIN-5479"frsq.ps f2, f5\n"                                    
			"maskand m2, m0, m0\n"                                
			"ffrc.ps f3, f5\n"                                    
			"fsub.ps f7, f4, f7, rmm\n"                           
			"fmax.ps f7, f7, f5\n"                                
			"fnot.pi f9, f3\n"                                    
			"fcvt.w.s x4, f0, rne\n"                              
			"fround.ps f8, f7, rne\n"                             

			"li x5, 0x42ca1d92c9226b1\n"
			"mova.m.x x5\n"                                       
			"fsw.ps f5, 765(x11)\n"                               
			"flog.ps f4, f2\n"                                    
			"fcvt.f16.ps f6, f2\n"                                
			"fsrli.pi f6, f7, 10\n"                               
			"fmul.ps f5, f2, f2, rne\n"                           
			"fpackrepb.pi f6, f0\n"                               
			"fcvt.wu.s x3, f1, rne\n"                             
			"fandi.pi f5, f4, 49\n"                               
			"fcvt.pw.ps f8, f6, rdn\n"                            
			"fslli.pi f4, f1, 12\n"                               
			"fsub.pi f4, f0, f0\n"                                
			"fltu.pi f7, f1, f0\n"                                
			"fmin.s f2, f4, f3\n"                                 
			"faddi.pi f6, f0, 274\n"                              
			"fexp.ps f8, f2\n"                                    
			"maskpopcz x4, m1\n"                                  
			"maskpopc x3, m1\n"                                   
			"flt.ps f8, f0, f5\n"                                 
			"faddi.pi f6, f2, 145\n"                              
			"maskpopcz x3, m1\n"                                  
			"fcvt.ps.pwu f9, f7, rup\n"                           
			"flt.ps f3, f5, f3\n"                                 
			"fnot.pi f8, f3\n"                                    
			"fandi.pi f6, f6, 152\n"                              
			"fcvt.pwu.ps f3, f7, rne\n"                           
			"fcvt.ps.pw f6, f4, rdn\n"                            
			"flt.s x4, f4, f7\n"                                  
			"flt.ps f3, f5, f5\n"                                 
			"fclass.ps f8, f7\n"                                  
			"fmax.ps f8, f3, f1\n"                                
			"frcp.ps f6, f5\n"                                    
			"fcvt.pw.ps f8, f7, rmm\n"                            
			"fbc.ps f3, 1589(x11)\n"                              
			"fcvt.f16.ps f3, f7\n"                                
			"fadd.ps f2, f7, f0, rmm\n"                           
			"flog.ps f8, f5\n"                                    
			"fmvz.x.ps x3, f1, 1\n"                               
			"fmv.x.s x4, f3\n"                                    
			"fmaxu.pi f9, f1, f3\n"                               
			"fmsub.ps f7, f4, f1, f4, rne\n"                      
			"fsra.pi f6, f5, f6\n"                                
			"fcvt.pwu.ps f7, f1, rtz\n"                           
			"flw.ps f5, -1910(x11)\n"                             
			"frcp.ps f5, f3\n"                                    
			"maskpopcz x3, m2\n"                                  

			"li x5, 0x8bb4f4540375d35\n"
			"mova.m.x x5\n"                                       
			"fcvt.ps.pwu f8, f2, rup\n"                           
			"fsw f1, -385(x11)\n"                                 
			"fsw.ps f2, 577(x11)\n"                               
			"mova.x.m x3\n"                                       
			"fsrli.pi f8, f6, 2\n"                                
			"fmulhu.pi f8, f7, f2\n"                              
			"fle.ps f5, f2, f0\n"                                 
			"fltm.pi m1, f5, f2\n"                                
			//RTLMIN-5479"frsq.ps f8, f7\n"                                    
			"fsgnjn.ps f7, f4, f3\n"                              
			"flq2 f6, 389(x11)\n"                                 

			"li x5, 0xfdb099d824d9f7a4\n"
			"mova.m.x x5\n"                                       
			"fmsub.s f8, f0, f6, f6, rmm\n"                       
			"fsw f0, -1757(x11)\n"                                
			"fnot.pi f5, f1\n"                                    
			//RTLMIN-5479"frsq.ps f5, f0\n"                                    
			"fsetm.pi m1, f3\n"                                   
			"fbci.pi f7, 51309\n"                                 
			"fsub.ps f6, f6, f1, rup\n"                           
			"fmul.s f7, f4, f4, rmm\n"                            

			"li x5, 0x86042684f7bd3ac4\n"
			"mova.m.x x5\n"                                       
			"fcvt.ps.pwu f9, f1, rne\n"                           
			"fbc.ps f5, 280(x11)\n"                               
			"fbci.ps f5, 354948\n"                                
			"maskand m0, m2, m2\n"                                
			"faddi.pi f7, f1, 508\n"                              
			"fpackreph.pi f5, f4\n"                               
			"fcvt.wu.s x3, f6, dyn\n"                             
			"fmax.pi f8, f0, f7\n"                                
			"flt.ps f2, f4, f2\n"                                 
			"flem.ps m1, f1, f5\n"                                
			//RTLMIN-5479"frsq.ps f6, f3\n"                                    
			"fandi.pi f6, f3, 228\n"                              
			"for.pi f6, f3, f4\n"                                 
			"fsw.ps f6, -1927(x11)\n"                             
			"fmulhu.pi f9, f2, f6\n"                              
			"fcmov.ps f8, f4, f0, f2\n"                           
			"feq.s x4, f3, f4\n"                                  
			"fclass.s x3, f0\n"                                   
			"maskxor m0, m1, m2\n"                                

			"li x5, 0x30f85321f68fb6a3\n"
			"mova.m.x x5\n"                                       
			"fsub.ps f3, f0, f1, rup\n"                           
			"fmul.ps f9, f3, f4, rup\n"                           
			"fcvt.w.s x3, f7, dyn\n"                              

			"li x5, 0xfb89724bd78942b9\n"
			"mova.m.x x5\n"                                       
			"frcp.ps f4, f0\n"                                    
			"fsetm.pi m2, f2\n"                                   
			"fminu.pi f4, f5, f7\n"                               
			"fmsub.s f6, f0, f3, f3, rtz\n"                       
			"fsat8.pi f6, f0\n"                                   
			"fpackrepb.pi f6, f6\n"                               
			"fnmsub.ps f5, f4, f0, f3, rup\n"                     
			"fcvt.ps.pwu f9, f5, rne\n"                           
			"fcvt.ps.pwu f5, f7, rdn\n"                           
			"mov.m.x m2, x11, 8\n"                                
			"fcvt.pw.ps f9, f4, rtz\n"                            
			"masknot m1, m0\n"                                    
			"fsrai.pi f7, f0, 10\n"                               
			"fsgnj.ps f7, f4, f1\n"                               
			"fbci.pi f4, 236394\n"                                
			"fle.pi f5, f2, f7\n"                                 
			"fandi.pi f3, f1, 181\n"                              
			"feq.ps f4, f3, f5\n"                                 
			"fmin.ps f5, f0, f6\n"                                
			"fround.ps f7, f4, dyn\n"                             
			//RTLMIN-5479"fsin.ps f8, f0\n"                                    
			"flt.ps f3, f5, f1\n"                                 
			"fmin.ps f7, f0, f5\n"                                
			"mov.m.x m0, x11, 13\n"                               
			"fbcx.ps f7, x11\n"                                   
			"fsrl.pi f8, f3, f1\n"                                
			"fcvt.f16.ps f9, f3\n"                                
			"fsetm.pi m1, f6\n"                                   
			"flem.ps m0, f1, f2\n"                                
			//RTLMIN-5479"fsin.ps f9, f7\n"                                    
			"fmax.pi f2, f3, f3\n"                                
			"fadd.s f8, f2, f3, rne\n"                            
			"fsgnjn.s f8, f3, f7\n"                               
			"feq.s x3, f7, f0\n"                                  
			"fcvt.w.s x4, f6, dyn\n"                              
			"maskpopc x3, m2\n"                                   
			"fsetm.pi m1, f0\n"                                   
			"fmv.x.s x4, f0\n"                                    
			"for.pi f7, f1, f3\n"                                 
			"fxor.pi f3, f5, f2\n"                                
			"flq2 f5, 759(x11)\n"                                 
			"fcvt.w.s x3, f2, rmm\n"                              
			"mov.m.x m0, x11, 41\n"                               
			"fsgnjx.s f7, f7, f2\n"                               
			"fle.ps f6, f4, f1\n"                                 
			"flt.ps f7, f0, f0\n"                                 
			"fsrli.pi f7, f1, 7\n"                                
			"fswizz.ps f5, f1, 41\n"                              
			"fsgnjx.ps f9, f3, f2\n"                              
			"fmvz.x.ps x3, f6, 1\n"                               
			"fmsub.s f8, f4, f3, f0, dyn\n"                       
			"fpackrepb.pi f7, f7\n"                               
			"feq.s x4, f1, f6\n"                                  
			"fmsub.s f3, f5, f7, f3, rtz\n"                       
			"faddi.pi f8, f5, 247\n"                              
			"fmax.ps f5, f7, f4\n"                                
			"fsrli.pi f5, f5, 12\n"                               
			"fslli.pi f5, f7, 2\n"                                

			"li x5, 0xabfa2a781747d7ef\n"
			"mova.m.x x5\n"                                       
			"fsub.s f2, f1, f0, rmm\n"                            
			"maskpopcz x3, m0\n"                                  
			"fltu.pi f7, f7, f1\n"                                
			"fadd.ps f7, f4, f7, rne\n"                           
			"fltm.ps m1, f4, f5\n"                                
			"fsgnjx.ps f5, f0, f4\n"                              
			"fsrli.pi f7, f5, 9\n"                                
			"fcvt.ps.pw f6, f6, rdn\n"                            
			"feq.s x4, f7, f6\n"                                  
			//RTLMIN-5479"frsq.ps f5, f6\n"                                    
			"fcvt.pwu.ps f2, f0, rup\n"                           
			"fnmadd.ps f7, f3, f5, f3, dyn\n"                     
			"fmadd.s f5, f5, f2, f4, rup\n"                       
			"fsq2 f7, -466(x11)\n"                                
			"fpackrepb.pi f9, f6\n"                               
			"fle.ps f7, f7, f1\n"                                 
			"maskpopc x3, m1\n"                                   
			"fsgnjx.ps f8, f1, f6\n"                              
			"fsgnj.s f2, f7, f3\n"                                

			"li x5, 0xefb97ac792e22b7a\n"
			"mova.m.x x5\n"                                       
			"fmvz.x.ps x3, f3, 6\n"                               
			"fmul.ps f2, f5, f1, rup\n"                           
			"fsgnjx.s f9, f1, f2\n"                               
			"fmulhu.pi f2, f3, f7\n"                              
			//RTLMIN-5479"frsq.ps f8, f7\n"                                    
			"fcmovm.ps f4, f1, f2\n"                              
			"masknot m1, m2\n"                                    
			"fmul.pi f3, f7, f1\n"                                
			//RTLMIN-5479"fsin.ps f7, f2\n"                                    
			"fsgnjn.s f8, f3, f0\n"                               
			"fmax.ps f5, f6, f4\n"                                
			"fsrl.pi f7, f2, f7\n"                                
			"fmax.pi f7, f2, f3\n"                                
			"masknot m0, m0\n"                                    
			"fmax.pi f6, f1, f5\n"                                
			"faddi.pi f6, f5, 163\n"                              
			"frcp.ps f7, f0\n"                                    
			"feqm.ps m1, f5, f2\n"                                
			"flt.ps f9, f4, f4\n"                                 
			"fsat8.pi f8, f6\n"                                   
			"feq.ps f6, f5, f1\n"                                 
			"fcvt.wu.s x3, f4, rne\n"                             
			"fsgnjx.s f6, f2, f7\n"                               
			"maskxor m0, m2, m0\n"                                
			"maskpopc x3, m1\n"                                   
			"fsrli.pi f6, f4, 5\n"                                
			"fsw f5, 332(x11)\n"                                  
			"fsq2 f1, -900(x11)\n"                                
			"maskor m0, m2, m0\n"                                 
			"feq.s x4, f1, f1\n"                                  
			"fsub.ps f2, f7, f2, rmm\n"                           
			"faddi.pi f6, f7, 280\n"                              
			"fmin.s f3, f7, f6\n"                                 
			"maskpopc x3, m0\n"                                   
			"fclass.s x3, f4\n"                                   
			"fsub.s f9, f5, f7, rup\n"                            
			"fcmov.ps f2, f7, f1, f5\n"                           
			"fcmov.ps f9, f0, f3, f3\n"                           
			"fcvt.ps.f16 f2, f7\n"                                
			"fltm.ps m0, f1, f3\n"                                
			"fnmadd.s f2, f2, f1, f4, rtz\n"                      
			"fnmsub.ps f6, f3, f1, f7, rmm\n"                     
			"fmsub.ps f5, f1, f0, f7, rne\n"                      
			"fbci.pi f9, 303619\n"                                
			"fsrai.pi f6, f4, 1\n"                                
			"maskpopc x4, m1\n"                                   
			"fnmadd.s f6, f0, f1, f6, rtz\n"                      
			"fandi.pi f2, f0, 243\n"                              
			"flt.ps f8, f0, f1\n"                                 
			"fnmsub.ps f8, f7, f6, f2, rup\n"                     
			"fcmovm.ps f2, f6, f6\n"                              
			"feq.s x4, f4, f6\n"                                  
			"ffrc.ps f4, f1\n"                                    
			"feq.ps f4, f7, f1\n"                                 
			"fbci.pi f7, 273850\n"                                
			"feqm.ps m0, f5, f2\n"                                
			//RTLMIN-5479"fsin.ps f5, f3\n"                                    
			"fnmsub.s f7, f4, f2, f7, rmm\n"                      
			"fcvt.s.w f2, x11, rne\n"                             
			"maskpopcz x3, m2\n"                                  
			"fbci.pi f6, 243238\n"                                
			"fmulhu.pi f4, f0, f5\n"                              
			"flq2 f6, -413(x11)\n"                                
			"fclass.ps f5, f4\n"                                  
			"fmulhu.pi f6, f2, f4\n"                              
			"fsgnjx.s f5, f5, f5\n"                               
			"fnot.pi f5, f3\n"                                    
			"fadd.ps f3, f4, f5, rne\n"                           
			"fcvt.ps.pw f4, f0, rdn\n"                            
			"fsgnj.ps f2, f7, f2\n"                               
			"flt.ps f2, f0, f4\n"                                 
			"fbci.pi f3, 136483\n"                                
			"fpackrepb.pi f5, f4\n"                               
			"faddi.pi f6, f1, 218\n"                              
			"fmvz.x.ps x4, f2, 5\n"                               
			"fsw.ps f2, -1631(x11)\n"                             
			"fcvt.pwu.ps f5, f1, rdn\n"                           
			"fsgnjn.s f8, f2, f7\n"                               
			"fltu.pi f4, f1, f4\n"                                
			"fcvt.ps.pwu f6, f7, rup\n"                           
			"fcmovm.ps f6, f7, f4\n"                              
			"fcvt.pwu.ps f5, f5, rup\n"                           
			"fmax.s f9, f4, f0\n"                                 
			//RTLMIN-5479"fsin.ps f6, f5\n"                                    
			"fcvt.w.s x3, f7, rne\n"                              
			"fsra.pi f7, f3, f0\n"                                
			//RTLMIN-5479"frsq.ps f5, f1\n"                                    
			"fbcx.ps f8, x11\n"                                   
			"fsrai.pi f5, f5, 6\n"                                
			"fsgnj.ps f4, f0, f5\n"                               
			"fmax.s f6, f0, f1\n"                                 
			"fmvz.x.ps x4, f4, 3\n"                               

			"li x5, 0xa0d75d90e8dc45a9\n"
			"mova.m.x x5\n"                                       
			"fmvz.x.ps x3, f5, 5\n"                               
			"fexp.ps f8, f1\n"                                    
			"fmin.pi f5, f1, f0\n"                                
			"fsq2 f5, -445(x11)\n"                                
			"fsgnj.ps f3, f7, f3\n"                               
			"mova.x.m x4\n"                                       
			"fnmsub.s f9, f6, f4, f4, rne\n"                      
			"fmvs.x.ps x4, f2, 6\n"                               
			"fcvt.ps.pwu f5, f0, rtz\n"                           
			"fcvt.pw.ps f6, f4, rmm\n"                            
			"fpackrepb.pi f6, f1\n"                               
			"fle.s x3, f5, f4\n"                                  
			"fsra.pi f9, f0, f1\n"                                
			"flt.ps f2, f7, f5\n"                                 
			"fcvt.ps.f16 f9, f5\n"                                
			"maskpopc x4, m2\n"                                   
			"fcvt.ps.pwu f6, f0, rne\n"                           
			"fle.ps f7, f4, f1\n"                                 
			"fandi.pi f2, f0, 40\n"                               
			"fmul.ps f7, f5, f2, rup\n"                           
			"fxor.pi f5, f4, f3\n"                                
			"fsub.pi f6, f6, f4\n"                                
			"fcvt.pw.ps f2, f1, rne\n"                            
			"fltm.pi m0, f7, f1\n"                                
			"fmsub.s f4, f4, f1, f1, rne\n"                       
			"fadd.pi f5, f5, f7\n"                                
			"fmin.ps f5, f7, f4\n"                                
			"fmin.s f3, f4, f3\n"                                 
			"fmul.pi f9, f6, f6\n"                                
			"fltm.pi m0, f4, f6\n"                                
			"flt.pi f7, f1, f2\n"                                 
			"flog.ps f3, f7\n"                                    
			"fmsub.ps f2, f2, f5, f4, rmm\n"                      
			"fnmadd.ps f6, f0, f4, f5, rne\n"                     
			"fmvz.x.ps x4, f3, 4\n"                               
			"mov.m.x m2, x11, 97\n"                               
			"feqm.ps m0, f1, f6\n"                                
			"fbc.ps f8, -403(x11)\n"                              
			"fadd.ps f7, f4, f3, rdn\n"                           
			"fmul.pi f5, f5, f1\n"                                

			"li x5, 0x39087282ca645391\n"
			"mova.m.x x5\n"                                       
			"feq.s x4, f2, f5\n"                                  
			"fmin.ps f9, f1, f7\n"                                
			"fnmadd.ps f7, f5, f2, f3, rne\n"                     
			"flog.ps f3, f3\n"                                    
			"fmadd.ps f4, f3, f2, f0, rtz\n"                      
			"fle.ps f9, f2, f7\n"                                 
			"fmin.pi f2, f4, f4\n"                                
			"flt.ps f8, f2, f3\n"                                 
			"fmul.s f5, f2, f4, rne\n"                            
			"frcp.ps f2, f6\n"                                    
			"fmvz.x.ps x3, f3, 7\n"                               
			"fround.ps f6, f1, rmm\n"                             
			"fsetm.pi m1, f0\n"                                   
			"fswizz.ps f8, f5, 8\n"                               
			"flq2 f5, 846(x11)\n"                                 
			"fandi.pi f2, f3, 454\n"                              
			//RTLMIN-5479"frsq.ps f5, f4\n"                                    
			"fcvt.s.wu f8, x11, rup\n"                            
			"fmulhu.pi f8, f1, f4\n"                              
			"flt.ps f2, f2, f5\n"                                 
			"fround.ps f5, f4, rdn\n"                             
			"feq.ps f7, f2, f2\n"                                 
			"fclass.s x4, f6\n"                                   
			"fmsub.ps f9, f0, f4, f2, rup\n"                      
			"fcvt.s.wu f3, x11, rtz\n"                            
			//RTLMIN-5479"fsin.ps f6, f7\n"                                    
			"flw f8, -298(x11)\n"                                 
			"maskor m0, m2, m2\n"                                 
			"fsrli.pi f4, f1, 0\n"                                
			"fnmsub.s f2, f4, f1, f6, rdn\n"                      
			"fcvt.w.s x4, f3, rmm\n"                              
			"fbcx.ps f3, x11\n"                                   
			"fcvt.s.w f5, x11, rdn\n"                             
			"fmax.pi f9, f3, f2\n"                                
			"fsgnj.s f9, f1, f1\n"                                
			"fadd.pi f7, f7, f1\n"                                
			"maskand m1, m1, m0\n"                                
			"mova.x.m x4\n"                                       
			"flog.ps f2, f1\n"                                    
			"fand.pi f6, f1, f3\n"                                
			"faddi.pi f2, f1, 37\n"                               
			"maskxor m2, m0, m2\n"                                
			"fltu.pi f9, f2, f1\n"                                
			"fadd.pi f5, f6, f2\n"                                
			"fnmsub.s f8, f3, f6, f1, dyn\n"                      
			"fcmov.ps f5, f2, f0, f1\n"                           
			"fmaxu.pi f9, f3, f2\n"                               
			"fbci.ps f5, 501746\n"                                
			"fnmsub.ps f4, f5, f1, f3, rne\n"                     
			"fltm.ps m1, f6, f3\n"                                
			"flog.ps f9, f2\n"                                    
			"fmul.pi f6, f4, f0\n"                                
			"fle.pi f9, f1, f0\n"                                 
			"mov.m.x m0, x11, 89\n"                               
			"fmin.ps f3, f6, f1\n"                                
			"flt.ps f3, f1, f6\n"                                 
			"flw.ps f4, 273(x11)\n"                               
			"fsgnjn.ps f9, f2, f1\n"                              
			"fmv.s.x f4, x11\n"                                   
			"feq.pi f7, f0, f2\n"                                 
			"fsrai.pi f6, f0, 11\n"                               
			"fmsub.s f5, f7, f5, f0, rne\n"                       
			"feq.pi f2, f0, f2\n"                                 
			"fnmadd.s f7, f1, f6, f3, rup\n"                      
			"fnmsub.s f5, f6, f4, f1, rmm\n"                      
			"fmaxu.pi f9, f4, f7\n"                               
			"fsub.ps f7, f0, f6, rmm\n"                           
			"fltm.ps m2, f0, f4\n"                                
			"feq.s x4, f5, f6\n"                                  
			"feq.s x4, f1, f7\n"                                  
			"fmulhu.pi f2, f0, f6\n"                              
			"maskor m2, m1, m1\n"                                 
			"fsrai.pi f5, f6, 13\n"                               
			"fpackrepb.pi f2, f4\n"                               
			"fmsub.ps f5, f1, f5, f5, rtz\n"                      
			"fle.s x3, f4, f1\n"                                  
			"fmin.pi f3, f0, f7\n"                                
			"fxor.pi f2, f1, f2\n"                                
			"fle.pi f2, f5, f6\n"                                 
			"fmadd.s f4, f4, f5, f3, rmm\n"                       
			"fsub.s f4, f6, f6, dyn\n"                            
			"fbc.ps f5, 348(x11)\n"                               
			"fcvt.f16.ps f2, f6\n"                                
			"fnmsub.ps f5, f7, f0, f6, rtz\n"                     
			"fslli.pi f2, f5, 1\n"                                
			"fsgnjn.ps f2, f6, f2\n"                              
			"fmulhu.pi f2, f6, f2\n"                              
			"fsub.ps f8, f3, f0, dyn\n"                           
			"fmin.pi f7, f7, f7\n"                                
			"fmin.ps f3, f5, f4\n"                                
			"for.pi f2, f1, f7\n"                                 
			"ffrc.ps f3, f7\n"                                    
			"fmaxu.pi f3, f3, f6\n"                               
			"fcvt.s.w f4, x11, rtz\n"                             
			"fmin.pi f8, f1, f7\n"                                
			"fmin.ps f7, f5, f0\n"                                
			"fnmsub.ps f2, f3, f3, f5, rdn\n"                     
			"fsgnjx.s f2, f1, f3\n"                               
			"fcvt.w.s x3, f6, rtz\n"                              
			"fmsub.s f7, f2, f3, f3, dyn\n"                       
			"fnmadd.ps f6, f4, f5, f2, rup\n"                     
			"fbcx.ps f8, x11\n"                                   
			"feq.s x4, f4, f1\n"                                  
			"feqm.ps m2, f6, f2\n"                                
			"fnmadd.s f5, f1, f6, f2, rtz\n"                      
			"fmin.pi f9, f2, f2\n"                                
			"fsrli.pi f5, f7, 6\n"                                
			"fsra.pi f3, f3, f5\n"                                
			"fadd.pi f2, f2, f3\n"                                
			"fsgnj.s f8, f5, f6\n"                                
			"fcvt.w.s x3, f6, rtz\n"                              
			"fsgnjn.s f9, f6, f2\n"                               
			"fbcx.ps f8, x11\n"                                   
			"fsq2 f3, 1186(x11)\n"                                
			"fmvs.x.ps x3, f6, 5\n"                               
			"fmin.s f4, f7, f7\n"                                 
			"fsrl.pi f5, f4, f6\n"                                
			"fsq2 f2, -462(x11)\n"                                
			"fcmovm.ps f5, f5, f4\n"                              
			"fcvt.ps.f16 f2, f2\n"                                
			"fcvt.s.w f6, x11, dyn\n"                             
			"fcvt.w.s x4, f4, rmm\n"                              
			"flt.ps f2, f6, f6\n"                                 
			"fsll.pi f9, f6, f2\n"                                
			"fmulhu.pi f3, f3, f3\n"                              
			"fsatu8.pi f6, f3\n"                                  
			"fminu.pi f3, f1, f0\n"                               
			"fadd.ps f2, f2, f1, rmm\n"                           
			"fsrai.pi f8, f3, 11\n"                               
			"fpackrepb.pi f7, f6\n"                               
			"fadd.pi f6, f1, f6\n"                                
			"fxor.pi f2, f6, f4\n"                                
			"fle.ps f9, f0, f5\n"                                 
			"fmul.ps f7, f6, f2, dyn\n"                           
			"fand.pi f9, f3, f0\n"                                
			"fcvt.wu.s x4, f6, rne\n"                             
			"fcvt.s.wu f2, x11, rtz\n"                            
			"fsrli.pi f4, f4, 13\n"                               
			"fmadd.s f4, f5, f3, f3, rup\n"                       
			"fsw.ps f1, -158(x11)\n"                              
			"fclass.ps f4, f5\n"                                  
			"fadd.s f4, f3, f1, rup\n"                            
			"fmin.pi f3, f1, f0\n"                                
			"fmul.ps f3, f7, f6, rdn\n"                           
			"fmin.s f5, f0, f0\n"                                 
			"fmadd.s f6, f6, f3, f4, rtz\n"                       
			"fle.ps f8, f1, f6\n"                                 
			"fmul.s f2, f0, f6, rup\n"                            
			"fmvs.x.ps x3, f5, 4\n"                               
			"maskand m0, m1, m0\n"                                
			"fbcx.ps f5, x11\n"                                   
			"fsgnjn.s f8, f2, f1\n"                               
			"fltm.ps m1, f7, f4\n"                                
			"flt.s x4, f2, f4\n"                                  
			"frcp.ps f2, f0\n"                                    
			"fsub.ps f7, f3, f7, rtz\n"                           
			"feq.pi f2, f7, f5\n"                                 
			"fandi.pi f4, f5, 394\n"                              
			"fsw.ps f1, -1534(x11)\n"                             
			"fmin.ps f5, f3, f0\n"                                
			"masknot m0, m0\n"                                    
			"fsll.pi f3, f3, f3\n"                                
			"flw.ps f4, 1281(x11)\n"                              
			"fsll.pi f4, f2, f0\n"                                
			"fnmsub.ps f7, f0, f1, f7, dyn\n"                     
			"fmax.pi f7, f2, f4\n"                                
			"fmax.pi f5, f4, f5\n"                                
			"fnot.pi f2, f0\n"                                    
			"fsw.ps f0, -952(x11)\n"                              
			"fcmov.ps f6, f3, f2, f3\n"                           
			"fsrl.pi f9, f2, f7\n"                                
			"flog.ps f6, f6\n"                                    
			"fcmovm.ps f2, f5, f6\n"                              
			"fminu.pi f8, f0, f2\n"                               
			"fslli.pi f2, f4, 6\n"                                
			"fmaxu.pi f3, f2, f4\n"                               
			"fmvz.x.ps x4, f3, 0\n"                               
			"fltm.ps m1, f1, f3\n"                                
			"fsgnj.ps f8, f3, f2\n"                               
			"fbc.ps f4, 2015(x11)\n"                              
			"fmin.s f2, f3, f3\n"                                 
			"fmadd.s f4, f6, f4, f1, rdn\n"                       
			"fadd.pi f5, f1, f3\n"                                
			"fsrli.pi f3, f3, 11\n"                               
			"fsgnjn.s f7, f2, f5\n"                               

			"li x5, 0xccd1be325771008e\n"
			"mova.m.x x5\n"                                       
			"masknot m2, m2\n"                                    
			"fcvt.wu.s x4, f7, rtz\n"                             
			"fadd.pi f3, f1, f4\n"                                
			"fmax.ps f9, f4, f0\n"                                
			"fsub.pi f2, f4, f3\n"                                
			"fminu.pi f9, f7, f7\n"                               
			"fsrai.pi f4, f0, 4\n"                                
			"fsw f4, -1949(x11)\n"                                
			"fmaxu.pi f5, f5, f5\n"                               
			"fsgnjx.ps f9, f0, f0\n"                              
			"flog.ps f6, f3\n"                                    
			"fmin.s f7, f2, f7\n"                                 
			"fbc.ps f7, -260(x11)\n"                              
			"fmax.pi f7, f4, f2\n"                                
			//RTLMIN-5479"fsin.ps f3, f2\n"                                    
			"fmvz.x.ps x3, f3, 5\n"                               
			"fmadd.s f9, f6, f3, f5, rup\n"                       
			"fmin.pi f4, f2, f0\n"                                
			"fand.pi f3, f3, f1\n"                                
			"fmvs.x.ps x4, f1, 5\n"                               
			"fmvs.x.ps x4, f0, 5\n"                               
			"fcvt.ps.f16 f9, f1\n"                                
			"fsrl.pi f2, f4, f1\n"                                
			"fltm.pi m1, f4, f0\n"                                
			"fltm.pi m1, f4, f2\n"                                
			"fmax.pi f7, f7, f4\n"                                
			"fltm.pi m2, f4, f1\n"                                
			"fmadd.s f9, f2, f7, f0, rtz\n"                       
			"fsgnj.s f3, f2, f0\n"                                
			"fsub.s f4, f7, f6, rup\n"                            
			"fnmsub.ps f2, f3, f4, f1, rdn\n"                     
			"for.pi f2, f7, f7\n"                                 
			"fcvt.ps.f16 f4, f6\n"                                
			"fltm.pi m1, f6, f3\n"                                
			"feqm.ps m0, f0, f6\n"                                
			"frcp.ps f4, f4\n"                                    
			"fmax.pi f9, f5, f0\n"                                
			"fclass.s x4, f4\n"                                   
			"fltu.pi f8, f5, f0\n"                                
			"fcvt.wu.s x3, f7, rdn\n"                             
			"fmin.pi f6, f4, f7\n"                                
			"fmv.x.s x4, f0\n"                                    
			"fclass.ps f9, f4\n"                                  
			"fcmovm.ps f3, f2, f3\n"                              
			"fle.s x4, f2, f6\n"                                  
			"fle.s x3, f3, f6\n"                                  
			"frcp.ps f9, f1\n"                                    
			"fmsub.ps f4, f1, f6, f7, rmm\n"                      
			"fmvz.x.ps x4, f3, 6\n"                               
			"fmadd.ps f7, f5, f3, f6, rne\n"                      
			"fsrl.pi f5, f4, f3\n"                                
			"flw f7, 1678(x11)\n"                                 
			"fadd.ps f5, f0, f0, dyn\n"                           
			//RTLMIN-5479"frsq.ps f9, f0\n"                                    
			"fcvt.ps.pwu f4, f3, rup\n"                           
			"fcvt.f16.ps f3, f0\n"                                
			"fnmadd.ps f8, f1, f1, f4, rup\n"                     
			"for.pi f6, f2, f0\n"                                 
			"fsgnj.ps f8, f3, f1\n"                               
			"feqm.ps m1, f5, f0\n"                                
			"fcvt.s.wu f8, x11, rdn\n"                            
			"fcvt.ps.pwu f5, f0, rne\n"                           
			"fmul.s f8, f6, f4, rdn\n"                            
			"fsgnj.ps f6, f7, f7\n"                               
			"fbc.ps f8, 1454(x11)\n"                              

			"li x5, 0xaf0181c5baf71c3b\n"
			"mova.m.x x5\n"                                       
			"fnmadd.s f6, f5, f2, f6, rne\n"                      
			"fsgnj.ps f2, f6, f0\n"                               
			"feqm.ps m2, f0, f0\n"                                
			"fadd.pi f3, f4, f7\n"                                
			"fsw f0, -767(x11)\n"                                 
			"fcvt.s.w f4, x11, rtz\n"                             
			"fnmadd.s f5, f0, f5, f4, rne\n"                      
			"fslli.pi f8, f0, 3\n"                                
			"fand.pi f3, f5, f0\n"                                
			"fsq2 f0, 913(x11)\n"                                 
			"maskor m0, m0, m1\n"                                 
			"fsgnjx.ps f8, f2, f2\n"                              
			"fsetm.pi m1, f2\n"                                   
			"fbci.pi f9, 95238\n"                                 
			"fexp.ps f6, f0\n"                                    
			"mova.x.m x3\n"                                       
			"flw f4, 1920(x11)\n"                                 
			"flt.ps f5, f7, f3\n"                                 
			"fxor.pi f7, f2, f4\n"                                
			"fmadd.ps f3, f1, f7, f0, rne\n"                      
			"fsw.ps f7, 523(x11)\n"                               
			"feqm.ps m0, f1, f1\n"                                
			"fmsub.ps f3, f4, f0, f3, rne\n"                      
			"fsgnj.s f7, f4, f1\n"                                
			"fmadd.ps f5, f3, f7, f5, rdn\n"                      
			"mov.m.x m2, x11, 77\n"                               
			"fbci.ps f9, 75766\n"                                 
			"fnmadd.ps f6, f7, f2, f4, rmm\n"                     
			"fmadd.s f3, f1, f5, f5, rmm\n"                       
			"fnot.pi f5, f6\n"                                    
			"fbci.ps f8, 483879\n"                                
			"fsgnj.s f6, f1, f3\n"                                
			"maskand m2, m2, m1\n"                                
			"fround.ps f2, f1, rup\n"                             
			"fcvt.s.w f8, x11, rup\n"                             
			"fcvt.ps.pwu f4, f7, rdn\n"                           
			"fsgnjx.s f4, f2, f0\n"                               
			"fcvt.w.s x3, f5, rne\n"                              
			"flt.pi f2, f7, f5\n"                                 
			//RTLMIN-5479"frsq.ps f3, f6\n"                                    
			"flw.ps f2, -865(x11)\n"                              
			"flq2 f9, -357(x11)\n"                                
			"flt.ps f9, f7, f0\n"                                 
			"maskor m1, m2, m1\n"                                 
			"feq.ps f9, f3, f7\n"                                 
			"faddi.pi f4, f6, 456\n"                              
			"fmin.s f8, f5, f2\n"                                 
			"fcvt.f16.ps f7, f7\n"                                
			"fsgnjx.ps f3, f1, f6\n"                              
			"flem.ps m0, f5, f3\n"                                
			"masknot m1, m1\n"                                    
			"fsw.ps f5, -337(x11)\n"                              
			//RTLMIN-5479"frsq.ps f7, f7\n"                                    
			"fsw.ps f5, -1578(x11)\n"                             
			"fsub.pi f3, f5, f2\n"                                
			"fmul.pi f8, f0, f5\n"                                
			"fadd.pi f2, f7, f1\n"                                
			"fmax.s f9, f0, f0\n"                                 
			"fsw.ps f3, 155(x11)\n"                               
			"flt.ps f4, f6, f6\n"                                 
			"fslli.pi f9, f3, 14\n"                               
			"maskpopc x4, m1\n"                                   
			"fle.pi f6, f4, f6\n"                                 
			"fandi.pi f9, f0, 152\n"                              
			"flog.ps f9, f2\n"                                    
			"fslli.pi f2, f3, 6\n"                                
			"fnmadd.s f5, f6, f5, f0, rtz\n"                      
			"flog.ps f7, f2\n"                                    
			"fsra.pi f5, f2, f2\n"                                
			"fsub.s f3, f2, f4, rtz\n"                            
			"fswizz.ps f7, f4, 86\n"                              
			"fsgnjx.ps f7, f1, f4\n"                              
			"fbcx.ps f9, x11\n"                                   
			"fxor.pi f8, f0, f5\n"                                
			"feq.s x3, f4, f3\n"                                  
			"fsq2 f2, -1540(x11)\n"                               
			"maskxor m1, m1, m2\n"                                
			"fsub.pi f8, f3, f2\n"                                
			"fsrli.pi f6, f7, 15\n"                               
			"flt.ps f9, f0, f7\n"                                 
			//RTLMIN-5479"fsin.ps f8, f5\n"                                    
			"fcvt.ps.pwu f2, f7, rmm\n"                           
			"fmv.x.s x3, f1\n"                                    
			"fsw f1, -278(x11)\n"                                 
			"fsq2 f7, -1444(x11)\n"                               
			"fmin.s f6, f7, f1\n"                                 
			"fmax.ps f5, f1, f6\n"                                
			"fpackreph.pi f8, f5\n"                               
			"fsw.ps f4, -1109(x11)\n"                             
			"fcvt.pw.ps f9, f0, rmm\n"                            
			"fmaxu.pi f5, f0, f4\n"                               
			"fcvt.ps.pw f4, f3, rdn\n"                            
			"fmin.pi f2, f2, f2\n"                                
			"fcvt.f16.ps f5, f5\n"                                
			"fslli.pi f3, f2, 9\n"                                
			"fbc.ps f8, 119(x11)\n"                               
			"fltm.pi m2, f3, f5\n"                                
			"fcvt.s.w f7, x11, rup\n"                             
			"fmaxu.pi f9, f1, f5\n"                               
			"feq.ps f4, f6, f3\n"                                 
			"fsgnj.s f4, f7, f4\n"                                
			"fnmadd.s f5, f3, f6, f7, rtz\n"                      
			"fle.pi f8, f0, f5\n"                                 
			"flog.ps f2, f2\n"                                    
			"ffrc.ps f2, f0\n"                                    
			"flt.pi f7, f6, f1\n"                                 
			"fmax.s f5, f0, f7\n"                                 
			"fnmsub.s f9, f3, f6, f3, rne\n"                      
			"fsra.pi f7, f1, f1\n"                                
			"fsw.ps f5, 1199(x11)\n"                              
			"for.pi f8, f7, f7\n"                                 
			"fpackrepb.pi f6, f5\n"                               
			"fnmadd.ps f9, f6, f3, f0, rup\n"                     
			"fsgnjx.ps f4, f0, f7\n"                              
			"feq.ps f6, f6, f2\n"                                 
			"flog.ps f7, f0\n"                                    
			"mova.x.m x3\n"                                       
			"fsgnjx.ps f4, f7, f6\n"                              
			"fsub.ps f3, f2, f4, rne\n"                           
			"feq.ps f3, f6, f7\n"                                 
			"fle.s x3, f3, f4\n"                                  
			"fminu.pi f3, f3, f5\n"                               
			"fsub.s f7, f3, f5, rmm\n"                            
			"for.pi f6, f4, f5\n"                                 
			"frcp.ps f9, f5\n"                                    
			"fmulhu.pi f9, f3, f4\n"                              
			"fpackrepb.pi f4, f2\n"                               
			"fslli.pi f8, f2, 12\n"                               
			"fltm.ps m1, f1, f7\n"                                
			"fcvt.ps.f16 f8, f6\n"                                
			"fminu.pi f3, f1, f0\n"                               
			"maskxor m1, m1, m0\n"                                
			"fmv.x.s x4, f7\n"                                    
			"mova.x.m x4\n"                                       
			"fslli.pi f2, f5, 4\n"                                
			"mova.x.m x3\n"                                       
			"fsgnjx.ps f6, f2, f3\n"                              
			"fsgnjn.ps f6, f7, f6\n"                              
			"fsat8.pi f2, f0\n"                                   
			"fsrl.pi f9, f5, f2\n"                                
			"fsll.pi f9, f0, f5\n"                                
			"fmv.s.x f9, x11\n"                                   
			"fsll.pi f9, f5, f7\n"                                
			//RTLMIN-5479"frsq.ps f6, f3\n"                                    
			"fmsub.ps f9, f7, f4, f2, rup\n"                      
			"fle.pi f5, f3, f6\n"                                 
			"maskand m0, m2, m2\n"                                
			"fmul.s f5, f5, f1, rmm\n"                            
			"maskor m1, m2, m2\n"                                 
			"fslli.pi f8, f4, 15\n"                               
			"fsgnjx.ps f5, f2, f0\n"                              
			"ffrc.ps f6, f3\n"                                    
			"ffrc.ps f5, f0\n"                                    
			"maskxor m0, m1, m1\n"                                
			"fmul.ps f4, f4, f0, rne\n"                           
			"fnmadd.s f6, f2, f2, f3, rmm\n"                      
			"fclass.s x3, f7\n"                                   
			"fnmsub.ps f9, f7, f6, f7, rtz\n"                     
			"fltu.pi f5, f5, f0\n"                                
			"fle.s x4, f0, f4\n"                                  
			"frcp.ps f3, f1\n"                                    
			"fsra.pi f3, f3, f0\n"                                
			//RTLMIN-5479"fsin.ps f8, f7\n"                                    
			"fmaxu.pi f2, f0, f6\n"                               
			"fsrai.pi f3, f3, 0\n"                                
			"mova.x.m x4\n"                                       
			"fsub.pi f2, f1, f5\n"                                
			"flw f4, 813(x11)\n"                                  
			"fclass.s x3, f2\n"                                   
			"fcvt.s.w f6, x11, dyn\n"                             
			"fsra.pi f3, f7, f4\n"                                
			"fadd.ps f6, f6, f7, rup\n"                           
			"feqm.ps m1, f7, f5\n"                                
			"fsgnj.s f9, f2, f3\n"                                
			"fmsub.s f9, f1, f7, f0, rdn\n"                       
			"fround.ps f6, f6, dyn\n"                             
			"masknot m0, m0\n"                                    
			"fsat8.pi f3, f5\n"                                   
			"fsub.s f3, f7, f1, rtz\n"                            
			"fsrli.pi f4, f3, 12\n"                               
			"fmadd.s f8, f2, f3, f2, dyn\n"                       
			"fsub.s f6, f0, f4, rmm\n"                            
			"flq2 f3, 439(x11)\n"                                 
			"fsatu8.pi f5, f4\n"                                  
			"fle.s x3, f1, f2\n"                                  
			"fltm.ps m2, f5, f0\n"                                
			"maskor m2, m1, m0\n"                                 
			"fbci.ps f5, 38822\n"                                 
			"fsgnjx.s f6, f2, f5\n"                               
			"fclass.s x3, f0\n"                                   
			"fsll.pi f5, f4, f5\n"                                
			"fxor.pi f7, f2, f3\n"                                
			"feq.pi f3, f5, f6\n"                                 
			"fsgnj.ps f4, f6, f6\n"                               
			"fmul.ps f7, f2, f1, rdn\n"                           
			"fcvt.f16.ps f6, f3\n"                                
			"fmax.pi f6, f7, f5\n"                                
			"fltm.ps m1, f6, f5\n"                                
			"fmulhu.pi f2, f7, f6\n"                              
			"frcp.ps f3, f5\n"                                    
			"fsetm.pi m0, f3\n"                                   
			"fsgnjn.ps f5, f0, f3\n"                              
			"fmvz.x.ps x3, f0, 3\n"                               
			"flt.s x3, f5, f3\n"                                  
			"fsub.ps f3, f5, f7, rdn\n"                           
			"fmin.ps f6, f4, f3\n"                                
			"fbc.ps f7, 1902(x11)\n"                              
			"fclass.ps f9, f2\n"                                  
			"flt.pi f2, f3, f7\n"                                 
			"fmul.s f6, f0, f1, rtz\n"                            
			"fmv.s.x f9, x11\n"                                   

			"li x5, 0x8a563c1a4ad0a5f7\n"
			"mova.m.x x5\n"                                       
			"fminu.pi f2, f0, f6\n"                               
			"fsgnjx.s f9, f7, f3\n"                               
			"fmul.pi f2, f2, f4\n"                                
			"fsatu8.pi f4, f0\n"                                  
			"fadd.s f7, f5, f2, rup\n"                            
			"mova.x.m x4\n"                                       
			"fmaxu.pi f9, f4, f2\n"                               
			"fsgnj.ps f7, f5, f6\n"                               
			"fmin.ps f3, f1, f6\n"                                
			"fmvz.x.ps x4, f1, 6\n"                               
			"fsra.pi f5, f0, f1\n"                                
			"fbci.pi f4, 123347\n"                                
			"fmulhu.pi f8, f7, f0\n"                              
			"flog.ps f2, f5\n"                                    
			"fsetm.pi m2, f3\n"                                   
			"fmvs.x.ps x4, f3, 4\n"                               
			"fsll.pi f4, f1, f3\n"                                
			"fswizz.ps f5, f6, 70\n"                              
			//RTLMIN-5479"fsin.ps f8, f3\n"                                    
			"fexp.ps f9, f5\n"                                    
			"fmaxu.pi f7, f6, f0\n"                               
			"fsub.s f9, f2, f4, rmm\n"                            
			"fsatu8.pi f8, f6\n"                                  
			"fmadd.ps f5, f3, f3, f5, dyn\n"                      
			"feq.pi f6, f5, f2\n"                                 
			"fsw.ps f5, -673(x11)\n"                              
			"fmul.pi f5, f0, f7\n"                                
			"fsat8.pi f7, f3\n"                                   
			"fcvt.ps.f16 f6, f5\n"                                
			"feqm.ps m1, f1, f6\n"                                
			"fswizz.ps f2, f6, 81\n"                              
			"fsat8.pi f8, f2\n"                                   
			"flog.ps f4, f5\n"                                    
			"faddi.pi f6, f5, 305\n"                              
			"fand.pi f6, f2, f4\n"                                
			"fcmov.ps f9, f2, f3, f0\n"                           
			"maskand m2, m2, m2\n"                                
			"fnmadd.s f6, f7, f5, f4, rne\n"                      
			"fcvt.ps.f16 f7, f2\n"                                
			"fmax.s f2, f7, f3\n"                                 
			"fclass.ps f6, f1\n"                                  
			"mova.x.m x3\n"                                       
			"fnmadd.ps f2, f7, f5, f1, rtz\n"                     
			"fclass.ps f5, f1\n"                                  
			"fclass.s x3, f1\n"                                   
			"fmin.s f7, f6, f3\n"                                 
			"fadd.pi f6, f4, f5\n"                                
			"fpackreph.pi f9, f3\n"                               
			"fmvz.x.ps x3, f1, 6\n"                               
			"fmin.ps f6, f4, f4\n"                                
			"fnot.pi f5, f4\n"                                    
			"fsgnjn.s f8, f3, f1\n"                               
			"fsat8.pi f6, f3\n"                                   
			"fmul.ps f9, f4, f1, rdn\n"                           
			"fcvt.ps.pw f5, f7, rne\n"                            
			"flt.pi f2, f1, f1\n"                                 
			"fcvt.pw.ps f2, f4, dyn\n"                            
			"fmul.s f7, f3, f4, rup\n"                            
			"fsw.ps f2, -780(x11)\n"                              
			"fcvt.s.wu f8, x11, dyn\n"                            
			"fnmadd.s f6, f1, f4, f3, rmm\n"                      
			"fexp.ps f8, f6\n"                                    
			//RTLMIN-5479"fsin.ps f3, f0\n"                                    
			"fmulh.pi f6, f4, f7\n"                               
			"fslli.pi f4, f0, 14\n"                               
			"fbci.pi f6, 364202\n"                                
			"maskpopc x4, m2\n"                                   
			"flw.ps f7, -643(x11)\n"                              
			"flw.ps f9, 299(x11)\n"                               
			"fround.ps f2, f4, rup\n"                             
			"for.pi f3, f1, f4\n"                                 
			"fcvt.s.wu f7, x11, dyn\n"                            
			"fsra.pi f6, f1, f0\n"                                
			"fltu.pi f3, f2, f0\n"                                
			"fcvt.s.wu f3, x11, dyn\n"                            
			"fnmsub.s f7, f2, f5, f1, rne\n"                      
			"fand.pi f5, f4, f6\n"                                
			"fcvt.ps.pw f7, f4, rne\n"                            
			"mov.m.x m1, x11, 24\n"                               
			"fcmov.ps f8, f1, f4, f2\n"                           
			"frcp.ps f4, f3\n"                                    
			"fbc.ps f3, -1357(x11)\n"                             
			"fsgnj.s f7, f0, f2\n"                                
			"fsrl.pi f6, f1, f4\n"                                
			"fnmsub.s f5, f7, f5, f0, dyn\n"                      
			"fcvt.s.wu f7, x11, rdn\n"                            
			"fswizz.ps f8, f0, 29\n"                              
			"fnot.pi f5, f2\n"                                    
			"fcvt.pw.ps f7, f6, rne\n"                            
			"fsrai.pi f5, f3, 3\n"                                
			"fsetm.pi m2, f6\n"                                   
			"flt.s x3, f3, f1\n"                                  
			"fle.ps f5, f0, f4\n"                                 
			"fsra.pi f6, f6, f6\n"                                
			"fmax.ps f8, f7, f1\n"                                
			"fcmovm.ps f8, f0, f3\n"                              
			"maskand m0, m2, m1\n"                                
			"flw.ps f7, 638(x11)\n"                               
			"fcmovm.ps f9, f3, f1\n"                              
			"fsgnj.s f3, f6, f3\n"                                
			"fcvt.ps.pw f4, f0, rtz\n"                            
			"fsgnj.s f4, f7, f3\n"                                
			"fcvt.s.wu f5, x11, rup\n"                            
			"flt.pi f2, f6, f0\n"                                 
			"flq2 f6, 968(x11)\n"                                 
			"flw.ps f9, -1390(x11)\n"                             
			"fmul.s f3, f4, f0, rmm\n"                            
			"fsgnj.s f2, f3, f7\n"                                
			"fsrli.pi f7, f4, 7\n"                                
			"fsgnj.ps f4, f4, f7\n"                               
			"flq2 f3, -1505(x11)\n"                               
			"fmax.s f7, f6, f2\n"                                 
			"fltu.pi f5, f0, f6\n"                                
			"feq.pi f6, f3, f3\n"                                 
			"fsgnj.s f8, f7, f5\n"                                
			"fexp.ps f2, f1\n"                                    
			"fmul.ps f6, f5, f5, rup\n"                           
			"flem.ps m2, f6, f6\n"                                
			"feq.s x4, f6, f7\n"                                  
			"maskxor m2, m0, m1\n"                                
			"fbci.ps f6, 34835\n"                                 
			"fmsub.ps f6, f3, f3, f5, rmm\n"                      
			"fmvs.x.ps x3, f7, 0\n"                               
			"fsrai.pi f2, f5, 5\n"                                

			"li x5, 0x30d70848d76fcea4\n"
			"mova.m.x x5\n"                                       
			"fadd.s f6, f3, f1, rne\n"                            
			"fsra.pi f7, f3, f6\n"                                
			"fcvt.ps.f16 f3, f4\n"                                
			"fand.pi f4, f4, f7\n"                                
			"fmv.s.x f7, x11\n"                                   
			"fmax.pi f8, f5, f6\n"                                
			"flt.ps f4, f2, f7\n"                                 
			"fsetm.pi m0, f4\n"                                   
			"fcmovm.ps f3, f5, f6\n"                              
			"fmin.pi f4, f3, f1\n"                                
			"fadd.ps f5, f1, f0, rmm\n"                           
			"fsgnj.ps f7, f7, f1\n"                               
			"fcvt.f16.ps f3, f3\n"                                
			"fsq2 f2, 1954(x11)\n"                                
			"fsrli.pi f8, f0, 7\n"                                
			"fmsub.ps f9, f4, f4, f2, dyn\n"                      
			"fmaxu.pi f5, f5, f4\n"                               
			"flt.pi f5, f6, f6\n"                                 
			"fsrl.pi f4, f3, f7\n"                                
			"fsatu8.pi f8, f4\n"                                  
			"flem.ps m2, f4, f0\n"                                
			"fsub.s f4, f6, f5, dyn\n"                            
			"fsgnj.ps f9, f3, f3\n"                               
			"fsrai.pi f4, f0, 7\n"                                
			"fmsub.ps f7, f4, f5, f3, rdn\n"                      
			"fmaxu.pi f2, f4, f7\n"                               
			"fcvt.wu.s x3, f1, rne\n"                             
			"feqm.ps m2, f4, f4\n"                                
			"fmv.s.x f2, x11\n"                                   
			"fmax.ps f3, f3, f0\n"                                
			"fbci.pi f4, 171376\n"                                
			"fminu.pi f2, f4, f0\n"                               
			"fsrl.pi f6, f6, f0\n"                                
			"flq2 f8, 1106(x11)\n"                                
			"fsub.pi f7, f1, f1\n"                                
			"fswizz.ps f7, f5, 59\n"                              
			"maskpopc x4, m0\n"                                   
			"fclass.s x3, f2\n"                                   
			"fcvt.ps.f16 f5, f5\n"                                
			"fsrl.pi f3, f5, f7\n"                                
			"maskor m2, m2, m0\n"                                 
			"fmsub.ps f5, f6, f0, f7, dyn\n"                      
			//RTLMIN-5479"frsq.ps f9, f5\n"                                    

			"li x5, 0xfa898efcb6307917\n"
			"mova.m.x x5\n"                                       
			"fbci.ps f6, 8549\n"                                  
			"fsgnjx.ps f6, f2, f0\n"                              
			"fclass.s x3, f2\n"                                   
			"maskpopcz x3, m1\n"                                  
			"fmax.pi f6, f1, f7\n"                                
			"flw f5, -2038(x11)\n"                                
			"fsgnj.ps f6, f7, f0\n"                               
			"fclass.s x4, f3\n"                                   
			"feqm.ps m1, f5, f4\n"                                
			"flt.ps f3, f2, f3\n"                                 
			"flw.ps f2, -1258(x11)\n"                             
			"fcmov.ps f6, f6, f1, f6\n"                           
			"fmaxu.pi f3, f4, f0\n"                               
			"fmvz.x.ps x3, f6, 7\n"                               
			"fle.s x3, f2, f0\n"                                  
			"fsgnj.s f2, f4, f6\n"                                
			"fmvs.x.ps x4, f2, 2\n"                               
			"maskpopc x4, m1\n"                                   
			"fbcx.ps f4, x11\n"                                   
			"fsetm.pi m2, f2\n"                                   
			"feq.ps f2, f1, f7\n"                                 
			"fsetm.pi m2, f2\n"                                   
			"flt.pi f7, f3, f5\n"                                 
			"fsrai.pi f5, f6, 13\n"                               
			"fbcx.ps f2, x11\n"                                   
			"maskor m1, m0, m0\n"                                 
			"fmadd.s f8, f0, f4, f4, rtz\n"                       
			"feqm.ps m2, f4, f2\n"                                
			"fcmov.ps f9, f7, f6, f6\n"                           
			"fsub.ps f7, f3, f4, rne\n"                           
			"fmax.pi f6, f0, f2\n"                                
			"fmv.s.x f5, x11\n"                                   
			"fmul.pi f3, f5, f6\n"                                
			"fmulh.pi f7, f0, f7\n"                               
			"fnmsub.s f8, f1, f2, f5, rup\n"                      
			"fnot.pi f3, f5\n"                                    
			"fmul.pi f8, f7, f6\n"                                
			"fsgnjx.s f5, f4, f3\n"                               
			"fbci.ps f2, 442363\n"                                
			"mov.m.x m1, x11, 35\n"                               
			"fcvt.pw.ps f7, f2, rtz\n"                            
			"fcvt.f16.ps f6, f0\n"                                
			"maskand m2, m1, m2\n"                                
			"masknot m2, m1\n"                                    
			"fmvs.x.ps x4, f7, 5\n"                               
			"fmvs.x.ps x4, f5, 4\n"                               
			"for.pi f9, f6, f3\n"                                 
			"fmin.pi f4, f5, f3\n"                                
			"fclass.s x3, f4\n"                                   
			"masknot m1, m1\n"                                    
			"fcvt.ps.pwu f2, f5, dyn\n"                           
			"faddi.pi f3, f4, 148\n"                              
			"flw.ps f7, -550(x11)\n"                              
			"fmul.ps f3, f0, f3, rdn\n"                           
			"mov.m.x m2, x11, 123\n"                              
			"fmv.x.s x3, f5\n"                                    
			"fcvt.pw.ps f8, f2, rne\n"                            
			"fmin.pi f6, f2, f3\n"                                
			"fmin.ps f3, f4, f0\n"                                
			"fmax.ps f2, f6, f0\n"                                
			"fsrai.pi f9, f5, 13\n"                               
			"fxor.pi f8, f6, f3\n"                                
			"fltm.ps m0, f7, f0\n"                                
			"faddi.pi f7, f6, 154\n"                              
			"mov.m.x m2, x11, 89\n"                               
			"fltu.pi f9, f2, f7\n"                                
			"fcvt.w.s x4, f6, rdn\n"                              
			"mov.m.x m2, x11, 61\n"                               
			"fcvt.s.wu f6, x11, rup\n"                            
			"fmsub.s f8, f3, f3, f6, rne\n"                       
			"fsgnjn.ps f8, f7, f5\n"                              
			"fmsub.s f7, f2, f3, f0, rdn\n"                       
			"fcvt.ps.pw f8, f1, rup\n"                            
			"fsrl.pi f9, f5, f5\n"                                
			"fmadd.s f7, f3, f0, f6, rtz\n"                       
			"for.pi f7, f6, f5\n"                                 
			"fmax.ps f9, f1, f5\n"                                
			"fcvt.ps.pwu f6, f0, rmm\n"                           
			"fsub.ps f8, f4, f4, rtz\n"                           
			"fsrai.pi f2, f2, 12\n"                               
			"flw f2, 146(x11)\n"                                  
			"fsw f1, -1482(x11)\n"                                
			"fnmsub.s f2, f3, f1, f7, rne\n"                      
			"fnot.pi f3, f3\n"                                    
			"flt.pi f2, f3, f4\n"                                 
			"fsatu8.pi f7, f7\n"                                  
			"fsll.pi f6, f1, f3\n"                                
			"fmin.s f7, f1, f2\n"                                 
			"fmax.ps f9, f6, f0\n"                                
			"fmax.ps f6, f7, f5\n"                                
			"maskand m2, m2, m1\n"                                
			"maskand m0, m0, m2\n"                                
			"fmul.pi f9, f3, f0\n"                                
			"fcvt.ps.pw f5, f0, rup\n"                            
			"maskpopc x4, m1\n"                                   
			"fmvz.x.ps x3, f0, 4\n"                               
			"fadd.s f2, f4, f6, rup\n"                            
			"fmul.pi f7, f3, f1\n"                                
			"fand.pi f4, f5, f3\n"                                
			"fcmov.ps f4, f7, f0, f6\n"                           
			"fmax.ps f2, f0, f3\n"                                
			"fsrai.pi f7, f0, 14\n"                               
			"fnot.pi f5, f5\n"                                    
			"masknot m1, m1\n"                                    
			"fsat8.pi f4, f5\n"                                   
			"fcvt.s.wu f8, x11, rtz\n"                            
			"fmax.ps f4, f4, f3\n"                                
			"flt.ps f3, f2, f0\n"                                 
			"fmin.ps f7, f5, f1\n"                                
			"fmv.x.s x4, f5\n"                                    
			"fmadd.s f7, f6, f5, f6, rup\n"                       
			"fmul.s f6, f7, f0, rup\n"                            
			"fsw.ps f0, 394(x11)\n"                               
			"feq.s x4, f3, f6\n"                                  
			"fcvt.wu.s x4, f3, rdn\n"                             
			"fnmsub.s f7, f2, f6, f7, dyn\n"                      
			"fmax.ps f3, f0, f7\n"                                
			"flt.ps f8, f7, f7\n"                                 
			"fmvs.x.ps x3, f0, 3\n"                               
			"fsgnj.ps f6, f4, f3\n"                               
			"fsub.s f5, f0, f1, rtz\n"                            
			"fmin.s f7, f3, f1\n"                                 
			"fnmadd.ps f4, f2, f4, f2, rup\n"                     
			"fmvz.x.ps x4, f0, 2\n"                               
			"fsetm.pi m1, f2\n"                                   
			"fltm.pi m2, f6, f1\n"                                
			"fsub.ps f2, f6, f1, rup\n"                           
			"fmulh.pi f8, f4, f4\n"                               
			"fbc.ps f9, -1804(x11)\n"                             
			"fsq2 f4, 973(x11)\n"                                 
			"fsetm.pi m0, f5\n"                                   
			"flem.ps m2, f4, f0\n"                                
			"fnmsub.s f5, f1, f5, f1, dyn\n"                      
			"fbci.ps f3, 446792\n"                                
			"fsrli.pi f9, f2, 1\n"                                
			"fclass.ps f9, f0\n"                                  
			"fsra.pi f9, f2, f7\n"                                
			"fcvt.pwu.ps f7, f7, rmm\n"                           
			"fsetm.pi m2, f0\n"                                   
			"fsgnjn.ps f5, f0, f2\n"                              
			"fsrai.pi f3, f6, 1\n"                                
			"fsgnj.s f2, f6, f6\n"                                
			"flt.s x3, f2, f2\n"                                  
			"fsw.ps f4, -1676(x11)\n"                             
			"flw f4, 1025(x11)\n"                                 
			"feq.ps f2, f2, f7\n"                                 
			"maskxor m2, m1, m2\n"                                
			"fminu.pi f3, f3, f5\n"                               
			"fmul.pi f7, f3, f5\n"                                
			"fsq2 f7, -779(x11)\n"                                
			"fexp.ps f9, f0\n"                                    
			"feqm.ps m2, f5, f2\n"                                
			"flt.ps f2, f6, f2\n"                                 
			"fmax.ps f5, f0, f0\n"                                
			"masknot m0, m0\n"                                    
			"fmul.s f7, f6, f7, rmm\n"                            
			"fadd.ps f7, f5, f0, rup\n"                           
			"fslli.pi f6, f5, 10\n"                               
			"fcvt.ps.pw f4, f5, rup\n"                            
			"fmul.ps f3, f0, f5, rup\n"                           
			"fsrai.pi f5, f3, 3\n"                                
			"fnmsub.ps f9, f5, f6, f2, rdn\n"                     
			"fmin.ps f5, f0, f3\n"                                
			"fmsub.ps f5, f4, f4, f5, rmm\n"                      
			"flt.s x3, f0, f4\n"                                  
			"fcmov.ps f2, f2, f1, f7\n"                           
			"fmax.s f3, f0, f0\n"                                 
			"fcvt.w.s x3, f5, rne\n"                              
			"fle.ps f6, f5, f0\n"                                 
			"fadd.pi f2, f4, f1\n"                                
			"fsgnjx.s f6, f2, f5\n"                               
			"fxor.pi f5, f5, f1\n"                                
			"fcvt.pwu.ps f3, f7, dyn\n"                           
			"fsrai.pi f5, f5, 6\n"                                
			"fmv.x.s x4, f4\n"                                    
			"fsub.s f4, f5, f2, rmm\n"                            
			"fsgnj.s f6, f5, f1\n"                                
			"frcp.ps f9, f2\n"                                    
			"fmul.ps f3, f2, f6, rtz\n"                           
			"fsll.pi f8, f5, f6\n"                                
			"maskpopcz x3, m1\n"                                  
			"fmin.ps f4, f1, f0\n"                                
			"fcvt.s.wu f4, x11, rne\n"                            
			"flq2 f8, 654(x11)\n"                                 
			"fbci.ps f7, 261260\n"                                
			"fnmadd.s f3, f1, f0, f7, rne\n"                      
			"fxor.pi f6, f3, f0\n"                                
			"fandi.pi f8, f4, 74\n"                               
			"maskor m2, m0, m0\n"                                 
			"fsrl.pi f5, f6, f7\n"                                
			"fmax.s f5, f6, f0\n"                                 
			"faddi.pi f4, f5, 407\n"                              
			"fbci.pi f8, 333515\n"                                
			"fmv.s.x f5, x11\n"                                   
			"fmadd.ps f6, f7, f2, f0, dyn\n"                      
			"fmadd.ps f5, f2, f7, f0, rtz\n"                      
			"fsat8.pi f5, f1\n"                                   
			"fandi.pi f4, f1, 423\n"                              
			"fsub.ps f4, f1, f3, rtz\n"                           
			//RTLMIN-5479"frsq.ps f2, f4\n"                                    
			"fsrli.pi f6, f0, 9\n"                                
			"flw.ps f3, -1233(x11)\n"                             
			"masknot m2, m0\n"                                    
			"fsatu8.pi f5, f1\n"                                  
			"fslli.pi f5, f7, 14\n"                               
			"fminu.pi f8, f2, f4\n"                               
			"fsrai.pi f9, f1, 4\n"                                
			"frcp.ps f2, f7\n"                                    
			"fle.s x3, f7, f7\n"                                  
			"fsub.ps f9, f4, f1, rdn\n"                           
			"fcvt.ps.f16 f5, f5\n"                                
			"fbc.ps f8, -1013(x11)\n"                             
			"fmadd.ps f8, f5, f5, f4, dyn\n"                      
			"flt.ps f3, f7, f6\n"                                 
			"fsgnjn.ps f7, f5, f3\n"                              
			"fpackrepb.pi f9, f1\n"                               
			"fadd.pi f7, f0, f5\n"                                
			"fsrl.pi f8, f0, f5\n"                                
			"fadd.ps f7, f3, f3, rdn\n"                           
			"fmsub.ps f5, f3, f4, f7, rmm\n"                      
			"fmaxu.pi f6, f7, f2\n"                               
			"fcvt.s.wu f3, x11, rdn\n"                            
			"flw.ps f2, -472(x11)\n"                              
			"fsgnjx.ps f8, f7, f7\n"                              
			"flt.s x3, f7, f3\n"                                  
			"maskor m1, m0, m2\n"                                 
			"fsatu8.pi f5, f0\n"                                  
			"fnmadd.ps f8, f2, f1, f2, rdn\n"                     
			"fcvt.ps.f16 f9, f1\n"                                
			"fsub.s f3, f5, f6, rup\n"                            
			"fmin.pi f4, f4, f4\n"                                
			"fpackreph.pi f3, f4\n"                               
			"fexp.ps f9, f1\n"                                    
			"fsub.pi f7, f0, f4\n"                                
			"fmul.ps f6, f0, f6, rne\n"                           
			"feq.s x3, f5, f7\n"                                  
			//RTLMIN-5479"fsin.ps f9, f3\n"                                    
			"flt.pi f2, f3, f0\n"                                 
			"fswizz.ps f9, f7, 114\n"                             
			"mov.m.x m1, x11, 66\n"                               
			"fcvt.ps.pw f4, f7, rne\n"                            
			"fandi.pi f6, f2, 403\n"                              
			"fmin.pi f5, f5, f7\n"                                
			"fminu.pi f3, f5, f6\n"                               
			"fsgnjn.s f5, f7, f7\n"                               
			"fsw f7, -1985(x11)\n"                                
			"fmulh.pi f7, f6, f5\n"                               
			"fcmov.ps f4, f1, f2, f2\n"                           
			"fpackreph.pi f4, f3\n"                               
			"maskor m0, m0, m0\n"                                 
			"fbci.pi f6, 523601\n"                                
			"fmulhu.pi f7, f4, f5\n"                              
			"fmax.s f5, f1, f3\n"                                 
			"fsub.ps f6, f7, f4, rmm\n"                           
			"fmv.s.x f6, x11\n"                                   
			"ffrc.ps f3, f3\n"                                    
			"fltu.pi f4, f6, f1\n"                                
			"maskpopc x3, m0\n"                                   
			"fcvt.s.w f4, x11, rne\n"                             
			"feq.s x4, f2, f2\n"                                  
			"fcvt.s.wu f4, x11, rdn\n"                            
			"fmul.pi f6, f0, f7\n"                                
			"fmadd.ps f9, f4, f4, f1, rtz\n"                      
			"flw f7, -1849(x11)\n"                                
			"ffrc.ps f7, f7\n"                                    
			"fcvt.f16.ps f6, f5\n"                                
			"fbci.ps f4, 106179\n"                                
			"maskor m0, m2, m1\n"                                 
			"flt.pi f3, f7, f6\n"                                 
			"fbci.ps f5, 443036\n"                                
			"feqm.ps m2, f3, f1\n"                                
			"fexp.ps f7, f6\n"                                    
			"maskand m0, m2, m2\n"                                
			"fmsub.s f7, f0, f2, f3, rdn\n"                       
			"fcvt.ps.pw f8, f3, rdn\n"                            
			"fexp.ps f3, f5\n"                                    
			"fle.s x4, f4, f3\n"                                  
			"fcvt.f16.ps f5, f3\n"                                
			"fbcx.ps f6, x11\n"                                   
			"fcmovm.ps f5, f5, f1\n"                              
			"fmin.s f8, f4, f2\n"                                 
			"fxor.pi f6, f6, f0\n"                                
			"fclass.s x3, f4\n"                                   
			"fcmovm.ps f9, f0, f7\n"                              
			"feq.s x4, f2, f5\n"                                  
			"fbci.pi f3, 522743\n"                                
			"fmin.pi f6, f4, f3\n"                                
			"fswizz.ps f7, f4, 45\n"                              
			"fsw f3, -1436(x11)\n"                                
			"frcp.ps f6, f3\n"                                    
			"maskxor m1, m2, m1\n"                                
			"fsgnjn.ps f4, f4, f0\n"                              
			"fcvt.w.s x3, f1, rdn\n"                              
			"fpackrepb.pi f4, f4\n"                               
			"fmax.pi f8, f2, f5\n"                                
			"fcvt.w.s x4, f1, rmm\n"                              
			"fclass.s x3, f7\n"                                   
			"fmvs.x.ps x3, f0, 3\n"                               
			"maskpopcz x3, m1\n"                                  
			"fsrai.pi f4, f2, 8\n"                                
			"fsub.s f9, f7, f6, rup\n"                            
			"fsatu8.pi f7, f6\n"                                  
			"flog.ps f7, f1\n"                                    
			"feqm.ps m1, f2, f1\n"                                
			"fcvt.wu.s x4, f7, rtz\n"                             
			"fmin.s f9, f7, f0\n"                                 
			"maskor m2, m0, m1\n"                                 
			"fsrai.pi f9, f1, 3\n"                                
			"fsub.s f3, f5, f3, rdn\n"                            
			"fsq2 f5, 559(x11)\n"                                 
			"fmax.s f9, f0, f7\n"                                 
			"fmul.pi f5, f1, f6\n"                                
			"feq.pi f6, f4, f4\n"                                 
			"fsat8.pi f7, f0\n"                                   
			"masknot m1, m2\n"                                    
			"maskor m1, m2, m2\n"                                 
			"fmsub.ps f7, f4, f7, f6, rne\n"                      
			"fround.ps f9, f7, dyn\n"                             
			"fmulh.pi f7, f7, f1\n"                               
			"fadd.ps f6, f0, f7, rup\n"                           

			"li x5, 0xc51daa8a7307d961\n"
			"mova.m.x x5\n"                                       
			"flem.ps m0, f5, f6\n"                                
			"fcvt.s.wu f7, x11, rtz\n"                            
			"maskor m0, m1, m2\n"                                 
			"faddi.pi f7, f4, 169\n"                              
			"fsgnjx.ps f2, f2, f7\n"                              
			"fsw.ps f6, 618(x11)\n"                               
			"fltm.pi m2, f6, f3\n"                                
			//RTLMIN-5479"fsin.ps f4, f0\n"                                    
			"fsgnj.s f5, f0, f1\n"                                
			"maskand m0, m2, m1\n"                                
			"fnmadd.ps f3, f5, f1, f3, dyn\n"                     
			"fswizz.ps f2, f6, 124\n"                             
			"fcmovm.ps f6, f4, f5\n"                              
			"flem.ps m2, f4, f6\n"                                
			"fsw.ps f2, 453(x11)\n"                               
			"fsrli.pi f5, f4, 8\n"                                
			"feq.pi f9, f3, f1\n"                                 
			"fnmsub.ps f4, f4, f1, f5, rdn\n"                     
			"maskpopcz x3, m1\n"                                  
			"for.pi f7, f3, f1\n"                                 
			"flem.ps m1, f6, f4\n"                                
			"fnot.pi f2, f5\n"                                    
			//RTLMIN-5479"fsin.ps f8, f0\n"                                    
			"fsgnjn.ps f8, f0, f4\n"                              
			"fcmovm.ps f4, f6, f4\n"                              
			"feq.ps f8, f7, f4\n"                                 
			"fsgnjn.s f7, f4, f2\n"                               
			"masknot m1, m0\n"                                    
			"fsrl.pi f2, f0, f0\n"                                
			"fpackreph.pi f5, f6\n"                               
			"feqm.ps m1, f6, f6\n"                                
			"fsatu8.pi f3, f7\n"                                  
			"fcmov.ps f3, f4, f2, f4\n"                           
			"fminu.pi f9, f7, f7\n"                               
			"fadd.ps f5, f2, f5, rdn\n"                           

			"li x5, 0xc04e9180eee985ea\n"
			"mova.m.x x5\n"                                       
			"fclass.s x4, f3\n"                                   
			"fsrai.pi f7, f7, 2\n"                                
			"fmadd.s f8, f4, f6, f0, rmm\n"                       
			"fsetm.pi m2, f3\n"                                   
			"flog.ps f6, f4\n"                                    
			"flog.ps f6, f1\n"                                    
			"feq.pi f7, f5, f2\n"                                 
			"fcvt.pw.ps f3, f2, rne\n"                            
			"feq.s x4, f2, f4\n"                                  
			"fsll.pi f2, f5, f0\n"                                
			"fbc.ps f5, 943(x11)\n"                               
			"fcvt.s.wu f6, x11, dyn\n"                            
			"fclass.ps f7, f0\n"                                  
			"fmv.s.x f4, x11\n"                                   
			"fminu.pi f3, f5, f5\n"                               
			"flt.s x3, f5, f1\n"                                  
			"feq.pi f8, f7, f6\n"                                 
			"fmadd.ps f5, f6, f4, f2, rup\n"                      
			"fpackrepb.pi f3, f3\n"                               
			"masknot m0, m0\n"                                    
			"fsatu8.pi f2, f3\n"                                  
			"fmax.pi f8, f1, f1\n"                                
			"fle.pi f3, f6, f6\n"                                 
			"fpackrepb.pi f6, f7\n"                               
			"flog.ps f7, f2\n"                                    
			"fmadd.ps f8, f3, f2, f3, rdn\n"                      
			"fcvt.pwu.ps f9, f3, dyn\n"                           
			"fclass.s x3, f2\n"                                   
			"fmv.x.s x4, f4\n"                                    
			"fbci.pi f2, 435397\n"                                
			"fbci.pi f7, 380273\n"                                
			"maskor m1, m2, m1\n"                                 
			"fnmsub.s f6, f4, f0, f7, rup\n"                      
			"fmin.pi f5, f2, f2\n"                                
			"fmadd.s f6, f7, f3, f0, rtz\n"                       
			"fsrli.pi f5, f3, 13\n"                               
			"maskxor m0, m0, m0\n"                                

			"li x5, 0xec93a6228b411923\n"
			"mova.m.x x5\n"                                       
			"fcvt.ps.pw f7, f2, rdn\n"                            
			"maskand m0, m1, m1\n"                                
			"fmax.pi f2, f0, f7\n"                                
			"fmulhu.pi f9, f2, f3\n"                              
			"flog.ps f6, f3\n"                                    
			"fle.pi f4, f4, f7\n"                                 
			"fxor.pi f6, f0, f7\n"                                
			"fclass.s x4, f4\n"                                   
			"fmul.s f2, f4, f2, dyn\n"                            
			"fle.ps f8, f3, f4\n"                                 
			"fexp.ps f2, f0\n"                                    
			"fmv.x.s x4, f5\n"                                    
			"fsw.ps f4, 1496(x11)\n"                              
			"fmv.s.x f5, x11\n"                                   
			"fsub.s f2, f2, f4, rup\n"                            
			"fmadd.s f9, f3, f1, f3, rmm\n"                       
			"fltm.pi m2, f1, f6\n"                                
			"fltm.pi m0, f2, f0\n"                                
			"flog.ps f2, f4\n"                                    
			"fsrli.pi f4, f7, 12\n"                               
			"ffrc.ps f6, f4\n"                                    
			"fnot.pi f9, f6\n"                                    
			"fcvt.f16.ps f4, f5\n"                                
			"fminu.pi f3, f4, f3\n"                               
			"fsw f2, 1715(x11)\n"                                 
			"fsgnjn.s f8, f0, f4\n"                               
			"fmax.s f6, f4, f4\n"                                 
			"fsub.s f5, f2, f2, rup\n"                            
			"maskpopcz x4, m1\n"                                  
			"flq2 f3, 1092(x11)\n"                                
			"fcvt.wu.s x4, f3, rtz\n"                             
			"fsw.ps f7, -1999(x11)\n"                             
			"fadd.pi f8, f4, f2\n"                                
			"maskpopcz x3, m0\n"                                  
			"mov.m.x m1, x11, 69\n"                               
			"flem.ps m0, f1, f2\n"                                
			"fsll.pi f7, f1, f0\n"                                
			"fmin.ps f4, f1, f4\n"                                
			"fmul.ps f8, f2, f4, rdn\n"                           
			"maskpopc x4, m2\n"                                   
			"fbci.ps f9, 65509\n"                                 
			"fbcx.ps f3, x11\n"                                   
			"fle.ps f3, f6, f1\n"                                 
			"fsgnjn.s f2, f0, f5\n"                               
			"fsetm.pi m0, f0\n"                                   
			"maskpopc x4, m1\n"                                   
			"fexp.ps f3, f4\n"                                    
			"fpackrepb.pi f5, f0\n"                               
			"maskpopc x4, m2\n"                                   
			"fclass.ps f9, f6\n"                                  
			"fadd.pi f7, f5, f4\n"                                
			"fsat8.pi f5, f4\n"                                   
			"fmv.x.s x3, f2\n"                                    
			"fmax.ps f8, f1, f1\n"                                
			"fbci.ps f9, 104437\n"                                
			"fcvt.w.s x4, f1, dyn\n"                              
			"maskor m1, m1, m1\n"                                 
			"flq2 f9, 1667(x11)\n"                                
			"fcvt.ps.pwu f7, f5, rup\n"                           
			"fmin.s f5, f1, f1\n"                                 
			"feq.ps f5, f1, f1\n"                                 
			"fpackrepb.pi f2, f3\n"                               
			"fsll.pi f2, f4, f0\n"                                
			"fcvt.s.w f5, x11, dyn\n"                             
			"fslli.pi f3, f3, 15\n"                               
			"fltm.ps m1, f6, f6\n"                                
			"feq.ps f9, f0, f7\n"                                 
			"fsat8.pi f7, f0\n"                                   
			"fmadd.s f2, f3, f5, f0, rtz\n"                       
			"fclass.s x4, f4\n"                                   
			"fsgnjx.s f2, f2, f0\n"                               
			"fsetm.pi m1, f3\n"                                   
			"flt.s x3, f1, f1\n"                                  
			"flem.ps m0, f0, f0\n"                                
			"fsatu8.pi f3, f3\n"                                  
			"fsgnjn.s f3, f7, f6\n"                               
			"fsgnjn.s f6, f1, f7\n"                               
			"fcvt.s.w f9, x11, dyn\n"                             
			"fswizz.ps f5, f7, 110\n"                             
			"fsrl.pi f3, f5, f0\n"                                
			"mova.x.m x3\n"                                       
			"flog.ps f9, f6\n"                                    
			"flt.ps f5, f7, f5\n"                                 
			"fmvz.x.ps x3, f5, 1\n"                               
			"fsq2 f0, 680(x11)\n"                                 
			"fmax.pi f9, f7, f2\n"                                
			"maskxor m0, m2, m2\n"                                
			"fcvt.ps.pw f5, f3, rdn\n"                            
			"flw.ps f6, -836(x11)\n"                              
			"masknot m0, m0\n"                                    
			"fxor.pi f7, f3, f4\n"                                
			"fminu.pi f3, f5, f7\n"                               
			"fcvt.ps.f16 f7, f0\n"                                
			"maskpopc x3, m0\n"                                   
			"fadd.ps f8, f5, f2, rup\n"                           
			"fcvt.w.s x4, f3, dyn\n"                              
			"fmax.pi f3, f6, f7\n"                                
			"fmvz.x.ps x3, f5, 6\n"                               
			"fnmsub.s f2, f6, f4, f4, rdn\n"                      
			"fsll.pi f3, f6, f7\n"                                
			"ffrc.ps f4, f6\n"                                    
			"fsgnjx.s f2, f1, f0\n"                               
			"fsub.pi f9, f0, f5\n"                                
			"fsra.pi f9, f4, f6\n"                                
			"fminu.pi f5, f5, f6\n"                               
			"fmul.ps f6, f5, f3, rne\n"                           
			"fmin.pi f5, f4, f0\n"                                
			"fmul.s f6, f7, f0, rtz\n"                            
			"fcvt.s.wu f4, x11, dyn\n"                            
			"for.pi f7, f6, f4\n"                                 
			"fsatu8.pi f9, f0\n"                                  
			//RTLMIN-5479"frsq.ps f9, f5\n"                                    
			"fmulhu.pi f7, f4, f7\n"                              
			"flt.ps f9, f7, f7\n"                                 
			"fltu.pi f3, f4, f4\n"                                
			"fmax.ps f2, f0, f7\n"                                
			"fmsub.s f9, f7, f3, f7, rdn\n"                       
			"fxor.pi f3, f1, f4\n"                                

			"li x5, 0x461c754dca557be0\n"
			"mova.m.x x5\n"                                       
			"ffrc.ps f6, f5\n"                                    
			"fmul.pi f6, f0, f3\n"                                
			"fsgnjn.s f2, f3, f2\n"                               
			"fmsub.s f5, f6, f3, f0, rdn\n"                       
			//RTLMIN-5479"frsq.ps f3, f4\n"                                    
			"fcvt.s.wu f7, x11, rup\n"                            
			"fslli.pi f7, f5, 5\n"                                
			"maskxor m1, m2, m0\n"                                
			"fsrli.pi f8, f0, 12\n"                               
			"fcvt.wu.s x3, f6, dyn\n"                             
			"fltu.pi f7, f4, f0\n"                                
			"faddi.pi f7, f3, 248\n"                              
			"fsgnjx.ps f5, f2, f4\n"                              
			"fmvs.x.ps x4, f3, 2\n"                               
			"fmsub.s f7, f7, f5, f2, dyn\n"                       
			"fltm.pi m0, f2, f5\n"                                
			"for.pi f3, f6, f6\n"                                 
			"fsgnjx.ps f4, f1, f5\n"                              
			"fsat8.pi f6, f1\n"                                   
			"fsw.ps f6, 1585(x11)\n"                              
			"fsra.pi f9, f6, f4\n"                                
			//RTLMIN-5479"frsq.ps f8, f3\n"                                    

			"li x5, 0xc4431b8e50f37ca6\n"
			"mova.m.x x5\n"                                       
			"fle.pi f2, f4, f3\n"                                 
			"fmin.s f7, f6, f7\n"                                 
			"fmaxu.pi f2, f2, f5\n"                               
			"fadd.pi f9, f1, f0\n"                                
			"fmvs.x.ps x3, f0, 2\n"                               
			"fnmadd.s f2, f4, f3, f2, rne\n"                      
			"fmulh.pi f3, f4, f2\n"                               
			"fnmsub.ps f6, f5, f2, f0, rtz\n"                     
			"fmin.pi f7, f3, f5\n"                                
			"fexp.ps f6, f0\n"                                    
			"fpackreph.pi f2, f3\n"                               
			"fmvz.x.ps x3, f7, 2\n"                               
			//RTLMIN-5479"fsin.ps f3, f3\n"                                    
			"fsub.ps f5, f6, f5, rdn\n"                           
			"fbcx.ps f4, x11\n"                                   
			"fmadd.ps f5, f6, f6, f5, rmm\n"                      
			//RTLMIN-5479"frsq.ps f8, f4\n"                                    
			"fnmsub.ps f9, f0, f4, f5, rne\n"                     
			"fmin.pi f4, f2, f4\n"                                
			"fpackrepb.pi f5, f3\n"                               
			"flq2 f6, 713(x11)\n"                                 
			"feq.s x3, f7, f2\n"                                  
			"fclass.s x4, f0\n"                                   
			"fswizz.ps f4, f1, 91\n"                              
			"fsgnjn.ps f2, f0, f1\n"                              
			"fmaxu.pi f9, f7, f5\n"                               
			"fpackreph.pi f8, f6\n"                               
			"feq.pi f7, f4, f2\n"                                 
			"fsll.pi f2, f7, f4\n"                                
			"fmax.s f6, f7, f2\n"                                 
			"fle.s x3, f3, f1\n"                                  
			"fcmov.ps f2, f2, f5, f6\n"                           
			"flt.s x4, f0, f4\n"                                  
			"fadd.pi f8, f3, f3\n"                                
			"maskand m2, m2, m0\n"                                
			//RTLMIN-5479"frsq.ps f3, f1\n"                                    
			"for.pi f2, f6, f2\n"                                 
			"flem.ps m1, f0, f6\n"                                
			"fcvt.f16.ps f2, f3\n"                                
			"fclass.ps f2, f6\n"                                  
			"fcvt.f16.ps f8, f4\n"                                
			"fmin.pi f6, f5, f7\n"                                
			"fround.ps f7, f7, rup\n"                             
			"fmul.ps f4, f2, f0, rtz\n"                           
			"maskxor m2, m1, m2\n"                                
			"fcvt.f16.ps f7, f5\n"                                
			"fmaxu.pi f3, f3, f5\n"                               
			"fcvt.wu.s x3, f7, rup\n"                             
			"fexp.ps f3, f2\n"                                    
			"fsrli.pi f3, f4, 0\n"                                
			"fxor.pi f2, f3, f5\n"                                
			//RTLMIN-5479"fsin.ps f8, f1\n"                                    
			"mova.x.m x3\n"                                       
			"fpackreph.pi f7, f5\n"                               
			"fsrli.pi f6, f4, 8\n"                                
			"for.pi f3, f5, f7\n"                                 
			"fcmovm.ps f6, f3, f2\n"                              
			"maskand m2, m1, m1\n"                                
			"feq.pi f5, f1, f4\n"                                 
			"flw.ps f2, -1827(x11)\n"                             
			"flt.ps f3, f4, f5\n"                                 
			"fmsub.ps f3, f0, f5, f0, rmm\n"                      
			"fcmov.ps f6, f2, f4, f6\n"                           
			"fclass.ps f4, f3\n"                                  
			"flog.ps f3, f0\n"                                    
			"fcvt.f16.ps f3, f6\n"                                
			"fsra.pi f3, f1, f5\n"                                
			"fxor.pi f9, f2, f1\n"                                
			"fsub.ps f5, f0, f3, rtz\n"                           
			"fmv.s.x f2, x11\n"                                   
			"fsw f3, 1416(x11)\n"                                 
			"fsetm.pi m1, f1\n"                                   
			"maskand m2, m0, m2\n"                                
			"fmvz.x.ps x4, f4, 4\n"                               
			"fslli.pi f9, f0, 11\n"                               
			"fmulhu.pi f7, f7, f2\n"                              
			"fcvt.ps.pw f7, f4, rne\n"                            
			//RTLMIN-5479"frsq.ps f6, f5\n"                                    
			"mov.m.x m0, x11, 76\n"                               
			"fsub.pi f7, f0, f4\n"                                
			"maskpopc x3, m0\n"                                   
			"fmulh.pi f7, f6, f3\n"                               
			"frcp.ps f6, f7\n"                                    
			"frcp.ps f6, f4\n"                                    
			"fsrai.pi f2, f5, 14\n"                               
			"maskor m2, m1, m0\n"                                 
			"fnmsub.s f7, f6, f1, f1, rmm\n"                      
			"fsw.ps f6, 1354(x11)\n"                              
			"fnmadd.ps f2, f5, f0, f4, rtz\n"                     
			"fclass.s x4, f5\n"                                   
			"fsetm.pi m2, f7\n"                                   
			"fmsub.s f4, f4, f5, f6, rtz\n"                       
			"fswizz.ps f4, f7, 21\n"                              
			"feq.pi f9, f4, f1\n"                                 
			"fsra.pi f3, f0, f5\n"                                
			"fadd.ps f2, f4, f0, rup\n"                           
			"maskor m1, m1, m1\n"                                 
			"fpackreph.pi f4, f6\n"                               
			"fsetm.pi m0, f6\n"                                   
			"fcvt.f16.ps f9, f2\n"                                
			"fbc.ps f5, -1771(x11)\n"                             
			"fsgnj.s f3, f5, f4\n"                                
			"fcvt.pw.ps f9, f5, dyn\n"                            
			"flq2 f6, 1344(x11)\n"                                
			"fmax.ps f8, f1, f5\n"                                
			"for.pi f4, f7, f5\n"                                 
			"fcvt.s.w f4, x11, dyn\n"                             
			"fswizz.ps f4, f3, 93\n"                              
			"mova.x.m x4\n"                                       
			"fmin.ps f4, f0, f5\n"                                
			"fsgnj.ps f8, f2, f5\n"                               
			"fsgnjn.ps f5, f1, f4\n"                              
			"fpackreph.pi f9, f2\n"                               
			"fsw f2, -19(x11)\n"                                  
			"fadd.pi f5, f4, f0\n"                                
			"fslli.pi f4, f3, 14\n"                               
			"fcvt.wu.s x3, f2, rmm\n"                             
			"ffrc.ps f2, f7\n"                                    
			"fswizz.ps f6, f1, 67\n"                              
			"fmin.s f4, f6, f0\n"                                 
			"fsgnjx.ps f2, f4, f3\n"                              
			"flw.ps f9, -823(x11)\n"                              
			"fle.pi f7, f0, f2\n"                                 
			"maskand m1, m2, m2\n"                                
			"fswizz.ps f3, f4, 123\n"                             
			"frcp.ps f8, f2\n"                                    
			"fpackrepb.pi f3, f4\n"                               
			"maskxor m0, m1, m0\n"                                
			"fadd.pi f5, f4, f2\n"                                
			"flem.ps m0, f6, f2\n"                                
			"fsub.pi f3, f7, f6\n"                                
			"fsgnj.ps f6, f1, f0\n"                               
			"fle.pi f7, f3, f6\n"                                 
			"masknot m0, m2\n"                                    
			"fcvt.s.wu f3, x11, dyn\n"                            
			"for.pi f7, f7, f2\n"                                 
			"fsub.ps f3, f4, f0, rne\n"                           
			"fmv.x.s x3, f6\n"                                    
			"mova.x.m x4\n"                                       
			"fmvs.x.ps x3, f5, 5\n"                               
			"fsgnj.s f7, f2, f5\n"                                
			"flt.s x4, f6, f0\n"                                  
			"masknot m2, m0\n"                                    
			"fcmov.ps f5, f4, f4, f1\n"                           
			"fle.pi f2, f5, f3\n"                                 
			"fcvt.s.w f5, x11, rtz\n"                             
			"fmax.pi f8, f2, f7\n"                                
			"fmsub.ps f8, f7, f0, f5, rdn\n"                      
			"fsub.pi f2, f2, f6\n"                                
			"fcvt.f16.ps f2, f6\n"                                
			"flt.ps f3, f6, f2\n"                                 
			"fcvt.w.s x4, f4, dyn\n"                              
			"fsgnjn.ps f4, f0, f3\n"                              
			"fmax.s f5, f7, f6\n"                                 
			"fle.s x3, f2, f1\n"                                  
			"fcvt.ps.pwu f3, f2, rtz\n"                           
			"fnmadd.s f8, f6, f4, f7, dyn\n"                      
			"fnmadd.ps f5, f6, f5, f2, rdn\n"                     
			"fadd.ps f6, f5, f5, rne\n"                           
			"fmadd.ps f8, f2, f7, f1, dyn\n"                      
			"fandi.pi f2, f0, 440\n"                              
			"fbci.ps f2, 241527\n"                                
			"fadd.s f7, f3, f3, rmm\n"                            
			"fcvt.pwu.ps f5, f3, rne\n"                           
			"feq.pi f6, f1, f2\n"                                 
			"fmul.pi f6, f7, f0\n"                                
			"flw.ps f5, -339(x11)\n"                              
			"fexp.ps f6, f5\n"                                    
			"fmulhu.pi f2, f2, f3\n"                              
			"fsgnjx.ps f9, f7, f6\n"                              
			"fnmadd.ps f6, f4, f0, f3, rmm\n"                     
			"fsrli.pi f2, f7, 15\n"                               
			"fmul.pi f6, f2, f7\n"                                
			"maskor m2, m2, m0\n"                                 
			"fmin.ps f3, f6, f6\n"                                
			"fsub.s f5, f1, f6, dyn\n"                            
			"fmin.ps f2, f0, f2\n"                                
			"fmulhu.pi f7, f2, f7\n"                              
			"fround.ps f4, f3, rup\n"                             
			"fsub.ps f4, f3, f4, dyn\n"                           
			"fnmsub.ps f4, f2, f4, f6, dyn\n"                     
			"fsub.ps f8, f7, f4, rup\n"                           
			"fmax.s f5, f2, f7\n"                                 
			"fmax.pi f7, f1, f6\n"                                
			"maskand m0, m2, m2\n"                                
			"fsub.ps f9, f0, f3, rne\n"                           
			"fmaxu.pi f2, f5, f7\n"                               
			"fsatu8.pi f6, f7\n"                                  
			"fand.pi f3, f3, f2\n"                                
			"fadd.ps f3, f2, f4, dyn\n"                           
			"feqm.ps m2, f7, f2\n"                                
			"fmadd.ps f8, f1, f0, f0, rmm\n"                      
			"fslli.pi f4, f3, 13\n"                               
			"fsub.s f9, f0, f5, rup\n"                            
			"flt.s x4, f2, f6\n"                                  
			"maskxor m0, m1, m2\n"                                
			"fsgnjx.ps f5, f2, f4\n"                              
			"fadd.ps f7, f6, f1, rdn\n"                           
			"fmaxu.pi f8, f7, f2\n"                               
			"fmin.pi f3, f3, f6\n"                                
			"masknot m2, m0\n"                                    
			"fcvt.wu.s x3, f4, dyn\n"                             
			"fcvt.pw.ps f7, f1, rup\n"                            
			"fsgnj.ps f3, f1, f1\n"                               
			"fadd.ps f8, f4, f6, dyn\n"                           
			"fsgnjx.ps f6, f5, f3\n"                              
			"for.pi f9, f6, f0\n"                                 
			"fcvt.w.s x4, f7, dyn\n"                              
			"fnmadd.s f6, f6, f4, f7, rne\n"                      
			"fmulhu.pi f6, f1, f7\n"                              
			"fcvt.pwu.ps f2, f4, dyn\n"                           
			//RTLMIN-5479"frsq.ps f4, f1\n"                                    
			"fand.pi f8, f0, f6\n"                                
			"fsgnjn.s f3, f0, f2\n"                               
			"fmin.s f7, f3, f7\n"                                 
			"fsgnj.s f8, f7, f0\n"                                
			"fmv.s.x f6, x11\n"                                   
			"fclass.ps f5, f5\n"                                  
			"fsrl.pi f5, f1, f5\n"                                
			"fclass.s x3, f4\n"                                   
			"fmsub.s f2, f7, f1, f2, dyn\n"                       
			"fadd.pi f4, f5, f0\n"                                
			"fcvt.pwu.ps f4, f7, dyn\n"                           
			"feqm.ps m2, f6, f5\n"                                
			"fcmovm.ps f2, f2, f6\n"                              
			"masknot m2, m2\n"                                    
			"fadd.pi f9, f6, f1\n"                                
			"fnmadd.ps f3, f4, f6, f1, rup\n"                     
			"fclass.s x3, f6\n"                                   
			"flq2 f4, 1248(x11)\n"                                
			"fcvt.wu.s x4, f6, rmm\n"                             
			"fmulhu.pi f3, f1, f2\n"                              
			"fcvt.pwu.ps f6, f6, rne\n"                           
			"fnmadd.ps f3, f7, f1, f7, rtz\n"                     
			"fcmov.ps f7, f5, f7, f6\n"                           
			"flog.ps f3, f7\n"                                    
			"fnmadd.s f7, f7, f3, f6, rtz\n"                      
			"frcp.ps f4, f7\n"                                    
			"fxor.pi f9, f4, f0\n"                                
			"fsub.s f6, f1, f1, dyn\n"                            
			"fmvz.x.ps x3, f6, 6\n"                               
			"fxor.pi f2, f0, f5\n"                                
			"fsw f4, 65(x11)\n"                                   
			"maskpopc x3, m2\n"                                   
			"feqm.ps m1, f7, f6\n"                                
			"fmin.s f5, f6, f4\n"                                 
			"fbci.pi f6, 401660\n"                                
			"faddi.pi f7, f5, 244\n"                              
			"fswizz.ps f8, f7, 26\n"                              
			"fround.ps f7, f4, dyn\n"                             
			"fltm.pi m0, f2, f7\n"                                
			"fpackreph.pi f9, f1\n"                               
			"fmax.s f2, f3, f0\n"                                 
			"fmin.s f7, f4, f1\n"                                 
			"fsra.pi f7, f7, f1\n"                                
			"fmvz.x.ps x4, f3, 6\n"                               
			"masknot m0, m0\n"                                    
			"fpackrepb.pi f5, f3\n"                               
			"fcvt.s.w f6, x11, dyn\n"                             
			"fslli.pi f9, f6, 8\n"                                
			//RTLMIN-5479"frsq.ps f4, f6\n"                                    
			"fxor.pi f8, f7, f1\n"                                
			"fadd.pi f4, f2, f3\n"                                
			"flw.ps f4, -1078(x11)\n"                             
			"fsq2 f2, 57(x11)\n"                                  
			"feq.pi f9, f5, f0\n"                                 
			"fcvt.w.s x4, f3, rne\n"                              
			"fadd.ps f7, f2, f6, rdn\n"                           
			"fcvt.ps.f16 f6, f7\n"                                
			//RTLMIN-5479"frsq.ps f8, f0\n"                                    
			"fand.pi f8, f1, f6\n"                                
			"frcp.ps f5, f0\n"                                    
			"fandi.pi f3, f0, 296\n"                              
			"flt.pi f4, f3, f6\n"                                 
			"fmax.pi f2, f4, f0\n"                                
			"for.pi f9, f4, f3\n"                                 
			"frcp.ps f3, f4\n"                                    
			"feq.ps f9, f2, f5\n"                                 
			"fadd.pi f8, f6, f0\n"                                
			"fsub.s f4, f0, f2, rtz\n"                            
			"ffrc.ps f4, f5\n"                                    
			"fsub.ps f5, f7, f2, dyn\n"                           
			"fbcx.ps f8, x11\n"                                   
			"fmulhu.pi f9, f4, f3\n"                              
			"maskxor m0, m0, m2\n"                                
			"fadd.pi f3, f0, f1\n"                                
			"maskand m1, m1, m2\n"                                
			"flt.pi f8, f0, f7\n"                                 
			"fmulhu.pi f4, f1, f5\n"                              
			"fnmsub.ps f6, f3, f3, f3, rdn\n"                     
			"fnmsub.ps f4, f6, f6, f3, dyn\n"                     
			"fmv.x.s x4, f0\n"                                    
			"fsub.s f3, f7, f0, rdn\n"                            
			"feq.pi f8, f1, f3\n"                                 
			"maskpopc x4, m1\n"                                   
			"fnmsub.s f2, f0, f0, f2, rne\n"                      
			"ffrc.ps f2, f3\n"                                    
			"fsgnjx.ps f8, f1, f5\n"                              
			"maskxor m2, m0, m2\n"                                
			"fsgnj.ps f4, f0, f2\n"                               
			"fxor.pi f2, f2, f0\n"                                
			"fle.pi f3, f2, f0\n"                                 
			"fsgnjx.s f5, f3, f2\n"                               
			"fmadd.s f9, f0, f7, f6, rup\n"                       
			"fsgnj.ps f9, f1, f2\n"                               
			"flem.ps m1, f1, f3\n"                                
			"fmv.s.x f8, x11\n"                                   
			"flq2 f5, 1796(x11)\n"                                
			"faddi.pi f8, f5, 372\n"                              
			"fmvz.x.ps x3, f4, 1\n"                               
			"fmax.ps f4, f6, f6\n"                                
			"fcmovm.ps f9, f6, f5\n"                              
			"for.pi f5, f2, f4\n"                                 
			"mov.m.x m0, x11, 14\n"                               
			"fswizz.ps f9, f4, 112\n"                             
			//RTLMIN-5479"frsq.ps f5, f7\n"                                    
			//RTLMIN-5479"frsq.ps f5, f3\n"                                    
			"fcvt.s.w f5, x11, rne\n"                             
			"fmaxu.pi f7, f5, f7\n"                               
			"fnmsub.ps f6, f1, f3, f3, rne\n"                     
			"fmul.pi f3, f1, f0\n"                                
			"fcmovm.ps f7, f1, f3\n"                              
			"fle.s x3, f5, f7\n"                                  
			"fnot.pi f9, f3\n"                                    
			"feq.s x4, f5, f6\n"                                  
			"fmadd.ps f3, f4, f6, f2, rdn\n"                      
			"for.pi f8, f4, f0\n"                                 
			"fmv.s.x f4, x11\n"                                   
			"fsetm.pi m0, f2\n"                                   
			"fmaxu.pi f4, f7, f0\n"                               
			"fmsub.s f5, f5, f3, f4, dyn\n"                       
			"for.pi f7, f3, f1\n"                                 
			"faddi.pi f7, f3, 9\n"                                
			"fmul.ps f9, f0, f7, rne\n"                           
			"fsra.pi f4, f5, f7\n"                                
			"for.pi f5, f4, f5\n"                                 
			"fle.pi f3, f4, f0\n"                                 
			"fround.ps f6, f5, rne\n"                             
			"fsgnjx.ps f6, f1, f4\n"                              
			"fcvt.ps.pwu f3, f4, rup\n"                           
			"fsw.ps f4, 1529(x11)\n"                              
			"fsub.s f4, f7, f4, rup\n"                            
			"fsatu8.pi f6, f2\n"                                  
			"fsrl.pi f4, f7, f7\n"                                
			"fsub.s f5, f2, f5, rdn\n"                            
			"fnot.pi f8, f3\n"                                    
			"fsgnj.ps f9, f3, f3\n"                               
			"fsub.pi f2, f2, f2\n"                                
			//RTLMIN-5479"fsin.ps f2, f0\n"                                    
			"fclass.ps f3, f4\n"                                  
			"flt.ps f5, f6, f2\n"                                 
			"fcvt.s.wu f7, x11, rmm\n"                            

			"li x5, 0x8557b469df1c0065\n"
			"mova.m.x x5\n"                                       
			"fcvt.ps.pwu f5, f0, rdn\n"                           
			"fand.pi f4, f0, f2\n"                                
			//RTLMIN-5479"frsq.ps f5, f2\n"                                    
			"fsetm.pi m1, f4\n"                                   
			"fmul.pi f3, f1, f1\n"                                
			"fand.pi f2, f3, f5\n"                                
			"fcvt.s.wu f8, x11, rdn\n"                            
			"fmaxu.pi f2, f0, f0\n"                               
			"faddi.pi f4, f4, 215\n"                              
			"fcmovm.ps f9, f6, f3\n"                              
			"fmaxu.pi f3, f5, f4\n"                               
			"fmul.s f5, f4, f5, rdn\n"                            
			"fmax.pi f2, f4, f2\n"                                
			"fswizz.ps f8, f7, 7\n"                               
			"fnmsub.s f3, f4, f0, f3, rup\n"                      
			"fsw f2, 257(x11)\n"                                  
			"fsetm.pi m1, f6\n"                                   
			"fmul.pi f2, f7, f7\n"                                
			"mov.m.x m1, x11, 54\n"                               
			"fmsub.s f2, f2, f1, f0, rdn\n"                       
			"fnot.pi f6, f3\n"                                    
			"fcvt.ps.pw f4, f0, rdn\n"                            
			"fclass.s x3, f0\n"                                   
			"fmsub.ps f6, f0, f1, f2, dyn\n"                      
			"flt.pi f6, f4, f1\n"                                 
			"fmvs.x.ps x3, f5, 7\n"                               
			"flog.ps f2, f4\n"                                    
			"fadd.s f8, f6, f1, rmm\n"                            
			"ffrc.ps f5, f6\n"                                    
			"fnot.pi f8, f2\n"                                    
			"frcp.ps f5, f2\n"                                    
			"fbc.ps f6, 110(x11)\n"                               
			"fnmadd.s f7, f5, f1, f7, rmm\n"                      
			"fmax.s f2, f2, f7\n"                                 
			"flog.ps f8, f6\n"                                    
			"fltu.pi f3, f3, f2\n"                                
			"fmax.ps f6, f3, f3\n"                                
			"fsgnjx.s f9, f4, f7\n"                               
			"fltm.pi m0, f2, f1\n"                                
			"fexp.ps f5, f4\n"                                    
			"fadd.pi f6, f4, f2\n"                                
			"fmin.s f7, f5, f4\n"                                 
			"fsrl.pi f8, f0, f6\n"                                
			"fminu.pi f9, f5, f5\n"                               
			"fsgnjn.ps f5, f5, f3\n"                              
			"fcvt.w.s x4, f4, rtz\n"                              
			"fsetm.pi m0, f4\n"                                   
			"fsgnj.ps f9, f4, f4\n"                               
			//RTLMIN-5479"frsq.ps f5, f6\n"                                    
			"fadd.ps f2, f0, f5, rup\n"                           
			"fsgnjx.s f5, f0, f3\n"                               
			"fsgnjn.s f5, f4, f0\n"                               
			"fltm.ps m0, f1, f0\n"                                
			"fmax.pi f4, f2, f4\n"                                
			"fsrl.pi f2, f5, f1\n"                                
			"flog.ps f8, f6\n"                                    
			"fminu.pi f9, f5, f7\n"                               
			"ffrc.ps f7, f6\n"                                    
			//RTLMIN-5479"frsq.ps f5, f2\n"                                    
			"mova.x.m x4\n"                                       
			"maskor m2, m2, m1\n"                                 
			"fclass.s x3, f4\n"                                   
			"fcvt.f16.ps f6, f4\n"                                
			"fsrli.pi f2, f2, 12\n"                               
			"fclass.s x3, f1\n"                                   
			"fmin.ps f2, f1, f7\n"                                
			"fsub.s f6, f2, f3, dyn\n"                            
			"maskpopc x4, m2\n"                                   
			"flt.pi f5, f2, f5\n"                                 
			"fcvt.pw.ps f3, f4, rne\n"                            
			"for.pi f8, f2, f2\n"                                 
			"flt.ps f6, f1, f1\n"                                 
			"fand.pi f5, f7, f6\n"                                
			"feqm.ps m1, f1, f2\n"                                
			"maskpopcz x3, m1\n"                                  
			"fcvt.ps.f16 f5, f5\n"                                
			"fcvt.pw.ps f4, f3, rup\n"                            
			"fclass.ps f4, f6\n"                                  
			"fsat8.pi f2, f0\n"                                   
			"fclass.s x4, f2\n"                                   
			"fltu.pi f9, f1, f4\n"                                
			"flt.ps f4, f7, f3\n"                                 
			"fswizz.ps f9, f5, 93\n"                              
			"fandi.pi f7, f5, 203\n"                              
			"fclass.s x4, f3\n"                                   
			"ffrc.ps f9, f5\n"                                    
			"fadd.pi f4, f0, f3\n"                                
			"fcvt.ps.pw f7, f5, rmm\n"                            
			"fcvt.f16.ps f7, f5\n"                                
			//RTLMIN-5479"fsin.ps f8, f1\n"                                    
			"fbc.ps f4, -1318(x11)\n"                             
			"fcvt.pwu.ps f2, f3, dyn\n"                           
			"fmv.x.s x3, f0\n"                                    
			"fsatu8.pi f9, f4\n"                                  
			"fmaxu.pi f2, f5, f4\n"                               
			"fmulhu.pi f5, f4, f6\n"                              
			"feqm.ps m1, f2, f2\n"                                
			"feqm.ps m2, f0, f1\n"                                
			"maskand m0, m1, m0\n"                                
			"flt.pi f5, f3, f3\n"                                 
			"fsgnj.s f5, f2, f5\n"                                
			"fnot.pi f9, f3\n"                                    
			"fcvt.s.wu f2, x11, rtz\n"                            
			"fltm.pi m2, f1, f2\n"                                
			//RTLMIN-5479"fsin.ps f9, f7\n"                                    

			"li x5, 0x9675dd631d17ba86\n"
			"mova.m.x x5\n"                                       
			"maskpopcz x4, m1\n"                                  
			"feq.pi f3, f1, f6\n"                                 
			"fpackreph.pi f8, f7\n"                               
			"fmvs.x.ps x3, f6, 5\n"                               
			"fsra.pi f5, f7, f1\n"                                
			"mova.x.m x4\n"                                       
			"fand.pi f2, f0, f1\n"                                
			"fminu.pi f8, f2, f7\n"                               
			"fsgnjx.ps f8, f0, f4\n"                              
			"fle.ps f8, f6, f2\n"                                 
			"fsub.s f2, f3, f7, rtz\n"                            
			"flt.ps f5, f2, f0\n"                                 
			"fsgnj.s f2, f0, f5\n"                                
			"fmvs.x.ps x4, f4, 0\n"                               
			"fmv.x.s x4, f2\n"                                    
			"fandi.pi f6, f2, 172\n"                              
			"fcvt.ps.pw f4, f2, rmm\n"                            
			"ffrc.ps f5, f2\n"                                    
			"fminu.pi f5, f4, f3\n"                               
			"flw f5, -1411(x11)\n"                                
			"fmul.ps f3, f5, f0, rne\n"                           
			"fmsub.ps f7, f3, f2, f0, dyn\n"                      
			"fmin.s f4, f1, f3\n"                                 
			"fsetm.pi m2, f6\n"                                   
			"fbcx.ps f5, x11\n"                                   
			"fandi.pi f3, f0, 95\n"                               
			"fsrli.pi f5, f4, 0\n"                                
			"feq.s x3, f2, f2\n"                                  
			"fsw.ps f3, -919(x11)\n"                              
			"fbci.ps f5, 76213\n"                                 
			"for.pi f8, f2, f4\n"                                 
			"fsatu8.pi f3, f5\n"                                  
			"fcmov.ps f9, f2, f5, f6\n"                           
			"fmulhu.pi f2, f7, f6\n"                              
			"frcp.ps f5, f5\n"                                    
			"fsq2 f1, 343(x11)\n"                                 
			"mova.x.m x3\n"                                       
			"fmax.ps f4, f1, f2\n"                                
			"fpackrepb.pi f5, f5\n"                               
			"fsw f7, 1127(x11)\n"                                 
			"fltm.ps m0, f3, f0\n"                                
			"fbc.ps f9, 325(x11)\n"                               
			"fmul.ps f9, f7, f3, rmm\n"                           
			"fmaxu.pi f3, f5, f4\n"                               
			"fsrl.pi f3, f0, f2\n"                                
			"fmadd.ps f5, f3, f3, f1, rdn\n"                      
			"fmin.ps f3, f0, f1\n"                                
			"fcmovm.ps f3, f4, f1\n"                              
			"flog.ps f2, f0\n"                                    
			"fmul.s f6, f7, f4, rmm\n"                            
			"fbci.pi f9, 207920\n"                                
			"fnot.pi f7, f7\n"                                    
			"fmvz.x.ps x3, f5, 7\n"                               
			"fltm.pi m0, f5, f2\n"                                
			"for.pi f4, f0, f7\n"                                 
			"fmulh.pi f7, f0, f1\n"                               
			"fminu.pi f4, f0, f2\n"                               
			"fadd.pi f9, f6, f0\n"                                
			"fle.pi f3, f2, f2\n"                                 
			"fcvt.s.wu f3, x11, rtz\n"                            
			"frcp.ps f3, f0\n"                                    
			"fsrli.pi f5, f0, 11\n"                               
			"fmin.ps f4, f0, f4\n"                                
			"fbci.pi f8, 166093\n"                                
			"fswizz.ps f7, f5, 71\n"                              
			"fsw.ps f5, 371(x11)\n"                               
			"fmax.ps f2, f1, f6\n"                                
			"maskpopcz x4, m1\n"                                  
			"fle.ps f2, f2, f3\n"                                 
			"fmadd.ps f4, f5, f3, f7, rup\n"                      
			"flw f9, -326(x11)\n"                                 
			"flem.ps m0, f0, f3\n"                                
			"flt.pi f4, f4, f4\n"                                 
			"fsatu8.pi f6, f3\n"                                  
			"fsq2 f5, -485(x11)\n"                                
			"fsatu8.pi f7, f4\n"                                  
			"fmvz.x.ps x3, f0, 4\n"                               
			"fsrl.pi f8, f4, f6\n"                                
			"fmulhu.pi f9, f7, f0\n"                              
			"maskxor m1, m0, m1\n"                                
			"fnmsub.s f7, f2, f6, f2, rtz\n"                      
			"fsetm.pi m2, f4\n"                                   
			"fcmov.ps f4, f6, f6, f4\n"                           
			"fmvz.x.ps x4, f5, 6\n"                               
			"fmadd.ps f6, f2, f0, f3, dyn\n"                      
			"masknot m1, m1\n"                                    
			"fmadd.s f5, f4, f3, f1, rtz\n"                       
			"fmulh.pi f3, f7, f6\n"                               
			"fmax.s f2, f0, f2\n"                                 
			"fbc.ps f5, 696(x11)\n"                               
			"flw f7, 632(x11)\n"                                  
			"fxor.pi f4, f4, f1\n"                                
			"fnmadd.ps f5, f1, f3, f5, rdn\n"                     
			"fcvt.pw.ps f2, f7, rup\n"                            
			"fmul.ps f2, f7, f1, rtz\n"                           
			"feq.s x3, f0, f1\n"                                  
			"fle.pi f4, f2, f7\n"                                 
			"fsrl.pi f5, f2, f4\n"                                
			"fandi.pi f7, f6, 398\n"                              
			"fsub.pi f2, f1, f0\n"                                
			"fadd.ps f6, f0, f1, dyn\n"                           
			"ffrc.ps f2, f7\n"                                    
			"fbcx.ps f7, x11\n"                                   
			"mov.m.x m0, x11, 43\n"                               
			"feq.pi f4, f6, f0\n"                                 
			"flq2 f5, -649(x11)\n"                                
			"fsub.ps f5, f3, f2, rup\n"                           
			"maskpopc x4, m1\n"                                   
			"fcmov.ps f9, f2, f0, f3\n"                           
			"fadd.pi f2, f3, f3\n"                                
			"fbci.pi f7, 30931\n"                                 
			"maskpopc x4, m1\n"                                   
			"fsra.pi f7, f1, f7\n"                                
			"flw f7, -1375(x11)\n"                                
			"fsrl.pi f9, f1, f2\n"                                
			"fadd.ps f3, f2, f1, rne\n"                           
			"fclass.s x3, f3\n"                                   
			"fcvt.wu.s x4, f2, rdn\n"                             
			"fcvt.s.wu f3, x11, rmm\n"                            
			"for.pi f9, f6, f4\n"                                 
			"fcvt.ps.f16 f5, f4\n"                                
			"fmv.s.x f3, x11\n"                                   
			"fmvz.x.ps x4, f1, 4\n"                               
			"mov.m.x m1, x11, 55\n"                               
			"feqm.ps m1, f4, f6\n"                                
			"fnmsub.ps f2, f0, f3, f3, rne\n"                     
			"fsub.ps f7, f3, f5, rdn\n"                           
			"maskor m0, m0, m2\n"                                 
			"fcvt.pwu.ps f9, f0, rdn\n"                           
			"fmadd.s f9, f0, f2, f1, rtz\n"                       
			"mova.x.m x3\n"                                       
			"fswizz.ps f7, f7, 115\n"                             
			"fsub.ps f8, f5, f7, rdn\n"                           
			"fcmovm.ps f8, f7, f0\n"                              
			"fmaxu.pi f6, f3, f4\n"                               
			"flq2 f5, 1343(x11)\n"                                
			"fcvt.f16.ps f8, f6\n"                                
			"fxor.pi f4, f0, f1\n"                                
			"fsgnjn.ps f7, f1, f0\n"                              
			"fcvt.ps.pwu f7, f5, rup\n"                           
			"fsgnjx.ps f2, f1, f3\n"                              
			"fmv.x.s x4, f2\n"                                    
			"fmadd.ps f7, f6, f2, f2, rtz\n"                      
			"fmsub.ps f3, f2, f2, f2, rtz\n"                      
			"fmv.x.s x3, f7\n"                                    
			"fsatu8.pi f6, f5\n"                                  
			"fsat8.pi f4, f7\n"                                   
			"mov.m.x m0, x11, 101\n"                              
			"fmax.pi f6, f1, f3\n"                                
			"maskpopc x3, m0\n"                                   
			"fclass.s x4, f2\n"                                   
			"fsrli.pi f8, f2, 2\n"                                
			"fmadd.s f6, f2, f1, f3, rdn\n"                       
			"fxor.pi f6, f1, f2\n"                                
			"fmv.s.x f5, x11\n"                                   
			"fand.pi f5, f3, f3\n"                                
			"ffrc.ps f9, f4\n"                                    
			"fsgnjn.ps f5, f7, f4\n"                              
			"fsq2 f3, 1829(x11)\n"                                
			"flw.ps f9, -1498(x11)\n"                             
			"fnmadd.s f2, f4, f1, f7, rne\n"                      
			"fle.ps f6, f0, f2\n"                                 
			"maskpopc x3, m1\n"                                   
			"fmax.pi f5, f0, f5\n"                                
			"fadd.ps f8, f7, f0, rne\n"                           
			"mova.x.m x3\n"                                       
			"flt.pi f2, f3, f0\n"                                 
			"fmaxu.pi f9, f5, f5\n"                               
			"fnmsub.ps f8, f1, f1, f2, rmm\n"                     
			"fmaxu.pi f6, f6, f6\n"                               
			"fmax.ps f8, f6, f3\n"                                
			"fsw f7, 329(x11)\n"                                  
			"mova.x.m x3\n"                                       
			"ffrc.ps f4, f5\n"                                    
			//RTLMIN-5479"fsin.ps f2, f2\n"                                    
			"fsgnjn.s f7, f0, f5\n"                               
			"fcvt.ps.pw f2, f3, rup\n"                            
			"fmv.x.s x3, f7\n"                                    
			"fsra.pi f9, f2, f3\n"                                
			"flw.ps f4, 316(x11)\n"                               
			"fmsub.ps f2, f1, f2, f5, rup\n"                      
			"flt.pi f5, f5, f4\n"                                 
			"flt.s x4, f0, f2\n"                                  
			"fbc.ps f5, 1454(x11)\n"                              
			"fexp.ps f7, f7\n"                                    
			"fsrai.pi f8, f1, 10\n"                               
			"fexp.ps f8, f6\n"                                    
			"fandi.pi f4, f3, 392\n"                              
			"fbci.ps f2, 54091\n"                                 
			"fmulhu.pi f3, f7, f1\n"                              
			"flw f7, -5(x11)\n"                                   
			"fminu.pi f7, f6, f3\n"                               
			"flq2 f4, 217(x11)\n"                                 
			"fltm.ps m2, f0, f7\n"                                
			"fround.ps f3, f6, rup\n"                             
			"fadd.pi f3, f5, f7\n"                                
			"fsrai.pi f6, f5, 2\n"                                
			"feq.s x3, f5, f5\n"                                  
			"feq.ps f7, f7, f0\n"                                 
			"fsw f1, 609(x11)\n"                                  
			"fslli.pi f4, f1, 9\n"                                
			"ffrc.ps f4, f1\n"                                    
			"fpackrepb.pi f9, f6\n"                               
			"fle.s x4, f2, f4\n"                                  
			"fand.pi f6, f1, f4\n"                                
			"fadd.ps f4, f0, f4, rup\n"                           
			"feq.pi f7, f5, f5\n"                                 
			"frcp.ps f4, f1\n"                                    
			"fcvt.ps.pwu f7, f4, rmm\n"                           
			"fcvt.s.wu f2, x11, rtz\n"                            
			"fcvt.ps.f16 f5, f7\n"                                
			"fexp.ps f4, f4\n"                                    
			"fmax.pi f4, f6, f7\n"                                
			"fmv.x.s x3, f6\n"                                    
			"fadd.s f2, f3, f2, dyn\n"                            
			"fnot.pi f4, f6\n"                                    
			"fmv.s.x f9, x11\n"                                   
			"fcvt.s.wu f8, x11, rne\n"                            
			"fmsub.s f6, f0, f5, f4, rtz\n"                       
			"fcmov.ps f8, f5, f0, f4\n"                           
			"feq.pi f7, f7, f2\n"                                 
			"fsra.pi f7, f4, f3\n"                                
			"feq.s x3, f1, f2\n"                                  
			"fsrli.pi f4, f1, 4\n"                                
			"fsub.pi f4, f5, f5\n"                                
			"fxor.pi f9, f1, f6\n"                                
			"fsgnjx.ps f7, f7, f3\n"                              
			"maskpopc x3, m1\n"                                   
			"fnmadd.ps f6, f6, f6, f1, rmm\n"                     
			"fcmov.ps f4, f2, f6, f7\n"                           
			"fmvs.x.ps x4, f6, 5\n"                               
			"maskor m1, m1, m0\n"                                 
			"fcvt.s.w f8, x11, rup\n"                             
			"masknot m2, m1\n"                                    
			"mov.m.x m2, x11, 76\n"                               
			"fround.ps f9, f4, dyn\n"                             
			"maskpopc x3, m1\n"                                   
			"fslli.pi f7, f2, 7\n"                                
			//RTLMIN-5479"frsq.ps f7, f5\n"                                    
			"fsw.ps f4, -334(x11)\n"                              
			"fsub.s f4, f1, f3, rdn\n"                            
			"fmvs.x.ps x4, f0, 6\n"                               
			"feq.pi f6, f5, f1\n"                                 
			"fsgnj.s f8, f2, f5\n"                                
			"fsrli.pi f8, f0, 4\n"                                
			"fltu.pi f4, f5, f7\n"                                
			"fcvt.ps.pwu f8, f4, rtz\n"                           
			"fsgnj.s f6, f1, f5\n"                                
			"fsgnjn.ps f7, f6, f5\n"                              
			//RTLMIN-5479"fsin.ps f8, f7\n"                                    
			"fmadd.ps f4, f2, f3, f7, rdn\n"                      
			"fsw f6, 532(x11)\n"                                  
			"fsetm.pi m1, f1\n"                                   
			"fpackrepb.pi f6, f2\n"                               
			"feq.ps f3, f3, f6\n"                                 
			//RTLMIN-5479"frsq.ps f5, f5\n"                                    
			"fsgnjn.ps f3, f4, f0\n"                              
			"fcmovm.ps f4, f5, f5\n"                              
			"fmax.s f7, f0, f7\n"                                 
			"fmaxu.pi f2, f1, f2\n"                               
			"fsrai.pi f6, f1, 4\n"                                
			"fbcx.ps f2, x11\n"                                   
			"fmax.pi f8, f5, f0\n"                                
			"fsra.pi f8, f4, f7\n"                                
			"fcvt.ps.f16 f7, f5\n"                                
			"fslli.pi f9, f0, 15\n"                               
			"fxor.pi f3, f4, f4\n"                                
			"frcp.ps f9, f7\n"                                    
			"fsw.ps f4, 1383(x11)\n"                              
			"fsgnjx.s f8, f7, f5\n"                               
			"fmax.s f2, f6, f1\n"                                 
			"flq2 f4, 1360(x11)\n"                                
			"fmadd.ps f5, f6, f3, f1, rne\n"                      

			"li x5, 0x32036173c1bdab78\n"
			"mova.m.x x5\n"                                       

			"li x5, 0xd84148c52e3c535a\n"
			"fscsr x5\n"
			"fmvs.x.ps x4, f0, 4\n"                               
			"flem.ps m0, f2, f3\n"                                
			"fsgnjn.s f4, f7, f2\n"                               
			"fcvt.s.wu f4, x11, dyn\n"                            
			"fle.s x4, f1, f6\n"                                  
			"fmax.s f6, f2, f6\n"                                 
			"fmul.ps f2, f4, f1, rmm\n"                           
			"fcvt.ps.f16 f4, f4\n"                                
			"fadd.pi f7, f6, f7\n"                                
			"fsub.s f7, f4, f5, rtz\n"                            
			"fsub.pi f5, f4, f3\n"                                
			"fcvt.ps.pwu f3, f6, rne\n"                           
			"fsgnjn.s f3, f2, f1\n"                               
			"fcvt.s.w f6, x11, rdn\n"                             
			"fminu.pi f6, f4, f1\n"                               
			"fadd.pi f2, f6, f5\n"                                
			"fslli.pi f5, f0, 3\n"                                
			//RTLMIN-5479"fsin.ps f4, f3\n"                                    
			"fsgnjn.ps f4, f7, f0\n"                              
			"fmsub.ps f7, f2, f7, f1, rtz\n"                      
			"fsw.ps f0, -396(x11)\n"                              
			"fcvt.pw.ps f2, f0, rup\n"                            
			"fmaxu.pi f2, f3, f7\n"                               
			"fmsub.s f9, f6, f3, f5, rtz\n"                       
			"fbcx.ps f5, x11\n"                                   
			"fnmsub.s f4, f7, f6, f5, rtz\n"                      
			"fmsub.s f2, f2, f4, f2, rdn\n"                       
			"fle.ps f4, f1, f3\n"                                 
			"fpackreph.pi f3, f2\n"                               
			"fswizz.ps f5, f2, 24\n"                              
			"fbc.ps f4, 1386(x11)\n"                              
			"feqm.ps m0, f3, f7\n"                                
			"fsgnjx.ps f7, f3, f0\n"                              
			"fmul.ps f6, f1, f5, rtz\n"                           
			"ffrc.ps f6, f1\n"                                    
			"fltm.ps m2, f0, f0\n"                                
			"fmax.s f8, f3, f4\n"                                 
			"fcvt.f16.ps f3, f1\n"                                
			"fxor.pi f7, f0, f2\n"                                

			"li x5, 0x5a017592d47710c0\n"
			"mova.m.x x5\n"                                       
			"flw f7, 171(x11)\n"                                  
			"flw.ps f3, 1746(x11)\n"                              
			"fnmsub.s f8, f3, f2, f3, rtz\n"                      
			"for.pi f3, f0, f7\n"                                 
			"fltu.pi f3, f7, f4\n"                                
			"fsgnjn.ps f4, f7, f1\n"                              
			"fsq2 f2, -1112(x11)\n"                               
			"fmsub.ps f6, f4, f2, f5, rmm\n"                      
			"fle.pi f4, f7, f1\n"                                 

			"li x5, 0xbe4e6f770b0aa4a4\n"
			"fscsr x5\n"
			"fmin.s f9, f0, f5\n"                                 
			"ffrc.ps f2, f0\n"                                    
			"fmvs.x.ps x3, f4, 0\n"                               
			"fcmovm.ps f6, f7, f5\n"                              
			"fmin.ps f2, f0, f0\n"                                
			"fsub.pi f2, f7, f0\n"                                
			"fmvs.x.ps x4, f6, 7\n"                               

			"li x5, 0xffffffff\n"
			"mova.m.x x5\n"

			"fsetm.pi m0, f0\n"                                   
			"fcvt.s.w f2, x11, rup\n"                             
			"fsrai.pi f9, f0, 6\n"                                
			"fcvt.pwu.ps f9, f5, rne\n"                           
			"fbc.ps f5, -788(x11)\n"                              
			"fcvt.f16.ps f2, f5\n"                                
			"flt.s x3, f3, f4\n"                                  
			"flt.ps f8, f0, f5\n"                                 
			"fsgnjx.s f3, f7, f3\n"                               
			"fmsub.ps f6, f3, f7, f0, dyn\n"                      
			"fmulhu.pi f3, f3, f0\n"                              
			"fsatu8.pi f9, f3\n"                                  
			"fcvt.ps.f16 f2, f2\n"                                
            

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
}
