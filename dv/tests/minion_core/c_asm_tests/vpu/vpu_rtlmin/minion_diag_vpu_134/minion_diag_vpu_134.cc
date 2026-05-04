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
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00008
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00010
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00014
			 0x80011111,                                                  // -9.7958E-41                                 /// 00018
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0001c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00020
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00028
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00030
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00038
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00048
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0004c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00054
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00058
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00060
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00068
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00078
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0007c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00080
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00084
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00088
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x3f028f5c,                                                  // 0.51                                        /// 00090
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00094
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000c4
			 0x55555555,                                                  // 4 random values                             /// 000c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f8
			 0x55555555,                                                  // 4 random values                             /// 000fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00100
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00108
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00114
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0011c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00120
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00128
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0012c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x4b000000,                                                  // 8388608.0                                   /// 00138
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x3f028f5c,                                                  // 0.51                                        /// 00140
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0c400000,                                                  // Leading 1s:                                 /// 00148
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0014c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00150
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00154
			 0x0c400000,                                                  // Leading 1s:                                 /// 00158
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00164
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0017c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x0c600000,                                                  // Leading 1s:                                 /// 00188
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00190
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00194
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001ac
			 0x00000000,                                                  // zero                                        /// 001b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001d0
			 0xcb000000,                                                  // -8388608.0                                  /// 001d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001e0
			 0x00000000,                                                  // zero                                        /// 001e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00200
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0020c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00210
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00214
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00218
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0021c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00220
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00224
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00228
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0022c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00234
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0023c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00244
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00248
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0024c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00254
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0025c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00260
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x3f028f5c,                                                  // 0.51                                        /// 00268
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0026c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00270
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00274
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0027c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00280
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00298
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0029c
			 0x3f028f5c,                                                  // 0.51                                        /// 002a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00300
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00304
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00308
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0030c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80000000,                                                  // -zero                                       /// 0031c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00324
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0032c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00338
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0033c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00340
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00344
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00354
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0035c
			 0x00000000,                                                  // zero                                        /// 00360
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00364
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00368
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0036c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00370
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00378
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0037c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x0c780000,                                                  // Leading 1s:                                 /// 00384
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0xff800000,                                                  // -inf                                        /// 0038c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00390
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a4
			 0xff800000,                                                  // -inf                                        /// 003a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d4
			 0x4b000000,                                                  // 8388608.0                                   /// 003d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x55555555,                                                  // 4 random values                             /// 003f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00400
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00408
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0040c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00410
			 0x00000000,                                                  // zero                                        /// 00414
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00418
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0041c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00420
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00424
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00434
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00438
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00440
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00444
			 0xbf028f5c,                                                  // -0.51                                       /// 00448
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0044c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00454
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0045c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00460
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00464
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00468
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0046c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00474
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00478
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0047c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00000000,                                                  // zero                                        /// 00484
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00488
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00490
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00494
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00498
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004e0
			 0x7fc00001,                                                  // signaling NaN                               /// 004e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00500
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00518
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00524
			 0x00000000,                                                  // zero                                        /// 00528
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0052c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x0e000007,                                                  // Trailing 1s:                                /// 00534
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0053c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00544
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00548
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e000003,                                                  // Trailing 1s:                                /// 00554
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0055c
			 0x55555555,                                                  // 4 random values                             /// 00560
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00568
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00574
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0057c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00580
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00588
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005bc
			 0x80000000,                                                  // -zero                                       /// 005c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005f8
			 0x4b000000,                                                  // 8388608.0                                   /// 005fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00600
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00608
			 0xff800000,                                                  // -inf                                        /// 0060c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00610
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00618
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00620
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00624
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00628
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0062c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00634
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0063c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00640
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00644
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0064c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00658
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0065c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00660
			 0x0c700000,                                                  // Leading 1s:                                 /// 00664
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0066c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00670
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00674
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00678
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00680
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00684
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0068c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00694
			 0x00011111,                                                  // 9.7958E-41                                  /// 00698
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c400000,                                                  // Leading 1s:                                 /// 00704
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00708
			 0x0e000003,                                                  // Trailing 1s:                                /// 0070c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x4b000000,                                                  // 8388608.0                                   /// 00718
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0071c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00724
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0072c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00730
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00734
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0073c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x7f800000,                                                  // inf                                         /// 00744
			 0x7f800000,                                                  // inf                                         /// 00748
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0074c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00750
			 0x0e000007,                                                  // Trailing 1s:                                /// 00754
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00758
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00760
			 0x0e000007,                                                  // Trailing 1s:                                /// 00764
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00768
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0076c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00770
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00774
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0077c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00780
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0e000003,                                                  // Trailing 1s:                                /// 00798
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80000000,                                                  // -zero                                       /// 007e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00800
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00804
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00808
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0080c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00810
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0081c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00828
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00834
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00838
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0c700000,                                                  // Leading 1s:                                 /// 00844
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00848
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0084c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00858
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0085c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00864
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00868
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00870
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00874
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00878
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0087c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x7fc00001,                                                  // signaling NaN                               /// 00888
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00890
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00894
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00898
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0089c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x80000000,                                                  // -zero                                       /// 008a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008cc
			 0x7fc00001,                                                  // signaling NaN                               /// 008d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008dc
			 0x00000000,                                                  // zero                                        /// 008e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00908
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00914
			 0x00011111,                                                  // 9.7958E-41                                  /// 00918
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0091c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00924
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00928
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0092c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00930
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00934
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00938
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0093c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00940
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00944
			 0xcb000000,                                                  // -8388608.0                                  /// 00948
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0094c
			 0x7f800000,                                                  // inf                                         /// 00950
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00954
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00958
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00960
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00964
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0096c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00974
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00978
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0097c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00980
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00984
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00988
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0098c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00990
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00994
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x55555555,                                                  // 4 random values                             /// 009a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009b0
			 0xffc00001,                                                  // -signaling NaN                              /// 009b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f4
			 0x55555555,                                                  // 4 random values                             /// 009f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a28
			 0xcb000000,                                                  // -8388608.0                                  /// 00a2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x7f800000,                                                  // inf                                         /// 00a54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x55555555,                                                  // 4 random values                             /// 00a6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a70
			 0x7f800000,                                                  // inf                                         /// 00a74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a78
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00aac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ab4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ab8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ac0
			 0x80000000,                                                  // -zero                                       /// 00ac4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ad4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00adc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00af0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00af4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00af8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00afc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b28
			 0x7fc00001,                                                  // signaling NaN                               /// 00b2c
			 0x55555555,                                                  // 4 random values                             /// 00b30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x55555555,                                                  // 4 random values                             /// 00b70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b88
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0xbf028f5c,                                                  // -0.51                                       /// 00b94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bc4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bf4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bf8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bfc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c04
			 0xff800000,                                                  // -inf                                        /// 00c08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x3f028f5c,                                                  // 0.51                                        /// 00c2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x55555555,                                                  // 4 random values                             /// 00c9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x33333333,                                                  // 4 random values                             /// 00ca4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x4b000000,                                                  // 8388608.0                                   /// 00d04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d18
			 0x7fc00001,                                                  // signaling NaN                               /// 00d1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d2c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00da0
			 0x3f028f5c,                                                  // 0.51                                        /// 00da4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x55555555,                                                  // 4 random values                             /// 00dac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00db4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dd8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ddc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x7fc00001,                                                  // signaling NaN                               /// 00de8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00df8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e30
			 0xff800000,                                                  // -inf                                        /// 00e34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e6c
			 0x7f800000,                                                  // inf                                         /// 00e70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xbf028f5c,                                                  // -0.51                                       /// 00e9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ebc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ec0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ecc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ed0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ef8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x80000000,                                                  // -zero                                       /// 00f00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f08
			 0x00000000,                                                  // zero                                        /// 00f0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f50
			 0x3f028f5c,                                                  // 0.51                                        /// 00f54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f58
			 0x33333333,                                                  // 4 random values                             /// 00f5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f64
			 0xbf028f5c,                                                  // -0.51                                       /// 00f68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f9c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fe4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ff0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0xffffffff // QNan                                          // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00000
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00004
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0000c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00010
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00018
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0001c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00020
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00024
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00034
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00038
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00040
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x4b000000,                                                  // 8388608.0                                   /// 00048
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00050
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00058
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0005c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0006c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00074
			 0x80011111,                                                  // -9.7958E-41                                 /// 00078
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00088
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00090
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x4b000000,                                                  // 8388608.0                                   /// 000c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xbf028f5c,                                                  // -0.51                                       /// 000d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000e0
			 0x80000000,                                                  // -zero                                       /// 000e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00104
			 0x55555555,                                                  // 4 random values                             /// 00108
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0010c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00114
			 0x80011111,                                                  // -9.7958E-41                                 /// 00118
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00120
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00128
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0012c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00130
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00138
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e000003,                                                  // Trailing 1s:                                /// 00144
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00148
			 0x7fc00001,                                                  // signaling NaN                               /// 0014c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00150
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00154
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00158
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0015c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00160
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00164
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00168
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0016c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00170
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00178
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0017c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00180
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0018c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00190
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x80000000,                                                  // -zero                                       /// 00198
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00000000,                                                  // zero                                        /// 001a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00200
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00204
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00208
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0020c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00210
			 0x7f800000,                                                  // inf                                         /// 00214
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00218
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00220
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00224
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00228
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00234
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00240
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00264
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80000000,                                                  // -zero                                       /// 00270
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00274
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0c600000,                                                  // Leading 1s:                                 /// 0027c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00280
			 0xbf028f5c,                                                  // -0.51                                       /// 00284
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00288
			 0x80000000,                                                  // -zero                                       /// 0028c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00290
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x7fc00001,                                                  // signaling NaN                               /// 002a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002c4
			 0x33333333,                                                  // 4 random values                             /// 002c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00300
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0030c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00310
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00314
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00318
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0031c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00320
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00324
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00328
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00330
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00334
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00338
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0033c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0c400000,                                                  // Leading 1s:                                 /// 00344
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00350
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00354
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00358
			 0x80011111,                                                  // -9.7958E-41                                 /// 0035c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00364
			 0x0c700000,                                                  // Leading 1s:                                 /// 00368
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00374
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00378
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0037c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0038c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0e000001,                                                  // Trailing 1s:                                /// 00394
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00000000,                                                  // zero                                        /// 0039c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003b0
			 0x7fc00001,                                                  // signaling NaN                               /// 003b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003e8
			 0x33333333,                                                  // 4 random values                             /// 003ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00408
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0040c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00410
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00418
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0041c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00420
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00424
			 0xff800000,                                                  // -inf                                        /// 00428
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0042c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00430
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00434
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00444
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0044c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00450
			 0x0e000003,                                                  // Trailing 1s:                                /// 00454
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00458
			 0x33333333,                                                  // 4 random values                             /// 0045c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00470
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00474
			 0x7fc00001,                                                  // signaling NaN                               /// 00478
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00488
			 0x0e000007,                                                  // Trailing 1s:                                /// 0048c
			 0x33333333,                                                  // 4 random values                             /// 00490
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00498
			 0x0c780000,                                                  // Leading 1s:                                 /// 0049c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 004b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00504
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00508
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00510
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00514
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00518
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0051c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00520
			 0x7fc00001,                                                  // signaling NaN                               /// 00524
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0052c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00548
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00550
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00554
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00560
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00568
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00570
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00574
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0057c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0058c
			 0xffc00001,                                                  // -signaling NaN                              /// 00590
			 0x0e000007,                                                  // Trailing 1s:                                /// 00594
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00598
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0059c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x7fc00001,                                                  // signaling NaN                               /// 005cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0xbf028f5c,                                                  // -0.51                                       /// 005f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00600
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00604
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00608
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00610
			 0x4b000000,                                                  // 8388608.0                                   /// 00614
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00618
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0061c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00620
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00624
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0062c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00630
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00634
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00638
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0063c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00640
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00644
			 0x55555555,                                                  // 4 random values                             /// 00648
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00654
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00658
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00660
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x80000000,                                                  // -zero                                       /// 0066c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0c780000,                                                  // Leading 1s:                                 /// 00674
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00678
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00680
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00684
			 0x0e000007,                                                  // Trailing 1s:                                /// 00688
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0068c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00690
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00694
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00698
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xff800000,                                                  // -inf                                        /// 006b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d0
			 0x7fc00001,                                                  // signaling NaN                               /// 006d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 006f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00700
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00704
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00714
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00718
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00724
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00728
			 0x7fc00001,                                                  // signaling NaN                               /// 0072c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0c780000,                                                  // Leading 1s:                                 /// 00734
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00738
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00744
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00748
			 0x0e000007,                                                  // Trailing 1s:                                /// 0074c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00750
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00754
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00758
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00760
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00764
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00768
			 0x80000000,                                                  // -zero                                       /// 0076c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00770
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00774
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00780
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00788
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00790
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00794
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0xffc00001,                                                  // -signaling NaN                              /// 007a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 007b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007dc
			 0xbf028f5c,                                                  // -0.51                                       /// 007e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00800
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00804
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00808
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80000000,                                                  // -zero                                       /// 00810
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00814
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0081c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00820
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00828
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00830
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00834
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0083c
			 0x3f028f5c,                                                  // 0.51                                        /// 00840
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00844
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00848
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00850
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00854
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00858
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00860
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00864
			 0x3f028f5c,                                                  // 0.51                                        /// 00868
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0086c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00870
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00874
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7fc00001,                                                  // signaling NaN                               /// 00880
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00884
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00888
			 0x55555555,                                                  // 4 random values                             /// 0088c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00890
			 0xff800000,                                                  // -inf                                        /// 00894
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0089c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x7fc00001,                                                  // signaling NaN                               /// 008d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x7f800000,                                                  // inf                                         /// 008ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00904
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0090c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00910
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00920
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00924
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00928
			 0x55555555,                                                  // 4 random values                             /// 0092c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00930
			 0x55555555,                                                  // 4 random values                             /// 00934
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00938
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0093c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00940
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00944
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00011111,                                                  // 9.7958E-41                                  /// 0094c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00950
			 0x0c780000,                                                  // Leading 1s:                                 /// 00954
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0095c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0e000001,                                                  // Trailing 1s:                                /// 00964
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0e000003,                                                  // Trailing 1s:                                /// 0096c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00974
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00978
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00984
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00988
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00990
			 0x80011111,                                                  // -9.7958E-41                                 /// 00994
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00998
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0099c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009a4
			 0xcb000000,                                                  // -8388608.0                                  /// 009a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b4
			 0x00000000,                                                  // zero                                        /// 009b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x7fc00001,                                                  // signaling NaN                               /// 009d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a10
			 0x55555555,                                                  // 4 random values                             /// 00a14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a30
			 0xffc00001,                                                  // -signaling NaN                              /// 00a34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aa0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aa4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aa8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00aac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ab8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ac0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ac8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00acc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ad0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00adc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ae0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00af4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00afc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b0c
			 0x7f800000,                                                  // inf                                         /// 00b10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b38
			 0xbf028f5c,                                                  // -0.51                                       /// 00b3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b54
			 0x3f028f5c,                                                  // 0.51                                        /// 00b58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b90
			 0x33333333,                                                  // 4 random values                             /// 00b94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b9c
			 0x80000000,                                                  // -zero                                       /// 00ba0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ba8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bcc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bdc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bf0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c10
			 0xff800000,                                                  // -inf                                        /// 00c14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ccc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ce0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ce8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d2c
			 0x00000000,                                                  // zero                                        /// 00d30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d34
			 0xcb000000,                                                  // -8388608.0                                  /// 00d38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x7f800000,                                                  // inf                                         /// 00d50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da0
			 0x33333333,                                                  // 4 random values                             /// 00da4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00db0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00db4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dcc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00de0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00de8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e04
			 0x4b000000,                                                  // 8388608.0                                   /// 00e08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x55555555,                                                  // 4 random values                             /// 00e34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e90
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ea4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ebc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ec8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ecc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ed0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ee0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ee4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80000000,                                                  // -zero                                       /// 00ef4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef8
			 0x7f800000,                                                  // inf                                         /// 00efc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fa4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fa8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fbc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80000000,                                                  // -zero                                       /// 00fd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fdc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fe4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fe8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ff0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ff4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x8f7fffff                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x572f2f98, /// 0x0
			 0x249db186, /// 0x4
			 0x5ab6a7fd, /// 0x8
			 0x1fdb325f, /// 0xc
			 0xf3fb93bb, /// 0x10
			 0x127c830f, /// 0x14
			 0x21411612, /// 0x18
			 0xc8f6d77a, /// 0x1c
			 0x851b1ee8, /// 0x20
			 0xfc7cb66a, /// 0x24
			 0x34d9db3b, /// 0x28
			 0x384fcfb6, /// 0x2c
			 0xce80904c, /// 0x30
			 0xc46691b8, /// 0x34
			 0x44580cce, /// 0x38
			 0x4b09c9b4, /// 0x3c
			 0xe6768a2a, /// 0x40
			 0x0ce2b690, /// 0x44
			 0x49262828, /// 0x48
			 0xee0a021c, /// 0x4c
			 0xaef202ff, /// 0x50
			 0xf5ba491b, /// 0x54
			 0xf41fa1ad, /// 0x58
			 0xb9f1d803, /// 0x5c
			 0xbe0b7fd1, /// 0x60
			 0xd7fae816, /// 0x64
			 0xbc5ca433, /// 0x68
			 0x98f44a5d, /// 0x6c
			 0xa80ee67c, /// 0x70
			 0x7214096d, /// 0x74
			 0xcf927eef, /// 0x78
			 0x86de4b84, /// 0x7c
			 0xe19270e7, /// 0x80
			 0x61cbfe17, /// 0x84
			 0x96bcda19, /// 0x88
			 0x4ac017c7, /// 0x8c
			 0x7586fc25, /// 0x90
			 0xddda944c, /// 0x94
			 0x20f17ef6, /// 0x98
			 0x48daf654, /// 0x9c
			 0x2d7b92f9, /// 0xa0
			 0xf096e96e, /// 0xa4
			 0x3d1592e5, /// 0xa8
			 0xa5533665, /// 0xac
			 0x8bc67eed, /// 0xb0
			 0x3a98357b, /// 0xb4
			 0xdc02e050, /// 0xb8
			 0x25e66f7d, /// 0xbc
			 0x606cfbf0, /// 0xc0
			 0x7a88f5e2, /// 0xc4
			 0x9d5b36a9, /// 0xc8
			 0xa4dc079f, /// 0xcc
			 0x7a5c156f, /// 0xd0
			 0xf183e7f2, /// 0xd4
			 0x40ac46ac, /// 0xd8
			 0x73a0e4ec, /// 0xdc
			 0x2d2946c3, /// 0xe0
			 0x7d5f9e2f, /// 0xe4
			 0x239fc76b, /// 0xe8
			 0x38b6e8e7, /// 0xec
			 0x3d5b7038, /// 0xf0
			 0xf6c8df9c, /// 0xf4
			 0x7b5ed897, /// 0xf8
			 0xf95e13d9, /// 0xfc
			 0x26b40171, /// 0x100
			 0xc99b7504, /// 0x104
			 0x027ef639, /// 0x108
			 0xb6f4ddb0, /// 0x10c
			 0xe7ce90db, /// 0x110
			 0xde5dcbad, /// 0x114
			 0x40db75b1, /// 0x118
			 0x6ba61ea3, /// 0x11c
			 0xcad2517c, /// 0x120
			 0x051c9ff9, /// 0x124
			 0xc62390fe, /// 0x128
			 0x359c9168, /// 0x12c
			 0xb00207c2, /// 0x130
			 0xb996b932, /// 0x134
			 0x202d2c2c, /// 0x138
			 0x0ad1bdef, /// 0x13c
			 0xffbf6a5b, /// 0x140
			 0x982a509e, /// 0x144
			 0x92289403, /// 0x148
			 0xaccc43cc, /// 0x14c
			 0xed54124b, /// 0x150
			 0xb76575e3, /// 0x154
			 0x6c3aa8f0, /// 0x158
			 0xecb665f0, /// 0x15c
			 0x2c7f381a, /// 0x160
			 0x34cf6173, /// 0x164
			 0x0063147b, /// 0x168
			 0x7c567d8a, /// 0x16c
			 0x5631ccb7, /// 0x170
			 0x3e02e2a5, /// 0x174
			 0x059a32f3, /// 0x178
			 0x5ec41faf, /// 0x17c
			 0xa0c62b8f, /// 0x180
			 0x34483802, /// 0x184
			 0xbf3dfd56, /// 0x188
			 0x320eb6b4, /// 0x18c
			 0xbd8fdb89, /// 0x190
			 0x42739c97, /// 0x194
			 0xf44afce6, /// 0x198
			 0x85771c5c, /// 0x19c
			 0x626f4de7, /// 0x1a0
			 0xa8ee4da4, /// 0x1a4
			 0x50f81d2e, /// 0x1a8
			 0x4be7fce0, /// 0x1ac
			 0xd52a9b60, /// 0x1b0
			 0x0b478a7e, /// 0x1b4
			 0x1f9451a0, /// 0x1b8
			 0xc206d5e3, /// 0x1bc
			 0xc0829c4d, /// 0x1c0
			 0xb07d3e0b, /// 0x1c4
			 0xb3b1ee3f, /// 0x1c8
			 0x8b8706fd, /// 0x1cc
			 0xe22064ec, /// 0x1d0
			 0x1d974c67, /// 0x1d4
			 0xb535b99b, /// 0x1d8
			 0x32f14942, /// 0x1dc
			 0x75f77b61, /// 0x1e0
			 0x1444fac2, /// 0x1e4
			 0x6ff8f2c5, /// 0x1e8
			 0xf6592933, /// 0x1ec
			 0xdd799111, /// 0x1f0
			 0xc3dd11f1, /// 0x1f4
			 0x52a1a124, /// 0x1f8
			 0x205168d8, /// 0x1fc
			 0xfca41186, /// 0x200
			 0x3b128aa3, /// 0x204
			 0xf61d3223, /// 0x208
			 0xb875817f, /// 0x20c
			 0xbf39c7c9, /// 0x210
			 0x6b8518d8, /// 0x214
			 0xbec95dde, /// 0x218
			 0xc650599b, /// 0x21c
			 0x6880b920, /// 0x220
			 0x5890b512, /// 0x224
			 0xc1d99c47, /// 0x228
			 0x8454fda4, /// 0x22c
			 0xfed23f95, /// 0x230
			 0x0c09fdb0, /// 0x234
			 0xd4c0524a, /// 0x238
			 0x0dd167b4, /// 0x23c
			 0xd0c9e675, /// 0x240
			 0xa7fe8ce0, /// 0x244
			 0x26c7817a, /// 0x248
			 0x6cbc80a1, /// 0x24c
			 0xdaeda095, /// 0x250
			 0xe9342336, /// 0x254
			 0xcb93ec35, /// 0x258
			 0xd745be20, /// 0x25c
			 0xc7b9dfac, /// 0x260
			 0x8f12a815, /// 0x264
			 0x2eafc9dc, /// 0x268
			 0xebd0f7e2, /// 0x26c
			 0xc25aaa39, /// 0x270
			 0x8b1466db, /// 0x274
			 0x5f4804b7, /// 0x278
			 0x958eafff, /// 0x27c
			 0x77a1a594, /// 0x280
			 0x53a93bc7, /// 0x284
			 0x39998ec4, /// 0x288
			 0xf8974ed3, /// 0x28c
			 0xb90e3d39, /// 0x290
			 0x6e4d0f05, /// 0x294
			 0xffa2fd9a, /// 0x298
			 0x7b072740, /// 0x29c
			 0x060501ac, /// 0x2a0
			 0xbc5f3ee8, /// 0x2a4
			 0x2391ca9d, /// 0x2a8
			 0x786e8c07, /// 0x2ac
			 0xc2cabb7a, /// 0x2b0
			 0xe81d0c20, /// 0x2b4
			 0xa1a13cbe, /// 0x2b8
			 0xcf1c839c, /// 0x2bc
			 0x65f1a23e, /// 0x2c0
			 0x5f0f4ef7, /// 0x2c4
			 0x58814962, /// 0x2c8
			 0x46698d3a, /// 0x2cc
			 0x453b6804, /// 0x2d0
			 0xce995e33, /// 0x2d4
			 0x62d5e3ac, /// 0x2d8
			 0xc389afcc, /// 0x2dc
			 0x9fa6786d, /// 0x2e0
			 0x403cbaec, /// 0x2e4
			 0xa8965615, /// 0x2e8
			 0xa012b50e, /// 0x2ec
			 0x195b6864, /// 0x2f0
			 0xa858d9a5, /// 0x2f4
			 0x76e87d92, /// 0x2f8
			 0x0aad5af2, /// 0x2fc
			 0xa240262f, /// 0x300
			 0xc654db9a, /// 0x304
			 0x5f9ceba9, /// 0x308
			 0xfc90eda7, /// 0x30c
			 0xd0994dc1, /// 0x310
			 0x9d792715, /// 0x314
			 0x01e1ea3a, /// 0x318
			 0xfa0927d0, /// 0x31c
			 0x129d9129, /// 0x320
			 0xf4abd459, /// 0x324
			 0x24551beb, /// 0x328
			 0xfc053973, /// 0x32c
			 0xe4c75f00, /// 0x330
			 0x07c8658e, /// 0x334
			 0xcf812aac, /// 0x338
			 0x9de43d55, /// 0x33c
			 0x851c375a, /// 0x340
			 0x6d32707f, /// 0x344
			 0xd88cee3c, /// 0x348
			 0x0fd245d5, /// 0x34c
			 0x1be4d7e3, /// 0x350
			 0xa13e9cb9, /// 0x354
			 0xb5ab51b3, /// 0x358
			 0x701c1924, /// 0x35c
			 0x839cb121, /// 0x360
			 0x3ad6c8e6, /// 0x364
			 0xfe7ddee7, /// 0x368
			 0x8428c9d5, /// 0x36c
			 0x50a33d48, /// 0x370
			 0xa1cfcb21, /// 0x374
			 0xbdfce738, /// 0x378
			 0x8b940d7c, /// 0x37c
			 0x1284fc22, /// 0x380
			 0xc477f25b, /// 0x384
			 0x50c3a07d, /// 0x388
			 0xaeca5764, /// 0x38c
			 0x8e917825, /// 0x390
			 0xd194e325, /// 0x394
			 0x9b4441e8, /// 0x398
			 0xaa9cb758, /// 0x39c
			 0xb74cc74c, /// 0x3a0
			 0x7cf68924, /// 0x3a4
			 0xd5afe223, /// 0x3a8
			 0xbffe744e, /// 0x3ac
			 0x760332a6, /// 0x3b0
			 0x3233700f, /// 0x3b4
			 0x84834012, /// 0x3b8
			 0x64bc1caa, /// 0x3bc
			 0x314ac636, /// 0x3c0
			 0x6be0f369, /// 0x3c4
			 0x3d400bb6, /// 0x3c8
			 0x4f64025c, /// 0x3cc
			 0x7eb85d16, /// 0x3d0
			 0xa7a552f6, /// 0x3d4
			 0xa2cffaac, /// 0x3d8
			 0x347e30d1, /// 0x3dc
			 0xaaf7fe5d, /// 0x3e0
			 0x022f0c82, /// 0x3e4
			 0xec240004, /// 0x3e8
			 0xd7892be2, /// 0x3ec
			 0xb5612ca9, /// 0x3f0
			 0xcd89e2de, /// 0x3f4
			 0x38137ff0, /// 0x3f8
			 0x9e484a1f, /// 0x3fc
			 0x871e28a6, /// 0x400
			 0x87520af1, /// 0x404
			 0x190bbb7a, /// 0x408
			 0x08306993, /// 0x40c
			 0x9fbe5680, /// 0x410
			 0x77fb9cfb, /// 0x414
			 0x9d549a71, /// 0x418
			 0x0102cca7, /// 0x41c
			 0x2b9f3fff, /// 0x420
			 0x415b3c57, /// 0x424
			 0x183bbd43, /// 0x428
			 0x3a9853fe, /// 0x42c
			 0xca74f0be, /// 0x430
			 0x5f0d4a95, /// 0x434
			 0xe8d6a9a8, /// 0x438
			 0xeec9b563, /// 0x43c
			 0x8baeea5c, /// 0x440
			 0xf7878990, /// 0x444
			 0x5e6ccd23, /// 0x448
			 0x7d5a2614, /// 0x44c
			 0xf28ac51c, /// 0x450
			 0xee02c99c, /// 0x454
			 0x5598d43b, /// 0x458
			 0xc4087f76, /// 0x45c
			 0x25e51673, /// 0x460
			 0x1c2cb030, /// 0x464
			 0x7e3d4ab1, /// 0x468
			 0x0ef19f54, /// 0x46c
			 0x7b37f128, /// 0x470
			 0x0163f654, /// 0x474
			 0x31af4601, /// 0x478
			 0x909c3556, /// 0x47c
			 0x069dddbd, /// 0x480
			 0x48e9653b, /// 0x484
			 0xf8b54acc, /// 0x488
			 0x744788cd, /// 0x48c
			 0x8aa806dd, /// 0x490
			 0x4867d087, /// 0x494
			 0xe5dbc77a, /// 0x498
			 0x08b103f1, /// 0x49c
			 0xca74fb50, /// 0x4a0
			 0xd40bbed0, /// 0x4a4
			 0x4a61fbef, /// 0x4a8
			 0x10f3f8da, /// 0x4ac
			 0xa8880ba1, /// 0x4b0
			 0x112d3d07, /// 0x4b4
			 0x243d7f32, /// 0x4b8
			 0x87d3859e, /// 0x4bc
			 0x95c8d33e, /// 0x4c0
			 0xbe5e5678, /// 0x4c4
			 0xc5acdb79, /// 0x4c8
			 0xaf69e420, /// 0x4cc
			 0x5932434f, /// 0x4d0
			 0xa837851f, /// 0x4d4
			 0xeb15fa29, /// 0x4d8
			 0xf5a3e355, /// 0x4dc
			 0xf6fc60fc, /// 0x4e0
			 0x93529e1b, /// 0x4e4
			 0x66d7f3b0, /// 0x4e8
			 0xbb42e8b4, /// 0x4ec
			 0xa61deaf0, /// 0x4f0
			 0x462fea70, /// 0x4f4
			 0x435b5649, /// 0x4f8
			 0x4f7e976f, /// 0x4fc
			 0x64ad5aae, /// 0x500
			 0x7077712a, /// 0x504
			 0xd0fc94e0, /// 0x508
			 0x1e816a5c, /// 0x50c
			 0x301bbbb7, /// 0x510
			 0x84d933fc, /// 0x514
			 0xd06afdf9, /// 0x518
			 0x32372737, /// 0x51c
			 0x5f954006, /// 0x520
			 0x3de85a54, /// 0x524
			 0xd627c2e8, /// 0x528
			 0x90855fba, /// 0x52c
			 0xdfffdfdb, /// 0x530
			 0x2f29b94a, /// 0x534
			 0xf31c3156, /// 0x538
			 0x97938dfd, /// 0x53c
			 0x2afe4c70, /// 0x540
			 0x5284227d, /// 0x544
			 0x87db6dea, /// 0x548
			 0xae56cdd5, /// 0x54c
			 0xd6a7e5a3, /// 0x550
			 0xdcc6a160, /// 0x554
			 0xde63cf99, /// 0x558
			 0xba664eb6, /// 0x55c
			 0xf6e53d31, /// 0x560
			 0x0f2f75a4, /// 0x564
			 0x6151be1a, /// 0x568
			 0x82900158, /// 0x56c
			 0xa1ab9c43, /// 0x570
			 0x3d9bc433, /// 0x574
			 0x1adfc447, /// 0x578
			 0xffbc133e, /// 0x57c
			 0x8b4b4d9b, /// 0x580
			 0x7df35e72, /// 0x584
			 0x598679fe, /// 0x588
			 0x6f36c989, /// 0x58c
			 0x8c9ee94b, /// 0x590
			 0xb5f49a8e, /// 0x594
			 0xa90334bf, /// 0x598
			 0xb2dfd701, /// 0x59c
			 0x4cf50caf, /// 0x5a0
			 0xdf788a2f, /// 0x5a4
			 0xc3cf7930, /// 0x5a8
			 0xaa24c201, /// 0x5ac
			 0x00fea022, /// 0x5b0
			 0x430287c1, /// 0x5b4
			 0xbe83256c, /// 0x5b8
			 0xbc41466e, /// 0x5bc
			 0xde1187d9, /// 0x5c0
			 0xc6beb0ce, /// 0x5c4
			 0xc3cd9f7f, /// 0x5c8
			 0x092ae3ca, /// 0x5cc
			 0xacc7f0cf, /// 0x5d0
			 0x41a66f98, /// 0x5d4
			 0xbb0ec690, /// 0x5d8
			 0x5906db14, /// 0x5dc
			 0x76efb8d3, /// 0x5e0
			 0x76d83ef3, /// 0x5e4
			 0xcb0196be, /// 0x5e8
			 0x8ad6b089, /// 0x5ec
			 0x4f5c1997, /// 0x5f0
			 0x326ffa6b, /// 0x5f4
			 0x56d09fbc, /// 0x5f8
			 0xc03b1dd1, /// 0x5fc
			 0xd2f2e939, /// 0x600
			 0xb40f7bc4, /// 0x604
			 0x96c8f923, /// 0x608
			 0x7877be09, /// 0x60c
			 0xcd47c9ce, /// 0x610
			 0x43b1cc10, /// 0x614
			 0x6aa6b7c4, /// 0x618
			 0x77e4a13e, /// 0x61c
			 0xd12b135e, /// 0x620
			 0xf59c6247, /// 0x624
			 0x4f5e50df, /// 0x628
			 0x039c5181, /// 0x62c
			 0xf6039ba0, /// 0x630
			 0xb74cee05, /// 0x634
			 0x35b25206, /// 0x638
			 0x607d7e4d, /// 0x63c
			 0xd89028c0, /// 0x640
			 0xfd394e1d, /// 0x644
			 0x3d486c5c, /// 0x648
			 0x6f291989, /// 0x64c
			 0x4aff451f, /// 0x650
			 0x9ed11e54, /// 0x654
			 0xac528346, /// 0x658
			 0x282e33bf, /// 0x65c
			 0x75ac7e80, /// 0x660
			 0x50c95b72, /// 0x664
			 0x6bb77fb4, /// 0x668
			 0x99a8cffd, /// 0x66c
			 0xb4b2b309, /// 0x670
			 0x1a9a137e, /// 0x674
			 0xf3012e51, /// 0x678
			 0x13c16cc7, /// 0x67c
			 0x1f30819e, /// 0x680
			 0xa5f10ff6, /// 0x684
			 0x448a30b9, /// 0x688
			 0x85f1d247, /// 0x68c
			 0x3d95afd8, /// 0x690
			 0x73526707, /// 0x694
			 0xdc844d16, /// 0x698
			 0x5fe7532e, /// 0x69c
			 0x9a746670, /// 0x6a0
			 0xbb13904d, /// 0x6a4
			 0x261b1d7d, /// 0x6a8
			 0xc630ddb7, /// 0x6ac
			 0x60f5025c, /// 0x6b0
			 0x48318d2f, /// 0x6b4
			 0x6bce0464, /// 0x6b8
			 0xa334b1b1, /// 0x6bc
			 0x55d5e80d, /// 0x6c0
			 0xc685c161, /// 0x6c4
			 0x612f43ac, /// 0x6c8
			 0x5a586987, /// 0x6cc
			 0x6493263b, /// 0x6d0
			 0x4c34e50e, /// 0x6d4
			 0x73599af4, /// 0x6d8
			 0x7b435820, /// 0x6dc
			 0xf2823abb, /// 0x6e0
			 0x58f1342a, /// 0x6e4
			 0x3296c600, /// 0x6e8
			 0xf93992e8, /// 0x6ec
			 0x2b69d162, /// 0x6f0
			 0x5ff18e32, /// 0x6f4
			 0x0ea587bb, /// 0x6f8
			 0xc0242634, /// 0x6fc
			 0x3eed3cb0, /// 0x700
			 0x4b575af3, /// 0x704
			 0x24d5a5ab, /// 0x708
			 0x02a273ae, /// 0x70c
			 0xb4799abd, /// 0x710
			 0x48983b22, /// 0x714
			 0xe8634dde, /// 0x718
			 0x4958ba5b, /// 0x71c
			 0xd3dfa7bc, /// 0x720
			 0x59bc9d9d, /// 0x724
			 0x606a3c2e, /// 0x728
			 0x6d24644f, /// 0x72c
			 0x3733540d, /// 0x730
			 0xc2b70867, /// 0x734
			 0xf602c8eb, /// 0x738
			 0xec1fd761, /// 0x73c
			 0x74b8f4f9, /// 0x740
			 0xd0bcb7e0, /// 0x744
			 0xa9a60656, /// 0x748
			 0x81386bf1, /// 0x74c
			 0xa4f67f5a, /// 0x750
			 0x40307e85, /// 0x754
			 0x94ab33ca, /// 0x758
			 0xf2280b15, /// 0x75c
			 0x6c93f0cd, /// 0x760
			 0x56a9c97f, /// 0x764
			 0x99611fd6, /// 0x768
			 0x20fe45ed, /// 0x76c
			 0x162c43fd, /// 0x770
			 0x4ca175f1, /// 0x774
			 0x8895e47d, /// 0x778
			 0x16b2b876, /// 0x77c
			 0xc44b3d95, /// 0x780
			 0xdb8827d0, /// 0x784
			 0xf332e1cb, /// 0x788
			 0x2eaa86f7, /// 0x78c
			 0x368d92f1, /// 0x790
			 0x1074e4ce, /// 0x794
			 0x3ff91a20, /// 0x798
			 0x86bdf4d6, /// 0x79c
			 0x934f795a, /// 0x7a0
			 0x11bd35d4, /// 0x7a4
			 0x15f3c1dd, /// 0x7a8
			 0xfa5c9838, /// 0x7ac
			 0x3ea7021d, /// 0x7b0
			 0xdb68e72a, /// 0x7b4
			 0x362d3885, /// 0x7b8
			 0x36d9faf7, /// 0x7bc
			 0x23b333b9, /// 0x7c0
			 0xff211bc6, /// 0x7c4
			 0x660fea12, /// 0x7c8
			 0x08719cbc, /// 0x7cc
			 0x974f3d2d, /// 0x7d0
			 0x7b107f55, /// 0x7d4
			 0xc5cbefd9, /// 0x7d8
			 0x07e3b525, /// 0x7dc
			 0x842c0c18, /// 0x7e0
			 0x2f786b14, /// 0x7e4
			 0xae943352, /// 0x7e8
			 0xe62daa8c, /// 0x7ec
			 0x5bb65210, /// 0x7f0
			 0xcd1e2f5f, /// 0x7f4
			 0xcb18bb71, /// 0x7f8
			 0xac5f764e, /// 0x7fc
			 0xb87155fc, /// 0x800
			 0xa4e19761, /// 0x804
			 0x3ad9eb4d, /// 0x808
			 0xefa658ad, /// 0x80c
			 0x13090e21, /// 0x810
			 0x5b221982, /// 0x814
			 0x5e3c1c9d, /// 0x818
			 0xd84e6bb0, /// 0x81c
			 0xfb0abe5a, /// 0x820
			 0x7b2d37bd, /// 0x824
			 0x088de33e, /// 0x828
			 0x557649a9, /// 0x82c
			 0x6becf5ec, /// 0x830
			 0x303614d8, /// 0x834
			 0x8f8dd494, /// 0x838
			 0xcdc5db66, /// 0x83c
			 0x07d19fc4, /// 0x840
			 0x46fe01bc, /// 0x844
			 0xceead925, /// 0x848
			 0x5ac91aad, /// 0x84c
			 0x673ca1ff, /// 0x850
			 0x916fc493, /// 0x854
			 0x6a1d8127, /// 0x858
			 0x921f2514, /// 0x85c
			 0x50046f23, /// 0x860
			 0x5dea1f0b, /// 0x864
			 0x68fa4ed9, /// 0x868
			 0xf55ced3d, /// 0x86c
			 0xbcfeb03e, /// 0x870
			 0xf7097f60, /// 0x874
			 0x9524cb56, /// 0x878
			 0x4e71c341, /// 0x87c
			 0xf78b6c5c, /// 0x880
			 0xca306141, /// 0x884
			 0xfd3fa48e, /// 0x888
			 0xb6ea5620, /// 0x88c
			 0x26997ad8, /// 0x890
			 0x9958d1eb, /// 0x894
			 0x11e0098e, /// 0x898
			 0x95f04e33, /// 0x89c
			 0x4b1a6be7, /// 0x8a0
			 0xb1681b89, /// 0x8a4
			 0x62a57036, /// 0x8a8
			 0x06b03e97, /// 0x8ac
			 0x66b37abd, /// 0x8b0
			 0xbc475f0d, /// 0x8b4
			 0x61d7faa3, /// 0x8b8
			 0x5d5d12ab, /// 0x8bc
			 0x84497b8f, /// 0x8c0
			 0xb9d7b095, /// 0x8c4
			 0xf102b76d, /// 0x8c8
			 0xefc709b9, /// 0x8cc
			 0xcf82cfd1, /// 0x8d0
			 0xf4619cc2, /// 0x8d4
			 0x32f0fadd, /// 0x8d8
			 0xeacb4767, /// 0x8dc
			 0x03d440e4, /// 0x8e0
			 0x7f605306, /// 0x8e4
			 0xb761005f, /// 0x8e8
			 0xcc0df753, /// 0x8ec
			 0x5f4415b1, /// 0x8f0
			 0xc093168e, /// 0x8f4
			 0x89f3e424, /// 0x8f8
			 0x6d123730, /// 0x8fc
			 0x273d1aa9, /// 0x900
			 0x35b73c3a, /// 0x904
			 0xd617a796, /// 0x908
			 0x4499be34, /// 0x90c
			 0xbb1357bc, /// 0x910
			 0x6eaefbcd, /// 0x914
			 0x3408a0a8, /// 0x918
			 0x35fb9fb6, /// 0x91c
			 0x310b4c10, /// 0x920
			 0x75d7d5ea, /// 0x924
			 0x5a49dd1d, /// 0x928
			 0x42ebb946, /// 0x92c
			 0xa1c2147b, /// 0x930
			 0x7e450175, /// 0x934
			 0xc3e10d73, /// 0x938
			 0xd6f1b4cf, /// 0x93c
			 0xaba6278b, /// 0x940
			 0xdcd237c9, /// 0x944
			 0x23d8faf8, /// 0x948
			 0xf7be54ca, /// 0x94c
			 0xb8796b8f, /// 0x950
			 0x6cbb7979, /// 0x954
			 0x68e4c23d, /// 0x958
			 0x0f013de5, /// 0x95c
			 0x4e039b28, /// 0x960
			 0xeed8761d, /// 0x964
			 0x24b32554, /// 0x968
			 0xbdb886f1, /// 0x96c
			 0x9eeba471, /// 0x970
			 0x114ab7f2, /// 0x974
			 0x049a6e91, /// 0x978
			 0xf1b91caf, /// 0x97c
			 0x8ee4f8d0, /// 0x980
			 0x3e15a4c3, /// 0x984
			 0x5e3da134, /// 0x988
			 0x8597c63b, /// 0x98c
			 0x77917fb5, /// 0x990
			 0x32edb4b1, /// 0x994
			 0x40e81bbd, /// 0x998
			 0x769e5b32, /// 0x99c
			 0xf92c1385, /// 0x9a0
			 0x3229dcd3, /// 0x9a4
			 0x17bfc900, /// 0x9a8
			 0x6cb3a63c, /// 0x9ac
			 0x9480e978, /// 0x9b0
			 0x61ed95b2, /// 0x9b4
			 0xf80aa601, /// 0x9b8
			 0x4f3e6698, /// 0x9bc
			 0x18c5f0a2, /// 0x9c0
			 0x73f6efdf, /// 0x9c4
			 0x7bc88fe4, /// 0x9c8
			 0x8c72305f, /// 0x9cc
			 0xcf18a6cb, /// 0x9d0
			 0xded98cbf, /// 0x9d4
			 0x896c6a13, /// 0x9d8
			 0xcef1ecf7, /// 0x9dc
			 0xdc3cdef8, /// 0x9e0
			 0x38ee5b1f, /// 0x9e4
			 0xafc6966d, /// 0x9e8
			 0x7f7a5197, /// 0x9ec
			 0x4bb2a81f, /// 0x9f0
			 0xd1f1f659, /// 0x9f4
			 0x1b7c2d30, /// 0x9f8
			 0x48edc1fe, /// 0x9fc
			 0xf9d0a060, /// 0xa00
			 0x980b7634, /// 0xa04
			 0x072cce23, /// 0xa08
			 0x9833be9d, /// 0xa0c
			 0x9f7e2277, /// 0xa10
			 0xc9754caa, /// 0xa14
			 0xeeb14dde, /// 0xa18
			 0x53ea10d5, /// 0xa1c
			 0x39c83061, /// 0xa20
			 0x89f3d087, /// 0xa24
			 0xda88cb6d, /// 0xa28
			 0x9f653e0f, /// 0xa2c
			 0x0947f1cc, /// 0xa30
			 0xbc5fe608, /// 0xa34
			 0xfb2f5330, /// 0xa38
			 0xab36fac3, /// 0xa3c
			 0x4490193b, /// 0xa40
			 0xe26595cf, /// 0xa44
			 0xdcaacf6a, /// 0xa48
			 0x66fe6987, /// 0xa4c
			 0x935f9aa4, /// 0xa50
			 0x7ed3be50, /// 0xa54
			 0x82f70489, /// 0xa58
			 0x92c6180a, /// 0xa5c
			 0x0aa02805, /// 0xa60
			 0xb45e67dd, /// 0xa64
			 0xb9b65945, /// 0xa68
			 0xe78a9063, /// 0xa6c
			 0x5a52efc3, /// 0xa70
			 0xaa8da7cc, /// 0xa74
			 0x0b95737f, /// 0xa78
			 0x9e5c0dbf, /// 0xa7c
			 0xdaf11dad, /// 0xa80
			 0xfc632170, /// 0xa84
			 0x8d7ad84b, /// 0xa88
			 0x7c588a4f, /// 0xa8c
			 0xb215fc7b, /// 0xa90
			 0x2af58355, /// 0xa94
			 0x80e50a66, /// 0xa98
			 0xcb7f40ba, /// 0xa9c
			 0xde84ed3e, /// 0xaa0
			 0x9c833cd2, /// 0xaa4
			 0x89a0323a, /// 0xaa8
			 0x7b738317, /// 0xaac
			 0xd04464ac, /// 0xab0
			 0xff81da03, /// 0xab4
			 0x6ada95d6, /// 0xab8
			 0x03a1f2a8, /// 0xabc
			 0xe891ee73, /// 0xac0
			 0xfee9e36c, /// 0xac4
			 0x7421827a, /// 0xac8
			 0xf2510551, /// 0xacc
			 0x5b021cff, /// 0xad0
			 0x5fe60c53, /// 0xad4
			 0x5dbf0005, /// 0xad8
			 0x1f378928, /// 0xadc
			 0x1b516ddf, /// 0xae0
			 0xbdec54b6, /// 0xae4
			 0xb3fdf030, /// 0xae8
			 0xa2328f12, /// 0xaec
			 0xf3acf4be, /// 0xaf0
			 0xd68412e7, /// 0xaf4
			 0x074956e8, /// 0xaf8
			 0xcf045194, /// 0xafc
			 0x467a0f89, /// 0xb00
			 0xfae2a5e2, /// 0xb04
			 0x317f3e6e, /// 0xb08
			 0x49365db6, /// 0xb0c
			 0xa2af605a, /// 0xb10
			 0xfb5b00c6, /// 0xb14
			 0x5cfc7200, /// 0xb18
			 0x8bd8af43, /// 0xb1c
			 0x85c5f9a4, /// 0xb20
			 0xa5409699, /// 0xb24
			 0x263e7279, /// 0xb28
			 0xc18ec00a, /// 0xb2c
			 0x417dbf1b, /// 0xb30
			 0xbc82f8c1, /// 0xb34
			 0xb45324ed, /// 0xb38
			 0xc4b39649, /// 0xb3c
			 0x048262fd, /// 0xb40
			 0xcde866d6, /// 0xb44
			 0x31e05e1b, /// 0xb48
			 0xaeda4ca7, /// 0xb4c
			 0xbf174ec5, /// 0xb50
			 0x6b649154, /// 0xb54
			 0xb88427f4, /// 0xb58
			 0x3d37d431, /// 0xb5c
			 0x10747c06, /// 0xb60
			 0x8843cfb9, /// 0xb64
			 0x1e7c7a6e, /// 0xb68
			 0xdf1512a0, /// 0xb6c
			 0x31c428d9, /// 0xb70
			 0x63e4fe57, /// 0xb74
			 0x45d97f4d, /// 0xb78
			 0x5683b171, /// 0xb7c
			 0xe35524b6, /// 0xb80
			 0xcddd0712, /// 0xb84
			 0x13d11c74, /// 0xb88
			 0x317aa59e, /// 0xb8c
			 0x935ae3ac, /// 0xb90
			 0xc267cf5c, /// 0xb94
			 0xfdaa876c, /// 0xb98
			 0xa7387ee3, /// 0xb9c
			 0xdef99b2b, /// 0xba0
			 0x79ebbb79, /// 0xba4
			 0xb19964b8, /// 0xba8
			 0x30c33a97, /// 0xbac
			 0x8ca912cb, /// 0xbb0
			 0x4a728b7b, /// 0xbb4
			 0x1c3d71ee, /// 0xbb8
			 0x5a9b523c, /// 0xbbc
			 0x042f53af, /// 0xbc0
			 0x912429cc, /// 0xbc4
			 0xa1ca5b49, /// 0xbc8
			 0xdba617af, /// 0xbcc
			 0xec3fd27c, /// 0xbd0
			 0xd4c8b44e, /// 0xbd4
			 0x419d5525, /// 0xbd8
			 0x88d355e3, /// 0xbdc
			 0x16ceba95, /// 0xbe0
			 0x6a040f6b, /// 0xbe4
			 0xdf614def, /// 0xbe8
			 0x060d8fc6, /// 0xbec
			 0x1e4fea9e, /// 0xbf0
			 0x90b6858c, /// 0xbf4
			 0x611146c6, /// 0xbf8
			 0x81f1fead, /// 0xbfc
			 0xc56d460c, /// 0xc00
			 0x7e755164, /// 0xc04
			 0xbfb4e23e, /// 0xc08
			 0x2ac6d307, /// 0xc0c
			 0xc655694d, /// 0xc10
			 0x51b35865, /// 0xc14
			 0x8915b828, /// 0xc18
			 0x20409d0a, /// 0xc1c
			 0xa36f848d, /// 0xc20
			 0x964d1d5a, /// 0xc24
			 0xa74b9bbf, /// 0xc28
			 0x01d76a4a, /// 0xc2c
			 0xc88c7799, /// 0xc30
			 0x6e9ed967, /// 0xc34
			 0x23aaf8b4, /// 0xc38
			 0x376814ec, /// 0xc3c
			 0x8d0a59b1, /// 0xc40
			 0x6b0fc09f, /// 0xc44
			 0x907d7a16, /// 0xc48
			 0x7ceb8c88, /// 0xc4c
			 0xcbfbb565, /// 0xc50
			 0x11e3c573, /// 0xc54
			 0x8cb3f55f, /// 0xc58
			 0x9c2e9ed8, /// 0xc5c
			 0x6270a88a, /// 0xc60
			 0xf851121d, /// 0xc64
			 0x63ef7042, /// 0xc68
			 0x1c03490e, /// 0xc6c
			 0xf59a0dcc, /// 0xc70
			 0xbdf66310, /// 0xc74
			 0x7c4410bc, /// 0xc78
			 0x047017be, /// 0xc7c
			 0x8fa8ff6c, /// 0xc80
			 0x08fc3cbd, /// 0xc84
			 0xc36b369f, /// 0xc88
			 0xba1e2b3d, /// 0xc8c
			 0x3a03cdb7, /// 0xc90
			 0x15f3d73e, /// 0xc94
			 0x6c3bdf0e, /// 0xc98
			 0x9c341deb, /// 0xc9c
			 0x9f227bb3, /// 0xca0
			 0x1caa97eb, /// 0xca4
			 0xc39038ca, /// 0xca8
			 0x7de67310, /// 0xcac
			 0x06fc2b11, /// 0xcb0
			 0x20989a39, /// 0xcb4
			 0xadea919f, /// 0xcb8
			 0xfccd64ad, /// 0xcbc
			 0xf3eba4f0, /// 0xcc0
			 0xc54e7d76, /// 0xcc4
			 0xc32c3b11, /// 0xcc8
			 0xf0fd216d, /// 0xccc
			 0x9ebbde0c, /// 0xcd0
			 0x6a9cdde7, /// 0xcd4
			 0xa48c85cd, /// 0xcd8
			 0x5632058e, /// 0xcdc
			 0x517263d8, /// 0xce0
			 0x4c478e09, /// 0xce4
			 0x78cb40d9, /// 0xce8
			 0xcd9d1ee2, /// 0xcec
			 0xf866684d, /// 0xcf0
			 0xf25f0d89, /// 0xcf4
			 0xf564015b, /// 0xcf8
			 0x1c041480, /// 0xcfc
			 0x4520e1dd, /// 0xd00
			 0xa2bcdb8d, /// 0xd04
			 0x8ad860f3, /// 0xd08
			 0x9b013d4e, /// 0xd0c
			 0xedf67485, /// 0xd10
			 0xa0e9b610, /// 0xd14
			 0x916e5b20, /// 0xd18
			 0xdf421f85, /// 0xd1c
			 0x8b98ea0a, /// 0xd20
			 0x80f3600e, /// 0xd24
			 0xb661b42f, /// 0xd28
			 0xe2e9d98e, /// 0xd2c
			 0xad44dc5b, /// 0xd30
			 0x42fb508a, /// 0xd34
			 0x269d19dd, /// 0xd38
			 0xf743efba, /// 0xd3c
			 0xbd2d16d8, /// 0xd40
			 0x36230522, /// 0xd44
			 0x652743dc, /// 0xd48
			 0xd76c75f8, /// 0xd4c
			 0x692c2fcc, /// 0xd50
			 0x93d0bddb, /// 0xd54
			 0x013901b8, /// 0xd58
			 0x63164210, /// 0xd5c
			 0xe523d4b6, /// 0xd60
			 0x3f021746, /// 0xd64
			 0x674d1339, /// 0xd68
			 0xdc46b5aa, /// 0xd6c
			 0x978a1deb, /// 0xd70
			 0x6f447b87, /// 0xd74
			 0x9f9120e6, /// 0xd78
			 0x41091528, /// 0xd7c
			 0x5e86a1d5, /// 0xd80
			 0xd80d4764, /// 0xd84
			 0x898a0a6e, /// 0xd88
			 0xbf13a6f3, /// 0xd8c
			 0x58fae7ff, /// 0xd90
			 0xe5286b7f, /// 0xd94
			 0xaf4fb637, /// 0xd98
			 0xa69d2581, /// 0xd9c
			 0xeaa38a39, /// 0xda0
			 0x174ebead, /// 0xda4
			 0x9a7b9cce, /// 0xda8
			 0xd3b0fe99, /// 0xdac
			 0x8584e9f0, /// 0xdb0
			 0xa73a4d15, /// 0xdb4
			 0x8107fa9c, /// 0xdb8
			 0x2cd2b481, /// 0xdbc
			 0x32c0f6b0, /// 0xdc0
			 0x9a3c228a, /// 0xdc4
			 0x992ab99c, /// 0xdc8
			 0x270194db, /// 0xdcc
			 0xb3118231, /// 0xdd0
			 0x2fb06316, /// 0xdd4
			 0xfb883d09, /// 0xdd8
			 0x24ea633a, /// 0xddc
			 0x0e09b56f, /// 0xde0
			 0xcf715820, /// 0xde4
			 0xb289173a, /// 0xde8
			 0x34f667cf, /// 0xdec
			 0x4508eacb, /// 0xdf0
			 0xbf83df14, /// 0xdf4
			 0x0f239168, /// 0xdf8
			 0xd5d07c2d, /// 0xdfc
			 0x5245d17b, /// 0xe00
			 0x4942ad49, /// 0xe04
			 0xd740325d, /// 0xe08
			 0x6c74ccc2, /// 0xe0c
			 0xc1825f16, /// 0xe10
			 0x0faacad5, /// 0xe14
			 0x9299c75e, /// 0xe18
			 0x3faaf892, /// 0xe1c
			 0x57a94b7d, /// 0xe20
			 0x62a881c0, /// 0xe24
			 0x9c829398, /// 0xe28
			 0x11edd633, /// 0xe2c
			 0x5ec59bac, /// 0xe30
			 0xda2a18ae, /// 0xe34
			 0x71a6cec6, /// 0xe38
			 0xa0c3dc17, /// 0xe3c
			 0x1b605c27, /// 0xe40
			 0x2036befa, /// 0xe44
			 0x30614826, /// 0xe48
			 0xd098b63c, /// 0xe4c
			 0x971ee597, /// 0xe50
			 0x2aeca0b7, /// 0xe54
			 0x377f656b, /// 0xe58
			 0x33ee2fdb, /// 0xe5c
			 0x1879b1a3, /// 0xe60
			 0x024e7c35, /// 0xe64
			 0xb8aeada8, /// 0xe68
			 0x66e1cd6a, /// 0xe6c
			 0x12dc8a06, /// 0xe70
			 0x794654a3, /// 0xe74
			 0xc0a741a4, /// 0xe78
			 0x5aaa03bc, /// 0xe7c
			 0x2d90c07b, /// 0xe80
			 0x5ea007dc, /// 0xe84
			 0x5e0e50f2, /// 0xe88
			 0x16739d3c, /// 0xe8c
			 0xbd5036e9, /// 0xe90
			 0x37ef4b9d, /// 0xe94
			 0xb83641bf, /// 0xe98
			 0x080d20d5, /// 0xe9c
			 0x7e1db974, /// 0xea0
			 0x88847222, /// 0xea4
			 0x36c7e688, /// 0xea8
			 0xc407a272, /// 0xeac
			 0xf9d6e9ab, /// 0xeb0
			 0x98ce1162, /// 0xeb4
			 0xfab01459, /// 0xeb8
			 0xeee653c4, /// 0xebc
			 0xfd46dad0, /// 0xec0
			 0xc3306f3d, /// 0xec4
			 0x52977e03, /// 0xec8
			 0x3226873a, /// 0xecc
			 0x97a7446d, /// 0xed0
			 0x13753c18, /// 0xed4
			 0x2b185738, /// 0xed8
			 0x5a3a228a, /// 0xedc
			 0xd4c04ed7, /// 0xee0
			 0x14db9614, /// 0xee4
			 0xcb1d59a9, /// 0xee8
			 0xa05fd0ed, /// 0xeec
			 0xc18e950e, /// 0xef0
			 0x3708b217, /// 0xef4
			 0xfa7b864d, /// 0xef8
			 0xe88aa3cc, /// 0xefc
			 0x342f263a, /// 0xf00
			 0x550a5da5, /// 0xf04
			 0x36951013, /// 0xf08
			 0x93551877, /// 0xf0c
			 0x3c84e61c, /// 0xf10
			 0x1d35a2e9, /// 0xf14
			 0x3c45c201, /// 0xf18
			 0xbfc8ba66, /// 0xf1c
			 0xf4fcad00, /// 0xf20
			 0x64b31b49, /// 0xf24
			 0x7e75ca07, /// 0xf28
			 0xd44e104c, /// 0xf2c
			 0x876e1905, /// 0xf30
			 0xf2ceff96, /// 0xf34
			 0x5a67222d, /// 0xf38
			 0xb46f2e7a, /// 0xf3c
			 0x33632ee0, /// 0xf40
			 0x24584fea, /// 0xf44
			 0x9e68844b, /// 0xf48
			 0x20503686, /// 0xf4c
			 0xd39e4305, /// 0xf50
			 0x4cd525b6, /// 0xf54
			 0xcf07f0d9, /// 0xf58
			 0x0c430109, /// 0xf5c
			 0xe7df75b9, /// 0xf60
			 0x04f017a3, /// 0xf64
			 0x40d3be41, /// 0xf68
			 0xb0a7a2e3, /// 0xf6c
			 0x4e5eda91, /// 0xf70
			 0xe41cfefa, /// 0xf74
			 0x0cc1f95d, /// 0xf78
			 0x4508a08c, /// 0xf7c
			 0x61b825de, /// 0xf80
			 0x54e7fdb5, /// 0xf84
			 0x21f4812b, /// 0xf88
			 0x54d350de, /// 0xf8c
			 0x4436c082, /// 0xf90
			 0xee846c59, /// 0xf94
			 0xa13780f8, /// 0xf98
			 0x48e30e63, /// 0xf9c
			 0xfb6e477a, /// 0xfa0
			 0x49db7147, /// 0xfa4
			 0xa9645cda, /// 0xfa8
			 0x950d03f7, /// 0xfac
			 0x28ea0252, /// 0xfb0
			 0x3a70d16e, /// 0xfb4
			 0xf1294387, /// 0xfb8
			 0xe1d51dae, /// 0xfbc
			 0x886bfb2b, /// 0xfc0
			 0xf7fb7d66, /// 0xfc4
			 0xd41ece45, /// 0xfc8
			 0x8e26aab3, /// 0xfcc
			 0xa719ef6d, /// 0xfd0
			 0x1bcb7d9b, /// 0xfd4
			 0x487b3396, /// 0xfd8
			 0x55e52108, /// 0xfdc
			 0x3d83f9bf, /// 0xfe0
			 0x98b6e6b2, /// 0xfe4
			 0xaa9085d3, /// 0xfe8
			 0x752ab851, /// 0xfec
			 0xa6db3515, /// 0xff0
			 0xd8c781ed, /// 0xff4
			 0x076a498a, /// 0xff8
			 0xd5adfe93 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00000
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00004
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00008
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0000c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00010
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00014
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0001c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00020
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00024
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00028
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xffc00001,                                                  // -signaling NaN                              /// 00030
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0xffc00001,                                                  // -signaling NaN                              /// 00040
			 0x0c600000,                                                  // Leading 1s:                                 /// 00044
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00048
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00050
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00054
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00058
			 0xffc00001,                                                  // -signaling NaN                              /// 0005c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x4b000000,                                                  // 8388608.0                                   /// 00074
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00078
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00080
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0008c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00090
			 0xbf028f5c,                                                  // -0.51                                       /// 00094
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000a4
			 0x33333333,                                                  // 4 random values                             /// 000a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000b4
			 0xcb000000,                                                  // -8388608.0                                  /// 000b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000bc
			 0x7fc00001,                                                  // signaling NaN                               /// 000c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d4
			 0xcb000000,                                                  // -8388608.0                                  /// 000d8
			 0xff800000,                                                  // -inf                                        /// 000dc
			 0xcb000000,                                                  // -8388608.0                                  /// 000e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00100
			 0xbf028f5c,                                                  // -0.51                                       /// 00104
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00108
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0010c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00114
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00118
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0011c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00124
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00128
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0012c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00134
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00138
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0013c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00148
			 0x80000000,                                                  // -zero                                       /// 0014c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00150
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00158
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x7fc00001,                                                  // signaling NaN                               /// 0016c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00170
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00174
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00178
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0017c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00180
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00184
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00188
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0018c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00190
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00194
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00198
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x33333333,                                                  // 4 random values                             /// 001b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00200
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00204
			 0xbf028f5c,                                                  // -0.51                                       /// 00208
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00210
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00214
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00218
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00220
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00228
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00230
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00238
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x33333333,                                                  // 4 random values                             /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00248
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00250
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00254
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00258
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0025c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00260
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00264
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00268
			 0x55555555,                                                  // 4 random values                             /// 0026c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00274
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00278
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00280
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00284
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00288
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0028c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00298
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00300
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00304
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00308
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00310
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00318
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00320
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00324
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0032c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00334
			 0x00011111,                                                  // 9.7958E-41                                  /// 00338
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00344
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00348
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0034c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00350
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00354
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00358
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00360
			 0x4b000000,                                                  // 8388608.0                                   /// 00364
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0036c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00374
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00384
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0038c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00390
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00398
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0xff800000,                                                  // -inf                                        /// 003b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 003f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00400
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00408
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0040c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0c400000,                                                  // Leading 1s:                                 /// 00414
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00420
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00424
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0042c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00430
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00434
			 0x80000000,                                                  // -zero                                       /// 00438
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0043c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00448
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0044c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00450
			 0x7f800000,                                                  // inf                                         /// 00454
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f800000,                                                  // inf                                         /// 0045c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00468
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0046c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00474
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0047c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00480
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00484
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00488
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0048c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00490
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00498
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004bc
			 0xff800000,                                                  // -inf                                        /// 004c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004e4
			 0x3f028f5c,                                                  // 0.51                                        /// 004e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00500
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00510
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00518
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0051c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00520
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00524
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0052c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00534
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00538
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0053c
			 0x3f028f5c,                                                  // 0.51                                        /// 00540
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00548
			 0x0e000001,                                                  // Trailing 1s:                                /// 0054c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00550
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00560
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00564
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00568
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0056c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00570
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00574
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00584
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00588
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00590
			 0xff800000,                                                  // -inf                                        /// 00594
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00598
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0059c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ac
			 0xff800000,                                                  // -inf                                        /// 005b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005bc
			 0x7f800000,                                                  // inf                                         /// 005c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00600
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00604
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00608
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0060c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00610
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00618
			 0x4b000000,                                                  // 8388608.0                                   /// 0061c
			 0xbf028f5c,                                                  // -0.51                                       /// 00620
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00624
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00628
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0062c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00630
			 0x0e000003,                                                  // Trailing 1s:                                /// 00634
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00638
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0063c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00640
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00644
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0065c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00660
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00664
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00668
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0066c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00670
			 0x0e000007,                                                  // Trailing 1s:                                /// 00674
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00678
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0067c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00684
			 0x33333333,                                                  // 4 random values                             /// 00688
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00690
			 0x55555555,                                                  // 4 random values                             /// 00694
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00698
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 006c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00700
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00704
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00710
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00714
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00718
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00728
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0072c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00730
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00734
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00738
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0073c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00740
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00744
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00748
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0074c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00750
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00760
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00764
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0xff800000,                                                  // -inf                                        /// 0076c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00770
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00778
			 0x80011111,                                                  // -9.7958E-41                                 /// 0077c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00784
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00788
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0078c
			 0x33333333,                                                  // 4 random values                             /// 00790
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x4b000000,                                                  // 8388608.0                                   /// 007a4
			 0xff800000,                                                  // -inf                                        /// 007a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00804
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00808
			 0xbf028f5c,                                                  // -0.51                                       /// 0080c
			 0x00000000,                                                  // zero                                        /// 00810
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00814
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0081c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00820
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00824
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00828
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0082c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00830
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0083c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00840
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00844
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00848
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0084c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00850
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00854
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0085c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00864
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00868
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0086c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00870
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00878
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0087c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00880
			 0xcb000000,                                                  // -8388608.0                                  /// 00884
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00888
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x7fc00001,                                                  // signaling NaN                               /// 00890
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00894
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0089c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x80000000,                                                  // -zero                                       /// 008c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x4b000000,                                                  // 8388608.0                                   /// 008e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00900
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00904
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0090c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00910
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00914
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00918
			 0x00011111,                                                  // 9.7958E-41                                  /// 0091c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00920
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00924
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00928
			 0xcb000000,                                                  // -8388608.0                                  /// 0092c
			 0x4b000000,                                                  // 8388608.0                                   /// 00930
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00934
			 0x0c600000,                                                  // Leading 1s:                                 /// 00938
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00944
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00948
			 0x0c700000,                                                  // Leading 1s:                                 /// 0094c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0xffc00001,                                                  // -signaling NaN                              /// 00964
			 0x0c400000,                                                  // Leading 1s:                                 /// 00968
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00970
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00978
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0097c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00984
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00988
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0098c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00990
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00994
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0099c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009cc
			 0xff800000,                                                  // -inf                                        /// 009d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a30
			 0x7f800000,                                                  // inf                                         /// 00a34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00aa0
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00acc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ad4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00adc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b08
			 0xff800000,                                                  // -inf                                        /// 00b0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0xcb000000,                                                  // -8388608.0                                  /// 00b38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b48
			 0x3f028f5c,                                                  // 0.51                                        /// 00b4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ba0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x33333333,                                                  // 4 random values                             /// 00bb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00be0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00be4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bf0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c18
			 0x80000000,                                                  // -zero                                       /// 00c1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c30
			 0xff800000,                                                  // -inf                                        /// 00c34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0xff800000,                                                  // -inf                                        /// 00c90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cbc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cc8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ccc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cdc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ce0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x80000000,                                                  // -zero                                       /// 00ce8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cec
			 0x80000000,                                                  // -zero                                       /// 00cf0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x7f800000,                                                  // inf                                         /// 00cf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cfc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x3f028f5c,                                                  // 0.51                                        /// 00d18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00db0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dc4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd4
			 0x33333333,                                                  // 4 random values                             /// 00dd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ddc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0xcb000000,                                                  // -8388608.0                                  /// 00de4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00df8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ebc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ec0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ec8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ed0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ed8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ee4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ee8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00efc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f20
			 0xff800000,                                                  // -inf                                        /// 00f24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f34
			 0x55555555,                                                  // 4 random values                             /// 00f38
			 0x7fc00001,                                                  // signaling NaN                               /// 00f3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f40
			 0x00000000,                                                  // zero                                        /// 00f44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fa4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb0
			 0x00000000,                                                  // zero                                        /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ff0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0f7fffff                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00000
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00008
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00014
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c700000,                                                  // Leading 1s:                                 /// 0001c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00020
			 0x0c400000,                                                  // Leading 1s:                                 /// 00024
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00028
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00034
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0003c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00040
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00044
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00050
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00054
			 0x33333333,                                                  // 4 random values                             /// 00058
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00064
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00068
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00078
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0007c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00084
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00088
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00090
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00094
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000ac
			 0x7fc00001,                                                  // signaling NaN                               /// 000b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b4
			 0x3f028f5c,                                                  // 0.51                                        /// 000b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x33333333,                                                  // 4 random values                             /// 000d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000e4
			 0x00000000,                                                  // zero                                        /// 000e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00100
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00104
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00108
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0010c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00114
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00124
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00128
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0012c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00130
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00134
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00138
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0013c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00144
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00148
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00150
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00158
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00164
			 0x0c780000,                                                  // Leading 1s:                                 /// 00168
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00170
			 0x0e000001,                                                  // Trailing 1s:                                /// 00174
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00180
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00184
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00188
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0018c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00194
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00198
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0019c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001cc
			 0xcb000000,                                                  // -8388608.0                                  /// 001d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001fc
			 0x7f800000,                                                  // inf                                         /// 00200
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00208
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00210
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x00000000,                                                  // zero                                        /// 00218
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00220
			 0x0e000001,                                                  // Trailing 1s:                                /// 00224
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00228
			 0x0c400000,                                                  // Leading 1s:                                 /// 0022c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00238
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0023c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00240
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00248
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00258
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0025c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00264
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0xcb000000,                                                  // -8388608.0                                  /// 0026c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00270
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00274
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00278
			 0x33333333,                                                  // 4 random values                             /// 0027c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00280
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00284
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00290
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x00000000,                                                  // zero                                        /// 002c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 002e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0xff800000,                                                  // -inf                                        /// 002ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00304
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0030c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00310
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x4b000000,                                                  // 8388608.0                                   /// 0031c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00328
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0032c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00330
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00334
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0033c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00344
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00348
			 0x7fc00001,                                                  // signaling NaN                               /// 0034c
			 0x55555555,                                                  // 4 random values                             /// 00350
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00358
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0035c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00360
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00368
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0036c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00370
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0037c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00384
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00388
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0038c
			 0x00000000,                                                  // zero                                        /// 00390
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00394
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00398
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0039c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0xbf028f5c,                                                  // -0.51                                       /// 00404
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0040c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00414
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00418
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0041c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00424
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0042c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00434
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00438
			 0x0e000003,                                                  // Trailing 1s:                                /// 0043c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00440
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0044c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00450
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00458
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0045c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00460
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00464
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0046c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00474
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0047c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00480
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00484
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00488
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00490
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0049c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x4b000000,                                                  // 8388608.0                                   /// 004b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d4
			 0x7f800000,                                                  // inf                                         /// 004d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00500
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00508
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0050c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00510
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0051c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00520
			 0x80000000,                                                  // -zero                                       /// 00524
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0052c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00534
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00544
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00558
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00560
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00564
			 0xbf028f5c,                                                  // -0.51                                       /// 00568
			 0x80011111,                                                  // -9.7958E-41                                 /// 0056c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00570
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00574
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00584
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0058c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00594
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00598
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0059c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00600
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00604
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00608
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0060c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00614
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00618
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00624
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00628
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0062c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00630
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00640
			 0xffc00001,                                                  // -signaling NaN                              /// 00644
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00648
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0064c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00650
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0c700000,                                                  // Leading 1s:                                 /// 00658
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00660
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00670
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00674
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00678
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0067c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00680
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00688
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0068c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00694
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00698
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00000000,                                                  // zero                                        /// 006c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00700
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00704
			 0xcb000000,                                                  // -8388608.0                                  /// 00708
			 0xffc00001,                                                  // -signaling NaN                              /// 0070c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00718
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0071c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00720
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00724
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00728
			 0xffc00001,                                                  // -signaling NaN                              /// 0072c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00730
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00734
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0xcb000000,                                                  // -8388608.0                                  /// 0073c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00740
			 0xff800000,                                                  // -inf                                        /// 00744
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00748
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00750
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00758
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0075c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00760
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00768
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0076c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00770
			 0x33333333,                                                  // 4 random values                             /// 00774
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00778
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00780
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00784
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0xcb000000,                                                  // -8388608.0                                  /// 00794
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00798
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0079c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000000,                                                  // -zero                                       /// 007a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00800
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00808
			 0x0c780000,                                                  // Leading 1s:                                 /// 0080c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00810
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00828
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00830
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00834
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x4b000000,                                                  // 8388608.0                                   /// 00840
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00848
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00860
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00864
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00868
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0087c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00880
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00884
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00888
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00890
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00894
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0089c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008dc
			 0xbf028f5c,                                                  // -0.51                                       /// 008e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00904
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00908
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00910
			 0x0c700000,                                                  // Leading 1s:                                 /// 00914
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00918
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00924
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0092c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00930
			 0x0e000001,                                                  // Trailing 1s:                                /// 00934
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00938
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0093c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00940
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00948
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00954
			 0x0e000001,                                                  // Trailing 1s:                                /// 00958
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0095c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00960
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00968
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0096c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00970
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00974
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0097c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00988
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0098c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00990
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00998
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0099c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x4b000000,                                                  // 8388608.0                                   /// 009dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x3f028f5c,                                                  // 0.51                                        /// 00a08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a34
			 0x4b000000,                                                  // 8388608.0                                   /// 00a38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80000000,                                                  // -zero                                       /// 00a4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a50
			 0x7fc00001,                                                  // signaling NaN                               /// 00a54
			 0x7f800000,                                                  // inf                                         /// 00a58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0xcb000000,                                                  // -8388608.0                                  /// 00a88
			 0xbf028f5c,                                                  // -0.51                                       /// 00a8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aa8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00abc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ac8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00acc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ad4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00adc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ae0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ae4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00af4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b4c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ba0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ba4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bc0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x80000000,                                                  // -zero                                       /// 00bc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bcc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bec
			 0xff800000,                                                  // -inf                                        /// 00bf0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bf4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c14
			 0x55555555,                                                  // 4 random values                             /// 00c18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c54
			 0x33333333,                                                  // 4 random values                             /// 00c58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c8c
			 0x7f800000,                                                  // inf                                         /// 00c90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ca4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cc0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ccc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x33333333,                                                  // 4 random values                             /// 00cdc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ce0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x3f028f5c,                                                  // 0.51                                        /// 00d1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x33333333,                                                  // 4 random values                             /// 00d70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d80
			 0xff800000,                                                  // -inf                                        /// 00d84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00da0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00de0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00de4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00de8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0xbf028f5c,                                                  // -0.51                                       /// 00df0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df8
			 0x7f800000,                                                  // inf                                         /// 00dfc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7fc00001,                                                  // signaling NaN                               /// 00e68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x55555555,                                                  // 4 random values                             /// 00e70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ea0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ec0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ec8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00edc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ee4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ee8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ef4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f18
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0xff800000,                                                  // -inf                                        /// 00f80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f90
			 0x7fc00001,                                                  // signaling NaN                               /// 00f94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd0
			 0x33333333,                                                  // 4 random values                             /// 00fd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xff800000,                                                  // -inf                                        /// 00fdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fe8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ff4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x00001000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x00000304,
			 0x00000720,
			 0x00000208,
			 0x00000750,
			 0x00000278,
			 0x000003b4,
			 0x00000448,
			 0x000000a0,

			 /// vpu register f2
			 0x41f80000,
			 0x41a00000,
			 0x41700000,
			 0x41b80000,
			 0x41800000,
			 0x41600000,
			 0x41e80000,
			 0x41000000,

			 /// vpu register f3
			 0x41400000,
			 0x40c00000,
			 0x40c00000,
			 0x3f800000,
			 0x40400000,
			 0x41c80000,
			 0x41600000,
			 0x41300000,

			 /// vpu register f4
			 0x41d80000,
			 0x40400000,
			 0x41500000,
			 0x41900000,
			 0x41e80000,
			 0x41700000,
			 0x41000000,
			 0x40a00000,

			 /// vpu register f5
			 0x40e00000,
			 0x41d00000,
			 0x41d80000,
			 0x41c80000,
			 0x41880000,
			 0x42000000,
			 0x41000000,
			 0x41700000,

			 /// vpu register f6
			 0x41800000,
			 0x40000000,
			 0x40000000,
			 0x41700000,
			 0x40a00000,
			 0x41d80000,
			 0x41d00000,
			 0x41f00000,

			 /// vpu register f7
			 0x41980000,
			 0x41800000,
			 0x42000000,
			 0x41e80000,
			 0x41700000,
			 0x41c80000,
			 0x41600000,
			 0x41a80000,

			 /// vpu register f8
			 0x40800000,
			 0x42000000,
			 0x41f00000,
			 0x40e00000,
			 0x41980000,
			 0x41e00000,
			 0x40400000,
			 0x41980000,

			 /// vpu register f9
			 0x41700000,
			 0x41a80000,
			 0x41300000,
			 0x41000000,
			 0x41a00000,
			 0x41a80000,
			 0x41000000,
			 0x41500000,

			 /// vpu register f10
			 0x40000000,
			 0x41700000,
			 0x41e80000,
			 0x41800000,
			 0x42000000,
			 0x40c00000,
			 0x41800000,
			 0x40c00000,

			 /// vpu register f11
			 0x41100000,
			 0x41980000,
			 0x41f80000,
			 0x41d00000,
			 0x41d80000,
			 0x41d80000,
			 0x40e00000,
			 0x41b80000,

			 /// vpu register f12
			 0x41200000,
			 0x41d00000,
			 0x41800000,
			 0x41e00000,
			 0x40e00000,
			 0x41400000,
			 0x41900000,
			 0x41a80000,

			 /// vpu register f13
			 0x41f00000,
			 0x3f800000,
			 0x41d00000,
			 0x41700000,
			 0x41a80000,
			 0x40e00000,
			 0x41c80000,
			 0x41f00000,

			 /// vpu register f14
			 0x40400000,
			 0x41c80000,
			 0x41400000,
			 0x41f80000,
			 0x40e00000,
			 0x41600000,
			 0x41c00000,
			 0x3f800000,

			 /// vpu register f15
			 0x41980000,
			 0x41300000,
			 0x40c00000,
			 0x41f80000,
			 0x41600000,
			 0x40000000,
			 0x41800000,
			 0x41d80000,

			 /// vpu register f16
			 0x41a80000,
			 0x3f800000,
			 0x41b80000,
			 0x41000000,
			 0x40400000,
			 0x40c00000,
			 0x41900000,
			 0x40c00000,

			 /// vpu register f17
			 0x41800000,
			 0x42000000,
			 0x40800000,
			 0x40400000,
			 0x41f80000,
			 0x41d00000,
			 0x41c80000,
			 0x40800000,

			 /// vpu register f18
			 0x41b80000,
			 0x41000000,
			 0x41600000,
			 0x41880000,
			 0x40c00000,
			 0x41500000,
			 0x41f00000,
			 0x41e80000,

			 /// vpu register f19
			 0x41880000,
			 0x41d00000,
			 0x41700000,
			 0x41a80000,
			 0x40c00000,
			 0x41980000,
			 0x41100000,
			 0x41c80000,

			 /// vpu register f20
			 0x41880000,
			 0x40000000,
			 0x40000000,
			 0x41100000,
			 0x40e00000,
			 0x41500000,
			 0x41b00000,
			 0x41d00000,

			 /// vpu register f21
			 0x41b00000,
			 0x41d00000,
			 0x41b00000,
			 0x41d00000,
			 0x41300000,
			 0x41600000,
			 0x40a00000,
			 0x41b80000,

			 /// vpu register f22
			 0x41e80000,
			 0x41700000,
			 0x41900000,
			 0x41f00000,
			 0x40400000,
			 0x40a00000,
			 0x41f80000,
			 0x41700000,

			 /// vpu register f23
			 0x40000000,
			 0x41880000,
			 0x40800000,
			 0x41a00000,
			 0x40c00000,
			 0x40e00000,
			 0x41e00000,
			 0x41f00000,

			 /// vpu register f24
			 0x41c00000,
			 0x40800000,
			 0x41400000,
			 0x41b80000,
			 0x41200000,
			 0x41c00000,
			 0x41a00000,
			 0x41e80000,

			 /// vpu register f25
			 0x3f800000,
			 0x41f80000,
			 0x41800000,
			 0x41e80000,
			 0x40a00000,
			 0x41e00000,
			 0x41d80000,
			 0x41500000,

			 /// vpu register f26
			 0x41300000,
			 0x40000000,
			 0x41600000,
			 0x41f80000,
			 0x41700000,
			 0x41600000,
			 0x3f800000,
			 0x41800000,

			 /// vpu register f27
			 0x41600000,
			 0x41000000,
			 0x41700000,
			 0x41900000,
			 0x41700000,
			 0x41900000,
			 0x41880000,
			 0x41e80000,

			 /// vpu register f28
			 0x41800000,
			 0x41d80000,
			 0x41000000,
			 0x41300000,
			 0x41d00000,
			 0x41700000,
			 0x41c80000,
			 0x40800000,

			 /// vpu register f29
			 0x41000000,
			 0x41c00000,
			 0x41c80000,
			 0x40c00000,
			 0x41400000,
			 0x41980000,
			 0x42000000,
			 0x40000000,

			 /// vpu register f30
			 0x41b80000,
			 0x41d80000,
			 0x40800000,
			 0x41200000,
			 0x40c00000,
			 0x41980000,
			 0x41b00000,
			 0x41e00000,

			 /// vpu register f31
			 0x41c80000,
			 0x40c00000,
			 0x41200000,
			 0x41d00000,
			 0x41d80000,
			 0x41b00000,
			 0x41c00000,
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
		"faddi.pi f10, f8, 235\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f20, f5, 453\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f17, f30, 93\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f23, f22, 279\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f7, f7, 362\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f19, f12, 91\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f5, f25, 43\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f7, f0, 78\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f17, f13, 4\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f23, f18, 54\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f15, f28, 81\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f3, f24, 110\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f5, f25, 103\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f23, f20, 125\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f19, f14, 509\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f9, f30, 480\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f25, f10, 350\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f13, f11, 484\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f3, f30, 206\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f4, f26, 77\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f15, f16, 100\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f19, f27, 175\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f22, f10, 428\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f6, f22, 486\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f10, f20, 230\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f9, f1, 194\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f9, f6, 1\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f28, f22, 379\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f29, f22, 489\n"                            ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f21, f8, 476\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f27, f27, 365\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f23, f16, 330\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f23, f11, 80\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f10, f10, 134\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f18, f28, 373\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f7, f7, 329\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f30, f1, 138\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f18, f26, 0\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f28, f24, 472\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f8, f29, 309\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f7, f5, 119\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f3, f29, 18\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f16, f10, 19\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f24, f19, 388\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f6, f29, 38\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f25, f8, 412\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f20, f27, 138\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f27, f24, 403\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f25, f7, 249\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f20, f27, 465\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f24, f12, 100\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f2, f24, 169\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f25, f26, 439\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f9, f8, 296\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f9, f4, 308\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f9, f24, 291\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f25, f27, 12\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f15, f10, 149\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f27, f29, 83\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f14, f18, 272\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f30, f1, 205\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f2, f28, 340\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f3, f18, 267\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f8, f14, 2\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f21, f8, 44\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f10, f24, 488\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f26, f3, 48\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f14, f4, 71\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f20, f25, 203\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f15, f22, 401\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f5, f16, 283\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f20, f18, 366\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f11, f25, 363\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f23, f18, 248\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f3, f11, 194\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f5, f10, 245\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f15, f19, 71\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f30, f5, 390\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f16, f19, 381\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f6, f0, 203\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f26, f17, 51\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f5, f29, 449\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f3, f4, 141\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f4, f10, 478\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f8, f20, 400\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f24, f7, 102\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f10, f28, 139\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f11, f18, 260\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f2, f0, 327\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f17, f8, 377\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f27, f0, 298\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f15, f29, 34\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f28, f27, 154\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f17, f24, 449\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f24, f14, 90\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f19, f3, 170\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f23, f11, 132\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f4, f22, 451\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"faddi.pi f15, f17, 504\n"                            ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
