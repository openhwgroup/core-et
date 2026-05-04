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
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00004
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0000c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00010
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00014
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x33333333,                                                  // 4 random values                             /// 0001c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00020
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0002c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00030
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0003c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00040
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00048
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0004c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00054
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00058
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00060
			 0x0e000003,                                                  // Trailing 1s:                                /// 00064
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00068
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00070
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00078
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0007c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00080
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00088
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00090
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00094
			 0x0e000001,                                                  // Trailing 1s:                                /// 00098
			 0x0c600000,                                                  // Leading 1s:                                 /// 0009c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000c0
			 0xbf028f5c,                                                  // -0.51                                       /// 000c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00100
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00108
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00114
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00118
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0011c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00120
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00124
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00128
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0012c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00130
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x4b000000,                                                  // 8388608.0                                   /// 0013c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00140
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00144
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00148
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00150
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00158
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00160
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00164
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00168
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00170
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00174
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00178
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00180
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00188
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0018c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00190
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00194
			 0x7f800000,                                                  // inf                                         /// 00198
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b8
			 0xff800000,                                                  // -inf                                        /// 001bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x55555555,                                                  // 4 random values                             /// 001d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x7f800000,                                                  // inf                                         /// 001dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00200
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00204
			 0x80000000,                                                  // -zero                                       /// 00208
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00214
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00218
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0021c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00220
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00224
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0022c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00238
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0023c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00240
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0024c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00250
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00258
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0025c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00268
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00274
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00278
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00280
			 0x0e000001,                                                  // Trailing 1s:                                /// 00284
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00290
			 0x0c780000,                                                  // Leading 1s:                                 /// 00294
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0029c
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002ec
			 0x7fc00001,                                                  // signaling NaN                               /// 002f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00300
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00304
			 0x0e000001,                                                  // Trailing 1s:                                /// 00308
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00314
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00318
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0031c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00320
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00324
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0032c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00334
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0033c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00340
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00344
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00348
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00358
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0035c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00364
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00368
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0036c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00374
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x33333333,                                                  // 4 random values                             /// 00388
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0038c
			 0xffc00001,                                                  // -signaling NaN                              /// 00390
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00398
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c600000,                                                  // Leading 1s:                                 /// 003a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003e0
			 0x7f800000,                                                  // inf                                         /// 003e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00400
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00408
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00410
			 0x0c600000,                                                  // Leading 1s:                                 /// 00414
			 0x3f028f5c,                                                  // 0.51                                        /// 00418
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00430
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00434
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00438
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0043c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00444
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00448
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0044c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00450
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0045c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00460
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00464
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00468
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0047c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00480
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00488
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0048c
			 0x80000000,                                                  // -zero                                       /// 00490
			 0x3f028f5c,                                                  // 0.51                                        /// 00494
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0049c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b8
			 0x3f028f5c,                                                  // 0.51                                        /// 004bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x7fc00001,                                                  // signaling NaN                               /// 004d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00500
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00504
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00508
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0050c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00518
			 0xcb000000,                                                  // -8388608.0                                  /// 0051c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0xbf028f5c,                                                  // -0.51                                       /// 00524
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00528
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0xcb000000,                                                  // -8388608.0                                  /// 00530
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00534
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00538
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0053c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00540
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00544
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0054c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00554
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00558
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0055c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00560
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xcb000000,                                                  // -8388608.0                                  /// 0056c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00570
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00578
			 0x0c780000,                                                  // Leading 1s:                                 /// 0057c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00580
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00584
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0058c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00594
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00598
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0059c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e4
			 0x00000000,                                                  // zero                                        /// 005e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00600
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00608
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0060c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00614
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00620
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x3f028f5c,                                                  // 0.51                                        /// 0062c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00630
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00634
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0063c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xffc00001,                                                  // -signaling NaN                              /// 00644
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0064c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00650
			 0xbf028f5c,                                                  // -0.51                                       /// 00654
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00658
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0065c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00660
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00664
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00668
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00680
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00684
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00694
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0069c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00700
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e000001,                                                  // Trailing 1s:                                /// 00708
			 0x0e000007,                                                  // Trailing 1s:                                /// 0070c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00710
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00718
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00720
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00724
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0072c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00730
			 0xcb000000,                                                  // -8388608.0                                  /// 00734
			 0x0e000003,                                                  // Trailing 1s:                                /// 00738
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00744
			 0xffc00001,                                                  // -signaling NaN                              /// 00748
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0074c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00750
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00754
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00758
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0075c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00760
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00764
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00768
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0076c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00774
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00778
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0077c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00780
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00784
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00788
			 0x0c400000,                                                  // Leading 1s:                                 /// 0078c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00790
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00794
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00798
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0079c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 007b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x80000000,                                                  // -zero                                       /// 007d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00800
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00804
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00808
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00814
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00818
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0081c
			 0x33333333,                                                  // 4 random values                             /// 00820
			 0xffc00001,                                                  // -signaling NaN                              /// 00824
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00828
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0082c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00830
			 0x0e000001,                                                  // Trailing 1s:                                /// 00834
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0083c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00840
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00844
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00848
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0xffc00001,                                                  // -signaling NaN                              /// 00854
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00858
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0085c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00860
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00864
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00000000,                                                  // zero                                        /// 0086c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00870
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00874
			 0x33333333,                                                  // 4 random values                             /// 00878
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00880
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00884
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00888
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00890
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00894
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00898
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008c4
			 0x00000000,                                                  // zero                                        /// 008c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 008d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00900
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00904
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00914
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00918
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0091c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00920
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00924
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00928
			 0xff800000,                                                  // -inf                                        /// 0092c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00930
			 0x80000000,                                                  // -zero                                       /// 00934
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0093c
			 0x55555555,                                                  // 4 random values                             /// 00940
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00948
			 0x00011111,                                                  // 9.7958E-41                                  /// 0094c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00950
			 0x33333333,                                                  // 4 random values                             /// 00954
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00958
			 0x0e000007,                                                  // Trailing 1s:                                /// 0095c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00960
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00964
			 0x7fc00001,                                                  // signaling NaN                               /// 00968
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0096c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c600000,                                                  // Leading 1s:                                 /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00988
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0098c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00990
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00994
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00998
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0099c
			 0xffc00001,                                                  // -signaling NaN                              /// 009a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b0
			 0x33333333,                                                  // 4 random values                             /// 009b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x55555555,                                                  // 4 random values                             /// 009c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c8
			 0xff800000,                                                  // -inf                                        /// 009cc
			 0x55555555,                                                  // 4 random values                             /// 009d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xbf028f5c,                                                  // -0.51                                       /// 009f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a34
			 0x55555555,                                                  // 4 random values                             /// 00a38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aa4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aa8
			 0x33333333,                                                  // 4 random values                             /// 00aac
			 0x33333333,                                                  // 4 random values                             /// 00ab0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ab8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00abc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ac0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ac8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ad4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00adc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae0
			 0x33333333,                                                  // 4 random values                             /// 00ae4
			 0x33333333,                                                  // 4 random values                             /// 00ae8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00af0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00afc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0xffc00001,                                                  // -signaling NaN                              /// 00b0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b54
			 0x3f028f5c,                                                  // 0.51                                        /// 00b58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b60
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b74
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b9c
			 0x33333333,                                                  // 4 random values                             /// 00ba0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ba4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ba8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bb8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bc4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x55555555,                                                  // 4 random values                             /// 00bd0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bdc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00be0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00be4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00be8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bf4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bf8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0xff800000,                                                  // -inf                                        /// 00c24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x4b000000,                                                  // 8388608.0                                   /// 00c4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c90
			 0x00000000,                                                  // zero                                        /// 00c94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ca4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ca8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cac
			 0x33333333,                                                  // 4 random values                             /// 00cb0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ccc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cdc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ce0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d24
			 0xffc00001,                                                  // -signaling NaN                              /// 00d28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d38
			 0x33333333,                                                  // 4 random values                             /// 00d3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xff800000,                                                  // -inf                                        /// 00d44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d60
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d8c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d9c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xff800000,                                                  // -inf                                        /// 00db4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00db8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e04
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e2c
			 0x00000000,                                                  // zero                                        /// 00e30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e78
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ea0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ea4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ea8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eb4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ebc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ecc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ed8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00edc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ee0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ee8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ef0
			 0x33333333,                                                  // 4 random values                             /// 00ef4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ef8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f10
			 0x7fc00001,                                                  // signaling NaN                               /// 00f14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f40
			 0x7f800000,                                                  // inf                                         /// 00f44
			 0xff800000,                                                  // -inf                                        /// 00f48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fa0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fc8
			 0x55555555,                                                  // 4 random values                             /// 00fcc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fdc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fe4
			 0x55555555,                                                  // 4 random values                             /// 00fe8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ff0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ff8
			 0x00800001 // min norm + 1ulp                               // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xc7a8955d, /// 0x0
			 0xa871404b, /// 0x4
			 0x875d81df, /// 0x8
			 0xebcd0c7c, /// 0xc
			 0x2c784ff6, /// 0x10
			 0x73f8ebe7, /// 0x14
			 0xcbbd72ec, /// 0x18
			 0x2e38840d, /// 0x1c
			 0xaa3c4f5b, /// 0x20
			 0x15844d7f, /// 0x24
			 0x4db15881, /// 0x28
			 0x61ae9661, /// 0x2c
			 0x99ca9ead, /// 0x30
			 0x740fe6ad, /// 0x34
			 0x09873e1d, /// 0x38
			 0x5bbacc83, /// 0x3c
			 0x7182763f, /// 0x40
			 0x240e25c5, /// 0x44
			 0x5062efa9, /// 0x48
			 0x91ef30b0, /// 0x4c
			 0xf52320da, /// 0x50
			 0xd5771c83, /// 0x54
			 0x095c1b8a, /// 0x58
			 0xf1a87208, /// 0x5c
			 0x598d5fb6, /// 0x60
			 0x0fadd70d, /// 0x64
			 0x5effbbf8, /// 0x68
			 0x63bedc17, /// 0x6c
			 0x6cd7d55a, /// 0x70
			 0x018e159a, /// 0x74
			 0x6b147cbb, /// 0x78
			 0x8c055a5a, /// 0x7c
			 0xf1fdb7fa, /// 0x80
			 0x9b0d0508, /// 0x84
			 0x75856618, /// 0x88
			 0xd9ff2045, /// 0x8c
			 0xb069618c, /// 0x90
			 0xacb1f0e3, /// 0x94
			 0x4762c797, /// 0x98
			 0x58266d0d, /// 0x9c
			 0x2a480fe3, /// 0xa0
			 0x49233481, /// 0xa4
			 0xe3db54ee, /// 0xa8
			 0xfb247a47, /// 0xac
			 0x944d0637, /// 0xb0
			 0xa1cac8f6, /// 0xb4
			 0xe014c6c5, /// 0xb8
			 0x117bc2a8, /// 0xbc
			 0xa1e4e536, /// 0xc0
			 0xd8d815ca, /// 0xc4
			 0xcd67e5f7, /// 0xc8
			 0x246b5a53, /// 0xcc
			 0x3521ea02, /// 0xd0
			 0x08badbfa, /// 0xd4
			 0x723d7ee4, /// 0xd8
			 0x2900a305, /// 0xdc
			 0xf078ebd3, /// 0xe0
			 0x89888fa6, /// 0xe4
			 0x98d01674, /// 0xe8
			 0xd02b82e3, /// 0xec
			 0x5b9f7cb3, /// 0xf0
			 0x194c12b3, /// 0xf4
			 0x71a8a590, /// 0xf8
			 0x1cf1a1b1, /// 0xfc
			 0x68c5174e, /// 0x100
			 0xd2e1adc8, /// 0x104
			 0x75e1a5aa, /// 0x108
			 0x66423de5, /// 0x10c
			 0xc7dbcd90, /// 0x110
			 0xe667495f, /// 0x114
			 0xc04ca87e, /// 0x118
			 0x7ecc4bb0, /// 0x11c
			 0xb7208b27, /// 0x120
			 0xdfb7a280, /// 0x124
			 0x6f7e8c7d, /// 0x128
			 0x54c63349, /// 0x12c
			 0xdf4b0d26, /// 0x130
			 0xbb177e13, /// 0x134
			 0xade06470, /// 0x138
			 0x0028c1a1, /// 0x13c
			 0xe2c8128a, /// 0x140
			 0xa8bd722a, /// 0x144
			 0x692951ec, /// 0x148
			 0xb2f50122, /// 0x14c
			 0x4de40edb, /// 0x150
			 0xa223826e, /// 0x154
			 0xd198a65c, /// 0x158
			 0x4bbda236, /// 0x15c
			 0xc290c5a2, /// 0x160
			 0xa9ff99ef, /// 0x164
			 0x79f46cce, /// 0x168
			 0x597a03f6, /// 0x16c
			 0x5414bb99, /// 0x170
			 0x3ed9e94b, /// 0x174
			 0x46409cb2, /// 0x178
			 0xf3e9710c, /// 0x17c
			 0x5972a7c0, /// 0x180
			 0x948c2807, /// 0x184
			 0xb66bc4bb, /// 0x188
			 0xbb6f87ec, /// 0x18c
			 0x0111b3b7, /// 0x190
			 0xcd469563, /// 0x194
			 0x6b374dc2, /// 0x198
			 0x3cbcff09, /// 0x19c
			 0x3b6f8335, /// 0x1a0
			 0x032b6ab7, /// 0x1a4
			 0x501d4ae0, /// 0x1a8
			 0x21ea9a28, /// 0x1ac
			 0x6076789d, /// 0x1b0
			 0x0f56f59f, /// 0x1b4
			 0xdf220a3a, /// 0x1b8
			 0xb2213f80, /// 0x1bc
			 0x91b4fefc, /// 0x1c0
			 0xf8f74873, /// 0x1c4
			 0x46f460f7, /// 0x1c8
			 0xb10b90fe, /// 0x1cc
			 0x871c4afe, /// 0x1d0
			 0x41d726a6, /// 0x1d4
			 0x273a07b4, /// 0x1d8
			 0xe69c65be, /// 0x1dc
			 0x668817b1, /// 0x1e0
			 0xe9d7409a, /// 0x1e4
			 0x7973d664, /// 0x1e8
			 0xdaaee685, /// 0x1ec
			 0x821ed826, /// 0x1f0
			 0x42d8eb14, /// 0x1f4
			 0xeff8c566, /// 0x1f8
			 0x1cef298a, /// 0x1fc
			 0x90d27887, /// 0x200
			 0x03c13ae8, /// 0x204
			 0x199d866f, /// 0x208
			 0xbc2baae6, /// 0x20c
			 0x084185eb, /// 0x210
			 0x80c17303, /// 0x214
			 0x69ab2911, /// 0x218
			 0xbd4445c6, /// 0x21c
			 0x6dbbaf33, /// 0x220
			 0x7a28877e, /// 0x224
			 0x02512038, /// 0x228
			 0x2cade412, /// 0x22c
			 0x3ab0da21, /// 0x230
			 0x96bebf54, /// 0x234
			 0xbf84c5c6, /// 0x238
			 0x4cd8e591, /// 0x23c
			 0xdc2a8c46, /// 0x240
			 0xee51615c, /// 0x244
			 0xb935ac0f, /// 0x248
			 0xc1bbdc30, /// 0x24c
			 0xb26272b8, /// 0x250
			 0x1c798eea, /// 0x254
			 0x59f70aec, /// 0x258
			 0x14f206c4, /// 0x25c
			 0x1be3e140, /// 0x260
			 0xd39a8d9a, /// 0x264
			 0xde15c360, /// 0x268
			 0x009baa85, /// 0x26c
			 0x9df436c9, /// 0x270
			 0xc187f8c8, /// 0x274
			 0x612da2f9, /// 0x278
			 0x43d8e991, /// 0x27c
			 0xd112673e, /// 0x280
			 0x6bff1af2, /// 0x284
			 0x725e1b24, /// 0x288
			 0x4db61b33, /// 0x28c
			 0xd8c8fb48, /// 0x290
			 0xdef465ae, /// 0x294
			 0x266ca51e, /// 0x298
			 0x9f5ac012, /// 0x29c
			 0x5dba9a08, /// 0x2a0
			 0x2e845df2, /// 0x2a4
			 0x4afeb5b1, /// 0x2a8
			 0x2beecfc7, /// 0x2ac
			 0x2accb9ff, /// 0x2b0
			 0xbd270921, /// 0x2b4
			 0x49a23b90, /// 0x2b8
			 0xb37e2bf3, /// 0x2bc
			 0x545c9665, /// 0x2c0
			 0xd59e892c, /// 0x2c4
			 0xa1ed41ad, /// 0x2c8
			 0x967f6b94, /// 0x2cc
			 0xc8bc5bbb, /// 0x2d0
			 0x56aad7c3, /// 0x2d4
			 0xf1de75b1, /// 0x2d8
			 0x28df4df8, /// 0x2dc
			 0x0a41ae1f, /// 0x2e0
			 0x8a541915, /// 0x2e4
			 0xaafb3bef, /// 0x2e8
			 0x3fbe62f9, /// 0x2ec
			 0x157735bc, /// 0x2f0
			 0x784af714, /// 0x2f4
			 0x9d78c29d, /// 0x2f8
			 0x0dd672d2, /// 0x2fc
			 0x651c789c, /// 0x300
			 0xfec99291, /// 0x304
			 0x2c926729, /// 0x308
			 0xdf58b590, /// 0x30c
			 0x4a92d68d, /// 0x310
			 0x4aa00ddb, /// 0x314
			 0x0e69ff4c, /// 0x318
			 0x181079c0, /// 0x31c
			 0x9759b228, /// 0x320
			 0x2a1eded1, /// 0x324
			 0x0870562b, /// 0x328
			 0x0d2e0221, /// 0x32c
			 0x5f648fe3, /// 0x330
			 0xd03bbe10, /// 0x334
			 0x7b9bfa45, /// 0x338
			 0x8b7b8c41, /// 0x33c
			 0xc9bc705a, /// 0x340
			 0x14be0ec3, /// 0x344
			 0xb18ef729, /// 0x348
			 0x6224f65c, /// 0x34c
			 0x7a7782cf, /// 0x350
			 0x69e37f46, /// 0x354
			 0x90ca6a60, /// 0x358
			 0x8ecafbf6, /// 0x35c
			 0xad36076c, /// 0x360
			 0x681e7d08, /// 0x364
			 0x76a5d8c7, /// 0x368
			 0xebf1a255, /// 0x36c
			 0x618a378c, /// 0x370
			 0x2f63e1cf, /// 0x374
			 0x94070d34, /// 0x378
			 0x106f3830, /// 0x37c
			 0xe2c12995, /// 0x380
			 0xde3f5f64, /// 0x384
			 0x4e4b3292, /// 0x388
			 0x94566c0a, /// 0x38c
			 0x8d1c36b7, /// 0x390
			 0x3a75ca72, /// 0x394
			 0x80969936, /// 0x398
			 0x8806a4b3, /// 0x39c
			 0xd9d12cb0, /// 0x3a0
			 0x190ddd6e, /// 0x3a4
			 0xaf05407e, /// 0x3a8
			 0x168511ea, /// 0x3ac
			 0xdecfcfc6, /// 0x3b0
			 0x5d8b73bc, /// 0x3b4
			 0x1194f8ce, /// 0x3b8
			 0x5c8ab150, /// 0x3bc
			 0x14b7db2e, /// 0x3c0
			 0x74fd84b1, /// 0x3c4
			 0x98023f73, /// 0x3c8
			 0xb8cbc673, /// 0x3cc
			 0xeabbf805, /// 0x3d0
			 0x0f905db7, /// 0x3d4
			 0xbad13602, /// 0x3d8
			 0xb9c2c9b5, /// 0x3dc
			 0x72f0b636, /// 0x3e0
			 0xc8eb8180, /// 0x3e4
			 0x8c2e46e1, /// 0x3e8
			 0x987d140d, /// 0x3ec
			 0x6a05c41f, /// 0x3f0
			 0xb178f27f, /// 0x3f4
			 0xc34ff290, /// 0x3f8
			 0x19920898, /// 0x3fc
			 0xeca27b2b, /// 0x400
			 0x8b6508fa, /// 0x404
			 0x03939b63, /// 0x408
			 0x8ad01361, /// 0x40c
			 0x3e9b7a0f, /// 0x410
			 0x3623951b, /// 0x414
			 0xe43c0abf, /// 0x418
			 0x36265fa2, /// 0x41c
			 0xb1b16f75, /// 0x420
			 0x273738db, /// 0x424
			 0x9d724a5c, /// 0x428
			 0xe2b1c38b, /// 0x42c
			 0x61cd8f3c, /// 0x430
			 0x0f89fcbb, /// 0x434
			 0x1aff27cd, /// 0x438
			 0xcd2ae9ff, /// 0x43c
			 0x62b7dfcf, /// 0x440
			 0xb86ac6ef, /// 0x444
			 0x982b84cb, /// 0x448
			 0xd585f62e, /// 0x44c
			 0x5cb9c24a, /// 0x450
			 0xf64a645b, /// 0x454
			 0x07761835, /// 0x458
			 0x66ce9307, /// 0x45c
			 0xdec44fb1, /// 0x460
			 0xebc0aa5f, /// 0x464
			 0xc127e099, /// 0x468
			 0x8dc55d81, /// 0x46c
			 0xb8e42067, /// 0x470
			 0xb727faa4, /// 0x474
			 0xe4ee66f9, /// 0x478
			 0xc13d6bc2, /// 0x47c
			 0x5f38dc77, /// 0x480
			 0x4bf1f2ae, /// 0x484
			 0x9538ea65, /// 0x488
			 0x7732ed97, /// 0x48c
			 0xa4d557c1, /// 0x490
			 0xab8b990e, /// 0x494
			 0x3f0ef3d4, /// 0x498
			 0xa0c521e9, /// 0x49c
			 0x1de9adfc, /// 0x4a0
			 0xfbb10cf3, /// 0x4a4
			 0xc78a0f3d, /// 0x4a8
			 0x6719cba7, /// 0x4ac
			 0x30ad3d87, /// 0x4b0
			 0xf1499647, /// 0x4b4
			 0x41f00499, /// 0x4b8
			 0xa2936e32, /// 0x4bc
			 0xf642e0b7, /// 0x4c0
			 0xc349c98c, /// 0x4c4
			 0x8a55c44e, /// 0x4c8
			 0xe5391a35, /// 0x4cc
			 0xd18e0406, /// 0x4d0
			 0xf46a5ca9, /// 0x4d4
			 0x7be4c88a, /// 0x4d8
			 0x6aff0791, /// 0x4dc
			 0x485322a3, /// 0x4e0
			 0x14e7df9c, /// 0x4e4
			 0xbce94230, /// 0x4e8
			 0x5707afff, /// 0x4ec
			 0xe3bef727, /// 0x4f0
			 0x462ab749, /// 0x4f4
			 0xa36ab7b4, /// 0x4f8
			 0xee1ed41c, /// 0x4fc
			 0x458f713e, /// 0x500
			 0x45b1caa3, /// 0x504
			 0x7012ef76, /// 0x508
			 0x9f6ab7b3, /// 0x50c
			 0x79448bd3, /// 0x510
			 0xf9c398d5, /// 0x514
			 0xcd1c55cd, /// 0x518
			 0x53c7f094, /// 0x51c
			 0xd4f6eaa2, /// 0x520
			 0x6a061ef4, /// 0x524
			 0x3abd1b19, /// 0x528
			 0x8aa63b4f, /// 0x52c
			 0x38000b5a, /// 0x530
			 0x508ab1b0, /// 0x534
			 0x8148861f, /// 0x538
			 0x62c14601, /// 0x53c
			 0x0ec0b8a3, /// 0x540
			 0xafc855c8, /// 0x544
			 0x7c8bf45f, /// 0x548
			 0x48a60a20, /// 0x54c
			 0x27de76f0, /// 0x550
			 0x4a0830dd, /// 0x554
			 0x67792476, /// 0x558
			 0x5424c770, /// 0x55c
			 0xd860effa, /// 0x560
			 0x455dbc34, /// 0x564
			 0x3c81581b, /// 0x568
			 0xb3e3f5fa, /// 0x56c
			 0xc232552b, /// 0x570
			 0x4e615830, /// 0x574
			 0x2072e1c2, /// 0x578
			 0x370d1263, /// 0x57c
			 0x9bc670aa, /// 0x580
			 0x42280d8e, /// 0x584
			 0x692b3824, /// 0x588
			 0x138743d5, /// 0x58c
			 0x7f266cb7, /// 0x590
			 0xb2bc753c, /// 0x594
			 0xd291c3d3, /// 0x598
			 0xd9fc8f57, /// 0x59c
			 0xb2457cd5, /// 0x5a0
			 0x0e85a50e, /// 0x5a4
			 0x27fa7304, /// 0x5a8
			 0xade337a9, /// 0x5ac
			 0x36244a2c, /// 0x5b0
			 0xf7cffbfa, /// 0x5b4
			 0xe813a905, /// 0x5b8
			 0x6861acbb, /// 0x5bc
			 0x8884aa24, /// 0x5c0
			 0x47f85483, /// 0x5c4
			 0xaceaa290, /// 0x5c8
			 0xeca75f5d, /// 0x5cc
			 0xc3a63389, /// 0x5d0
			 0x37d34473, /// 0x5d4
			 0xfd06af2f, /// 0x5d8
			 0x004afb74, /// 0x5dc
			 0x68813572, /// 0x5e0
			 0x4943bd4c, /// 0x5e4
			 0x6919997e, /// 0x5e8
			 0x9b3db3b5, /// 0x5ec
			 0x993bedbf, /// 0x5f0
			 0xe2a12ce1, /// 0x5f4
			 0x00599db3, /// 0x5f8
			 0xbb058dae, /// 0x5fc
			 0x6c5175be, /// 0x600
			 0x3574b5cd, /// 0x604
			 0x43391ea0, /// 0x608
			 0x7c2b4bab, /// 0x60c
			 0x3587074f, /// 0x610
			 0xaed6adf1, /// 0x614
			 0x74d5991b, /// 0x618
			 0xa56f7783, /// 0x61c
			 0xfcdf56dc, /// 0x620
			 0x0da28db6, /// 0x624
			 0x300b5a3f, /// 0x628
			 0xfd036848, /// 0x62c
			 0xbb3e7ebb, /// 0x630
			 0xc11efc38, /// 0x634
			 0xca2372e8, /// 0x638
			 0x1fbce81b, /// 0x63c
			 0xa50f524a, /// 0x640
			 0x56e7f853, /// 0x644
			 0x9521c137, /// 0x648
			 0x24dceb1c, /// 0x64c
			 0x158136eb, /// 0x650
			 0x8b8de4d0, /// 0x654
			 0x560de012, /// 0x658
			 0x7383ae3a, /// 0x65c
			 0xd4ffa82e, /// 0x660
			 0x7dc8e195, /// 0x664
			 0x800d82aa, /// 0x668
			 0xa4505c2e, /// 0x66c
			 0x5890b7bb, /// 0x670
			 0x6c3fba4c, /// 0x674
			 0x31e2f9f0, /// 0x678
			 0x6e5b23f0, /// 0x67c
			 0x166b4f6a, /// 0x680
			 0xafb3a6fa, /// 0x684
			 0x2e04a834, /// 0x688
			 0x4dab3f95, /// 0x68c
			 0x78986114, /// 0x690
			 0xa4bf67cb, /// 0x694
			 0xf7b78fcf, /// 0x698
			 0x794ee15a, /// 0x69c
			 0xd894e2ea, /// 0x6a0
			 0x72394d26, /// 0x6a4
			 0x71f5c070, /// 0x6a8
			 0x7e67eded, /// 0x6ac
			 0x2d5091fe, /// 0x6b0
			 0xbd66f501, /// 0x6b4
			 0xd5d3b309, /// 0x6b8
			 0xb84bd1ea, /// 0x6bc
			 0xdb6d5fdb, /// 0x6c0
			 0x3ae779f3, /// 0x6c4
			 0xe28e674d, /// 0x6c8
			 0xe654580f, /// 0x6cc
			 0x430fdc8e, /// 0x6d0
			 0xcf862f00, /// 0x6d4
			 0x3d3996bb, /// 0x6d8
			 0xf2a5c72e, /// 0x6dc
			 0xa89df30f, /// 0x6e0
			 0x9f52956b, /// 0x6e4
			 0xf697bcba, /// 0x6e8
			 0xf8a4947f, /// 0x6ec
			 0xdddfe975, /// 0x6f0
			 0x66c01cd6, /// 0x6f4
			 0xcc7d4782, /// 0x6f8
			 0xe539ebcf, /// 0x6fc
			 0x02d0506e, /// 0x700
			 0x484aad7a, /// 0x704
			 0xbda18ef7, /// 0x708
			 0x29fc6c49, /// 0x70c
			 0x763b27ea, /// 0x710
			 0x51493fd1, /// 0x714
			 0x2578400f, /// 0x718
			 0xf58f64dd, /// 0x71c
			 0xb9c0e794, /// 0x720
			 0x09b5a3e8, /// 0x724
			 0xbd32bbd5, /// 0x728
			 0x75db1588, /// 0x72c
			 0xec83f006, /// 0x730
			 0x69b5960b, /// 0x734
			 0x3ddf10b1, /// 0x738
			 0x1d38b834, /// 0x73c
			 0xe0aac7ad, /// 0x740
			 0x5db70ce3, /// 0x744
			 0xc111e246, /// 0x748
			 0x1dd311bd, /// 0x74c
			 0x04974d29, /// 0x750
			 0x95aceda1, /// 0x754
			 0x20fdac01, /// 0x758
			 0x92dbe575, /// 0x75c
			 0x1e6a9c3f, /// 0x760
			 0x2636008d, /// 0x764
			 0x83d76dbf, /// 0x768
			 0x31ccfd28, /// 0x76c
			 0x7b2220ca, /// 0x770
			 0x5fba4286, /// 0x774
			 0xf6bee2b7, /// 0x778
			 0xe0fab442, /// 0x77c
			 0x6009cb7e, /// 0x780
			 0x09561e73, /// 0x784
			 0xedf85757, /// 0x788
			 0xe26c60d4, /// 0x78c
			 0x34ac33a5, /// 0x790
			 0x511acc2f, /// 0x794
			 0x2691470f, /// 0x798
			 0x954cb07b, /// 0x79c
			 0x94a1f467, /// 0x7a0
			 0xe6d4ed2a, /// 0x7a4
			 0x5321da0c, /// 0x7a8
			 0xc3340e50, /// 0x7ac
			 0x30b4cf8d, /// 0x7b0
			 0x13031b93, /// 0x7b4
			 0x43da28af, /// 0x7b8
			 0xebaf4483, /// 0x7bc
			 0x31eed260, /// 0x7c0
			 0xe9bcc377, /// 0x7c4
			 0x08fd7af7, /// 0x7c8
			 0x4cb0f4e4, /// 0x7cc
			 0x443cedac, /// 0x7d0
			 0x7dc9f36a, /// 0x7d4
			 0xe7940380, /// 0x7d8
			 0xb239c964, /// 0x7dc
			 0x05793908, /// 0x7e0
			 0x2000ecd2, /// 0x7e4
			 0xea739cfa, /// 0x7e8
			 0x13bf1eee, /// 0x7ec
			 0xa5581384, /// 0x7f0
			 0xb44133ef, /// 0x7f4
			 0xaea2d3c2, /// 0x7f8
			 0x19ff1762, /// 0x7fc
			 0x5aeb5eea, /// 0x800
			 0x628cc890, /// 0x804
			 0x16bd4c83, /// 0x808
			 0x4099d005, /// 0x80c
			 0xa8792465, /// 0x810
			 0x0a760f96, /// 0x814
			 0x0df74894, /// 0x818
			 0xba02219a, /// 0x81c
			 0xc33d865d, /// 0x820
			 0xa30a2967, /// 0x824
			 0xb37a3050, /// 0x828
			 0x3ebbf98d, /// 0x82c
			 0xb3c926ee, /// 0x830
			 0xf8ade49e, /// 0x834
			 0xbe5d3fef, /// 0x838
			 0xe0f007b4, /// 0x83c
			 0x1aad22c8, /// 0x840
			 0xa3994f21, /// 0x844
			 0xc92a5bfb, /// 0x848
			 0x7123a66d, /// 0x84c
			 0x4c8146a7, /// 0x850
			 0x32cd5386, /// 0x854
			 0x7714b896, /// 0x858
			 0x1dc5bb72, /// 0x85c
			 0xdc359349, /// 0x860
			 0x382a59ea, /// 0x864
			 0x053e48ac, /// 0x868
			 0x64974597, /// 0x86c
			 0x10e147d1, /// 0x870
			 0x048e489f, /// 0x874
			 0x4d76bfa0, /// 0x878
			 0x4c5bf8fc, /// 0x87c
			 0xdfc3f86a, /// 0x880
			 0x1655ef76, /// 0x884
			 0x308d6acc, /// 0x888
			 0x7adf9a06, /// 0x88c
			 0xde3f902c, /// 0x890
			 0x7ae2c771, /// 0x894
			 0xe5ca031a, /// 0x898
			 0xc4c641ed, /// 0x89c
			 0xa77ac6b5, /// 0x8a0
			 0xefbcb096, /// 0x8a4
			 0xbab96a0d, /// 0x8a8
			 0xb854f8f2, /// 0x8ac
			 0x09a138de, /// 0x8b0
			 0x810d4c12, /// 0x8b4
			 0xaa1e089d, /// 0x8b8
			 0x6d764034, /// 0x8bc
			 0x95db8e0c, /// 0x8c0
			 0x9cbb5971, /// 0x8c4
			 0x408580ba, /// 0x8c8
			 0x76465fc9, /// 0x8cc
			 0x10d7cf40, /// 0x8d0
			 0x8f8f9156, /// 0x8d4
			 0xb41693a0, /// 0x8d8
			 0x49524595, /// 0x8dc
			 0x439cc875, /// 0x8e0
			 0xbd4bdbe8, /// 0x8e4
			 0x51cb3875, /// 0x8e8
			 0xdb4471ee, /// 0x8ec
			 0x056347d9, /// 0x8f0
			 0xbd1078d5, /// 0x8f4
			 0xa99f76a4, /// 0x8f8
			 0x4855597c, /// 0x8fc
			 0xc2e9f833, /// 0x900
			 0x1c8b8e13, /// 0x904
			 0xcdfc2fa5, /// 0x908
			 0xed8efb87, /// 0x90c
			 0x65a0c5b1, /// 0x910
			 0x0f6097cc, /// 0x914
			 0x08172c21, /// 0x918
			 0x0d9f3967, /// 0x91c
			 0xb1643d26, /// 0x920
			 0x27a4ce89, /// 0x924
			 0x3618b17a, /// 0x928
			 0x4779d99a, /// 0x92c
			 0x65229667, /// 0x930
			 0x193678de, /// 0x934
			 0xa97aa57a, /// 0x938
			 0x14108fc8, /// 0x93c
			 0x26326164, /// 0x940
			 0x78d4f50e, /// 0x944
			 0x7a47e324, /// 0x948
			 0x88e9b883, /// 0x94c
			 0xb121c3e2, /// 0x950
			 0xead9dcb6, /// 0x954
			 0x43a257e4, /// 0x958
			 0xc3f11fe1, /// 0x95c
			 0xb2145055, /// 0x960
			 0x3462f9b8, /// 0x964
			 0x3b746730, /// 0x968
			 0xe871530f, /// 0x96c
			 0x85aeb406, /// 0x970
			 0x70dfb166, /// 0x974
			 0xa4f39bf8, /// 0x978
			 0x31044e7d, /// 0x97c
			 0x94d20a5d, /// 0x980
			 0x8b8e6f6d, /// 0x984
			 0xd4192f8b, /// 0x988
			 0xcb699de0, /// 0x98c
			 0x720ca5e3, /// 0x990
			 0xdc56fcc7, /// 0x994
			 0xd4a04a01, /// 0x998
			 0xa3e86810, /// 0x99c
			 0xdbc47b2e, /// 0x9a0
			 0xeaf0fb54, /// 0x9a4
			 0x435886fb, /// 0x9a8
			 0x086df6f5, /// 0x9ac
			 0xd0508e1b, /// 0x9b0
			 0x15fffa25, /// 0x9b4
			 0x85ad667b, /// 0x9b8
			 0x94d630b6, /// 0x9bc
			 0xfe0359b0, /// 0x9c0
			 0x50490fec, /// 0x9c4
			 0x1be04790, /// 0x9c8
			 0x629d9bab, /// 0x9cc
			 0x6b416b86, /// 0x9d0
			 0x96a66493, /// 0x9d4
			 0x0d5b443f, /// 0x9d8
			 0x6618496f, /// 0x9dc
			 0x7ca650ae, /// 0x9e0
			 0xa6bad0a9, /// 0x9e4
			 0xf4d9a94d, /// 0x9e8
			 0xb5d0836c, /// 0x9ec
			 0xd98980c3, /// 0x9f0
			 0xb8b1c98b, /// 0x9f4
			 0x74406cc1, /// 0x9f8
			 0x66f46dbe, /// 0x9fc
			 0x595387ed, /// 0xa00
			 0x231e7ff4, /// 0xa04
			 0x4ee56bfd, /// 0xa08
			 0x2baa2996, /// 0xa0c
			 0x1d554cf6, /// 0xa10
			 0x34243101, /// 0xa14
			 0x1aef9876, /// 0xa18
			 0x29c707d6, /// 0xa1c
			 0x6c7e8373, /// 0xa20
			 0x95cdfd5f, /// 0xa24
			 0xbaf8c11a, /// 0xa28
			 0xdc2f098a, /// 0xa2c
			 0x1851b59d, /// 0xa30
			 0x601fb30f, /// 0xa34
			 0x9d46b523, /// 0xa38
			 0xb7699b2b, /// 0xa3c
			 0xa9ef125d, /// 0xa40
			 0x5236aea3, /// 0xa44
			 0x16967129, /// 0xa48
			 0xaf6daf58, /// 0xa4c
			 0x1255eb77, /// 0xa50
			 0x2ccc3391, /// 0xa54
			 0x860c89a8, /// 0xa58
			 0xa5262965, /// 0xa5c
			 0x67108913, /// 0xa60
			 0xc8d20b1b, /// 0xa64
			 0x9ba7a182, /// 0xa68
			 0x6940b6e3, /// 0xa6c
			 0x8d785def, /// 0xa70
			 0x6c8d2c86, /// 0xa74
			 0x8a2033d8, /// 0xa78
			 0x0c3ab142, /// 0xa7c
			 0x21f716fc, /// 0xa80
			 0xc8afdc49, /// 0xa84
			 0x48110d5b, /// 0xa88
			 0xdc531686, /// 0xa8c
			 0x02f8c244, /// 0xa90
			 0x8aae64dd, /// 0xa94
			 0x28aeebc3, /// 0xa98
			 0x13a782cc, /// 0xa9c
			 0x1bfda666, /// 0xaa0
			 0xc3417cea, /// 0xaa4
			 0x428f5b88, /// 0xaa8
			 0x62b5be58, /// 0xaac
			 0xf2f60a41, /// 0xab0
			 0xd9d77682, /// 0xab4
			 0xaf76026c, /// 0xab8
			 0x280a57a5, /// 0xabc
			 0xffb37ca8, /// 0xac0
			 0x7cd341b2, /// 0xac4
			 0x6f721c04, /// 0xac8
			 0x1900ce48, /// 0xacc
			 0xde1364cb, /// 0xad0
			 0xa5e5d7c0, /// 0xad4
			 0x0910eaaf, /// 0xad8
			 0x5a030eab, /// 0xadc
			 0xe742bd6a, /// 0xae0
			 0x7ed25d6d, /// 0xae4
			 0xd9967937, /// 0xae8
			 0xb7dc115a, /// 0xaec
			 0x7b8c931d, /// 0xaf0
			 0x8a026537, /// 0xaf4
			 0x64b00b58, /// 0xaf8
			 0xff2656c4, /// 0xafc
			 0x40730e6d, /// 0xb00
			 0xbc1bfd65, /// 0xb04
			 0xe74da36c, /// 0xb08
			 0x551d251e, /// 0xb0c
			 0xf098e55c, /// 0xb10
			 0x31c5c2cd, /// 0xb14
			 0x1b89fa38, /// 0xb18
			 0x823c615b, /// 0xb1c
			 0x95636140, /// 0xb20
			 0x579ac9a5, /// 0xb24
			 0x9d9639c4, /// 0xb28
			 0x2dffd41a, /// 0xb2c
			 0x69137007, /// 0xb30
			 0x3fec7f54, /// 0xb34
			 0x5ee27055, /// 0xb38
			 0x4b56f762, /// 0xb3c
			 0x31db2e3e, /// 0xb40
			 0x1f44ca0e, /// 0xb44
			 0x232a0062, /// 0xb48
			 0x665ee88c, /// 0xb4c
			 0x58890869, /// 0xb50
			 0x24a80c6d, /// 0xb54
			 0xda5a52b0, /// 0xb58
			 0x8efd31ee, /// 0xb5c
			 0x98221139, /// 0xb60
			 0xacf27c75, /// 0xb64
			 0x55ee6b5d, /// 0xb68
			 0x33d37820, /// 0xb6c
			 0x837d0774, /// 0xb70
			 0xd1181e99, /// 0xb74
			 0x3d129a6e, /// 0xb78
			 0x75253900, /// 0xb7c
			 0xdd634f6b, /// 0xb80
			 0x577e521d, /// 0xb84
			 0x8a5a0e87, /// 0xb88
			 0x2002f2b5, /// 0xb8c
			 0xcf738e3d, /// 0xb90
			 0x0984d44f, /// 0xb94
			 0xdfcfd3f9, /// 0xb98
			 0x059cdc5c, /// 0xb9c
			 0x360c4588, /// 0xba0
			 0xe54888d6, /// 0xba4
			 0x689c8726, /// 0xba8
			 0x2e66ebd3, /// 0xbac
			 0x9fd62722, /// 0xbb0
			 0xb515aa5c, /// 0xbb4
			 0x024fac6d, /// 0xbb8
			 0x9a81d8e3, /// 0xbbc
			 0x23a26c7a, /// 0xbc0
			 0x8fa7912f, /// 0xbc4
			 0xd12ad6a1, /// 0xbc8
			 0x912f7701, /// 0xbcc
			 0x8f3d8429, /// 0xbd0
			 0xb116a750, /// 0xbd4
			 0x2713aaed, /// 0xbd8
			 0x96dc5247, /// 0xbdc
			 0xe4b0a873, /// 0xbe0
			 0x6a5b692b, /// 0xbe4
			 0xb040660d, /// 0xbe8
			 0xdfa52f3d, /// 0xbec
			 0xa0f9bea2, /// 0xbf0
			 0x244185a8, /// 0xbf4
			 0x94c8e3c6, /// 0xbf8
			 0x16e94bfd, /// 0xbfc
			 0x0a6fe8de, /// 0xc00
			 0xc3b6a78e, /// 0xc04
			 0x93425a80, /// 0xc08
			 0x467825ff, /// 0xc0c
			 0x001d6571, /// 0xc10
			 0x30edcb79, /// 0xc14
			 0x8bf2ea28, /// 0xc18
			 0x542f41a2, /// 0xc1c
			 0xf9e21c08, /// 0xc20
			 0xb4024a68, /// 0xc24
			 0x65f49bba, /// 0xc28
			 0x950bbe02, /// 0xc2c
			 0xb8a642b1, /// 0xc30
			 0x84b84932, /// 0xc34
			 0x2dbfae26, /// 0xc38
			 0x874a61c3, /// 0xc3c
			 0x000ca2fd, /// 0xc40
			 0x501c7636, /// 0xc44
			 0x96fe5cfd, /// 0xc48
			 0x0fbf4969, /// 0xc4c
			 0x9f141d39, /// 0xc50
			 0x099493ae, /// 0xc54
			 0x5c32e950, /// 0xc58
			 0x37db6521, /// 0xc5c
			 0x313080ad, /// 0xc60
			 0x0328e306, /// 0xc64
			 0x6e51d4d6, /// 0xc68
			 0x2c3d0ec6, /// 0xc6c
			 0xd6a10b32, /// 0xc70
			 0x82dded5f, /// 0xc74
			 0xe2090c92, /// 0xc78
			 0xdbd2aa89, /// 0xc7c
			 0x3e26e9b7, /// 0xc80
			 0xfe8718dd, /// 0xc84
			 0xf90a2b8b, /// 0xc88
			 0x719b2865, /// 0xc8c
			 0xc4958944, /// 0xc90
			 0x1585e8d7, /// 0xc94
			 0x09466be8, /// 0xc98
			 0xbcf90623, /// 0xc9c
			 0x35d1986a, /// 0xca0
			 0x637d4a52, /// 0xca4
			 0xebfe0af0, /// 0xca8
			 0x057c4ddd, /// 0xcac
			 0x2dc4ce82, /// 0xcb0
			 0xbd8aed9c, /// 0xcb4
			 0xbe55f798, /// 0xcb8
			 0x0644c3fb, /// 0xcbc
			 0xdaf9e7ac, /// 0xcc0
			 0x7a43a7fd, /// 0xcc4
			 0xccfb9b34, /// 0xcc8
			 0xd38e5e88, /// 0xccc
			 0x7be60435, /// 0xcd0
			 0x7eaa31c3, /// 0xcd4
			 0xc9a79816, /// 0xcd8
			 0xa776ce36, /// 0xcdc
			 0x15f6c720, /// 0xce0
			 0xbf395316, /// 0xce4
			 0x35819d0c, /// 0xce8
			 0x75acf5ed, /// 0xcec
			 0x2ed7bd84, /// 0xcf0
			 0xa3026e9c, /// 0xcf4
			 0x41581d79, /// 0xcf8
			 0x49a25524, /// 0xcfc
			 0x10c4bf04, /// 0xd00
			 0x6fc1d989, /// 0xd04
			 0xd5d432ec, /// 0xd08
			 0x4cbbf547, /// 0xd0c
			 0xcf8f7348, /// 0xd10
			 0xd862fe1c, /// 0xd14
			 0x7f4e9c06, /// 0xd18
			 0x34cb2f8e, /// 0xd1c
			 0x6e50f080, /// 0xd20
			 0xd824bc70, /// 0xd24
			 0xb754df18, /// 0xd28
			 0x34a082bc, /// 0xd2c
			 0x45186bdb, /// 0xd30
			 0xb650df0b, /// 0xd34
			 0x2f78bb86, /// 0xd38
			 0xa9b157d1, /// 0xd3c
			 0x081651a9, /// 0xd40
			 0x8612f1c1, /// 0xd44
			 0xb952a6b2, /// 0xd48
			 0xbe1c7b95, /// 0xd4c
			 0xbd7dcfea, /// 0xd50
			 0xad537cf8, /// 0xd54
			 0xdc35d0f6, /// 0xd58
			 0x14706684, /// 0xd5c
			 0x86f8b094, /// 0xd60
			 0xfaf0f9cb, /// 0xd64
			 0xbecaef95, /// 0xd68
			 0x9721ba33, /// 0xd6c
			 0xf06f89fe, /// 0xd70
			 0x65eb9286, /// 0xd74
			 0xab19d073, /// 0xd78
			 0xb8dcb989, /// 0xd7c
			 0x9e46c7fd, /// 0xd80
			 0xc86ecb13, /// 0xd84
			 0x8991ab04, /// 0xd88
			 0x0f0a53ef, /// 0xd8c
			 0x87987cdf, /// 0xd90
			 0xc9032914, /// 0xd94
			 0x30f226b6, /// 0xd98
			 0xf8fe783d, /// 0xd9c
			 0x85e6ecfa, /// 0xda0
			 0x8d19f383, /// 0xda4
			 0xbe888608, /// 0xda8
			 0xe70c6c58, /// 0xdac
			 0x72685e2b, /// 0xdb0
			 0xb6f9f460, /// 0xdb4
			 0xc4e3c9ef, /// 0xdb8
			 0x13c284ea, /// 0xdbc
			 0xb05d4056, /// 0xdc0
			 0xee220797, /// 0xdc4
			 0x6c0c12b1, /// 0xdc8
			 0x7a785da5, /// 0xdcc
			 0x024ec820, /// 0xdd0
			 0x53ffdd24, /// 0xdd4
			 0xecc99ec7, /// 0xdd8
			 0x87079cf8, /// 0xddc
			 0x309b2f00, /// 0xde0
			 0xab53913a, /// 0xde4
			 0xf5b4be7a, /// 0xde8
			 0x299b6c55, /// 0xdec
			 0x81054385, /// 0xdf0
			 0xde6e7dc8, /// 0xdf4
			 0x161eb0c0, /// 0xdf8
			 0x83f80866, /// 0xdfc
			 0x0255f6f9, /// 0xe00
			 0x71d2e093, /// 0xe04
			 0xed83abfa, /// 0xe08
			 0x5a79fba7, /// 0xe0c
			 0xdebbbd5b, /// 0xe10
			 0x3d92b7ad, /// 0xe14
			 0x613f85da, /// 0xe18
			 0xfc15c48f, /// 0xe1c
			 0x29e5059b, /// 0xe20
			 0xed997a70, /// 0xe24
			 0x770e1a42, /// 0xe28
			 0x620cca3f, /// 0xe2c
			 0x94ae56b4, /// 0xe30
			 0x01a18bb6, /// 0xe34
			 0x5f734b70, /// 0xe38
			 0xc0b9d6af, /// 0xe3c
			 0xb025f3f4, /// 0xe40
			 0x268bad3b, /// 0xe44
			 0x72f77646, /// 0xe48
			 0x1e4d0509, /// 0xe4c
			 0xa7a3ce46, /// 0xe50
			 0xad396c12, /// 0xe54
			 0x3d41c68e, /// 0xe58
			 0x13228e76, /// 0xe5c
			 0x2e971046, /// 0xe60
			 0x4c34931b, /// 0xe64
			 0xfd177067, /// 0xe68
			 0x4bf95fac, /// 0xe6c
			 0x3e576616, /// 0xe70
			 0x447b5442, /// 0xe74
			 0xf508999e, /// 0xe78
			 0x45119daf, /// 0xe7c
			 0xeb8bf2dd, /// 0xe80
			 0xd194f1f7, /// 0xe84
			 0x758c6f73, /// 0xe88
			 0xa879f349, /// 0xe8c
			 0xda24ed29, /// 0xe90
			 0x41a9a688, /// 0xe94
			 0xa885cdb3, /// 0xe98
			 0x603f64ff, /// 0xe9c
			 0x99109329, /// 0xea0
			 0x4d970cb4, /// 0xea4
			 0x1946585b, /// 0xea8
			 0x573ac802, /// 0xeac
			 0x32beaafe, /// 0xeb0
			 0x9e7c6116, /// 0xeb4
			 0x1a213fd7, /// 0xeb8
			 0x09ff4264, /// 0xebc
			 0xe5650f33, /// 0xec0
			 0x6771df0a, /// 0xec4
			 0xa6856cc7, /// 0xec8
			 0x41430383, /// 0xecc
			 0x66e792f6, /// 0xed0
			 0xe2d817a2, /// 0xed4
			 0xd23bff05, /// 0xed8
			 0xb4f61fe9, /// 0xedc
			 0x7c96b49e, /// 0xee0
			 0x25ed8fa2, /// 0xee4
			 0x07c0d60b, /// 0xee8
			 0xbc078187, /// 0xeec
			 0x35616b15, /// 0xef0
			 0x8ed55ac5, /// 0xef4
			 0x5451c403, /// 0xef8
			 0x2cbcccf4, /// 0xefc
			 0xc256cc44, /// 0xf00
			 0x449f16d0, /// 0xf04
			 0x0d27da2e, /// 0xf08
			 0xb287c92e, /// 0xf0c
			 0x5ede7958, /// 0xf10
			 0x98002423, /// 0xf14
			 0x2b1f1660, /// 0xf18
			 0xfaf8b1a8, /// 0xf1c
			 0xe5d820fa, /// 0xf20
			 0xa45d2131, /// 0xf24
			 0x174236d8, /// 0xf28
			 0x06f7d182, /// 0xf2c
			 0x2a324b39, /// 0xf30
			 0xacf1c049, /// 0xf34
			 0xb50f1a83, /// 0xf38
			 0x3c469006, /// 0xf3c
			 0x022ae5d8, /// 0xf40
			 0x2a88e19f, /// 0xf44
			 0x2bc36d86, /// 0xf48
			 0x896c71ed, /// 0xf4c
			 0x5824084b, /// 0xf50
			 0x8cf1609d, /// 0xf54
			 0x6c09de42, /// 0xf58
			 0x33cbc1ff, /// 0xf5c
			 0xa04df642, /// 0xf60
			 0xab189a84, /// 0xf64
			 0x160c0164, /// 0xf68
			 0x08d0e3d1, /// 0xf6c
			 0xaf9d98db, /// 0xf70
			 0xe7a2982f, /// 0xf74
			 0xd0c74c1e, /// 0xf78
			 0x4d6bf150, /// 0xf7c
			 0x2ffb0844, /// 0xf80
			 0xf5ea80d4, /// 0xf84
			 0x8c7085ad, /// 0xf88
			 0x5da4156e, /// 0xf8c
			 0xa1909c69, /// 0xf90
			 0xdaba04df, /// 0xf94
			 0xbefdd82a, /// 0xf98
			 0x4dc45b94, /// 0xf9c
			 0xf83ded74, /// 0xfa0
			 0x1299aa6a, /// 0xfa4
			 0x2d723587, /// 0xfa8
			 0x033e4502, /// 0xfac
			 0x3f530875, /// 0xfb0
			 0xd6601baf, /// 0xfb4
			 0x91eb9908, /// 0xfb8
			 0xf97a7861, /// 0xfbc
			 0x1e4a9843, /// 0xfc0
			 0xc5ce83bb, /// 0xfc4
			 0x3fe243b4, /// 0xfc8
			 0x573260f9, /// 0xfcc
			 0xd489cc31, /// 0xfd0
			 0xca7f863f, /// 0xfd4
			 0xa4d4fefe, /// 0xfd8
			 0xffc6d0e9, /// 0xfdc
			 0x52b43dab, /// 0xfe0
			 0x87e4eb11, /// 0xfe4
			 0x9062b53b, /// 0xfe8
			 0x41342705, /// 0xfec
			 0x97f5388e, /// 0xff0
			 0xe25524a1, /// 0xff4
			 0xcc472d4c, /// 0xff8
			 0x0747e518 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xe7f5c551, /// 0x0
			 0x18774375, /// 0x4
			 0x4c2ee2b8, /// 0x8
			 0xc695e849, /// 0xc
			 0xd7c9bcdc, /// 0x10
			 0x4ff0899f, /// 0x14
			 0x5524bff9, /// 0x18
			 0xaf8b3980, /// 0x1c
			 0x8950dd30, /// 0x20
			 0xedeefc24, /// 0x24
			 0xcd6cd345, /// 0x28
			 0xbef56e72, /// 0x2c
			 0x3d80acf1, /// 0x30
			 0xfde5ce7a, /// 0x34
			 0x3c08e6b5, /// 0x38
			 0x728d4d9f, /// 0x3c
			 0xe8bafd38, /// 0x40
			 0x3dffbdf9, /// 0x44
			 0x7b9757eb, /// 0x48
			 0x04995b5e, /// 0x4c
			 0xd46a11b2, /// 0x50
			 0x92eaee6f, /// 0x54
			 0x0c666c21, /// 0x58
			 0x48815355, /// 0x5c
			 0x95f1a2d9, /// 0x60
			 0xccfba71c, /// 0x64
			 0x2e0df484, /// 0x68
			 0x46faa62e, /// 0x6c
			 0x4b28eb17, /// 0x70
			 0xae193079, /// 0x74
			 0xf541433d, /// 0x78
			 0x8c3eb5e4, /// 0x7c
			 0xe81f3003, /// 0x80
			 0x9d0a4bc8, /// 0x84
			 0x05fa988e, /// 0x88
			 0x0678895e, /// 0x8c
			 0xcced318e, /// 0x90
			 0x13b7e2bc, /// 0x94
			 0xc912bf93, /// 0x98
			 0x3b17966e, /// 0x9c
			 0x74509f7d, /// 0xa0
			 0x0ee0463b, /// 0xa4
			 0x10377ac8, /// 0xa8
			 0xf9ee0002, /// 0xac
			 0x15543c62, /// 0xb0
			 0x7d6e0763, /// 0xb4
			 0x224c2575, /// 0xb8
			 0x1f18dfea, /// 0xbc
			 0x285e9a05, /// 0xc0
			 0x4dc4e7a7, /// 0xc4
			 0x11f090bc, /// 0xc8
			 0x11f18b5b, /// 0xcc
			 0x6a808364, /// 0xd0
			 0x96fb267d, /// 0xd4
			 0x9e0ceb43, /// 0xd8
			 0x333143d1, /// 0xdc
			 0x9b79f1c5, /// 0xe0
			 0x8bc58ab9, /// 0xe4
			 0x542cd14f, /// 0xe8
			 0xd6e6db64, /// 0xec
			 0x76f55477, /// 0xf0
			 0x2942ce52, /// 0xf4
			 0x7ee99c34, /// 0xf8
			 0x6d31421f, /// 0xfc
			 0x5b4e5151, /// 0x100
			 0x453f50b9, /// 0x104
			 0x817dbd8e, /// 0x108
			 0x1d7d846c, /// 0x10c
			 0xfebf3ff6, /// 0x110
			 0xb82e1110, /// 0x114
			 0x3ef9563b, /// 0x118
			 0x79ed8149, /// 0x11c
			 0x726d2c10, /// 0x120
			 0x90a41b97, /// 0x124
			 0x3f98631d, /// 0x128
			 0x8afe9af6, /// 0x12c
			 0x9763f082, /// 0x130
			 0x4f7d5344, /// 0x134
			 0xc85d1562, /// 0x138
			 0xeaa1a691, /// 0x13c
			 0x9fd04ace, /// 0x140
			 0xd219c8cb, /// 0x144
			 0x8cb467d1, /// 0x148
			 0xac77ec66, /// 0x14c
			 0x45c61a04, /// 0x150
			 0xf5f96937, /// 0x154
			 0x4b5e8c90, /// 0x158
			 0x0e7bac9d, /// 0x15c
			 0x6d2c2cb3, /// 0x160
			 0xd1cfa84a, /// 0x164
			 0x5f3fda3b, /// 0x168
			 0xc0728db8, /// 0x16c
			 0xd550eefd, /// 0x170
			 0x5fab4f94, /// 0x174
			 0x7cb4c3b6, /// 0x178
			 0xf442e1b7, /// 0x17c
			 0xcfa65c85, /// 0x180
			 0xe317a4df, /// 0x184
			 0xbf2ac623, /// 0x188
			 0x11fb01ff, /// 0x18c
			 0xc791399b, /// 0x190
			 0xe81be7a3, /// 0x194
			 0x23eabc81, /// 0x198
			 0x214ff908, /// 0x19c
			 0xbc05bfef, /// 0x1a0
			 0x3bd5d36a, /// 0x1a4
			 0x254991cd, /// 0x1a8
			 0x95d109d7, /// 0x1ac
			 0xb3ecec94, /// 0x1b0
			 0xca310414, /// 0x1b4
			 0x3605e85d, /// 0x1b8
			 0x5d0a31cd, /// 0x1bc
			 0x89e2b2c3, /// 0x1c0
			 0xf51f4244, /// 0x1c4
			 0xd120a16b, /// 0x1c8
			 0xaef9448d, /// 0x1cc
			 0xc3da3ad8, /// 0x1d0
			 0x2bf88d2a, /// 0x1d4
			 0x07a96c56, /// 0x1d8
			 0x8ebc701b, /// 0x1dc
			 0xa13a5932, /// 0x1e0
			 0x66c63071, /// 0x1e4
			 0x21704342, /// 0x1e8
			 0x365e78c0, /// 0x1ec
			 0x236185ba, /// 0x1f0
			 0xee174b26, /// 0x1f4
			 0xcbaeca69, /// 0x1f8
			 0x69ddbb75, /// 0x1fc
			 0xf97a5629, /// 0x200
			 0x0cbc84a5, /// 0x204
			 0xf0ccb20a, /// 0x208
			 0xce99cd9d, /// 0x20c
			 0xde2f5cb8, /// 0x210
			 0xc2d357ef, /// 0x214
			 0xce11363c, /// 0x218
			 0x43aca85b, /// 0x21c
			 0xf87451ab, /// 0x220
			 0x8caa2945, /// 0x224
			 0xaa770ba2, /// 0x228
			 0xc70ca0da, /// 0x22c
			 0xdc7b3609, /// 0x230
			 0xe5a4b7d6, /// 0x234
			 0x7e7b0dea, /// 0x238
			 0x13fdd55a, /// 0x23c
			 0x8ad72a3a, /// 0x240
			 0x131284e9, /// 0x244
			 0xe3745a51, /// 0x248
			 0xbdcb5cc8, /// 0x24c
			 0x13b7dc00, /// 0x250
			 0x74e92bff, /// 0x254
			 0xe28ad88b, /// 0x258
			 0xaa035f3b, /// 0x25c
			 0x5fbdff17, /// 0x260
			 0x97615b21, /// 0x264
			 0x61c450d1, /// 0x268
			 0x49398a1c, /// 0x26c
			 0xf6dd677c, /// 0x270
			 0x643650bc, /// 0x274
			 0x82fb269e, /// 0x278
			 0x222136e4, /// 0x27c
			 0xc2c4dae2, /// 0x280
			 0x2fcca982, /// 0x284
			 0x57ba152c, /// 0x288
			 0x1016eb52, /// 0x28c
			 0xeac76da3, /// 0x290
			 0x62a5a8b1, /// 0x294
			 0x27408f44, /// 0x298
			 0x6f8e29fa, /// 0x29c
			 0xbf3f8032, /// 0x2a0
			 0x42bcbeaf, /// 0x2a4
			 0xbba6a8d6, /// 0x2a8
			 0x79ee7745, /// 0x2ac
			 0xdfdfe130, /// 0x2b0
			 0x6a09850c, /// 0x2b4
			 0x574e265a, /// 0x2b8
			 0x83a077bd, /// 0x2bc
			 0xef1912e9, /// 0x2c0
			 0x50967582, /// 0x2c4
			 0x36b16209, /// 0x2c8
			 0xf5944162, /// 0x2cc
			 0x13380c2e, /// 0x2d0
			 0xd64b3410, /// 0x2d4
			 0xda82886d, /// 0x2d8
			 0x3faab3b6, /// 0x2dc
			 0x3aa2a642, /// 0x2e0
			 0xba59a926, /// 0x2e4
			 0xdfeeddd7, /// 0x2e8
			 0xe615d332, /// 0x2ec
			 0x0d599682, /// 0x2f0
			 0xf6f0f01a, /// 0x2f4
			 0xed300828, /// 0x2f8
			 0x34e9d3d6, /// 0x2fc
			 0xa33442c4, /// 0x300
			 0x93a1af41, /// 0x304
			 0xd38f269f, /// 0x308
			 0xc76184cf, /// 0x30c
			 0x0babc505, /// 0x310
			 0x381f58a7, /// 0x314
			 0x46039bb8, /// 0x318
			 0x52fe212c, /// 0x31c
			 0x004d60ce, /// 0x320
			 0xa0b72c39, /// 0x324
			 0x60de191f, /// 0x328
			 0xb7bb4a72, /// 0x32c
			 0xc95650bd, /// 0x330
			 0xf00dc3fb, /// 0x334
			 0xcbefcc07, /// 0x338
			 0xecfc7465, /// 0x33c
			 0x751ad6a8, /// 0x340
			 0xaebbddae, /// 0x344
			 0xc1f61447, /// 0x348
			 0x39d95ac3, /// 0x34c
			 0x2eff370a, /// 0x350
			 0xc3bc79f7, /// 0x354
			 0xa2b14506, /// 0x358
			 0x27317f76, /// 0x35c
			 0x30bbf71c, /// 0x360
			 0x60b4b1b6, /// 0x364
			 0x3c12b55a, /// 0x368
			 0xeb33edec, /// 0x36c
			 0x84b2981b, /// 0x370
			 0xe6dcb7dc, /// 0x374
			 0x0c141d0c, /// 0x378
			 0x1ceed208, /// 0x37c
			 0xc8b024da, /// 0x380
			 0x49b1c928, /// 0x384
			 0xf7e51c4b, /// 0x388
			 0xf3fc00db, /// 0x38c
			 0x5c29e725, /// 0x390
			 0x68242505, /// 0x394
			 0x616c3b37, /// 0x398
			 0xce5823b1, /// 0x39c
			 0x485b3157, /// 0x3a0
			 0xdcb243c0, /// 0x3a4
			 0x22eacfcd, /// 0x3a8
			 0xeba14898, /// 0x3ac
			 0xde7a2a6b, /// 0x3b0
			 0xc864d6be, /// 0x3b4
			 0x7ce9647a, /// 0x3b8
			 0xb8332741, /// 0x3bc
			 0x1bdf83b5, /// 0x3c0
			 0x379ef837, /// 0x3c4
			 0xb3f1a6a6, /// 0x3c8
			 0xab98ec2a, /// 0x3cc
			 0xf465f89f, /// 0x3d0
			 0x222daf72, /// 0x3d4
			 0x08106912, /// 0x3d8
			 0x44978032, /// 0x3dc
			 0x0fce8c39, /// 0x3e0
			 0x5dcf45ab, /// 0x3e4
			 0xdbfc8416, /// 0x3e8
			 0xcd962df1, /// 0x3ec
			 0xa3b04db8, /// 0x3f0
			 0x76b1120d, /// 0x3f4
			 0x1f20adbf, /// 0x3f8
			 0xc8cc8acc, /// 0x3fc
			 0x8df706cd, /// 0x400
			 0x274ecb92, /// 0x404
			 0x38ec221e, /// 0x408
			 0x4eff85cd, /// 0x40c
			 0x5ebac3b0, /// 0x410
			 0x7c6f185c, /// 0x414
			 0x7cb0a7dc, /// 0x418
			 0x31bd4245, /// 0x41c
			 0x3e52eb31, /// 0x420
			 0x8a203458, /// 0x424
			 0xb65f28c7, /// 0x428
			 0x992e4420, /// 0x42c
			 0x9ace90e7, /// 0x430
			 0xe2cb4575, /// 0x434
			 0x25c93713, /// 0x438
			 0x105c0e94, /// 0x43c
			 0x7f0f9584, /// 0x440
			 0x77e2b449, /// 0x444
			 0x8b263f9a, /// 0x448
			 0xe1c57e88, /// 0x44c
			 0x485e47ae, /// 0x450
			 0xe4bb9a64, /// 0x454
			 0x66f7d5da, /// 0x458
			 0xbe32c896, /// 0x45c
			 0xb4e80ba0, /// 0x460
			 0x277b771b, /// 0x464
			 0x09eec5dd, /// 0x468
			 0x69ad8f97, /// 0x46c
			 0xe385e8cf, /// 0x470
			 0x24b3961d, /// 0x474
			 0x117112ee, /// 0x478
			 0xfc40bb2c, /// 0x47c
			 0xd6884a11, /// 0x480
			 0xd19999b3, /// 0x484
			 0x7d46aeed, /// 0x488
			 0x390e15e4, /// 0x48c
			 0xa43a9639, /// 0x490
			 0x4dc523dc, /// 0x494
			 0x50aaa796, /// 0x498
			 0x75ab2e8b, /// 0x49c
			 0x8986185a, /// 0x4a0
			 0x5a3af8d6, /// 0x4a4
			 0xc388dc09, /// 0x4a8
			 0xb60bb7ba, /// 0x4ac
			 0x4e05cf1b, /// 0x4b0
			 0xe1a24b52, /// 0x4b4
			 0xe4981e94, /// 0x4b8
			 0x3421c415, /// 0x4bc
			 0x0e0972d5, /// 0x4c0
			 0x010871e9, /// 0x4c4
			 0x049e0f35, /// 0x4c8
			 0x383785c5, /// 0x4cc
			 0x93eb78c7, /// 0x4d0
			 0xbcd153a3, /// 0x4d4
			 0x94e5e6ac, /// 0x4d8
			 0x58092042, /// 0x4dc
			 0x63ab142f, /// 0x4e0
			 0x536b629a, /// 0x4e4
			 0x50407097, /// 0x4e8
			 0xe144aabf, /// 0x4ec
			 0x100cae3e, /// 0x4f0
			 0x05455623, /// 0x4f4
			 0x41dbd526, /// 0x4f8
			 0x8304a216, /// 0x4fc
			 0x98b2d6ee, /// 0x500
			 0xccd49507, /// 0x504
			 0x0167b5af, /// 0x508
			 0x0e0eaf09, /// 0x50c
			 0x7eb35b0c, /// 0x510
			 0x2db2fd49, /// 0x514
			 0x51dfe8df, /// 0x518
			 0x4ecda5df, /// 0x51c
			 0x0b94ab71, /// 0x520
			 0xb5336618, /// 0x524
			 0xcc293e4b, /// 0x528
			 0xa4605b51, /// 0x52c
			 0x9f208a1b, /// 0x530
			 0x4f85cc27, /// 0x534
			 0x467ee73d, /// 0x538
			 0xb0a0db9f, /// 0x53c
			 0x43d2d728, /// 0x540
			 0x8fa2d189, /// 0x544
			 0xe88939f1, /// 0x548
			 0x732325f0, /// 0x54c
			 0x840491ee, /// 0x550
			 0x68712f68, /// 0x554
			 0x2ee71974, /// 0x558
			 0xfbe68729, /// 0x55c
			 0x93bfcf3c, /// 0x560
			 0x7823d686, /// 0x564
			 0x3d80dcc5, /// 0x568
			 0xaef8b6bd, /// 0x56c
			 0xa0e10682, /// 0x570
			 0x86cae44f, /// 0x574
			 0xe7d319e1, /// 0x578
			 0x96b8e574, /// 0x57c
			 0x59649d50, /// 0x580
			 0x78a013c8, /// 0x584
			 0x826c3250, /// 0x588
			 0xd233e68b, /// 0x58c
			 0x8e5e8ac0, /// 0x590
			 0x48e16026, /// 0x594
			 0xad6165f9, /// 0x598
			 0x45166a06, /// 0x59c
			 0xf1374a7c, /// 0x5a0
			 0x69a78ffd, /// 0x5a4
			 0xc4ea3247, /// 0x5a8
			 0x8ec9da74, /// 0x5ac
			 0x68eb8cd9, /// 0x5b0
			 0x4ecca775, /// 0x5b4
			 0xfa18f7ca, /// 0x5b8
			 0x3435fd59, /// 0x5bc
			 0xc984cc8d, /// 0x5c0
			 0x1dde523b, /// 0x5c4
			 0xd866989e, /// 0x5c8
			 0x68a2abf7, /// 0x5cc
			 0x9fe0e9be, /// 0x5d0
			 0x4085e880, /// 0x5d4
			 0x9b69f805, /// 0x5d8
			 0xaeb98931, /// 0x5dc
			 0x044b8db7, /// 0x5e0
			 0x83a4546b, /// 0x5e4
			 0xe8111637, /// 0x5e8
			 0xcfbb37d7, /// 0x5ec
			 0xc7895b1a, /// 0x5f0
			 0x6a61d17e, /// 0x5f4
			 0xac99e265, /// 0x5f8
			 0x66c4a66a, /// 0x5fc
			 0xacc47f42, /// 0x600
			 0xd5ca02b1, /// 0x604
			 0xd0834059, /// 0x608
			 0x7db40d93, /// 0x60c
			 0x92927466, /// 0x610
			 0xa88faac5, /// 0x614
			 0x3abd3cba, /// 0x618
			 0x17765cca, /// 0x61c
			 0xd0add250, /// 0x620
			 0x26710aeb, /// 0x624
			 0x3d77c52b, /// 0x628
			 0x8cf7878b, /// 0x62c
			 0xf5571c4d, /// 0x630
			 0x86f0c5a9, /// 0x634
			 0x7276f279, /// 0x638
			 0x6b0c748b, /// 0x63c
			 0x1c064ced, /// 0x640
			 0x56b08fc2, /// 0x644
			 0xf6bda3c6, /// 0x648
			 0x0ab67e68, /// 0x64c
			 0x6d656bf8, /// 0x650
			 0xbfbb0e1c, /// 0x654
			 0x5e295761, /// 0x658
			 0x70163c5d, /// 0x65c
			 0xa700055c, /// 0x660
			 0x62b2ee04, /// 0x664
			 0x654c25f5, /// 0x668
			 0x98fccf69, /// 0x66c
			 0xc21f6323, /// 0x670
			 0x16913622, /// 0x674
			 0x518ee4ca, /// 0x678
			 0xfbb2c514, /// 0x67c
			 0x04430bb6, /// 0x680
			 0x84cabed7, /// 0x684
			 0xad072475, /// 0x688
			 0xd226e250, /// 0x68c
			 0x4092ef35, /// 0x690
			 0x9c56d7fe, /// 0x694
			 0x4d58cc7a, /// 0x698
			 0x198e3814, /// 0x69c
			 0x2b45d6b6, /// 0x6a0
			 0x58107f01, /// 0x6a4
			 0x0b45385c, /// 0x6a8
			 0xc04283fb, /// 0x6ac
			 0x5b52d745, /// 0x6b0
			 0x32d78b5f, /// 0x6b4
			 0xc916bfac, /// 0x6b8
			 0x5092610f, /// 0x6bc
			 0x6de0c99f, /// 0x6c0
			 0xb1c86583, /// 0x6c4
			 0xffe5a179, /// 0x6c8
			 0xebf987c9, /// 0x6cc
			 0x64023d76, /// 0x6d0
			 0x97ed2787, /// 0x6d4
			 0xba790bc2, /// 0x6d8
			 0x8d1caec9, /// 0x6dc
			 0xfeb8eba6, /// 0x6e0
			 0xa0a8f84a, /// 0x6e4
			 0x39a05248, /// 0x6e8
			 0xd48de900, /// 0x6ec
			 0x6d91a9b9, /// 0x6f0
			 0xf59ff6ae, /// 0x6f4
			 0x282c6f91, /// 0x6f8
			 0x8c02a240, /// 0x6fc
			 0x5a0be9c5, /// 0x700
			 0x0aef315d, /// 0x704
			 0x2216f35e, /// 0x708
			 0x11e514ca, /// 0x70c
			 0x88071511, /// 0x710
			 0x79224168, /// 0x714
			 0x713d4ef0, /// 0x718
			 0x27c9af9e, /// 0x71c
			 0x9cbd7b3f, /// 0x720
			 0xe9ca57af, /// 0x724
			 0x094679e7, /// 0x728
			 0xef14ad11, /// 0x72c
			 0x011a9a71, /// 0x730
			 0x98a409ec, /// 0x734
			 0x1aecf643, /// 0x738
			 0x5df79116, /// 0x73c
			 0xe293885c, /// 0x740
			 0xec7df4a9, /// 0x744
			 0xf87f82df, /// 0x748
			 0xc4a61c3c, /// 0x74c
			 0x27e586c1, /// 0x750
			 0x2f50b4a8, /// 0x754
			 0xe14e64b8, /// 0x758
			 0xe56fee67, /// 0x75c
			 0xc406f7f7, /// 0x760
			 0x1fc1f07d, /// 0x764
			 0xa0250ceb, /// 0x768
			 0x70d680e6, /// 0x76c
			 0x3b8999fd, /// 0x770
			 0x4a48b423, /// 0x774
			 0x01bd1175, /// 0x778
			 0x4aafbbcd, /// 0x77c
			 0x4b2e3784, /// 0x780
			 0x550c82d4, /// 0x784
			 0xe6ff2f89, /// 0x788
			 0x4ba5d244, /// 0x78c
			 0x6aecff89, /// 0x790
			 0xb3c6a810, /// 0x794
			 0x617a66cb, /// 0x798
			 0x3a595939, /// 0x79c
			 0xcd9d9df1, /// 0x7a0
			 0xdc76ab3e, /// 0x7a4
			 0x5871cfc7, /// 0x7a8
			 0xe45c1c3e, /// 0x7ac
			 0xf02d6b59, /// 0x7b0
			 0xabea3039, /// 0x7b4
			 0x30937d2f, /// 0x7b8
			 0x37b67616, /// 0x7bc
			 0x095e5148, /// 0x7c0
			 0x6658e41d, /// 0x7c4
			 0xc3bbb8ae, /// 0x7c8
			 0x04c3ea13, /// 0x7cc
			 0xe996140d, /// 0x7d0
			 0x65c02a3e, /// 0x7d4
			 0x49fc9f03, /// 0x7d8
			 0x2a4abb71, /// 0x7dc
			 0x76ed3f08, /// 0x7e0
			 0x634331b8, /// 0x7e4
			 0xcf0103b0, /// 0x7e8
			 0x4af018f9, /// 0x7ec
			 0xeeeee4b6, /// 0x7f0
			 0x0983a72c, /// 0x7f4
			 0xab5eea5f, /// 0x7f8
			 0xcfaa2429, /// 0x7fc
			 0x47a4fdfc, /// 0x800
			 0x1b58d7a4, /// 0x804
			 0xc4a85760, /// 0x808
			 0xcff55772, /// 0x80c
			 0xb6429c91, /// 0x810
			 0x390797f0, /// 0x814
			 0x5b123ded, /// 0x818
			 0x942dc3de, /// 0x81c
			 0x7ab1cdff, /// 0x820
			 0x7fadad1f, /// 0x824
			 0xa29ab0d6, /// 0x828
			 0xc1511ff5, /// 0x82c
			 0xf1733623, /// 0x830
			 0x0c89b774, /// 0x834
			 0x87305100, /// 0x838
			 0x32ca3eec, /// 0x83c
			 0xb6e297cc, /// 0x840
			 0x7d310888, /// 0x844
			 0x65044343, /// 0x848
			 0xe3198253, /// 0x84c
			 0x7159171f, /// 0x850
			 0x3b2c86dd, /// 0x854
			 0x2a16cdc9, /// 0x858
			 0xb99e11d2, /// 0x85c
			 0xb06bacdb, /// 0x860
			 0x169567dd, /// 0x864
			 0xc33f8333, /// 0x868
			 0x721d75ff, /// 0x86c
			 0x51b3c6e4, /// 0x870
			 0xf12d64d3, /// 0x874
			 0x9181d798, /// 0x878
			 0x9ce758c6, /// 0x87c
			 0x807e4d99, /// 0x880
			 0x1fee1dbb, /// 0x884
			 0xa4af4262, /// 0x888
			 0xff5b5778, /// 0x88c
			 0x3ca09cc5, /// 0x890
			 0x31b83802, /// 0x894
			 0x60069afd, /// 0x898
			 0xc3dd8cf6, /// 0x89c
			 0x828a64a1, /// 0x8a0
			 0x04c9dc80, /// 0x8a4
			 0x63c30a92, /// 0x8a8
			 0xd9445400, /// 0x8ac
			 0xd30938ab, /// 0x8b0
			 0x816c0de6, /// 0x8b4
			 0xc655109b, /// 0x8b8
			 0xd177bd7b, /// 0x8bc
			 0x1aa08611, /// 0x8c0
			 0x402b38cd, /// 0x8c4
			 0x4eb2b689, /// 0x8c8
			 0xfbceec9d, /// 0x8cc
			 0xe1480207, /// 0x8d0
			 0x6b259c4b, /// 0x8d4
			 0x59b014a9, /// 0x8d8
			 0xdcefbd14, /// 0x8dc
			 0x8f7a9913, /// 0x8e0
			 0xf3af784d, /// 0x8e4
			 0x655c7129, /// 0x8e8
			 0x1b38d62b, /// 0x8ec
			 0x81679c18, /// 0x8f0
			 0x587fccea, /// 0x8f4
			 0x19153fdf, /// 0x8f8
			 0xe071d696, /// 0x8fc
			 0x1efff8c9, /// 0x900
			 0xcbd2426f, /// 0x904
			 0xaee5d10a, /// 0x908
			 0x9f179e11, /// 0x90c
			 0xaa0a1189, /// 0x910
			 0x15836c62, /// 0x914
			 0x090bccd1, /// 0x918
			 0xfea29ca8, /// 0x91c
			 0xed02feec, /// 0x920
			 0xff34b3e0, /// 0x924
			 0x5c09bd50, /// 0x928
			 0x2ff9f14f, /// 0x92c
			 0x8711b495, /// 0x930
			 0x0d1f415a, /// 0x934
			 0x13d95c90, /// 0x938
			 0x40965547, /// 0x93c
			 0xd73f0f6f, /// 0x940
			 0x4402b394, /// 0x944
			 0x5b8e70f7, /// 0x948
			 0x9ed77dc7, /// 0x94c
			 0xbad08c9d, /// 0x950
			 0xef2e9db1, /// 0x954
			 0x1b931d4c, /// 0x958
			 0x5c04a990, /// 0x95c
			 0x1e354c9b, /// 0x960
			 0x23071190, /// 0x964
			 0xef370ef3, /// 0x968
			 0x40242811, /// 0x96c
			 0x9af93196, /// 0x970
			 0xd2d7c896, /// 0x974
			 0xa6aecc0a, /// 0x978
			 0x4194b364, /// 0x97c
			 0xd0024ca8, /// 0x980
			 0x4e7b9896, /// 0x984
			 0x500ab7e8, /// 0x988
			 0x5acfcf4f, /// 0x98c
			 0x33f6466e, /// 0x990
			 0xc6507715, /// 0x994
			 0x2b01d5f5, /// 0x998
			 0x016a5d06, /// 0x99c
			 0xd03f8ef3, /// 0x9a0
			 0x2b19e28c, /// 0x9a4
			 0xf7f6bb7c, /// 0x9a8
			 0xdf8bc7d0, /// 0x9ac
			 0x1ed4a6d1, /// 0x9b0
			 0x327b2360, /// 0x9b4
			 0x6c78609b, /// 0x9b8
			 0x9d54ed23, /// 0x9bc
			 0xfcebb360, /// 0x9c0
			 0xb63dd02a, /// 0x9c4
			 0x5e147f89, /// 0x9c8
			 0x2f289171, /// 0x9cc
			 0xc29d6f4d, /// 0x9d0
			 0x80f74846, /// 0x9d4
			 0x7c7f2797, /// 0x9d8
			 0x3526d45e, /// 0x9dc
			 0x675e43cb, /// 0x9e0
			 0xf767272f, /// 0x9e4
			 0x043ead3c, /// 0x9e8
			 0xf8bb3d2f, /// 0x9ec
			 0x4cf0ee8f, /// 0x9f0
			 0x18dd7e78, /// 0x9f4
			 0x09eb2c95, /// 0x9f8
			 0xcaa7aab7, /// 0x9fc
			 0x82589ad7, /// 0xa00
			 0xb4a9d182, /// 0xa04
			 0x6508db4b, /// 0xa08
			 0x83c15513, /// 0xa0c
			 0xd82629b3, /// 0xa10
			 0x50213abe, /// 0xa14
			 0x9d668e97, /// 0xa18
			 0xd61b0239, /// 0xa1c
			 0xb481e2c9, /// 0xa20
			 0xf25d2e74, /// 0xa24
			 0x5c647d1f, /// 0xa28
			 0xb81f378b, /// 0xa2c
			 0x0bafc82c, /// 0xa30
			 0x4b375f80, /// 0xa34
			 0x60856da4, /// 0xa38
			 0x56c857de, /// 0xa3c
			 0xa592b2c8, /// 0xa40
			 0x0eda5b9b, /// 0xa44
			 0x225766ba, /// 0xa48
			 0x7ce3dae8, /// 0xa4c
			 0x1d798d58, /// 0xa50
			 0xa3c91613, /// 0xa54
			 0xb9c70ec7, /// 0xa58
			 0xc3140c3a, /// 0xa5c
			 0xce14399a, /// 0xa60
			 0x83f9d438, /// 0xa64
			 0x6701fafd, /// 0xa68
			 0x62d66646, /// 0xa6c
			 0x4fae54c6, /// 0xa70
			 0xb76bd54b, /// 0xa74
			 0xb827aa0b, /// 0xa78
			 0x633f4694, /// 0xa7c
			 0x2cf95bf6, /// 0xa80
			 0x0bb04427, /// 0xa84
			 0xb675a910, /// 0xa88
			 0x62518047, /// 0xa8c
			 0xf01a011d, /// 0xa90
			 0xc4caa61c, /// 0xa94
			 0x76e33c0e, /// 0xa98
			 0x8d2f15e2, /// 0xa9c
			 0x7166e74c, /// 0xaa0
			 0x5fce6d8b, /// 0xaa4
			 0x22e85af9, /// 0xaa8
			 0x667c1c02, /// 0xaac
			 0x98549a9d, /// 0xab0
			 0x8cd9796f, /// 0xab4
			 0xd8d66faf, /// 0xab8
			 0xe1e1c4c3, /// 0xabc
			 0x50c92645, /// 0xac0
			 0x9f1d94bf, /// 0xac4
			 0xb76e7ae8, /// 0xac8
			 0x2cd36456, /// 0xacc
			 0x2d12168c, /// 0xad0
			 0xc1accbae, /// 0xad4
			 0xc2aedab9, /// 0xad8
			 0x0b046f1b, /// 0xadc
			 0xf2aec822, /// 0xae0
			 0xab696101, /// 0xae4
			 0x98a0a540, /// 0xae8
			 0x2712d529, /// 0xaec
			 0xce59f6ad, /// 0xaf0
			 0xddfb60c3, /// 0xaf4
			 0x788e9fb6, /// 0xaf8
			 0x0f4af703, /// 0xafc
			 0xfde9e717, /// 0xb00
			 0x21f992f8, /// 0xb04
			 0x26df2a4b, /// 0xb08
			 0x4c9bf694, /// 0xb0c
			 0x6ce1c671, /// 0xb10
			 0xcea0e999, /// 0xb14
			 0xd5ce48dc, /// 0xb18
			 0xcbf02eff, /// 0xb1c
			 0x20cf74ae, /// 0xb20
			 0x77523e85, /// 0xb24
			 0x421df707, /// 0xb28
			 0x30d891e7, /// 0xb2c
			 0xbafc829b, /// 0xb30
			 0x00addd1c, /// 0xb34
			 0xc531c982, /// 0xb38
			 0x9b3e7c79, /// 0xb3c
			 0x50cc990b, /// 0xb40
			 0x84531ede, /// 0xb44
			 0x40bdb306, /// 0xb48
			 0x04b61301, /// 0xb4c
			 0x3ce5aa07, /// 0xb50
			 0x5c01f06d, /// 0xb54
			 0x9f7dfee6, /// 0xb58
			 0x8f3155a0, /// 0xb5c
			 0xf246156e, /// 0xb60
			 0x2b96c18d, /// 0xb64
			 0x65c2f81e, /// 0xb68
			 0xad674fa0, /// 0xb6c
			 0xb038afe2, /// 0xb70
			 0x53cbc967, /// 0xb74
			 0x7765b8b4, /// 0xb78
			 0x4c533c07, /// 0xb7c
			 0x62894d3c, /// 0xb80
			 0xe603a958, /// 0xb84
			 0x64f32222, /// 0xb88
			 0x0cd85e57, /// 0xb8c
			 0xcfd5cc6f, /// 0xb90
			 0xd1262875, /// 0xb94
			 0x8b01a72e, /// 0xb98
			 0xcc1ef630, /// 0xb9c
			 0xad9147d2, /// 0xba0
			 0x310c3e44, /// 0xba4
			 0x8ecd600a, /// 0xba8
			 0x0b195afc, /// 0xbac
			 0x657f99b1, /// 0xbb0
			 0xa0d3a629, /// 0xbb4
			 0x893e91a0, /// 0xbb8
			 0xb63876f2, /// 0xbbc
			 0xab92c83c, /// 0xbc0
			 0x70cb815c, /// 0xbc4
			 0xd0a06362, /// 0xbc8
			 0xe8bc74d5, /// 0xbcc
			 0x9d82d6cc, /// 0xbd0
			 0x4fca5f7f, /// 0xbd4
			 0xc87f3477, /// 0xbd8
			 0x4f06f12e, /// 0xbdc
			 0xaf0c63ba, /// 0xbe0
			 0x1eab0557, /// 0xbe4
			 0x940bf897, /// 0xbe8
			 0x4b95a889, /// 0xbec
			 0x71c8ed6f, /// 0xbf0
			 0x8856db73, /// 0xbf4
			 0xdc5a94b9, /// 0xbf8
			 0x0c94e9ec, /// 0xbfc
			 0xb5f74c28, /// 0xc00
			 0xef2d78d5, /// 0xc04
			 0xc917e47f, /// 0xc08
			 0x0386e016, /// 0xc0c
			 0x4dff44f3, /// 0xc10
			 0x161bda71, /// 0xc14
			 0x9245fe8c, /// 0xc18
			 0x54ad94be, /// 0xc1c
			 0xdaf543cb, /// 0xc20
			 0x45e23b7f, /// 0xc24
			 0x6479c005, /// 0xc28
			 0x3493ddf7, /// 0xc2c
			 0x169c2745, /// 0xc30
			 0x40279905, /// 0xc34
			 0xb6621818, /// 0xc38
			 0xc9b78676, /// 0xc3c
			 0x86a62aef, /// 0xc40
			 0x070eae6b, /// 0xc44
			 0xe5d9bd0b, /// 0xc48
			 0x08787054, /// 0xc4c
			 0xee847b27, /// 0xc50
			 0xe95f4f16, /// 0xc54
			 0x879d6227, /// 0xc58
			 0x0e71f6dc, /// 0xc5c
			 0x313e8fd7, /// 0xc60
			 0xcadb1b45, /// 0xc64
			 0xa8734021, /// 0xc68
			 0x75781a7f, /// 0xc6c
			 0x2d51e99d, /// 0xc70
			 0x8fb6d8cf, /// 0xc74
			 0x684c12cd, /// 0xc78
			 0x141fd4fd, /// 0xc7c
			 0xe8a4fa07, /// 0xc80
			 0x99657c3e, /// 0xc84
			 0xcab94965, /// 0xc88
			 0xf95a3c0d, /// 0xc8c
			 0x5f29bbdf, /// 0xc90
			 0xce12efa6, /// 0xc94
			 0x7b8e1886, /// 0xc98
			 0xc57531b9, /// 0xc9c
			 0xf62a767a, /// 0xca0
			 0x3cb8709d, /// 0xca4
			 0x7873aeea, /// 0xca8
			 0xe42e105b, /// 0xcac
			 0x3a84368c, /// 0xcb0
			 0xaab3d662, /// 0xcb4
			 0xdd49de06, /// 0xcb8
			 0x082d9275, /// 0xcbc
			 0x88a93893, /// 0xcc0
			 0x42476712, /// 0xcc4
			 0x6304c791, /// 0xcc8
			 0x3fe528f3, /// 0xccc
			 0x2ed1f241, /// 0xcd0
			 0x8769f29d, /// 0xcd4
			 0x66088da3, /// 0xcd8
			 0xf8ef3606, /// 0xcdc
			 0x0955452e, /// 0xce0
			 0x232ec987, /// 0xce4
			 0xc14e2392, /// 0xce8
			 0x447bf9d2, /// 0xcec
			 0x47a67da1, /// 0xcf0
			 0x6f705592, /// 0xcf4
			 0xa98ccb74, /// 0xcf8
			 0x4a3abc8f, /// 0xcfc
			 0x9948b153, /// 0xd00
			 0xc63ef296, /// 0xd04
			 0x9b866977, /// 0xd08
			 0xfd117a8a, /// 0xd0c
			 0x9b9af809, /// 0xd10
			 0x6cd13991, /// 0xd14
			 0xf01a3863, /// 0xd18
			 0x0d3438fc, /// 0xd1c
			 0x9f188017, /// 0xd20
			 0x3ddc0dc4, /// 0xd24
			 0x78a8b8f3, /// 0xd28
			 0x1f4602f6, /// 0xd2c
			 0x5fa813b9, /// 0xd30
			 0xfba0d8e1, /// 0xd34
			 0xb984253f, /// 0xd38
			 0x8f83b22a, /// 0xd3c
			 0x8e18f022, /// 0xd40
			 0x4a6570be, /// 0xd44
			 0xf21873b4, /// 0xd48
			 0x778449eb, /// 0xd4c
			 0x797a8cb5, /// 0xd50
			 0x08beb366, /// 0xd54
			 0xafc0a96a, /// 0xd58
			 0x541b972a, /// 0xd5c
			 0xa76e4441, /// 0xd60
			 0xa9759108, /// 0xd64
			 0x8b9131b4, /// 0xd68
			 0x11ec00c0, /// 0xd6c
			 0x3518453d, /// 0xd70
			 0x27a9de6f, /// 0xd74
			 0x198a6d89, /// 0xd78
			 0x3cde2529, /// 0xd7c
			 0x839124cd, /// 0xd80
			 0xe07bf853, /// 0xd84
			 0x5f090715, /// 0xd88
			 0x44bb09a9, /// 0xd8c
			 0xa3222de3, /// 0xd90
			 0xc6afa762, /// 0xd94
			 0x5d11126d, /// 0xd98
			 0x0c98fb28, /// 0xd9c
			 0x5909f147, /// 0xda0
			 0x7eaf0584, /// 0xda4
			 0x3afb5848, /// 0xda8
			 0x2c415d47, /// 0xdac
			 0x747451bb, /// 0xdb0
			 0x47e29914, /// 0xdb4
			 0x67317b4d, /// 0xdb8
			 0x31a60315, /// 0xdbc
			 0x71ebd206, /// 0xdc0
			 0x493263b8, /// 0xdc4
			 0x2034b9d8, /// 0xdc8
			 0x51d88451, /// 0xdcc
			 0x22cb9547, /// 0xdd0
			 0x5f4bb9d2, /// 0xdd4
			 0x6d7edbd7, /// 0xdd8
			 0x73f965d5, /// 0xddc
			 0xb80f3330, /// 0xde0
			 0x7b5b33ba, /// 0xde4
			 0x183ad1d9, /// 0xde8
			 0x23666907, /// 0xdec
			 0xf0ebf1f2, /// 0xdf0
			 0xf91865d9, /// 0xdf4
			 0xabbe3727, /// 0xdf8
			 0xcc2e951e, /// 0xdfc
			 0x7b20efb9, /// 0xe00
			 0xf85941f2, /// 0xe04
			 0x9abc5fa3, /// 0xe08
			 0x4c2bceb6, /// 0xe0c
			 0x9f3c73d4, /// 0xe10
			 0x66f6621f, /// 0xe14
			 0xcbce8570, /// 0xe18
			 0xa62ff934, /// 0xe1c
			 0xbddaad47, /// 0xe20
			 0x727e9425, /// 0xe24
			 0x3ade9c82, /// 0xe28
			 0xb1c1033e, /// 0xe2c
			 0x6c799aa5, /// 0xe30
			 0x2077ae3c, /// 0xe34
			 0x6768e519, /// 0xe38
			 0xf77f4990, /// 0xe3c
			 0x44230ac3, /// 0xe40
			 0x8552399c, /// 0xe44
			 0x6bc0a750, /// 0xe48
			 0x30860f31, /// 0xe4c
			 0x8290d878, /// 0xe50
			 0x12c3aaa8, /// 0xe54
			 0x3c74f8e5, /// 0xe58
			 0x64760461, /// 0xe5c
			 0x97508950, /// 0xe60
			 0x9f7a8694, /// 0xe64
			 0x320e5421, /// 0xe68
			 0x979fbb76, /// 0xe6c
			 0x96b6ba92, /// 0xe70
			 0x46b54b97, /// 0xe74
			 0x04c3f6e3, /// 0xe78
			 0xb79d1d0e, /// 0xe7c
			 0x9b7f0782, /// 0xe80
			 0x0de929fe, /// 0xe84
			 0x352a4d68, /// 0xe88
			 0x059805a9, /// 0xe8c
			 0x01ff0747, /// 0xe90
			 0x4d29915d, /// 0xe94
			 0x8025e02a, /// 0xe98
			 0xe07e2ac4, /// 0xe9c
			 0x819d56b0, /// 0xea0
			 0x0b6eb2db, /// 0xea4
			 0x00eed44b, /// 0xea8
			 0x699c8f5c, /// 0xeac
			 0x7be26c46, /// 0xeb0
			 0x4c4bcb1f, /// 0xeb4
			 0x946bdb76, /// 0xeb8
			 0xef3def41, /// 0xebc
			 0x6a8fdd15, /// 0xec0
			 0x1520c054, /// 0xec4
			 0x1ade42ab, /// 0xec8
			 0x4b0b7602, /// 0xecc
			 0xec5bbd02, /// 0xed0
			 0x9d2648b9, /// 0xed4
			 0x91c968ad, /// 0xed8
			 0x85074239, /// 0xedc
			 0x3cb2fe6a, /// 0xee0
			 0x8ac1bc17, /// 0xee4
			 0x285f0e0f, /// 0xee8
			 0x5dd4307b, /// 0xeec
			 0x9ed679d9, /// 0xef0
			 0xebbf297d, /// 0xef4
			 0xda5bf6a4, /// 0xef8
			 0xa1f03036, /// 0xefc
			 0x4690c1ad, /// 0xf00
			 0x17e0a891, /// 0xf04
			 0xa5ee8125, /// 0xf08
			 0x7092b8ec, /// 0xf0c
			 0x008d461b, /// 0xf10
			 0x9ebd747d, /// 0xf14
			 0x5c034b84, /// 0xf18
			 0xf67817fe, /// 0xf1c
			 0xafbae754, /// 0xf20
			 0x6af593e7, /// 0xf24
			 0x36db16a7, /// 0xf28
			 0x04b36ac3, /// 0xf2c
			 0xcd3ead86, /// 0xf30
			 0xeb7b3654, /// 0xf34
			 0xf55fd3c3, /// 0xf38
			 0xbcfa3518, /// 0xf3c
			 0x8191c239, /// 0xf40
			 0xbf99a5b9, /// 0xf44
			 0x4e4f1928, /// 0xf48
			 0x18de56e2, /// 0xf4c
			 0xacc4023d, /// 0xf50
			 0xe8d3f20d, /// 0xf54
			 0x0bbe5e56, /// 0xf58
			 0x898b8cce, /// 0xf5c
			 0x26adc478, /// 0xf60
			 0xbcc9bda8, /// 0xf64
			 0xb4c8dc61, /// 0xf68
			 0x42e51f85, /// 0xf6c
			 0x917b004e, /// 0xf70
			 0xfc174a2c, /// 0xf74
			 0xc50ebfcd, /// 0xf78
			 0x0872f310, /// 0xf7c
			 0xa273fe01, /// 0xf80
			 0x1cfe00b9, /// 0xf84
			 0x6f05f20b, /// 0xf88
			 0xe43626ef, /// 0xf8c
			 0x8a893de8, /// 0xf90
			 0xc1fc474b, /// 0xf94
			 0xcd339892, /// 0xf98
			 0x97218bca, /// 0xf9c
			 0xdc8d99d0, /// 0xfa0
			 0xd5f01a77, /// 0xfa4
			 0xd57dd279, /// 0xfa8
			 0x68e7d162, /// 0xfac
			 0x31468244, /// 0xfb0
			 0xb87135f3, /// 0xfb4
			 0x6d206cf4, /// 0xfb8
			 0xef9dd20c, /// 0xfbc
			 0xfe25093d, /// 0xfc0
			 0x73a2b829, /// 0xfc4
			 0x6751193a, /// 0xfc8
			 0x5689863c, /// 0xfcc
			 0x7f64c46f, /// 0xfd0
			 0x93c4e64d, /// 0xfd4
			 0xf5c43601, /// 0xfd8
			 0xda0a0f23, /// 0xfdc
			 0x7005ac7c, /// 0xfe0
			 0x2e85422a, /// 0xfe4
			 0x04123fed, /// 0xfe8
			 0x566b4acd, /// 0xfec
			 0xfae13f5e, /// 0xff0
			 0xb9219569, /// 0xff4
			 0xca8d931c, /// 0xff8
			 0xe7735c37 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x6e27ef0d, /// 0x0
			 0xd546659a, /// 0x4
			 0xace391b1, /// 0x8
			 0xcb320255, /// 0xc
			 0x17d2f874, /// 0x10
			 0xd3501671, /// 0x14
			 0x3d7037e5, /// 0x18
			 0x6d617c00, /// 0x1c
			 0x4a6dceec, /// 0x20
			 0x3279b0c6, /// 0x24
			 0x4778bbdb, /// 0x28
			 0x5204af64, /// 0x2c
			 0xc2b43b99, /// 0x30
			 0x2e22f9cb, /// 0x34
			 0x78b37054, /// 0x38
			 0x08e35824, /// 0x3c
			 0xd0348ea9, /// 0x40
			 0x4f58003f, /// 0x44
			 0x9a2fafab, /// 0x48
			 0x6963e685, /// 0x4c
			 0x875bc64a, /// 0x50
			 0xee5b2e31, /// 0x54
			 0x901e6bd4, /// 0x58
			 0x56211510, /// 0x5c
			 0x5021037a, /// 0x60
			 0xd3b39013, /// 0x64
			 0x6e35db93, /// 0x68
			 0xf359aa9f, /// 0x6c
			 0xf8e99c04, /// 0x70
			 0xede336ce, /// 0x74
			 0x9a69e7c5, /// 0x78
			 0xe78ba196, /// 0x7c
			 0x4b8e785a, /// 0x80
			 0x2933cc1e, /// 0x84
			 0x0f94e04d, /// 0x88
			 0x5482be02, /// 0x8c
			 0xfeb6194d, /// 0x90
			 0x40209385, /// 0x94
			 0x27b496f7, /// 0x98
			 0x61d8d2d4, /// 0x9c
			 0xde5841fc, /// 0xa0
			 0x23a51ba5, /// 0xa4
			 0x5c59d2c1, /// 0xa8
			 0x09f9d5cb, /// 0xac
			 0x3cb3f5c9, /// 0xb0
			 0x9ba8852e, /// 0xb4
			 0x9ccbca34, /// 0xb8
			 0x2195d2eb, /// 0xbc
			 0xda5b44b1, /// 0xc0
			 0x0bb85297, /// 0xc4
			 0x19abec0d, /// 0xc8
			 0x0c97ec0b, /// 0xcc
			 0x12952304, /// 0xd0
			 0x1205632d, /// 0xd4
			 0xe8f663bb, /// 0xd8
			 0x44b45563, /// 0xdc
			 0x8051b7cd, /// 0xe0
			 0xd5002ef3, /// 0xe4
			 0x51260cf1, /// 0xe8
			 0xa67f620b, /// 0xec
			 0x67cf505b, /// 0xf0
			 0xd36ef30d, /// 0xf4
			 0x2d08b649, /// 0xf8
			 0x6197d5bb, /// 0xfc
			 0xd35e63da, /// 0x100
			 0x9029da76, /// 0x104
			 0x2fab0319, /// 0x108
			 0xba7708c1, /// 0x10c
			 0xf4fb42ac, /// 0x110
			 0x55694471, /// 0x114
			 0x8081c2e9, /// 0x118
			 0x8753b04f, /// 0x11c
			 0x663062e9, /// 0x120
			 0x6a404be5, /// 0x124
			 0x6c106818, /// 0x128
			 0xa8567225, /// 0x12c
			 0x80c9583c, /// 0x130
			 0xc6ad872b, /// 0x134
			 0xdf9a8016, /// 0x138
			 0xf5d25f09, /// 0x13c
			 0xd96f6cbe, /// 0x140
			 0xa2fabf0b, /// 0x144
			 0x0c6b5df1, /// 0x148
			 0xd80e9464, /// 0x14c
			 0x98c199b6, /// 0x150
			 0xbe9f3d25, /// 0x154
			 0x1ab0e90e, /// 0x158
			 0x89719de1, /// 0x15c
			 0x14415a1d, /// 0x160
			 0x2ea5513c, /// 0x164
			 0x590d3980, /// 0x168
			 0xbb3a9abc, /// 0x16c
			 0x4ce77883, /// 0x170
			 0xfb62bc40, /// 0x174
			 0x75e476df, /// 0x178
			 0x562e73eb, /// 0x17c
			 0xcd50c6eb, /// 0x180
			 0xbfd46efd, /// 0x184
			 0x70d113f4, /// 0x188
			 0x9bf00677, /// 0x18c
			 0x3249bc1d, /// 0x190
			 0x07c26340, /// 0x194
			 0x0634310d, /// 0x198
			 0xcac2aa78, /// 0x19c
			 0x6dd8665f, /// 0x1a0
			 0x9eade4df, /// 0x1a4
			 0x943f9f64, /// 0x1a8
			 0xbd678f9f, /// 0x1ac
			 0xd3f7add1, /// 0x1b0
			 0xc5971c80, /// 0x1b4
			 0x84eba324, /// 0x1b8
			 0x346dd02a, /// 0x1bc
			 0x17ce0466, /// 0x1c0
			 0xc05774e4, /// 0x1c4
			 0x749db813, /// 0x1c8
			 0x79045deb, /// 0x1cc
			 0xd1d32f10, /// 0x1d0
			 0x9b87e071, /// 0x1d4
			 0x0a7deb67, /// 0x1d8
			 0xb5eba736, /// 0x1dc
			 0x537b6cd4, /// 0x1e0
			 0x5f18e863, /// 0x1e4
			 0xb9b5ac9b, /// 0x1e8
			 0xea498a9f, /// 0x1ec
			 0x7fff707b, /// 0x1f0
			 0x9eb96a22, /// 0x1f4
			 0x8fbcdb15, /// 0x1f8
			 0xb2363f75, /// 0x1fc
			 0x5b36d09f, /// 0x200
			 0x3bbafb0b, /// 0x204
			 0xbfb5c044, /// 0x208
			 0xc79629e7, /// 0x20c
			 0x39460bc2, /// 0x210
			 0xb2b3ad03, /// 0x214
			 0x8582ccf5, /// 0x218
			 0x89dc9fd9, /// 0x21c
			 0x7ac19175, /// 0x220
			 0x1b95b2c6, /// 0x224
			 0x10ccddbd, /// 0x228
			 0xdd58a32c, /// 0x22c
			 0x608322c7, /// 0x230
			 0x76c05c42, /// 0x234
			 0x46b0b620, /// 0x238
			 0xc5738537, /// 0x23c
			 0x627f45bb, /// 0x240
			 0x80974c8a, /// 0x244
			 0x0ebe544d, /// 0x248
			 0xa461ae20, /// 0x24c
			 0x0510eae6, /// 0x250
			 0x375f167d, /// 0x254
			 0x93f3432a, /// 0x258
			 0x685eaf31, /// 0x25c
			 0x8218cf06, /// 0x260
			 0x17a3bc12, /// 0x264
			 0x95d27420, /// 0x268
			 0x5e871cc4, /// 0x26c
			 0x4ecbec16, /// 0x270
			 0xc56259fb, /// 0x274
			 0x2c54e143, /// 0x278
			 0x28dfd67e, /// 0x27c
			 0xd2307677, /// 0x280
			 0x3864d342, /// 0x284
			 0x7c0f9232, /// 0x288
			 0xf9503ac7, /// 0x28c
			 0x89cc09a2, /// 0x290
			 0x3d9d4fc9, /// 0x294
			 0x8957e2b2, /// 0x298
			 0x1bd2f803, /// 0x29c
			 0x49c4292c, /// 0x2a0
			 0xd4d06e3b, /// 0x2a4
			 0x3a5ca83a, /// 0x2a8
			 0x4c323a57, /// 0x2ac
			 0x2112ff72, /// 0x2b0
			 0x478ec1ea, /// 0x2b4
			 0x2984a72d, /// 0x2b8
			 0x0740e4b8, /// 0x2bc
			 0x75242b0d, /// 0x2c0
			 0x00c4adf7, /// 0x2c4
			 0xf365c680, /// 0x2c8
			 0xbf6e4d5f, /// 0x2cc
			 0x20a8fe6e, /// 0x2d0
			 0x2cb99082, /// 0x2d4
			 0xa55627b5, /// 0x2d8
			 0x1786ac4f, /// 0x2dc
			 0xe54bcc77, /// 0x2e0
			 0x3473ced1, /// 0x2e4
			 0xc0fe9192, /// 0x2e8
			 0xf6cc99cc, /// 0x2ec
			 0xd8a394fa, /// 0x2f0
			 0xae597ecc, /// 0x2f4
			 0xc468947f, /// 0x2f8
			 0xce07213c, /// 0x2fc
			 0xb14afa73, /// 0x300
			 0x37ab4c11, /// 0x304
			 0x7fc3cb61, /// 0x308
			 0x9ad5696b, /// 0x30c
			 0x2339a5ec, /// 0x310
			 0xf2521402, /// 0x314
			 0x52f24c74, /// 0x318
			 0xbce7c71d, /// 0x31c
			 0x74369ae3, /// 0x320
			 0x815c68ec, /// 0x324
			 0x768b6329, /// 0x328
			 0x5c32ad17, /// 0x32c
			 0x8cb0b27f, /// 0x330
			 0xf6728a3c, /// 0x334
			 0xc3e9ee2f, /// 0x338
			 0xeb9ff1f4, /// 0x33c
			 0x87504f6b, /// 0x340
			 0x25f09c8e, /// 0x344
			 0xcb855259, /// 0x348
			 0xfda7d85c, /// 0x34c
			 0x83a94387, /// 0x350
			 0x307dfe5c, /// 0x354
			 0x7a711de8, /// 0x358
			 0x9b46ca2a, /// 0x35c
			 0xaa51706b, /// 0x360
			 0x19ad8903, /// 0x364
			 0x3158e0fc, /// 0x368
			 0xd8926cc4, /// 0x36c
			 0x2c73db33, /// 0x370
			 0x90045256, /// 0x374
			 0x2d2eaf0d, /// 0x378
			 0x29ad8485, /// 0x37c
			 0x8491aea7, /// 0x380
			 0x9effbbd1, /// 0x384
			 0xfc4d97e6, /// 0x388
			 0x61963763, /// 0x38c
			 0xef081cd4, /// 0x390
			 0x5507ee5c, /// 0x394
			 0x936bce95, /// 0x398
			 0xcafc451b, /// 0x39c
			 0xe7ab5d30, /// 0x3a0
			 0xfe64e03d, /// 0x3a4
			 0x9e97b14c, /// 0x3a8
			 0xc082df2f, /// 0x3ac
			 0xcd9e4b50, /// 0x3b0
			 0x5cb877a8, /// 0x3b4
			 0x34658ea2, /// 0x3b8
			 0x2b6a930e, /// 0x3bc
			 0xbcb81d91, /// 0x3c0
			 0xdcefd7b5, /// 0x3c4
			 0xf93c3519, /// 0x3c8
			 0x50b172eb, /// 0x3cc
			 0x66aa9750, /// 0x3d0
			 0x5094d3b5, /// 0x3d4
			 0x76d03c53, /// 0x3d8
			 0xc401d107, /// 0x3dc
			 0x0c0b3903, /// 0x3e0
			 0xca1729df, /// 0x3e4
			 0x2db41bfa, /// 0x3e8
			 0x85796b81, /// 0x3ec
			 0xd8b04d17, /// 0x3f0
			 0xccfb39ed, /// 0x3f4
			 0x21e7e63a, /// 0x3f8
			 0xbfac05e4, /// 0x3fc
			 0xef1e6185, /// 0x400
			 0xb8fcaee1, /// 0x404
			 0xb1914f60, /// 0x408
			 0x7dcd7d04, /// 0x40c
			 0xc4885c53, /// 0x410
			 0xda54aeda, /// 0x414
			 0x6d375563, /// 0x418
			 0xbfdb2d97, /// 0x41c
			 0x3fd6d7f2, /// 0x420
			 0xe2fd0346, /// 0x424
			 0x0da0329b, /// 0x428
			 0x0cd854fb, /// 0x42c
			 0x7e29892a, /// 0x430
			 0x87e63637, /// 0x434
			 0x6d439423, /// 0x438
			 0xdefa6739, /// 0x43c
			 0xb6ee54d7, /// 0x440
			 0x16fe483c, /// 0x444
			 0x59edcf9c, /// 0x448
			 0xdcf179c3, /// 0x44c
			 0xc7cc632e, /// 0x450
			 0x94cbb5a1, /// 0x454
			 0x51305dc5, /// 0x458
			 0xb4c29d0d, /// 0x45c
			 0x1ed2c1f7, /// 0x460
			 0x87f4b418, /// 0x464
			 0xe6d42fe7, /// 0x468
			 0xcac9b8af, /// 0x46c
			 0xa500532d, /// 0x470
			 0xb894f197, /// 0x474
			 0x1c31a4d4, /// 0x478
			 0xea9dfcb6, /// 0x47c
			 0xc68de454, /// 0x480
			 0xea08bc55, /// 0x484
			 0x1155058e, /// 0x488
			 0x0ae6d042, /// 0x48c
			 0xd64dbc28, /// 0x490
			 0x38200c1b, /// 0x494
			 0xbc7de458, /// 0x498
			 0xbfffef75, /// 0x49c
			 0x858f8ecb, /// 0x4a0
			 0x203af443, /// 0x4a4
			 0xd171059a, /// 0x4a8
			 0xac925b3d, /// 0x4ac
			 0xd4410f3c, /// 0x4b0
			 0xce851558, /// 0x4b4
			 0xf213eab2, /// 0x4b8
			 0x1423cf49, /// 0x4bc
			 0xe4ee457b, /// 0x4c0
			 0x9b50105d, /// 0x4c4
			 0x9655b693, /// 0x4c8
			 0x0fc16569, /// 0x4cc
			 0x3abe8d9d, /// 0x4d0
			 0x49eac491, /// 0x4d4
			 0xbced36a6, /// 0x4d8
			 0x64a828f8, /// 0x4dc
			 0x0b1117a8, /// 0x4e0
			 0x8cbcdd6d, /// 0x4e4
			 0xb76484c3, /// 0x4e8
			 0x72ea8039, /// 0x4ec
			 0xee57df36, /// 0x4f0
			 0xbb30d76b, /// 0x4f4
			 0x6e9c1897, /// 0x4f8
			 0x27941937, /// 0x4fc
			 0xdb8ffe54, /// 0x500
			 0x17318a06, /// 0x504
			 0x20c4f1ee, /// 0x508
			 0x2e64a3c6, /// 0x50c
			 0x4415edef, /// 0x510
			 0xe0e7c074, /// 0x514
			 0x5eaddb4e, /// 0x518
			 0xddd66dbe, /// 0x51c
			 0x0a37b5d2, /// 0x520
			 0x28feaf66, /// 0x524
			 0xcab01f28, /// 0x528
			 0x837b7391, /// 0x52c
			 0x2e3e0f58, /// 0x530
			 0x32f9e338, /// 0x534
			 0x46e65086, /// 0x538
			 0xc273f943, /// 0x53c
			 0x0e46bc64, /// 0x540
			 0x3940a3ef, /// 0x544
			 0xdd8c826b, /// 0x548
			 0x2dc45e47, /// 0x54c
			 0x0de91116, /// 0x550
			 0x4ebed242, /// 0x554
			 0x40f716da, /// 0x558
			 0x5dc17c93, /// 0x55c
			 0xb9e173a2, /// 0x560
			 0x37a949ee, /// 0x564
			 0x12a65414, /// 0x568
			 0x9eb94e77, /// 0x56c
			 0x3db36f5a, /// 0x570
			 0x0c997c9e, /// 0x574
			 0xc1bbeb58, /// 0x578
			 0xb34ebe46, /// 0x57c
			 0x61746f43, /// 0x580
			 0x3463297d, /// 0x584
			 0x46af39b0, /// 0x588
			 0xe8b44eec, /// 0x58c
			 0x3a4ea863, /// 0x590
			 0x99a106f9, /// 0x594
			 0xb18cee06, /// 0x598
			 0x629d4f2c, /// 0x59c
			 0xbc28559e, /// 0x5a0
			 0x5c57b3e6, /// 0x5a4
			 0x3e35ac6d, /// 0x5a8
			 0x91fe128a, /// 0x5ac
			 0x144293e5, /// 0x5b0
			 0x8ba55bc3, /// 0x5b4
			 0x1544031d, /// 0x5b8
			 0x0afc56ce, /// 0x5bc
			 0x47a015db, /// 0x5c0
			 0xd6e3c72f, /// 0x5c4
			 0x0a5fc556, /// 0x5c8
			 0x556a58f6, /// 0x5cc
			 0x1354743d, /// 0x5d0
			 0xa3376810, /// 0x5d4
			 0x829d56ed, /// 0x5d8
			 0x3a58c2fb, /// 0x5dc
			 0x98048160, /// 0x5e0
			 0x2c8d545e, /// 0x5e4
			 0x587f9923, /// 0x5e8
			 0xcd2bc423, /// 0x5ec
			 0x4144c42c, /// 0x5f0
			 0x2e5793d5, /// 0x5f4
			 0x7e1ecb74, /// 0x5f8
			 0xcc936d7d, /// 0x5fc
			 0xeb5b70fe, /// 0x600
			 0x64296990, /// 0x604
			 0x22a53b74, /// 0x608
			 0xbf8a1a7a, /// 0x60c
			 0xb5c74097, /// 0x610
			 0xda658b23, /// 0x614
			 0xaaa4c533, /// 0x618
			 0x0d961278, /// 0x61c
			 0xa7e3717c, /// 0x620
			 0xa0b333f7, /// 0x624
			 0xda7d68a4, /// 0x628
			 0x36cdd9a4, /// 0x62c
			 0x40230330, /// 0x630
			 0xed3e8664, /// 0x634
			 0x15e9c2e4, /// 0x638
			 0xff2da1b4, /// 0x63c
			 0x0e545dec, /// 0x640
			 0xa1594b2c, /// 0x644
			 0xfbadc227, /// 0x648
			 0xa2580191, /// 0x64c
			 0xed9b1ec7, /// 0x650
			 0x65b6cff0, /// 0x654
			 0x27dd5f0b, /// 0x658
			 0x805792ce, /// 0x65c
			 0x5cd13d40, /// 0x660
			 0x91189b97, /// 0x664
			 0x48dd31ba, /// 0x668
			 0x740b614c, /// 0x66c
			 0x111690f4, /// 0x670
			 0xb726539b, /// 0x674
			 0x9344e5f2, /// 0x678
			 0x86fa7fa6, /// 0x67c
			 0x055aba9e, /// 0x680
			 0x4ad5f89c, /// 0x684
			 0xc9dca503, /// 0x688
			 0x89fdef94, /// 0x68c
			 0xb81b89a1, /// 0x690
			 0x56473264, /// 0x694
			 0xe92054ef, /// 0x698
			 0xc5e81949, /// 0x69c
			 0xf63c3195, /// 0x6a0
			 0xf9d1739e, /// 0x6a4
			 0x94c7b359, /// 0x6a8
			 0x93246a6b, /// 0x6ac
			 0x3dc537cc, /// 0x6b0
			 0x84589d62, /// 0x6b4
			 0xa4818278, /// 0x6b8
			 0x23a830ad, /// 0x6bc
			 0x91428d65, /// 0x6c0
			 0x09922a5e, /// 0x6c4
			 0xc18f1311, /// 0x6c8
			 0x323c47f6, /// 0x6cc
			 0x6234ffca, /// 0x6d0
			 0x92c1c6b8, /// 0x6d4
			 0xba76e740, /// 0x6d8
			 0xfb525881, /// 0x6dc
			 0xa053e2f3, /// 0x6e0
			 0x4205f4eb, /// 0x6e4
			 0x3ca833f8, /// 0x6e8
			 0xeb14b123, /// 0x6ec
			 0x8ed9eb84, /// 0x6f0
			 0x3d3b49d4, /// 0x6f4
			 0x9b815677, /// 0x6f8
			 0xfdea491a, /// 0x6fc
			 0xe00e9fc5, /// 0x700
			 0xeeaa6588, /// 0x704
			 0xcfe8b5e5, /// 0x708
			 0xc2001454, /// 0x70c
			 0xb8b36a2c, /// 0x710
			 0x1b0f8634, /// 0x714
			 0xf602f2bf, /// 0x718
			 0x0699019c, /// 0x71c
			 0x566d2d15, /// 0x720
			 0x924290a7, /// 0x724
			 0xad3be72c, /// 0x728
			 0xcc60344b, /// 0x72c
			 0x9f1d395b, /// 0x730
			 0x87107961, /// 0x734
			 0xccf9e2d3, /// 0x738
			 0x31f6d421, /// 0x73c
			 0x4eebc2f8, /// 0x740
			 0xf4bc77b7, /// 0x744
			 0xe11c1300, /// 0x748
			 0x1e2120bb, /// 0x74c
			 0x97a03b94, /// 0x750
			 0xab20d9fb, /// 0x754
			 0x0d924bf8, /// 0x758
			 0xfdc1e1a9, /// 0x75c
			 0x2ec8a07f, /// 0x760
			 0x2fe92ee6, /// 0x764
			 0x94ddda0f, /// 0x768
			 0xf06e8037, /// 0x76c
			 0xdf57d228, /// 0x770
			 0x0ad1177f, /// 0x774
			 0x5d3893f9, /// 0x778
			 0x94b490c8, /// 0x77c
			 0xe87d054c, /// 0x780
			 0xe65d3472, /// 0x784
			 0xddcea0d9, /// 0x788
			 0xd62adac9, /// 0x78c
			 0xd0bea020, /// 0x790
			 0x49181d28, /// 0x794
			 0xef2d72b9, /// 0x798
			 0x7e52168a, /// 0x79c
			 0x216aba69, /// 0x7a0
			 0x892c8957, /// 0x7a4
			 0xa506e7a8, /// 0x7a8
			 0x5ad5f169, /// 0x7ac
			 0x787ae196, /// 0x7b0
			 0x7833cbc1, /// 0x7b4
			 0x71d509da, /// 0x7b8
			 0xbfb35dc7, /// 0x7bc
			 0x6cfdca45, /// 0x7c0
			 0x1eb5b615, /// 0x7c4
			 0xc4246d11, /// 0x7c8
			 0x693d64ae, /// 0x7cc
			 0x1877509e, /// 0x7d0
			 0x8c694600, /// 0x7d4
			 0x7e1e62df, /// 0x7d8
			 0x406c7fa7, /// 0x7dc
			 0x3ea5531d, /// 0x7e0
			 0xc467fab7, /// 0x7e4
			 0x3500c2ea, /// 0x7e8
			 0x2f83235e, /// 0x7ec
			 0x9e8b9dfd, /// 0x7f0
			 0x9956c35b, /// 0x7f4
			 0x2000b669, /// 0x7f8
			 0xa9d47a88, /// 0x7fc
			 0xa65730c8, /// 0x800
			 0xb0609962, /// 0x804
			 0x0df10e39, /// 0x808
			 0x6634344a, /// 0x80c
			 0x9c7bd756, /// 0x810
			 0x76675e73, /// 0x814
			 0xcd6f3950, /// 0x818
			 0xe3a50ad6, /// 0x81c
			 0xbf5a4098, /// 0x820
			 0xb5b1d8ca, /// 0x824
			 0x386913e9, /// 0x828
			 0xe8aa7e10, /// 0x82c
			 0x5b7da2c0, /// 0x830
			 0x99edc06d, /// 0x834
			 0xcccb58a2, /// 0x838
			 0xb0420176, /// 0x83c
			 0x42655aa2, /// 0x840
			 0x29053388, /// 0x844
			 0x40bfa94a, /// 0x848
			 0xf70621da, /// 0x84c
			 0x65c9659e, /// 0x850
			 0x97ce1186, /// 0x854
			 0xf74cd530, /// 0x858
			 0x360f45e6, /// 0x85c
			 0x6af3e5e3, /// 0x860
			 0x5ae77e25, /// 0x864
			 0xf3eb271e, /// 0x868
			 0xc9197bbf, /// 0x86c
			 0xfbecaab5, /// 0x870
			 0xdc62840f, /// 0x874
			 0x45f77a3a, /// 0x878
			 0xaacb05fb, /// 0x87c
			 0x0b1181d2, /// 0x880
			 0xa988b704, /// 0x884
			 0xecf3837a, /// 0x888
			 0x5402fcb1, /// 0x88c
			 0x6f20de9a, /// 0x890
			 0x683f1763, /// 0x894
			 0x97b59843, /// 0x898
			 0x2ebf3cfe, /// 0x89c
			 0x807a6717, /// 0x8a0
			 0xbdc97d37, /// 0x8a4
			 0x08c6e063, /// 0x8a8
			 0xca7d0448, /// 0x8ac
			 0x551bb503, /// 0x8b0
			 0x098e5766, /// 0x8b4
			 0xe4d59300, /// 0x8b8
			 0x26cc28af, /// 0x8bc
			 0xdb35fcc2, /// 0x8c0
			 0xd142ac17, /// 0x8c4
			 0xf4a15230, /// 0x8c8
			 0x8a741ec4, /// 0x8cc
			 0x4818239f, /// 0x8d0
			 0x7d6fe55a, /// 0x8d4
			 0x2b4993d7, /// 0x8d8
			 0x3a031650, /// 0x8dc
			 0xf49d71d2, /// 0x8e0
			 0x2398f779, /// 0x8e4
			 0x4ec1ea88, /// 0x8e8
			 0xae7b8e69, /// 0x8ec
			 0xd3197d4d, /// 0x8f0
			 0x2ecc9d41, /// 0x8f4
			 0xa459e148, /// 0x8f8
			 0xb59d169c, /// 0x8fc
			 0x74ca15ab, /// 0x900
			 0xb4a002bb, /// 0x904
			 0x985fa937, /// 0x908
			 0x3e461198, /// 0x90c
			 0xe50d2030, /// 0x910
			 0x390ca8fc, /// 0x914
			 0x7f5ee798, /// 0x918
			 0xd0be5a8d, /// 0x91c
			 0xeee972fb, /// 0x920
			 0x25918849, /// 0x924
			 0x7579da16, /// 0x928
			 0xc247d4a6, /// 0x92c
			 0x1fd437e7, /// 0x930
			 0x10d94861, /// 0x934
			 0x1a602a7c, /// 0x938
			 0x3bdd9f84, /// 0x93c
			 0x9c4484b6, /// 0x940
			 0xfd4381a9, /// 0x944
			 0x4fe020ec, /// 0x948
			 0xecf23340, /// 0x94c
			 0x9d3b717f, /// 0x950
			 0x4ef2b4ad, /// 0x954
			 0xd76ae98c, /// 0x958
			 0x0f35c8c2, /// 0x95c
			 0x3fca3b32, /// 0x960
			 0x1e2688a3, /// 0x964
			 0x64937f89, /// 0x968
			 0x955f8c15, /// 0x96c
			 0x62ef76ca, /// 0x970
			 0xca593dbd, /// 0x974
			 0x4fb092bd, /// 0x978
			 0xb7a88154, /// 0x97c
			 0x694760b3, /// 0x980
			 0xe757aad5, /// 0x984
			 0x5af3d2a8, /// 0x988
			 0x09cc2400, /// 0x98c
			 0x902a8c5c, /// 0x990
			 0x43549440, /// 0x994
			 0x67993b0b, /// 0x998
			 0x3e1c7a5f, /// 0x99c
			 0xcb9d6aad, /// 0x9a0
			 0x50d154f5, /// 0x9a4
			 0xbf032c4b, /// 0x9a8
			 0x54033f68, /// 0x9ac
			 0xb0dafcdb, /// 0x9b0
			 0x0c2366e8, /// 0x9b4
			 0x3311fc25, /// 0x9b8
			 0x0559b96b, /// 0x9bc
			 0x6594faed, /// 0x9c0
			 0xafc8e7e7, /// 0x9c4
			 0x0385dcd3, /// 0x9c8
			 0x060b9a51, /// 0x9cc
			 0xccb99801, /// 0x9d0
			 0x23196d75, /// 0x9d4
			 0xca035536, /// 0x9d8
			 0x28cb653f, /// 0x9dc
			 0x4bce7721, /// 0x9e0
			 0x57ad756d, /// 0x9e4
			 0xc368aa81, /// 0x9e8
			 0xb05de1d0, /// 0x9ec
			 0x189b88a6, /// 0x9f0
			 0x29e0d815, /// 0x9f4
			 0x42c5c131, /// 0x9f8
			 0xa7d3eaab, /// 0x9fc
			 0xa917e4f4, /// 0xa00
			 0x6d89f3cf, /// 0xa04
			 0x2fd42c8c, /// 0xa08
			 0x1dfb9b59, /// 0xa0c
			 0x1adb6178, /// 0xa10
			 0x2770e577, /// 0xa14
			 0x80b9f2dc, /// 0xa18
			 0x32189c39, /// 0xa1c
			 0x509871df, /// 0xa20
			 0xbbf06ba6, /// 0xa24
			 0x560595a3, /// 0xa28
			 0xa98f217e, /// 0xa2c
			 0x5430ac5e, /// 0xa30
			 0x0a09c60d, /// 0xa34
			 0x2252df48, /// 0xa38
			 0xeb4c3e4e, /// 0xa3c
			 0x43107226, /// 0xa40
			 0x19e6fc4c, /// 0xa44
			 0x8de56e67, /// 0xa48
			 0x8f5baa03, /// 0xa4c
			 0xe961b6e4, /// 0xa50
			 0x46d9ab39, /// 0xa54
			 0x61a4e368, /// 0xa58
			 0x8622efae, /// 0xa5c
			 0xec0640bb, /// 0xa60
			 0x4547cfe0, /// 0xa64
			 0xd89fea1f, /// 0xa68
			 0x212ca117, /// 0xa6c
			 0x73a978aa, /// 0xa70
			 0x491df2f9, /// 0xa74
			 0x493f350c, /// 0xa78
			 0x37255c89, /// 0xa7c
			 0x0b07b708, /// 0xa80
			 0x6e67560f, /// 0xa84
			 0x934662d8, /// 0xa88
			 0x73c5591e, /// 0xa8c
			 0x4a5c9432, /// 0xa90
			 0x394c3904, /// 0xa94
			 0x90307fb0, /// 0xa98
			 0x5a67b052, /// 0xa9c
			 0x3c894753, /// 0xaa0
			 0x492a6cd9, /// 0xaa4
			 0xb2fe59bc, /// 0xaa8
			 0x9fc8a90a, /// 0xaac
			 0xaacd8658, /// 0xab0
			 0x9ae4ea9c, /// 0xab4
			 0xd23b9acf, /// 0xab8
			 0x2d84cd65, /// 0xabc
			 0xa2c20eb1, /// 0xac0
			 0x66b6ee7c, /// 0xac4
			 0x4286346b, /// 0xac8
			 0x81b90641, /// 0xacc
			 0x9706ba50, /// 0xad0
			 0x93565dec, /// 0xad4
			 0x73bc2b8d, /// 0xad8
			 0xdb9ba4de, /// 0xadc
			 0x75d76974, /// 0xae0
			 0xa17bfea8, /// 0xae4
			 0xbece1d49, /// 0xae8
			 0x0827e070, /// 0xaec
			 0x86e09eee, /// 0xaf0
			 0x37f1550e, /// 0xaf4
			 0xcb1cd1cc, /// 0xaf8
			 0xa91f2d44, /// 0xafc
			 0xd6e8e834, /// 0xb00
			 0xd9a28b17, /// 0xb04
			 0xed959b19, /// 0xb08
			 0x9be19966, /// 0xb0c
			 0xe95d0a08, /// 0xb10
			 0x62acdea5, /// 0xb14
			 0x33751be2, /// 0xb18
			 0x868a96bb, /// 0xb1c
			 0x64fc9b69, /// 0xb20
			 0x5fb450d7, /// 0xb24
			 0x08a5b088, /// 0xb28
			 0x30477966, /// 0xb2c
			 0x758fc201, /// 0xb30
			 0x3a6874c9, /// 0xb34
			 0x601b2bcf, /// 0xb38
			 0x846c5846, /// 0xb3c
			 0x88835eb6, /// 0xb40
			 0xd7d8f6f9, /// 0xb44
			 0xe2266c13, /// 0xb48
			 0x5049e47e, /// 0xb4c
			 0xc054783f, /// 0xb50
			 0x9fa3e066, /// 0xb54
			 0x05f11ebc, /// 0xb58
			 0x650c3640, /// 0xb5c
			 0xdb7e0753, /// 0xb60
			 0xc81b5c93, /// 0xb64
			 0xb25b1325, /// 0xb68
			 0x90dd51b6, /// 0xb6c
			 0x95396285, /// 0xb70
			 0xeb64a255, /// 0xb74
			 0xfa3e156a, /// 0xb78
			 0x9f675148, /// 0xb7c
			 0x1573272f, /// 0xb80
			 0xab897484, /// 0xb84
			 0xff1477e5, /// 0xb88
			 0x43dc0f35, /// 0xb8c
			 0x63abd236, /// 0xb90
			 0xe05730ce, /// 0xb94
			 0x5164f3a8, /// 0xb98
			 0x3b54827a, /// 0xb9c
			 0x88ce1c04, /// 0xba0
			 0xe87fb184, /// 0xba4
			 0xcb1374d1, /// 0xba8
			 0x53679555, /// 0xbac
			 0x7415a90e, /// 0xbb0
			 0xf15aa42d, /// 0xbb4
			 0x26476650, /// 0xbb8
			 0xea2995c1, /// 0xbbc
			 0x7bb11c15, /// 0xbc0
			 0x4e8e52bc, /// 0xbc4
			 0x2f435ba8, /// 0xbc8
			 0xab0da602, /// 0xbcc
			 0x7ee37537, /// 0xbd0
			 0x845269fb, /// 0xbd4
			 0x2422d2fa, /// 0xbd8
			 0x754100e5, /// 0xbdc
			 0x69e6b20a, /// 0xbe0
			 0x1cee00f1, /// 0xbe4
			 0x8b666b16, /// 0xbe8
			 0x89e9cba2, /// 0xbec
			 0xa033cc18, /// 0xbf0
			 0x33082fb5, /// 0xbf4
			 0x89178148, /// 0xbf8
			 0x2e0e8b1e, /// 0xbfc
			 0xa365d682, /// 0xc00
			 0x972fa65b, /// 0xc04
			 0x0c100c7e, /// 0xc08
			 0x8556b5ed, /// 0xc0c
			 0x25e665e7, /// 0xc10
			 0xfeb644e4, /// 0xc14
			 0x75670b83, /// 0xc18
			 0x967632be, /// 0xc1c
			 0xea3ebbda, /// 0xc20
			 0xdf0183d1, /// 0xc24
			 0x8bca00c6, /// 0xc28
			 0xa5ed3169, /// 0xc2c
			 0xd4a33848, /// 0xc30
			 0xe0773495, /// 0xc34
			 0xaf2cb570, /// 0xc38
			 0x9ceb31b3, /// 0xc3c
			 0x243a8ba9, /// 0xc40
			 0x40a73a6b, /// 0xc44
			 0xac544d97, /// 0xc48
			 0x75619c05, /// 0xc4c
			 0x1bcf143b, /// 0xc50
			 0xae09a354, /// 0xc54
			 0x2ac70a4f, /// 0xc58
			 0xe13445ac, /// 0xc5c
			 0x985368d9, /// 0xc60
			 0xcc409830, /// 0xc64
			 0xef44f0fc, /// 0xc68
			 0x59460e25, /// 0xc6c
			 0xde1c3cf9, /// 0xc70
			 0x783b2e45, /// 0xc74
			 0xadc4dd6e, /// 0xc78
			 0x6d5099c4, /// 0xc7c
			 0x65a5b9a3, /// 0xc80
			 0xa975ca42, /// 0xc84
			 0x9eabd42b, /// 0xc88
			 0x737328d1, /// 0xc8c
			 0xbed18f1f, /// 0xc90
			 0x13099382, /// 0xc94
			 0x17e6e9b9, /// 0xc98
			 0x708758f7, /// 0xc9c
			 0x6a81a50a, /// 0xca0
			 0x91e0642a, /// 0xca4
			 0x9940c09d, /// 0xca8
			 0xf04a1ee8, /// 0xcac
			 0xeeb5f256, /// 0xcb0
			 0xac55345f, /// 0xcb4
			 0xe264f356, /// 0xcb8
			 0x6b780ecf, /// 0xcbc
			 0xaff02251, /// 0xcc0
			 0x656cfe2d, /// 0xcc4
			 0x5598b1bb, /// 0xcc8
			 0x5a921c3b, /// 0xccc
			 0x4f3fd35b, /// 0xcd0
			 0x7c916485, /// 0xcd4
			 0xb2875ef9, /// 0xcd8
			 0x9b79243b, /// 0xcdc
			 0x561aafc2, /// 0xce0
			 0xe0b236dc, /// 0xce4
			 0xa77091a5, /// 0xce8
			 0x296f33a0, /// 0xcec
			 0xd08411ca, /// 0xcf0
			 0x576c45b7, /// 0xcf4
			 0x69801508, /// 0xcf8
			 0xa21a6b72, /// 0xcfc
			 0x5fb36a7e, /// 0xd00
			 0x83dc6c21, /// 0xd04
			 0x7dd41f4c, /// 0xd08
			 0x8c8047aa, /// 0xd0c
			 0xa7b2c4a8, /// 0xd10
			 0xdd521962, /// 0xd14
			 0xa312c6de, /// 0xd18
			 0x5663ba3b, /// 0xd1c
			 0x38e3d582, /// 0xd20
			 0xc92aa711, /// 0xd24
			 0xc42c7e97, /// 0xd28
			 0xdac98214, /// 0xd2c
			 0xef90c64e, /// 0xd30
			 0x891ea60e, /// 0xd34
			 0xb43f046c, /// 0xd38
			 0x94c48ef4, /// 0xd3c
			 0xde40f25f, /// 0xd40
			 0xe1896fd6, /// 0xd44
			 0xec65a2ca, /// 0xd48
			 0x7af552b4, /// 0xd4c
			 0x637ca226, /// 0xd50
			 0x9c0c1512, /// 0xd54
			 0xc78788de, /// 0xd58
			 0x74fab7a0, /// 0xd5c
			 0xc7ba07a7, /// 0xd60
			 0x5c835d0a, /// 0xd64
			 0xf8f6869d, /// 0xd68
			 0x49dab244, /// 0xd6c
			 0x577fcc54, /// 0xd70
			 0x2341b5b4, /// 0xd74
			 0x85ae91bd, /// 0xd78
			 0x823dcebf, /// 0xd7c
			 0x045dcb00, /// 0xd80
			 0xbc77fafb, /// 0xd84
			 0xb5ae78d9, /// 0xd88
			 0xbc4a2f01, /// 0xd8c
			 0xa2a84266, /// 0xd90
			 0x01de5f57, /// 0xd94
			 0x8b4b0ad4, /// 0xd98
			 0x633f9b8c, /// 0xd9c
			 0x4db09466, /// 0xda0
			 0x3c4bfbef, /// 0xda4
			 0x889af4ae, /// 0xda8
			 0x741727de, /// 0xdac
			 0xc933287f, /// 0xdb0
			 0x150ef8be, /// 0xdb4
			 0x990a3ed4, /// 0xdb8
			 0x405da25b, /// 0xdbc
			 0x892bac25, /// 0xdc0
			 0x2ae79505, /// 0xdc4
			 0x795f9d3a, /// 0xdc8
			 0x77eb9d63, /// 0xdcc
			 0xf5730e0f, /// 0xdd0
			 0xaa140f63, /// 0xdd4
			 0x2aba78c5, /// 0xdd8
			 0xb756ddb8, /// 0xddc
			 0x2516a0a8, /// 0xde0
			 0x4509f02d, /// 0xde4
			 0x13fbb5f1, /// 0xde8
			 0xe9aeb98b, /// 0xdec
			 0x61eee269, /// 0xdf0
			 0x51278890, /// 0xdf4
			 0x874ccc19, /// 0xdf8
			 0x51a89875, /// 0xdfc
			 0x80ac11e7, /// 0xe00
			 0xf6a2b8c9, /// 0xe04
			 0xb553eb22, /// 0xe08
			 0xab26b25f, /// 0xe0c
			 0x1f1fd49e, /// 0xe10
			 0x02e42c88, /// 0xe14
			 0xd34d13c1, /// 0xe18
			 0x59d03640, /// 0xe1c
			 0x86b7cdbb, /// 0xe20
			 0xd1981aae, /// 0xe24
			 0xe0766f54, /// 0xe28
			 0xacd9e2e2, /// 0xe2c
			 0xd5df4131, /// 0xe30
			 0xd5d37996, /// 0xe34
			 0xdb6f07d6, /// 0xe38
			 0x96024101, /// 0xe3c
			 0x8388a49d, /// 0xe40
			 0x24535695, /// 0xe44
			 0x84bd7ef4, /// 0xe48
			 0x9eca8297, /// 0xe4c
			 0xa9c32072, /// 0xe50
			 0x9ca2fa96, /// 0xe54
			 0xcce34905, /// 0xe58
			 0xd52670cc, /// 0xe5c
			 0x688c91d3, /// 0xe60
			 0xb9362dfb, /// 0xe64
			 0x6d424428, /// 0xe68
			 0x4aece725, /// 0xe6c
			 0x24d3925a, /// 0xe70
			 0x43033f76, /// 0xe74
			 0x99a5ac98, /// 0xe78
			 0x60f4fcd5, /// 0xe7c
			 0xbf04ee47, /// 0xe80
			 0xcc880b66, /// 0xe84
			 0xbf49ca3e, /// 0xe88
			 0x86f84e84, /// 0xe8c
			 0x1458fc79, /// 0xe90
			 0xe3cc5826, /// 0xe94
			 0x0746dca8, /// 0xe98
			 0x929451eb, /// 0xe9c
			 0x96506ef8, /// 0xea0
			 0x83cdba19, /// 0xea4
			 0xf000a14a, /// 0xea8
			 0x463427f2, /// 0xeac
			 0x02bbd06e, /// 0xeb0
			 0x96c2d156, /// 0xeb4
			 0x41f4c4fd, /// 0xeb8
			 0xc51b0bc4, /// 0xebc
			 0x836c580f, /// 0xec0
			 0x149d565e, /// 0xec4
			 0x2decd5bf, /// 0xec8
			 0xe4b42860, /// 0xecc
			 0xf2115632, /// 0xed0
			 0x56463989, /// 0xed4
			 0xb0414055, /// 0xed8
			 0x9b052774, /// 0xedc
			 0x0482a17e, /// 0xee0
			 0x5567669a, /// 0xee4
			 0x9fb5fd9b, /// 0xee8
			 0xe3277ca3, /// 0xeec
			 0x4deaac44, /// 0xef0
			 0x40b283e0, /// 0xef4
			 0x5411fc0f, /// 0xef8
			 0x4d42e1c7, /// 0xefc
			 0xca84485c, /// 0xf00
			 0x20df8aea, /// 0xf04
			 0x76b0a573, /// 0xf08
			 0xece350bd, /// 0xf0c
			 0x0a23b092, /// 0xf10
			 0x3fe3b02e, /// 0xf14
			 0xd5ed2756, /// 0xf18
			 0x18181813, /// 0xf1c
			 0xb13498a7, /// 0xf20
			 0xaf3b6437, /// 0xf24
			 0x0eb0033c, /// 0xf28
			 0xdb2dfeec, /// 0xf2c
			 0x0c648159, /// 0xf30
			 0x6c4c1ebc, /// 0xf34
			 0xb2ce86a4, /// 0xf38
			 0x3ead9a9c, /// 0xf3c
			 0xabe128ee, /// 0xf40
			 0x150baf56, /// 0xf44
			 0xf25dbbb6, /// 0xf48
			 0x5818c0a6, /// 0xf4c
			 0xa7241327, /// 0xf50
			 0x1dab94bf, /// 0xf54
			 0x331c6ca4, /// 0xf58
			 0x26d6cd84, /// 0xf5c
			 0xce733043, /// 0xf60
			 0x5ec355b0, /// 0xf64
			 0x8e1f076e, /// 0xf68
			 0x41512b88, /// 0xf6c
			 0x7b9956bf, /// 0xf70
			 0x08e6080e, /// 0xf74
			 0x441886d2, /// 0xf78
			 0xa43a2804, /// 0xf7c
			 0x2e4ca7b1, /// 0xf80
			 0x2a315a57, /// 0xf84
			 0x0b30e7a1, /// 0xf88
			 0xb351a0ef, /// 0xf8c
			 0x9906dead, /// 0xf90
			 0x1f157c02, /// 0xf94
			 0x39024d28, /// 0xf98
			 0x8c3bbe44, /// 0xf9c
			 0xa9468ab0, /// 0xfa0
			 0x0eab6fab, /// 0xfa4
			 0x2551dab7, /// 0xfa8
			 0x6cfc8283, /// 0xfac
			 0x056afe78, /// 0xfb0
			 0x202e6eb9, /// 0xfb4
			 0x8c3359b5, /// 0xfb8
			 0x8d614f07, /// 0xfbc
			 0x6a9fa58b, /// 0xfc0
			 0x284946d1, /// 0xfc4
			 0x553ab8af, /// 0xfc8
			 0x84be23a7, /// 0xfcc
			 0x6abf78bd, /// 0xfd0
			 0x91be1858, /// 0xfd4
			 0xbcda79ab, /// 0xfd8
			 0x7a132b83, /// 0xfdc
			 0x3d246dff, /// 0xfe0
			 0xab89cf5b, /// 0xfe4
			 0xdbdf09af, /// 0xfe8
			 0x2d7c6e7d, /// 0xfec
			 0x4f0fb668, /// 0xff0
			 0x06999167, /// 0xff4
			 0x55d54a9b, /// 0xff8
			 0x8417babb /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00000
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00008
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0000c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00010
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00014
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00018
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0001c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00020
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00028
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0002c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x55555555,                                                  // 4 random values                             /// 0003c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00040
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00044
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00048
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0004c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00050
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00054
			 0x0e000007,                                                  // Trailing 1s:                                /// 00058
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00060
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00068
			 0x00000000,                                                  // zero                                        /// 0006c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00070
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00074
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00078
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00080
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00088
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00090
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00098
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0009c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f8
			 0x4b000000,                                                  // 8388608.0                                   /// 000fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00108
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0010c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00114
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00118
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0011c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00120
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x55555555,                                                  // 4 random values                             /// 00128
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00138
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00140
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00148
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00150
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00154
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00158
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0015c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00160
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00164
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0016c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0xbf028f5c,                                                  // -0.51                                       /// 00174
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00178
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0017c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00180
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00184
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00194
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00198
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001b0
			 0x80000000,                                                  // -zero                                       /// 001b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001fc
			 0xff800000,                                                  // -inf                                        /// 00200
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00204
			 0x33333333,                                                  // 4 random values                             /// 00208
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00210
			 0x0c780000,                                                  // Leading 1s:                                 /// 00214
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00218
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0021c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00224
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00228
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0022c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00230
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00234
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00238
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00240
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00244
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00248
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0024c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00250
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00254
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x0c780000,                                                  // Leading 1s:                                 /// 0025c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00260
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00264
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00268
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00274
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00278
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00280
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00284
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00294
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00298
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0029c
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002dc
			 0xcb000000,                                                  // -8388608.0                                  /// 002e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00304
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00308
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00310
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00314
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00318
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0031c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00320
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00324
			 0x80011111,                                                  // -9.7958E-41                                 /// 00328
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0032c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00330
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00334
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0033c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00340
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00344
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00348
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0034c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00350
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00354
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00360
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00364
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0036c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00370
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00378
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00380
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00384
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00388
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0038c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00390
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00394
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00398
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0039c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x55555555,                                                  // 4 random values                             /// 003ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c8
			 0x80000000,                                                  // -zero                                       /// 003cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0040c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00414
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00418
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00420
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00424
			 0x55555555,                                                  // 4 random values                             /// 00428
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0042c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00011111,                                                  // 9.7958E-41                                  /// 00434
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00440
			 0x7fc00001,                                                  // signaling NaN                               /// 00444
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00448
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0044c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00450
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00458
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00464
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0046c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00474
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00478
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0047c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00480
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00488
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0048c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00490
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00494
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00498
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0049c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 004f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004f8
			 0x4b000000,                                                  // 8388608.0                                   /// 004fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00500
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00504
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00514
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x33333333,                                                  // 4 random values                             /// 00528
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0052c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00530
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00534
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00538
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0053c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00540
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0054c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00550
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00554
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00558
			 0x0c600000,                                                  // Leading 1s:                                 /// 0055c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00560
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00564
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0056c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c600000,                                                  // Leading 1s:                                 /// 0057c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00580
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00584
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00588
			 0x0c400000,                                                  // Leading 1s:                                 /// 0058c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00594
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00598
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00600
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00604
			 0x80011111,                                                  // -9.7958E-41                                 /// 00608
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0060c
			 0x3f028f5c,                                                  // 0.51                                        /// 00610
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0061c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00620
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00624
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00628
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00630
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00634
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00644
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00648
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0064c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00650
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00654
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00658
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00660
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00664
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0066c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00670
			 0x0c700000,                                                  // Leading 1s:                                 /// 00674
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0067c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00680
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00684
			 0x80000000,                                                  // -zero                                       /// 00688
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00690
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00694
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0069c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006a8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006f4
			 0x4b000000,                                                  // 8388608.0                                   /// 006f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00708
			 0x0c780000,                                                  // Leading 1s:                                 /// 0070c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00710
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c700000,                                                  // Leading 1s:                                 /// 00718
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0071c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00724
			 0x0c400000,                                                  // Leading 1s:                                 /// 00728
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0072c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00730
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00734
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00738
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0073c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00740
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00744
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00000000,                                                  // zero                                        /// 0074c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00750
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00754
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00758
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0075c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00760
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00764
			 0x7fc00001,                                                  // signaling NaN                               /// 00768
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0076c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00774
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00778
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0c700000,                                                  // Leading 1s:                                 /// 00788
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00794
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00798
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0079c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00800
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00804
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00808
			 0x55555555,                                                  // 4 random values                             /// 0080c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00810
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00814
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00818
			 0x80011111,                                                  // -9.7958E-41                                 /// 0081c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00828
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00834
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0084c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00850
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00854
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00858
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0085c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00864
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00868
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0086c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00870
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00874
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00880
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00888
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0088c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00890
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00898
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008e0
			 0xff800000,                                                  // -inf                                        /// 008e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f8
			 0xcb000000,                                                  // -8388608.0                                  /// 008fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00900
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00908
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0091c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00928
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0092c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00934
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00938
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00940
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00948
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00950
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00958
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00960
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00964
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00968
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00970
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00980
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00984
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00988
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0e000007,                                                  // Trailing 1s:                                /// 00994
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00998
			 0x55555555,                                                  // 4 random values                             /// 0099c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009b4
			 0x7f800000,                                                  // inf                                         /// 009b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0xcb000000,                                                  // -8388608.0                                  /// 009fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xcb000000,                                                  // -8388608.0                                  /// 00a38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x33333333,                                                  // 4 random values                             /// 00aa4
			 0xcb000000,                                                  // -8388608.0                                  /// 00aa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ab0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ab8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00abc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00acc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ad4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00adc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ae4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ae8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00af4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00afc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b18
			 0xffc00001,                                                  // -signaling NaN                              /// 00b1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x55555555,                                                  // 4 random values                             /// 00bb8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bc0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bc8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bd4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bd8
			 0xff800000,                                                  // -inf                                        /// 00bdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bfc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x7f800000,                                                  // inf                                         /// 00c3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cb8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cc0
			 0xff800000,                                                  // -inf                                        /// 00cc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ccc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cfc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xcb000000,                                                  // -8388608.0                                  /// 00d6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x7fc00001,                                                  // signaling NaN                               /// 00d98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00db8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dcc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00de4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00de8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dfc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0xffc00001,                                                  // -signaling NaN                              /// 00e88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x80000000,                                                  // -zero                                       /// 00e9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eb0
			 0x80000000,                                                  // -zero                                       /// 00eb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ec8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ecc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ed8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00edc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ee4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ef0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00efc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f98
			 0x80000000,                                                  // -zero                                       /// 00f9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fa0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fa8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fe0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ff4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff8
			 0xffc00000                                                  // -cquiet NaN                                 /// last
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
			 0x000006d0,
			 0x000000ec,
			 0x00000194,
			 0x00000238,
			 0x000001b0,
			 0x00000080,
			 0x000002ac,
			 0x000007cc,

			 /// vpu register f2
			 0x40a00000,
			 0x41800000,
			 0x41100000,
			 0x41100000,
			 0x41880000,
			 0x41b80000,
			 0x41b00000,
			 0x40e00000,

			 /// vpu register f3
			 0x41d00000,
			 0x41c80000,
			 0x41100000,
			 0x41f00000,
			 0x41c80000,
			 0x41a80000,
			 0x41900000,
			 0x41e00000,

			 /// vpu register f4
			 0x41800000,
			 0x41300000,
			 0x41a80000,
			 0x41d00000,
			 0x40e00000,
			 0x41b80000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f5
			 0x41d00000,
			 0x40000000,
			 0x41a80000,
			 0x41100000,
			 0x3f800000,
			 0x41c00000,
			 0x41300000,
			 0x41a80000,

			 /// vpu register f6
			 0x41200000,
			 0x41a80000,
			 0x41200000,
			 0x41e80000,
			 0x40400000,
			 0x41600000,
			 0x40800000,
			 0x41100000,

			 /// vpu register f7
			 0x41880000,
			 0x40e00000,
			 0x41000000,
			 0x41a00000,
			 0x41d80000,
			 0x41e00000,
			 0x41900000,
			 0x42000000,

			 /// vpu register f8
			 0x41d80000,
			 0x41f80000,
			 0x41300000,
			 0x41100000,
			 0x41200000,
			 0x40e00000,
			 0x41600000,
			 0x41600000,

			 /// vpu register f9
			 0x42000000,
			 0x41f80000,
			 0x41000000,
			 0x41400000,
			 0x40800000,
			 0x41d00000,
			 0x40a00000,
			 0x40000000,

			 /// vpu register f10
			 0x41500000,
			 0x41a80000,
			 0x40800000,
			 0x41f80000,
			 0x41400000,
			 0x41b80000,
			 0x41e80000,
			 0x41200000,

			 /// vpu register f11
			 0x41f00000,
			 0x40e00000,
			 0x40e00000,
			 0x41b00000,
			 0x3f800000,
			 0x41500000,
			 0x41e80000,
			 0x41e80000,

			 /// vpu register f12
			 0x40800000,
			 0x41f80000,
			 0x41a00000,
			 0x41880000,
			 0x3f800000,
			 0x41f00000,
			 0x42000000,
			 0x41100000,

			 /// vpu register f13
			 0x40800000,
			 0x40c00000,
			 0x40e00000,
			 0x41600000,
			 0x41500000,
			 0x41f00000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f14
			 0x41880000,
			 0x41900000,
			 0x41300000,
			 0x41d00000,
			 0x41600000,
			 0x40c00000,
			 0x41600000,
			 0x42000000,

			 /// vpu register f15
			 0x41b80000,
			 0x41900000,
			 0x41e80000,
			 0x41500000,
			 0x41a00000,
			 0x41e00000,
			 0x41900000,
			 0x41a00000,

			 /// vpu register f16
			 0x41b80000,
			 0x3f800000,
			 0x40e00000,
			 0x41880000,
			 0x41a80000,
			 0x41f00000,
			 0x40a00000,
			 0x40e00000,

			 /// vpu register f17
			 0x41880000,
			 0x41d80000,
			 0x41e80000,
			 0x41b80000,
			 0x41400000,
			 0x41b80000,
			 0x41c80000,
			 0x41500000,

			 /// vpu register f18
			 0x41600000,
			 0x40800000,
			 0x41000000,
			 0x41900000,
			 0x41a00000,
			 0x41880000,
			 0x41800000,
			 0x41200000,

			 /// vpu register f19
			 0x3f800000,
			 0x41e80000,
			 0x41800000,
			 0x41600000,
			 0x41c80000,
			 0x41100000,
			 0x41b80000,
			 0x41900000,

			 /// vpu register f20
			 0x41e80000,
			 0x41000000,
			 0x41e00000,
			 0x41a80000,
			 0x40e00000,
			 0x41300000,
			 0x41980000,
			 0x40a00000,

			 /// vpu register f21
			 0x41700000,
			 0x41980000,
			 0x41400000,
			 0x41600000,
			 0x42000000,
			 0x41600000,
			 0x41d80000,
			 0x41300000,

			 /// vpu register f22
			 0x41d00000,
			 0x41a80000,
			 0x41f00000,
			 0x41d80000,
			 0x41800000,
			 0x41800000,
			 0x40400000,
			 0x41b80000,

			 /// vpu register f23
			 0x41900000,
			 0x41300000,
			 0x41400000,
			 0x41600000,
			 0x41400000,
			 0x40400000,
			 0x41600000,
			 0x41a00000,

			 /// vpu register f24
			 0x40800000,
			 0x41900000,
			 0x41400000,
			 0x41b00000,
			 0x41600000,
			 0x41b00000,
			 0x42000000,
			 0x3f800000,

			 /// vpu register f25
			 0x41880000,
			 0x41e80000,
			 0x41980000,
			 0x41b00000,
			 0x41980000,
			 0x41100000,
			 0x41e80000,
			 0x40e00000,

			 /// vpu register f26
			 0x42000000,
			 0x41900000,
			 0x41b80000,
			 0x3f800000,
			 0x41400000,
			 0x41f80000,
			 0x41400000,
			 0x40800000,

			 /// vpu register f27
			 0x41e00000,
			 0x42000000,
			 0x41900000,
			 0x40e00000,
			 0x3f800000,
			 0x41d00000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f28
			 0x41980000,
			 0x40400000,
			 0x41a80000,
			 0x42000000,
			 0x41a00000,
			 0x41800000,
			 0x41200000,
			 0x41d80000,

			 /// vpu register f29
			 0x41c00000,
			 0x41400000,
			 0x41b80000,
			 0x41e80000,
			 0x41d00000,
			 0x40400000,
			 0x41700000,
			 0x40c00000,

			 /// vpu register f30
			 0x41600000,
			 0x41b80000,
			 0x41980000,
			 0x41000000,
			 0x41a00000,
			 0x41100000,
			 0x3f800000,
			 0x41b80000,

			 /// vpu register f31
			 0x41400000,
			 0x41d80000,
			 0x41600000,
			 0x41100000,
			 0x40a00000,
			 0x41100000,
			 0x41200000,
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
		"famoxorg.pi f4, f1 (x12)\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f9, f1 (x12)\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f18, f1 (x13)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f2, f1 (x13)\n"                          ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f30, f1 (x11)\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f9, f1 (x13)\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f3, f1 (x11)\n"                          ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f30, f1 (x13)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f14, f1 (x12)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f30, f1 (x11)\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f24, f1 (x14)\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f4, f1 (x14)\n"                          ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f21, f1 (x14)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f22, f1 (x14)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f13, f1 (x12)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f10, f1 (x11)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f5, f1 (x11)\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f15, f1 (x14)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f11, f1 (x13)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f27, f1 (x11)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f25, f1 (x13)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f15, f1 (x15)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f11, f1 (x14)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f9, f1 (x12)\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f2, f1 (x13)\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f27, f1 (x15)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f17, f1 (x12)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f25, f1 (x12)\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f14, f1 (x15)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f22, f1 (x12)\n"                         ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f21, f1 (x15)\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f7, f1 (x14)\n"                          ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f20, f1 (x13)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f13, f1 (x11)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f29, f1 (x15)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f26, f1 (x12)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f3, f1 (x13)\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f10, f1 (x15)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f23, f1 (x12)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f16, f1 (x15)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f21, f1 (x11)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f29, f1 (x15)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f25, f1 (x15)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f16, f1 (x11)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f3, f1 (x13)\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f23, f1 (x12)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f16, f1 (x14)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f26, f1 (x13)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f6, f1 (x11)\n"                          ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f12, f1 (x13)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f23, f1 (x12)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f2, f1 (x13)\n"                          ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f4, f1 (x12)\n"                          ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f17, f1 (x13)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f22, f1 (x13)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f22, f1 (x11)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f23, f1 (x12)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f11, f1 (x14)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f29, f1 (x11)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f21, f1 (x11)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f11, f1 (x11)\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f23, f1 (x12)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f6, f1 (x15)\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f2, f1 (x11)\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f17, f1 (x11)\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f11, f1 (x11)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f15, f1 (x12)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f17, f1 (x14)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f6, f1 (x13)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f30, f1 (x13)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f26, f1 (x14)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f14, f1 (x11)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f25, f1 (x15)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f11, f1 (x11)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f19, f1 (x15)\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f6, f1 (x13)\n"                          ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f8, f1 (x11)\n"                          ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f12, f1 (x15)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f7, f1 (x12)\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f22, f1 (x12)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f17, f1 (x11)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f21, f1 (x13)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f27, f1 (x13)\n"                         ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f25, f1 (x12)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f28, f1 (x11)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f3, f1 (x15)\n"                          ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f13, f1 (x14)\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f7, f1 (x11)\n"                          ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f13, f1 (x14)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f25, f1 (x11)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f28, f1 (x12)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f21, f1 (x15)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f12, f1 (x12)\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f17, f1 (x11)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f9, f1 (x13)\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f18, f1 (x12)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f23, f1 (x12)\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f2, f1 (x11)\n"                          ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorg.pi f15, f1 (x11)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
