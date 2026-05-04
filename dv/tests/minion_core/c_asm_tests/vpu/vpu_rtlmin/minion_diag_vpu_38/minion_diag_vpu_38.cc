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
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00008
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0e000001,                                                  // Trailing 1s:                                /// 00018
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00020
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00024
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00028
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00038
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0003c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00040
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00044
			 0x7fc00001,                                                  // signaling NaN                               /// 00048
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00050
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x3f028f5c,                                                  // 0.51                                        /// 00058
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0005c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00064
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00068
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0006c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00070
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00074
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0007c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00088
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00090
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 000b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x7f800000,                                                  // inf                                         /// 000d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x55555555,                                                  // 4 random values                             /// 000e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00100
			 0x33333333,                                                  // 4 random values                             /// 00104
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00110
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0011c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00120
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00128
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00130
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00134
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00148
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00150
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00154
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00158
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0015c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00164
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00168
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0016c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00174
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00178
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00180
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00190
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00194
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00198
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x4b000000,                                                  // 8388608.0                                   /// 001b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b8
			 0xffc00001,                                                  // -signaling NaN                              /// 001bc
			 0x7f800000,                                                  // inf                                         /// 001c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001c4
			 0x00000000,                                                  // zero                                        /// 001c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d8
			 0x4b000000,                                                  // 8388608.0                                   /// 001dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001e4
			 0xff800000,                                                  // -inf                                        /// 001e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x00000000,                                                  // zero                                        /// 001f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00200
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00210
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00220
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00224
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00228
			 0x3f028f5c,                                                  // 0.51                                        /// 0022c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00230
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00234
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00240
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00244
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00250
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00258
			 0x3f028f5c,                                                  // 0.51                                        /// 0025c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00264
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x4b000000,                                                  // 8388608.0                                   /// 0026c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00274
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00278
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00280
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0028c
			 0x00000000,                                                  // zero                                        /// 00290
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00294
			 0x0c700000,                                                  // Leading 1s:                                 /// 00298
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0029c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00300
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00304
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00308
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0030c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00310
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00314
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00320
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00328
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0032c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00330
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00334
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00340
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x0e000001,                                                  // Trailing 1s:                                /// 00348
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0034c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00354
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0035c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00360
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00364
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00368
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00374
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00378
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00384
			 0x00011111,                                                  // 9.7958E-41                                  /// 00388
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0038c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00390
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00394
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00398
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00404
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00408
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0040c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00410
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00414
			 0x4b000000,                                                  // 8388608.0                                   /// 00418
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0041c
			 0x00000000,                                                  // zero                                        /// 00420
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00424
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00428
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0042c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0043c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00440
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00444
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00454
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00460
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00468
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0046c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00470
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00478
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0047c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00480
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00484
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00490
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00494
			 0x00011111,                                                  // 9.7958E-41                                  /// 00498
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004cc
			 0xbf028f5c,                                                  // -0.51                                       /// 004d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004ec
			 0x55555555,                                                  // 4 random values                             /// 004f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00504
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00508
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0050c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00514
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00518
			 0x3f028f5c,                                                  // 0.51                                        /// 0051c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00530
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00538
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0053c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00540
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x33333333,                                                  // 4 random values                             /// 0054c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00550
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00554
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00558
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x55555555,                                                  // 4 random values                             /// 00568
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0056c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00574
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0057c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00580
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00584
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00588
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00594
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0059c
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00600
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00604
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00608
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0060c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00610
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00614
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00628
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00630
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00634
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0063c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00640
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00644
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00648
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0064c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00650
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00658
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x7fc00001,                                                  // signaling NaN                               /// 00664
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00668
			 0x0c400000,                                                  // Leading 1s:                                 /// 0066c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00670
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00674
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00678
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0067c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00680
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00684
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00688
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00690
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00694
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00698
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x55555555,                                                  // 4 random values                             /// 006a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00704
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0070c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00710
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00714
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00720
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00728
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00738
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0073c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0e000003,                                                  // Trailing 1s:                                /// 00744
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00748
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00754
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00758
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0075c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00768
			 0x0c600000,                                                  // Leading 1s:                                 /// 0076c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00774
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00780
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00784
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00788
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0xffc00001,                                                  // -signaling NaN                              /// 00790
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00794
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00798
			 0x7f800000,                                                  // inf                                         /// 0079c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a0
			 0x55555555,                                                  // 4 random values                             /// 007a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c0
			 0xbf028f5c,                                                  // -0.51                                       /// 007c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007d8
			 0xff800000,                                                  // -inf                                        /// 007dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007f8
			 0x7fc00001,                                                  // signaling NaN                               /// 007fc
			 0x55555555,                                                  // 4 random values                             /// 00800
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00804
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00808
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0080c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0081c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00820
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00838
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0083c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00840
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0084c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00850
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0085c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00860
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00864
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0086c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00888
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00894
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00898
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d0
			 0xbf028f5c,                                                  // -0.51                                       /// 008d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00904
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00908
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0090c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00910
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00000000,                                                  // zero                                        /// 00918
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0091c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00928
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x3f028f5c,                                                  // 0.51                                        /// 00938
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0093c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00940
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00944
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0094c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00960
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00964
			 0x7f800000,                                                  // inf                                         /// 00968
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0096c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00970
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00978
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00980
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00984
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00988
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00998
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0099c
			 0x0e000007,                                                  // Trailing 1s:                                /// 009a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x33333333,                                                  // 4 random values                             /// 009bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009c0
			 0xbf028f5c,                                                  // -0.51                                       /// 009c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d8
			 0x3f028f5c,                                                  // 0.51                                        /// 009dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xbf028f5c,                                                  // -0.51                                       /// 00a08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a7c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00aa0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aa8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ab0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00abc
			 0xff800000,                                                  // -inf                                        /// 00ac0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ac4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00acc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ad0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad8
			 0x7f800000,                                                  // inf                                         /// 00adc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ae0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00af0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00afc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b20
			 0xffc00001,                                                  // -signaling NaN                              /// 00b24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x7fc00001,                                                  // signaling NaN                               /// 00b84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b94
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ba0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ba8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bb0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bd4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00be0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bf0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bfc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c28
			 0x55555555,                                                  // 4 random values                             /// 00c2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c40
			 0x7f800000,                                                  // inf                                         /// 00c44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c70
			 0x80000000,                                                  // -zero                                       /// 00c74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c94
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x33333333,                                                  // 4 random values                             /// 00ca4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ca8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cdc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ce0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d60
			 0x3f028f5c,                                                  // 0.51                                        /// 00d64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00da8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00db4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00db8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00de0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x00000000,                                                  // zero                                        /// 00de8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e04
			 0x80000000,                                                  // -zero                                       /// 00e08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e2c
			 0x55555555,                                                  // 4 random values                             /// 00e30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e44
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e68
			 0xcb000000,                                                  // -8388608.0                                  /// 00e6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00eb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ec0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ef8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00efc
			 0x00000000,                                                  // zero                                        /// 00f00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f6c
			 0x33333333,                                                  // 4 random values                             /// 00f70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x3f028f5c,                                                  // 0.51                                        /// 00f94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fa0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fa4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x55555555,                                                  // 4 random values                             /// 00fcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fe4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ff0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ff4
			 0x00000000,                                                  // zero                                        /// 00ff8
			 0x80800000 // min norm                                      // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00000
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00004
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00008
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0001c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00020
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0002c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00030
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00034
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00044
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00048
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0004c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00058
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00064
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00068
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00070
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00074
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00078
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0007c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00080
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00084
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00088
			 0x0e000003,                                                  // Trailing 1s:                                /// 0008c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00094
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000ac
			 0xff800000,                                                  // -inf                                        /// 000b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000dc
			 0x80000000,                                                  // -zero                                       /// 000e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00100
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00104
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00108
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0010c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00118
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0011c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00124
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00128
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00134
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00138
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00144
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00148
			 0x0e000003,                                                  // Trailing 1s:                                /// 0014c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00154
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00158
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00160
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00168
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0016c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00170
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0c780000,                                                  // Leading 1s:                                 /// 00178
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0017c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00184
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00188
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0018c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00190
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00194
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0019c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d4
			 0x33333333,                                                  // 4 random values                             /// 001d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00204
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00208
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00210
			 0x4b000000,                                                  // 8388608.0                                   /// 00214
			 0x7fc00001,                                                  // signaling NaN                               /// 00218
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0021c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00224
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00228
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0022c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00234
			 0x0c780000,                                                  // Leading 1s:                                 /// 00238
			 0x7fc00001,                                                  // signaling NaN                               /// 0023c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00240
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00244
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00248
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0024c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00254
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00258
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0025c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00260
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00268
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0026c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00270
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00274
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0027c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00280
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00000000,                                                  // zero                                        /// 00288
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0028c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00290
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00294
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00298
			 0xbf028f5c,                                                  // -0.51                                       /// 0029c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xcb000000,                                                  // -8388608.0                                  /// 002c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00304
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00308
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00310
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0031c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00320
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00324
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00328
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0032c
			 0xff800000,                                                  // -inf                                        /// 00330
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00334
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00338
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0033c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00340
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00344
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00348
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00350
			 0x80000000,                                                  // -zero                                       /// 00354
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00358
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0035c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00360
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00364
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00370
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00374
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00000000,                                                  // zero                                        /// 00380
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00384
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0038c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0039c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e0
			 0x55555555,                                                  // 4 random values                             /// 003e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00404
			 0x0c600000,                                                  // Leading 1s:                                 /// 00408
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0040c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00410
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00414
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00420
			 0x80000000,                                                  // -zero                                       /// 00424
			 0xffc00001,                                                  // -signaling NaN                              /// 00428
			 0x0c600000,                                                  // Leading 1s:                                 /// 0042c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00430
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00438
			 0x80000000,                                                  // -zero                                       /// 0043c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x3f028f5c,                                                  // 0.51                                        /// 00444
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0044c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00450
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00458
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0045c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00460
			 0x7f800000,                                                  // inf                                         /// 00464
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00468
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0046c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00474
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00480
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00484
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00488
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00494
			 0x0e000001,                                                  // Trailing 1s:                                /// 00498
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0049c
			 0x4b000000,                                                  // 8388608.0                                   /// 004a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x00000000,                                                  // zero                                        /// 004d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00508
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00510
			 0x0e000007,                                                  // Trailing 1s:                                /// 00514
			 0x0c400000,                                                  // Leading 1s:                                 /// 00518
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0051c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7f800000,                                                  // inf                                         /// 00524
			 0x0e000001,                                                  // Trailing 1s:                                /// 00528
			 0x0e000007,                                                  // Trailing 1s:                                /// 0052c
			 0x4b000000,                                                  // 8388608.0                                   /// 00530
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00534
			 0xff800000,                                                  // -inf                                        /// 00538
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00540
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00544
			 0xffc00001,                                                  // -signaling NaN                              /// 00548
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0054c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00550
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00554
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0055c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00564
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00568
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0056c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0057c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00580
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00588
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00590
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00594
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00598
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0059c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0xbf028f5c,                                                  // -0.51                                       /// 00604
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0060c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00614
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00618
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0061c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00620
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00624
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00011111,                                                  // 9.7958E-41                                  /// 0062c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00638
			 0x0e000001,                                                  // Trailing 1s:                                /// 0063c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00640
			 0x00011111,                                                  // 9.7958E-41                                  /// 00644
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00648
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00650
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0c780000,                                                  // Leading 1s:                                 /// 00658
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00660
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00664
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0066c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00670
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00674
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0067c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00684
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00688
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00698
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0069c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006b4
			 0x80000000,                                                  // -zero                                       /// 006b8
			 0x4b000000,                                                  // 8388608.0                                   /// 006bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006dc
			 0x00000000,                                                  // zero                                        /// 006e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00700
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00704
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x0c700000,                                                  // Leading 1s:                                 /// 0070c
			 0x7f800000,                                                  // inf                                         /// 00710
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00724
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00728
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0072c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00730
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00734
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0073c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00740
			 0x0c600000,                                                  // Leading 1s:                                 /// 00744
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00748
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0074c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00750
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00758
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00768
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00778
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0077c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0c600000,                                                  // Leading 1s:                                 /// 00784
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00788
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00794
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00798
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007bc
			 0x33333333,                                                  // 4 random values                             /// 007c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007cc
			 0x00000000,                                                  // zero                                        /// 007d0
			 0x7fc00001,                                                  // signaling NaN                               /// 007d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007e4
			 0x80000000,                                                  // -zero                                       /// 007e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00804
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00808
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0080c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00810
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0081c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00824
			 0x33333333,                                                  // 4 random values                             /// 00828
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0082c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00834
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x4b000000,                                                  // 8388608.0                                   /// 00844
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00848
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0084c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00850
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00854
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00858
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00860
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0086c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00870
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00874
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00880
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0088c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00890
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0089c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xff800000,                                                  // -inf                                        /// 008ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00904
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00908
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0090c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00910
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00914
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00918
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0091c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x7f800000,                                                  // inf                                         /// 00928
			 0x0c780000,                                                  // Leading 1s:                                 /// 0092c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00930
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00934
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0093c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00944
			 0x7fc00001,                                                  // signaling NaN                               /// 00948
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00950
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00954
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0095c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00960
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00968
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0096c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00970
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7f800000,                                                  // inf                                         /// 00978
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00980
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00988
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0098c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00990
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00994
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00998
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0099c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e0
			 0x3f028f5c,                                                  // 0.51                                        /// 009e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x4b000000,                                                  // 8388608.0                                   /// 00a68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x55555555,                                                  // 4 random values                             /// 00a88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a9c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aa4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ab0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ac4
			 0x00000000,                                                  // zero                                        /// 00ac8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00acc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ad0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00af8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00afc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b20
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ba4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bbc
			 0xff800000,                                                  // -inf                                        /// 00bc0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bdc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00be0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00be4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00be8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bf4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c00
			 0x00000000,                                                  // zero                                        /// 00c04
			 0xff800000,                                                  // -inf                                        /// 00c08
			 0x3f028f5c,                                                  // 0.51                                        /// 00c0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c28
			 0x7fc00001,                                                  // signaling NaN                               /// 00c2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c40
			 0x7f800000,                                                  // inf                                         /// 00c44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ca0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ccc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000000,                                                  // -zero                                       /// 00cdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ce8
			 0x00000000,                                                  // zero                                        /// 00cec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cfc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d34
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00da0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00da8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00db0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00db4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dcc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ddc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00de4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00df4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00df8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x80000000,                                                  // -zero                                       /// 00e20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x3f028f5c,                                                  // 0.51                                        /// 00e54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e7c
			 0x55555555,                                                  // 4 random values                             /// 00e80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00ea0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ec4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ec8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ecc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ee4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ee8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ef8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x33333333,                                                  // 4 random values                             /// 00f10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fb8
			 0x55555555,                                                  // 4 random values                             /// 00fbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x7fc00001,                                                  // signaling NaN                               /// 00fd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fe4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fec
			 0xcb000000,                                                  // -8388608.0                                  /// 00ff0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ff4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x4b000000                                                  // 8388608.0                                   /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xb08f9ce4, /// 0x0
			 0xbf400054, /// 0x4
			 0x822d7bd4, /// 0x8
			 0x0d1f4494, /// 0xc
			 0x2fa0acfb, /// 0x10
			 0x2c98c5f0, /// 0x14
			 0x904bde7e, /// 0x18
			 0xf01c3444, /// 0x1c
			 0xa9282318, /// 0x20
			 0x144e80d1, /// 0x24
			 0x955b9533, /// 0x28
			 0xee3dd655, /// 0x2c
			 0x9a405efb, /// 0x30
			 0x0a9f3b2d, /// 0x34
			 0x0a4b0010, /// 0x38
			 0xb3cfe061, /// 0x3c
			 0x214a9d34, /// 0x40
			 0xfa8d94e1, /// 0x44
			 0xa4a8dc68, /// 0x48
			 0x697bbf55, /// 0x4c
			 0xf8889bbf, /// 0x50
			 0xa2bf6c4b, /// 0x54
			 0x2369060f, /// 0x58
			 0x302e411b, /// 0x5c
			 0x084adef3, /// 0x60
			 0x3c28316e, /// 0x64
			 0x78dcf2f2, /// 0x68
			 0x35bb050d, /// 0x6c
			 0xb3b64e4b, /// 0x70
			 0x86417a11, /// 0x74
			 0x682f3a18, /// 0x78
			 0x23f816d6, /// 0x7c
			 0xe66ed595, /// 0x80
			 0x42af4f1a, /// 0x84
			 0xce8b39c4, /// 0x88
			 0x62337249, /// 0x8c
			 0x5d5fcc7b, /// 0x90
			 0x65429c17, /// 0x94
			 0xc475368a, /// 0x98
			 0x39572faf, /// 0x9c
			 0x4c4d7a49, /// 0xa0
			 0x8e71be83, /// 0xa4
			 0x5f421b7f, /// 0xa8
			 0x32ae6518, /// 0xac
			 0x5cce86a8, /// 0xb0
			 0x36d05dba, /// 0xb4
			 0xac9f4970, /// 0xb8
			 0x8de5dd7a, /// 0xbc
			 0x9185e012, /// 0xc0
			 0xe89f9bb3, /// 0xc4
			 0x31b56fb0, /// 0xc8
			 0xca48c350, /// 0xcc
			 0x10c8f63f, /// 0xd0
			 0x4a544639, /// 0xd4
			 0xed054fdc, /// 0xd8
			 0x1ba79649, /// 0xdc
			 0xb1ef1229, /// 0xe0
			 0x76936ee5, /// 0xe4
			 0xa44bbdb9, /// 0xe8
			 0x10f16d2e, /// 0xec
			 0xf7459e6b, /// 0xf0
			 0xf4d6cee4, /// 0xf4
			 0x04cf6286, /// 0xf8
			 0xa2027fcb, /// 0xfc
			 0xcd6bdceb, /// 0x100
			 0x655ff827, /// 0x104
			 0xa6d20429, /// 0x108
			 0x80e07e26, /// 0x10c
			 0xf7f2c587, /// 0x110
			 0x5f661e3e, /// 0x114
			 0xa10e857b, /// 0x118
			 0x026774cc, /// 0x11c
			 0xa6d2fc07, /// 0x120
			 0xc94b89b5, /// 0x124
			 0x1adcda6a, /// 0x128
			 0x3cb281e9, /// 0x12c
			 0x4f0e700a, /// 0x130
			 0xb5277652, /// 0x134
			 0xd64707b9, /// 0x138
			 0x2c58f4f8, /// 0x13c
			 0x3133588f, /// 0x140
			 0x76bf2881, /// 0x144
			 0x2de0085a, /// 0x148
			 0x5451e5e9, /// 0x14c
			 0xb8eb7cf1, /// 0x150
			 0xcd325d2e, /// 0x154
			 0x2d267fe9, /// 0x158
			 0x12906bc6, /// 0x15c
			 0xcb7ae014, /// 0x160
			 0x65de2c62, /// 0x164
			 0xf059c7a4, /// 0x168
			 0xf09d5cf6, /// 0x16c
			 0x6f8697cc, /// 0x170
			 0x3eb05901, /// 0x174
			 0x4af036e5, /// 0x178
			 0x9f334ea7, /// 0x17c
			 0xebc29b52, /// 0x180
			 0x398cdc9e, /// 0x184
			 0xbf94e5d5, /// 0x188
			 0x9536ceec, /// 0x18c
			 0xbf9cbcdd, /// 0x190
			 0x6de1be11, /// 0x194
			 0x37540e12, /// 0x198
			 0xef14570d, /// 0x19c
			 0x06bc6874, /// 0x1a0
			 0x2786c448, /// 0x1a4
			 0x44f7b5fb, /// 0x1a8
			 0xb6b944bd, /// 0x1ac
			 0xfeec4721, /// 0x1b0
			 0x82b9ad8f, /// 0x1b4
			 0x9e045dde, /// 0x1b8
			 0x03a57950, /// 0x1bc
			 0x75af031a, /// 0x1c0
			 0x84228487, /// 0x1c4
			 0xb88372c5, /// 0x1c8
			 0x6dcd0b40, /// 0x1cc
			 0x75348321, /// 0x1d0
			 0x968d4673, /// 0x1d4
			 0x36c465a6, /// 0x1d8
			 0x2ea2be9e, /// 0x1dc
			 0x04e7e54c, /// 0x1e0
			 0x49d48ce1, /// 0x1e4
			 0x03588ba1, /// 0x1e8
			 0xeb32934c, /// 0x1ec
			 0xf7ff39a0, /// 0x1f0
			 0x571ae90f, /// 0x1f4
			 0xe057bc37, /// 0x1f8
			 0xe3c56451, /// 0x1fc
			 0x3370c3dd, /// 0x200
			 0x6c91b99e, /// 0x204
			 0x1b771c20, /// 0x208
			 0x0af507bc, /// 0x20c
			 0x60988e94, /// 0x210
			 0x2305aa18, /// 0x214
			 0xd2f4c28a, /// 0x218
			 0xd3d6c7ca, /// 0x21c
			 0x9ab4a0cd, /// 0x220
			 0xf35e2a83, /// 0x224
			 0x99831853, /// 0x228
			 0x03917d04, /// 0x22c
			 0xc04f1b21, /// 0x230
			 0x24e03fe9, /// 0x234
			 0x9d4f0a09, /// 0x238
			 0xe37df515, /// 0x23c
			 0xd70a20a6, /// 0x240
			 0xce1f8872, /// 0x244
			 0x06f943c2, /// 0x248
			 0x5aa8a7d2, /// 0x24c
			 0xde545592, /// 0x250
			 0x91a123b3, /// 0x254
			 0x6b42b6c3, /// 0x258
			 0x53f87fb3, /// 0x25c
			 0x6d7d8200, /// 0x260
			 0xecb99e55, /// 0x264
			 0x967e8f88, /// 0x268
			 0xe7adb91b, /// 0x26c
			 0x9e242595, /// 0x270
			 0xb62d5464, /// 0x274
			 0x8d4c68a4, /// 0x278
			 0xd2112a3e, /// 0x27c
			 0x5215b1e9, /// 0x280
			 0x4c081611, /// 0x284
			 0xf25dead9, /// 0x288
			 0xc40e7155, /// 0x28c
			 0xe01b9704, /// 0x290
			 0x2e3d1f9d, /// 0x294
			 0xf45ae9d1, /// 0x298
			 0x3295374f, /// 0x29c
			 0x20abafb6, /// 0x2a0
			 0x3fa00070, /// 0x2a4
			 0xef7e08d3, /// 0x2a8
			 0x00c59f27, /// 0x2ac
			 0x969885fd, /// 0x2b0
			 0x0046ee4a, /// 0x2b4
			 0x21db6d1b, /// 0x2b8
			 0x24ae29d9, /// 0x2bc
			 0xc1bfcdfb, /// 0x2c0
			 0x8b7a60f6, /// 0x2c4
			 0xd51f1c98, /// 0x2c8
			 0x432ca459, /// 0x2cc
			 0xec4123be, /// 0x2d0
			 0xea51edb2, /// 0x2d4
			 0xe84afa7c, /// 0x2d8
			 0x03d236cd, /// 0x2dc
			 0x67e66c62, /// 0x2e0
			 0xb84a7fd7, /// 0x2e4
			 0xc365b1f2, /// 0x2e8
			 0x4237c71b, /// 0x2ec
			 0x2549d957, /// 0x2f0
			 0x00a1aeb6, /// 0x2f4
			 0x91574fbb, /// 0x2f8
			 0x62e50a18, /// 0x2fc
			 0xdb786ab5, /// 0x300
			 0xa1134116, /// 0x304
			 0x88204371, /// 0x308
			 0x10a6902f, /// 0x30c
			 0xa752c967, /// 0x310
			 0x173ddd2c, /// 0x314
			 0x40aa5447, /// 0x318
			 0xf07cf5d6, /// 0x31c
			 0xfc97ebd6, /// 0x320
			 0xfac943d2, /// 0x324
			 0xe915acd7, /// 0x328
			 0x11076f1c, /// 0x32c
			 0xc2a1a7b6, /// 0x330
			 0x6b13631c, /// 0x334
			 0x8e9475a3, /// 0x338
			 0x01e5937e, /// 0x33c
			 0x09644d53, /// 0x340
			 0xf1290259, /// 0x344
			 0x1e841087, /// 0x348
			 0x95846a9a, /// 0x34c
			 0xd0be0806, /// 0x350
			 0x3e47f2af, /// 0x354
			 0xe2ac2b2a, /// 0x358
			 0x931b8636, /// 0x35c
			 0xf5cc0ddd, /// 0x360
			 0xfc83f907, /// 0x364
			 0x9fc31118, /// 0x368
			 0xfb25d412, /// 0x36c
			 0xda87fd38, /// 0x370
			 0x953d7f63, /// 0x374
			 0x8f913f3b, /// 0x378
			 0x698013e2, /// 0x37c
			 0x04771ac1, /// 0x380
			 0x35f2ae57, /// 0x384
			 0x37e7f4d0, /// 0x388
			 0xf8034663, /// 0x38c
			 0x93bed22b, /// 0x390
			 0x181fcb4e, /// 0x394
			 0xc3ca1f3e, /// 0x398
			 0x41d35578, /// 0x39c
			 0xb2cc92da, /// 0x3a0
			 0xf75b0a73, /// 0x3a4
			 0x90927c10, /// 0x3a8
			 0x8cb1b80f, /// 0x3ac
			 0x0efa7c7f, /// 0x3b0
			 0x1074310f, /// 0x3b4
			 0xf423a3df, /// 0x3b8
			 0xf7def463, /// 0x3bc
			 0xef61d658, /// 0x3c0
			 0x34c6eece, /// 0x3c4
			 0xdf4785c3, /// 0x3c8
			 0x72bc5dfe, /// 0x3cc
			 0x3ecdf49a, /// 0x3d0
			 0x74ccda2c, /// 0x3d4
			 0xdc30114c, /// 0x3d8
			 0xedf70a1a, /// 0x3dc
			 0x88fd6c65, /// 0x3e0
			 0x8e92ef57, /// 0x3e4
			 0x35aea979, /// 0x3e8
			 0xe2cda737, /// 0x3ec
			 0xde38d5eb, /// 0x3f0
			 0x0164a62c, /// 0x3f4
			 0xabc0c743, /// 0x3f8
			 0x8498b7a9, /// 0x3fc
			 0x8c972329, /// 0x400
			 0xa5797d0f, /// 0x404
			 0xd1d30914, /// 0x408
			 0xf2e77964, /// 0x40c
			 0x2a43f17c, /// 0x410
			 0x5c84aac0, /// 0x414
			 0xf274fe9b, /// 0x418
			 0x003dd7af, /// 0x41c
			 0x283eba6d, /// 0x420
			 0xfedc3bd6, /// 0x424
			 0x3b2f1458, /// 0x428
			 0x47739885, /// 0x42c
			 0x141905ff, /// 0x430
			 0x2dad11ca, /// 0x434
			 0xe2e02e76, /// 0x438
			 0xdeffa0ce, /// 0x43c
			 0xdc9eeb98, /// 0x440
			 0x661fb366, /// 0x444
			 0xe96b2e15, /// 0x448
			 0x2213c0f0, /// 0x44c
			 0x1b078b6b, /// 0x450
			 0x184b7072, /// 0x454
			 0xcda9f5c3, /// 0x458
			 0x170ea9ae, /// 0x45c
			 0x2b498ac5, /// 0x460
			 0xc4ccf06a, /// 0x464
			 0x438d2f01, /// 0x468
			 0xdc7341f1, /// 0x46c
			 0xb3639ac8, /// 0x470
			 0xb3a4d7e1, /// 0x474
			 0xbbc6e4bc, /// 0x478
			 0xb69b800a, /// 0x47c
			 0xa82d0716, /// 0x480
			 0x7e10b787, /// 0x484
			 0xcc9e6bdc, /// 0x488
			 0x4e4ce1fe, /// 0x48c
			 0xe5aa5188, /// 0x490
			 0x96a10dd6, /// 0x494
			 0x32f6ff79, /// 0x498
			 0x895dd5f6, /// 0x49c
			 0x57510f5c, /// 0x4a0
			 0xea230fc8, /// 0x4a4
			 0xbf2a626c, /// 0x4a8
			 0x3f8e7547, /// 0x4ac
			 0xe21b359d, /// 0x4b0
			 0xbe92d980, /// 0x4b4
			 0x915518d5, /// 0x4b8
			 0x0c971286, /// 0x4bc
			 0xf72b334b, /// 0x4c0
			 0x687bbbbb, /// 0x4c4
			 0x927f0410, /// 0x4c8
			 0xc8f5e531, /// 0x4cc
			 0x1ac0377c, /// 0x4d0
			 0xe9707210, /// 0x4d4
			 0xc24b97ec, /// 0x4d8
			 0x7d4b4cf0, /// 0x4dc
			 0x7df66630, /// 0x4e0
			 0x1146d73b, /// 0x4e4
			 0x1d051428, /// 0x4e8
			 0x35c16f19, /// 0x4ec
			 0x909f39ee, /// 0x4f0
			 0x25f15014, /// 0x4f4
			 0x19f6b3a7, /// 0x4f8
			 0x664a8c37, /// 0x4fc
			 0x49a89138, /// 0x500
			 0x7e8aac99, /// 0x504
			 0xd93603d4, /// 0x508
			 0x96b6f754, /// 0x50c
			 0x6562d2a7, /// 0x510
			 0x35eed4d6, /// 0x514
			 0x6ceb45be, /// 0x518
			 0x8de56f56, /// 0x51c
			 0x0a7281ae, /// 0x520
			 0x7de54ac7, /// 0x524
			 0x8e5dec79, /// 0x528
			 0x97dd263d, /// 0x52c
			 0x2b1b6e22, /// 0x530
			 0x7d2fd544, /// 0x534
			 0xe04581d1, /// 0x538
			 0x3eaa764c, /// 0x53c
			 0x0610e00c, /// 0x540
			 0x35721657, /// 0x544
			 0x189492fc, /// 0x548
			 0x5fd6878f, /// 0x54c
			 0x878c61b0, /// 0x550
			 0x5c0151b2, /// 0x554
			 0xa5c994d8, /// 0x558
			 0xc17e9c27, /// 0x55c
			 0xb91f11ed, /// 0x560
			 0xebffd5ff, /// 0x564
			 0x7ef4ee51, /// 0x568
			 0x2522d5dc, /// 0x56c
			 0x38f99fdd, /// 0x570
			 0x4dbe9ba6, /// 0x574
			 0xf0223e08, /// 0x578
			 0x9e2d7c9e, /// 0x57c
			 0x226ed3f5, /// 0x580
			 0x83b4feab, /// 0x584
			 0xa9fb0754, /// 0x588
			 0x10e069fc, /// 0x58c
			 0x30968aad, /// 0x590
			 0x7b43d0b5, /// 0x594
			 0x957dca79, /// 0x598
			 0xbff3307a, /// 0x59c
			 0x19338545, /// 0x5a0
			 0x3bc0d87e, /// 0x5a4
			 0x722a655e, /// 0x5a8
			 0xbcde4858, /// 0x5ac
			 0xf7192a1d, /// 0x5b0
			 0x5bda7cbe, /// 0x5b4
			 0x6ffde0d1, /// 0x5b8
			 0x20d6e225, /// 0x5bc
			 0x18eec6f3, /// 0x5c0
			 0x3f018758, /// 0x5c4
			 0x698c32ef, /// 0x5c8
			 0x28ccdd8d, /// 0x5cc
			 0x4a3cbc12, /// 0x5d0
			 0xdd8b9f02, /// 0x5d4
			 0x64b61904, /// 0x5d8
			 0x07f81317, /// 0x5dc
			 0x5a8e6d8e, /// 0x5e0
			 0xba52bbe0, /// 0x5e4
			 0x29467096, /// 0x5e8
			 0xf19909d3, /// 0x5ec
			 0x01a3ce70, /// 0x5f0
			 0x3c401de1, /// 0x5f4
			 0xa87bc16e, /// 0x5f8
			 0x8bc8bf4f, /// 0x5fc
			 0x22dc5600, /// 0x600
			 0xda7e26f1, /// 0x604
			 0x51ee022e, /// 0x608
			 0x7f3d889b, /// 0x60c
			 0x623e0415, /// 0x610
			 0x355b817e, /// 0x614
			 0x17eed1f4, /// 0x618
			 0xce35c0d7, /// 0x61c
			 0xcabc118b, /// 0x620
			 0xa46b806a, /// 0x624
			 0x362a463e, /// 0x628
			 0x3b3260b9, /// 0x62c
			 0x97fc32a9, /// 0x630
			 0x854b720e, /// 0x634
			 0x37f4f113, /// 0x638
			 0x080a7c8f, /// 0x63c
			 0x94b99d90, /// 0x640
			 0x0b3201d3, /// 0x644
			 0x1b21590d, /// 0x648
			 0xdf1ae588, /// 0x64c
			 0x0bc0d615, /// 0x650
			 0x6cc083e6, /// 0x654
			 0xe44e139b, /// 0x658
			 0xabdbdda0, /// 0x65c
			 0x39597d27, /// 0x660
			 0xed5af85f, /// 0x664
			 0x81ed537f, /// 0x668
			 0x0e9f849f, /// 0x66c
			 0x7c5665d4, /// 0x670
			 0x5755f66d, /// 0x674
			 0x5f1e2651, /// 0x678
			 0xfe6086fa, /// 0x67c
			 0xd33688ea, /// 0x680
			 0xbb1a8c71, /// 0x684
			 0xe40fd00e, /// 0x688
			 0xf5457dbe, /// 0x68c
			 0x9c9cf1c6, /// 0x690
			 0xfeda9255, /// 0x694
			 0xce4d0977, /// 0x698
			 0x06a54d95, /// 0x69c
			 0x93ddd51a, /// 0x6a0
			 0x14bc7a13, /// 0x6a4
			 0x19fe70dd, /// 0x6a8
			 0xb9871a9b, /// 0x6ac
			 0xc562f13d, /// 0x6b0
			 0xc71e5107, /// 0x6b4
			 0x02693255, /// 0x6b8
			 0x2d55efae, /// 0x6bc
			 0x1a5087bf, /// 0x6c0
			 0x6abdfac3, /// 0x6c4
			 0xd481dd02, /// 0x6c8
			 0x2a2d3be6, /// 0x6cc
			 0x09d9eb3c, /// 0x6d0
			 0xf477427f, /// 0x6d4
			 0x02a22613, /// 0x6d8
			 0x9281f2e1, /// 0x6dc
			 0xa51e1f71, /// 0x6e0
			 0x23ac4813, /// 0x6e4
			 0xe46cbd21, /// 0x6e8
			 0xf9d41c57, /// 0x6ec
			 0x9ce03936, /// 0x6f0
			 0x43ad17a4, /// 0x6f4
			 0x54b439ba, /// 0x6f8
			 0xfdccb209, /// 0x6fc
			 0xc1040cad, /// 0x700
			 0x6c7bd7ce, /// 0x704
			 0x46fdea0b, /// 0x708
			 0x217f5ea5, /// 0x70c
			 0x1febd3d2, /// 0x710
			 0x7f3f6098, /// 0x714
			 0x4f85dd97, /// 0x718
			 0x0e714f03, /// 0x71c
			 0xbff66aa3, /// 0x720
			 0xc7e01732, /// 0x724
			 0x5e7b8323, /// 0x728
			 0xae6aa016, /// 0x72c
			 0x33fb7442, /// 0x730
			 0x40c9bf05, /// 0x734
			 0x07881413, /// 0x738
			 0x359e5f96, /// 0x73c
			 0x0768dda7, /// 0x740
			 0x52631b49, /// 0x744
			 0x94e5735a, /// 0x748
			 0xf07fa607, /// 0x74c
			 0xcd6a576c, /// 0x750
			 0x3b5e6b75, /// 0x754
			 0x446b8490, /// 0x758
			 0x6bb2b4d6, /// 0x75c
			 0xb07cf828, /// 0x760
			 0x2e6965fc, /// 0x764
			 0x2047351e, /// 0x768
			 0x2d1827b2, /// 0x76c
			 0x3c3239dd, /// 0x770
			 0x819f06bf, /// 0x774
			 0x40082394, /// 0x778
			 0x3c01c41b, /// 0x77c
			 0x7f499758, /// 0x780
			 0xc19c15d5, /// 0x784
			 0xb571f4aa, /// 0x788
			 0x80e2ce36, /// 0x78c
			 0x09df4c4a, /// 0x790
			 0xf3a14a87, /// 0x794
			 0x12668d52, /// 0x798
			 0x9311750d, /// 0x79c
			 0x2628b2aa, /// 0x7a0
			 0xe14ad11d, /// 0x7a4
			 0xa44b6599, /// 0x7a8
			 0xec8555c4, /// 0x7ac
			 0xda66f4a1, /// 0x7b0
			 0xc2df4142, /// 0x7b4
			 0x4f77fd5c, /// 0x7b8
			 0xe82e98da, /// 0x7bc
			 0x36383869, /// 0x7c0
			 0xc5ee386c, /// 0x7c4
			 0x6ca62882, /// 0x7c8
			 0xe9fa1033, /// 0x7cc
			 0x500ff69e, /// 0x7d0
			 0x54982534, /// 0x7d4
			 0x571614d4, /// 0x7d8
			 0x2b11cc39, /// 0x7dc
			 0x1c3fa666, /// 0x7e0
			 0x280165c5, /// 0x7e4
			 0x03473586, /// 0x7e8
			 0x82a73e7c, /// 0x7ec
			 0x56b9c42d, /// 0x7f0
			 0xa4e8b3c3, /// 0x7f4
			 0xef8970ae, /// 0x7f8
			 0x75306d75, /// 0x7fc
			 0xb25c71a7, /// 0x800
			 0xbb21afb7, /// 0x804
			 0x7b09c408, /// 0x808
			 0x93576df3, /// 0x80c
			 0x2f64092c, /// 0x810
			 0xbb98ace0, /// 0x814
			 0x187156ef, /// 0x818
			 0x480e5ad8, /// 0x81c
			 0x7c0f9bdc, /// 0x820
			 0x043588c9, /// 0x824
			 0x4bbe9dce, /// 0x828
			 0x5ce0f9e2, /// 0x82c
			 0x444b5f80, /// 0x830
			 0x0677afbf, /// 0x834
			 0x66de0608, /// 0x838
			 0x6c0548a8, /// 0x83c
			 0x5235a243, /// 0x840
			 0x388102cc, /// 0x844
			 0xaece31f2, /// 0x848
			 0x0fa98aea, /// 0x84c
			 0x2e05667c, /// 0x850
			 0x0bc36f81, /// 0x854
			 0x386ba677, /// 0x858
			 0x03431260, /// 0x85c
			 0x6dd2956b, /// 0x860
			 0xf0545c6e, /// 0x864
			 0xd73c7166, /// 0x868
			 0xbb75157a, /// 0x86c
			 0x2911c2f0, /// 0x870
			 0xc45aedb8, /// 0x874
			 0x8221ea6a, /// 0x878
			 0x03af36eb, /// 0x87c
			 0x4d15a005, /// 0x880
			 0x80e58865, /// 0x884
			 0xef061f40, /// 0x888
			 0xa7ecb80a, /// 0x88c
			 0xa5760530, /// 0x890
			 0x0ef73190, /// 0x894
			 0xb43c7403, /// 0x898
			 0x697ee523, /// 0x89c
			 0xb4d02c83, /// 0x8a0
			 0x931eb008, /// 0x8a4
			 0xb43cd798, /// 0x8a8
			 0xbd2234fa, /// 0x8ac
			 0x96379781, /// 0x8b0
			 0xa92cc428, /// 0x8b4
			 0xebbcc23e, /// 0x8b8
			 0xc5254aed, /// 0x8bc
			 0xafb14419, /// 0x8c0
			 0x68b43d03, /// 0x8c4
			 0x8690ed84, /// 0x8c8
			 0x4a58e6f4, /// 0x8cc
			 0xc245e398, /// 0x8d0
			 0x2c5064ec, /// 0x8d4
			 0x75df3592, /// 0x8d8
			 0x715d3f38, /// 0x8dc
			 0xbb065557, /// 0x8e0
			 0x92e14245, /// 0x8e4
			 0xb1e5292c, /// 0x8e8
			 0x40538313, /// 0x8ec
			 0x661465dc, /// 0x8f0
			 0xd7605b46, /// 0x8f4
			 0x298fe7e3, /// 0x8f8
			 0xfbe18223, /// 0x8fc
			 0xa4855cf7, /// 0x900
			 0xb12b9427, /// 0x904
			 0x6d26bda0, /// 0x908
			 0x3bc54644, /// 0x90c
			 0xe4550906, /// 0x910
			 0x7e21c35e, /// 0x914
			 0x4f52edfb, /// 0x918
			 0xfb96f402, /// 0x91c
			 0xf0318b1d, /// 0x920
			 0x37a24341, /// 0x924
			 0x27ded17b, /// 0x928
			 0xe9b41f48, /// 0x92c
			 0x89c4d73b, /// 0x930
			 0xaf83113a, /// 0x934
			 0x75a81869, /// 0x938
			 0x7ecff6b4, /// 0x93c
			 0x03f35fa7, /// 0x940
			 0xe49d92b6, /// 0x944
			 0x638e5d81, /// 0x948
			 0x75be7f62, /// 0x94c
			 0xf1ec4e99, /// 0x950
			 0x8be18905, /// 0x954
			 0x132ef337, /// 0x958
			 0xbd97fbc3, /// 0x95c
			 0xbd54aa11, /// 0x960
			 0x68afec62, /// 0x964
			 0x737ccb06, /// 0x968
			 0x5c05d9bf, /// 0x96c
			 0x39dc7999, /// 0x970
			 0xe910614e, /// 0x974
			 0xb56d7127, /// 0x978
			 0xd3458000, /// 0x97c
			 0xa0277b7e, /// 0x980
			 0xa3b911d1, /// 0x984
			 0x92ec4c07, /// 0x988
			 0x5df03832, /// 0x98c
			 0xc19c6c9d, /// 0x990
			 0xf0bef1e7, /// 0x994
			 0x84abc396, /// 0x998
			 0xd551d0c3, /// 0x99c
			 0x58aefeb6, /// 0x9a0
			 0xb04f45cc, /// 0x9a4
			 0x2701159a, /// 0x9a8
			 0xb1f77475, /// 0x9ac
			 0x4c88c611, /// 0x9b0
			 0xf8db003d, /// 0x9b4
			 0x19c2e92e, /// 0x9b8
			 0xfb61d37e, /// 0x9bc
			 0x6aa77fd7, /// 0x9c0
			 0x8ff423a5, /// 0x9c4
			 0x87114e34, /// 0x9c8
			 0xb405aaf7, /// 0x9cc
			 0x9e52b1c1, /// 0x9d0
			 0xf88ce6e9, /// 0x9d4
			 0xc49dc185, /// 0x9d8
			 0x484975f2, /// 0x9dc
			 0x0a59a66e, /// 0x9e0
			 0xb426cbe4, /// 0x9e4
			 0x7cfd66d1, /// 0x9e8
			 0x84e978d3, /// 0x9ec
			 0x9a3045aa, /// 0x9f0
			 0xe7e40c8e, /// 0x9f4
			 0xd667a88c, /// 0x9f8
			 0xfdb1cc11, /// 0x9fc
			 0xda8fbd63, /// 0xa00
			 0xc09632ba, /// 0xa04
			 0x48721219, /// 0xa08
			 0xaedd2237, /// 0xa0c
			 0x066bffcd, /// 0xa10
			 0xbadc1918, /// 0xa14
			 0x7c7d4964, /// 0xa18
			 0xfb4c7be2, /// 0xa1c
			 0x56082f12, /// 0xa20
			 0x3736e82c, /// 0xa24
			 0xe53debdb, /// 0xa28
			 0x9f0adadd, /// 0xa2c
			 0xe99442b3, /// 0xa30
			 0x2a3a436a, /// 0xa34
			 0x56e28c24, /// 0xa38
			 0x944189ae, /// 0xa3c
			 0xef47a1cc, /// 0xa40
			 0x21c5dc8e, /// 0xa44
			 0xfe0e9638, /// 0xa48
			 0xbacc5c28, /// 0xa4c
			 0x04fff002, /// 0xa50
			 0x538bf8f9, /// 0xa54
			 0xcc844a47, /// 0xa58
			 0x38275e4c, /// 0xa5c
			 0xef66f1e5, /// 0xa60
			 0x7a84f462, /// 0xa64
			 0x0c2b7d89, /// 0xa68
			 0x92a2535e, /// 0xa6c
			 0xb6d7b65d, /// 0xa70
			 0x84d2cc4b, /// 0xa74
			 0xe6905717, /// 0xa78
			 0x5c4f02f4, /// 0xa7c
			 0xf4ec8a57, /// 0xa80
			 0x79697057, /// 0xa84
			 0xc3ea7373, /// 0xa88
			 0x836d7469, /// 0xa8c
			 0x7d2ba812, /// 0xa90
			 0x2404e01d, /// 0xa94
			 0x2bc97990, /// 0xa98
			 0x8dfd5b95, /// 0xa9c
			 0xd72d9123, /// 0xaa0
			 0x0f207768, /// 0xaa4
			 0x5e521b38, /// 0xaa8
			 0x8ee8857c, /// 0xaac
			 0x4323834d, /// 0xab0
			 0x2ce3206e, /// 0xab4
			 0x4c8beeaf, /// 0xab8
			 0xfe1dcdd4, /// 0xabc
			 0xf1462cef, /// 0xac0
			 0x9ae2fa7f, /// 0xac4
			 0x2d67c717, /// 0xac8
			 0x2678ee71, /// 0xacc
			 0xc434909a, /// 0xad0
			 0x15664379, /// 0xad4
			 0x05ede986, /// 0xad8
			 0xdf9d948c, /// 0xadc
			 0xd45285e1, /// 0xae0
			 0x12878076, /// 0xae4
			 0xb8296f0a, /// 0xae8
			 0x0d55e47a, /// 0xaec
			 0x2152755e, /// 0xaf0
			 0x00548a5b, /// 0xaf4
			 0xaf4c04d5, /// 0xaf8
			 0xacad5b16, /// 0xafc
			 0xea93950e, /// 0xb00
			 0x956f71f5, /// 0xb04
			 0x9a2dbac8, /// 0xb08
			 0x8eb13af4, /// 0xb0c
			 0x7f75d285, /// 0xb10
			 0xbcf4c56e, /// 0xb14
			 0x77069ac3, /// 0xb18
			 0x6e48b7d1, /// 0xb1c
			 0xd9db3d0f, /// 0xb20
			 0x813c1897, /// 0xb24
			 0x6f92b272, /// 0xb28
			 0x93bf27ce, /// 0xb2c
			 0x533d55de, /// 0xb30
			 0xa91492f8, /// 0xb34
			 0x62c9e492, /// 0xb38
			 0x517ffccf, /// 0xb3c
			 0xad1c2ec2, /// 0xb40
			 0x561cd245, /// 0xb44
			 0x353726cf, /// 0xb48
			 0x75d0aaf3, /// 0xb4c
			 0x82ebbd9b, /// 0xb50
			 0x2308a50b, /// 0xb54
			 0x6f74e193, /// 0xb58
			 0xcbb58fa9, /// 0xb5c
			 0x4176089a, /// 0xb60
			 0xa0bbb294, /// 0xb64
			 0xf80d3db8, /// 0xb68
			 0xbc118ca9, /// 0xb6c
			 0xc06ef53b, /// 0xb70
			 0x011df80c, /// 0xb74
			 0x2bf678a4, /// 0xb78
			 0xdb7f1578, /// 0xb7c
			 0x2df3eadb, /// 0xb80
			 0xf20823e4, /// 0xb84
			 0xf4d34840, /// 0xb88
			 0x12c29bed, /// 0xb8c
			 0x757a0db0, /// 0xb90
			 0x57f93fcd, /// 0xb94
			 0x3405d8f8, /// 0xb98
			 0x82713efc, /// 0xb9c
			 0xf8c595ba, /// 0xba0
			 0xff9b59b1, /// 0xba4
			 0x0291281b, /// 0xba8
			 0xbf3657aa, /// 0xbac
			 0x7bd6968e, /// 0xbb0
			 0x5a587eae, /// 0xbb4
			 0xd7a6ff6e, /// 0xbb8
			 0x7f65a83a, /// 0xbbc
			 0x3ac01e19, /// 0xbc0
			 0xdc65f65b, /// 0xbc4
			 0x1584726a, /// 0xbc8
			 0x41dcc02d, /// 0xbcc
			 0x085c98ae, /// 0xbd0
			 0x4de0eea1, /// 0xbd4
			 0x0cadae8c, /// 0xbd8
			 0x46f2ebc4, /// 0xbdc
			 0xe535c384, /// 0xbe0
			 0x817e27af, /// 0xbe4
			 0x59374683, /// 0xbe8
			 0x1abbd87a, /// 0xbec
			 0x41a35e56, /// 0xbf0
			 0x46eec531, /// 0xbf4
			 0x55c8f495, /// 0xbf8
			 0xc8ea5b54, /// 0xbfc
			 0x77fafa0c, /// 0xc00
			 0x32a90683, /// 0xc04
			 0x5c3dc685, /// 0xc08
			 0x3cc83d01, /// 0xc0c
			 0x98c979fe, /// 0xc10
			 0x2f225bae, /// 0xc14
			 0xefe9492a, /// 0xc18
			 0xc3eb3093, /// 0xc1c
			 0xe03e3fcc, /// 0xc20
			 0x2c34d926, /// 0xc24
			 0xb233ba49, /// 0xc28
			 0x16983989, /// 0xc2c
			 0xe18b8e89, /// 0xc30
			 0x5849375c, /// 0xc34
			 0x5b5a4f00, /// 0xc38
			 0xf3e2e1ba, /// 0xc3c
			 0xcc6e8232, /// 0xc40
			 0xe9ba6437, /// 0xc44
			 0x1b4ba6cd, /// 0xc48
			 0x3d388826, /// 0xc4c
			 0xaf35c376, /// 0xc50
			 0x4a632c78, /// 0xc54
			 0x97d9d756, /// 0xc58
			 0x3c99f05d, /// 0xc5c
			 0xb345ea00, /// 0xc60
			 0xa85eb2ee, /// 0xc64
			 0x951dbc26, /// 0xc68
			 0xf8c7d90f, /// 0xc6c
			 0x9556cec2, /// 0xc70
			 0xf563affd, /// 0xc74
			 0xf8e63558, /// 0xc78
			 0x2d832046, /// 0xc7c
			 0x44c7e5d7, /// 0xc80
			 0x6ffdccc2, /// 0xc84
			 0xf6d86e69, /// 0xc88
			 0x9eaeea81, /// 0xc8c
			 0x476a716f, /// 0xc90
			 0xe83e9c7d, /// 0xc94
			 0x92bd296c, /// 0xc98
			 0x07895b2f, /// 0xc9c
			 0x8790ec8f, /// 0xca0
			 0x8a28aaa1, /// 0xca4
			 0xcd07229f, /// 0xca8
			 0x6df8bc82, /// 0xcac
			 0xb2f32c77, /// 0xcb0
			 0x20b198a3, /// 0xcb4
			 0x27bd303a, /// 0xcb8
			 0x7b0dc7f4, /// 0xcbc
			 0xf52cf347, /// 0xcc0
			 0x13984474, /// 0xcc4
			 0x6fe82b33, /// 0xcc8
			 0xd079fc1e, /// 0xccc
			 0xe4b1c516, /// 0xcd0
			 0x144e881d, /// 0xcd4
			 0xa796cfc5, /// 0xcd8
			 0x6d462288, /// 0xcdc
			 0x42921c87, /// 0xce0
			 0x6dda249b, /// 0xce4
			 0xc53e7b20, /// 0xce8
			 0x8635df01, /// 0xcec
			 0xbc41b09a, /// 0xcf0
			 0x284f0b5a, /// 0xcf4
			 0x389644f0, /// 0xcf8
			 0x609cbea3, /// 0xcfc
			 0xf37c7fce, /// 0xd00
			 0xbe12080e, /// 0xd04
			 0x14b9a7d0, /// 0xd08
			 0xc718f210, /// 0xd0c
			 0x91730e23, /// 0xd10
			 0xc1db9996, /// 0xd14
			 0xccd7b312, /// 0xd18
			 0xf3df3ebf, /// 0xd1c
			 0x1ee8edcf, /// 0xd20
			 0xb6f341c6, /// 0xd24
			 0xdc16a400, /// 0xd28
			 0x2c0c0074, /// 0xd2c
			 0xd6e3334e, /// 0xd30
			 0xc7c58a79, /// 0xd34
			 0x968c2d12, /// 0xd38
			 0x8cdb6e13, /// 0xd3c
			 0xf285e775, /// 0xd40
			 0x8e808121, /// 0xd44
			 0x40f3a4a7, /// 0xd48
			 0x1419f972, /// 0xd4c
			 0xa5e01b31, /// 0xd50
			 0x0c7faf9c, /// 0xd54
			 0x7343de48, /// 0xd58
			 0x20693b2b, /// 0xd5c
			 0x74c73e92, /// 0xd60
			 0xca6e801e, /// 0xd64
			 0x114f5985, /// 0xd68
			 0xbdd30f32, /// 0xd6c
			 0x79151cd4, /// 0xd70
			 0x78a693e9, /// 0xd74
			 0x8c81d7c1, /// 0xd78
			 0xcef1e472, /// 0xd7c
			 0xb9ebdf7a, /// 0xd80
			 0x44104c4c, /// 0xd84
			 0x593087d7, /// 0xd88
			 0x25f162e4, /// 0xd8c
			 0x6855082f, /// 0xd90
			 0xde83c8c3, /// 0xd94
			 0x88d331a4, /// 0xd98
			 0x5eacdc1c, /// 0xd9c
			 0x4b75dd59, /// 0xda0
			 0x62fbde25, /// 0xda4
			 0xe825c67b, /// 0xda8
			 0xa1d2d88a, /// 0xdac
			 0x80165c4d, /// 0xdb0
			 0x56ce9fdd, /// 0xdb4
			 0x9599522e, /// 0xdb8
			 0x7b2321c3, /// 0xdbc
			 0x2a1efbbb, /// 0xdc0
			 0x24c80479, /// 0xdc4
			 0xbd8a7984, /// 0xdc8
			 0x162dc20a, /// 0xdcc
			 0x00c4432b, /// 0xdd0
			 0x0ca492f3, /// 0xdd4
			 0xcca1d00b, /// 0xdd8
			 0x81ab8852, /// 0xddc
			 0xc5ec2056, /// 0xde0
			 0x3ee2d4f8, /// 0xde4
			 0x8f1b6c1b, /// 0xde8
			 0x21d40657, /// 0xdec
			 0x72970750, /// 0xdf0
			 0x672134ab, /// 0xdf4
			 0xb42b97ad, /// 0xdf8
			 0xd20aed85, /// 0xdfc
			 0x3122f47a, /// 0xe00
			 0x88c40bea, /// 0xe04
			 0xf9f75abd, /// 0xe08
			 0xa0eedbb5, /// 0xe0c
			 0xf3684580, /// 0xe10
			 0x934d4a3e, /// 0xe14
			 0x3ce8d4f2, /// 0xe18
			 0xe99d1380, /// 0xe1c
			 0xcf400817, /// 0xe20
			 0x311a2fa3, /// 0xe24
			 0xdba7b28f, /// 0xe28
			 0x7d561628, /// 0xe2c
			 0x0f1d2616, /// 0xe30
			 0xabd626ab, /// 0xe34
			 0x822e71aa, /// 0xe38
			 0xd7d0ea33, /// 0xe3c
			 0x7773aa6c, /// 0xe40
			 0x04dae64c, /// 0xe44
			 0xd03673c9, /// 0xe48
			 0x258ca4ae, /// 0xe4c
			 0x94f1fc54, /// 0xe50
			 0x3a94acac, /// 0xe54
			 0x3aad10bd, /// 0xe58
			 0x086c1268, /// 0xe5c
			 0xd0e62950, /// 0xe60
			 0x0efddbad, /// 0xe64
			 0x05d1d6c1, /// 0xe68
			 0xa2dd5ac1, /// 0xe6c
			 0x7bcc6c23, /// 0xe70
			 0x0dd21894, /// 0xe74
			 0x5c34271a, /// 0xe78
			 0xa1480c85, /// 0xe7c
			 0xded4d405, /// 0xe80
			 0xc1d6cb72, /// 0xe84
			 0xeb6d8772, /// 0xe88
			 0xb9cec847, /// 0xe8c
			 0x375b1d12, /// 0xe90
			 0x8fa40fe6, /// 0xe94
			 0x506f8fef, /// 0xe98
			 0x3109f49e, /// 0xe9c
			 0x1c302562, /// 0xea0
			 0x6a75f9be, /// 0xea4
			 0xe32bc494, /// 0xea8
			 0xce487fa0, /// 0xeac
			 0x0d271d44, /// 0xeb0
			 0x75106ea3, /// 0xeb4
			 0xfc290e0e, /// 0xeb8
			 0xa871e9d8, /// 0xebc
			 0xf5b3eab4, /// 0xec0
			 0x8061a5e3, /// 0xec4
			 0xfb87405c, /// 0xec8
			 0x68cc1d9d, /// 0xecc
			 0x1b1ccf99, /// 0xed0
			 0x29e7b086, /// 0xed4
			 0xc75036d2, /// 0xed8
			 0xa8381cdd, /// 0xedc
			 0xd0b52d9d, /// 0xee0
			 0x6997cb35, /// 0xee4
			 0x17b63d01, /// 0xee8
			 0xeb18ac69, /// 0xeec
			 0x8346b284, /// 0xef0
			 0x890cd605, /// 0xef4
			 0x8c464f91, /// 0xef8
			 0x40f21ac7, /// 0xefc
			 0x18b842f5, /// 0xf00
			 0xf6154f6d, /// 0xf04
			 0x30b900c2, /// 0xf08
			 0xa75b956d, /// 0xf0c
			 0x868c5b29, /// 0xf10
			 0x42d54a0f, /// 0xf14
			 0x4a8ee6fb, /// 0xf18
			 0x7e615c98, /// 0xf1c
			 0x03856b2d, /// 0xf20
			 0xa1254c17, /// 0xf24
			 0x76ab9e0b, /// 0xf28
			 0xee716b67, /// 0xf2c
			 0x05ed1108, /// 0xf30
			 0xeea04720, /// 0xf34
			 0x1a6b9c48, /// 0xf38
			 0xc0d48d15, /// 0xf3c
			 0xfb5f03d8, /// 0xf40
			 0xd46ee6c9, /// 0xf44
			 0x16a54fe8, /// 0xf48
			 0xfe535a43, /// 0xf4c
			 0x8b039240, /// 0xf50
			 0x8a75254b, /// 0xf54
			 0xac381c0b, /// 0xf58
			 0xfe96d4d4, /// 0xf5c
			 0xd1d5191e, /// 0xf60
			 0x5544a5ab, /// 0xf64
			 0x479858b7, /// 0xf68
			 0xcaad43e0, /// 0xf6c
			 0xaf13d31e, /// 0xf70
			 0xc0de5f76, /// 0xf74
			 0x14c7f9b5, /// 0xf78
			 0x3d250840, /// 0xf7c
			 0x4dfb2ae2, /// 0xf80
			 0xe1c01501, /// 0xf84
			 0x76558dcc, /// 0xf88
			 0x6e960a2d, /// 0xf8c
			 0x3737aae1, /// 0xf90
			 0x07e94388, /// 0xf94
			 0x1534f902, /// 0xf98
			 0x8055eabe, /// 0xf9c
			 0x724bb12f, /// 0xfa0
			 0xe55b6b89, /// 0xfa4
			 0xa8a15832, /// 0xfa8
			 0x4acb124d, /// 0xfac
			 0x98e37a1c, /// 0xfb0
			 0x1981d383, /// 0xfb4
			 0xfe92b027, /// 0xfb8
			 0x04c6e5e0, /// 0xfbc
			 0x7445cdaf, /// 0xfc0
			 0x71e591fc, /// 0xfc4
			 0xcbc7049a, /// 0xfc8
			 0xf170f423, /// 0xfcc
			 0xfac6006a, /// 0xfd0
			 0x21895262, /// 0xfd4
			 0xa74612e7, /// 0xfd8
			 0xd2ee01bb, /// 0xfdc
			 0x935c4878, /// 0xfe0
			 0x79dc5e60, /// 0xfe4
			 0x06538c08, /// 0xfe8
			 0x82633287, /// 0xfec
			 0x87ac22e4, /// 0xff0
			 0xbd6e8ad0, /// 0xff4
			 0x96e38273, /// 0xff8
			 0x09585c64 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x443645a6, /// 0x0
			 0x8fdb53db, /// 0x4
			 0xbe8eb8a3, /// 0x8
			 0xc28d2ecb, /// 0xc
			 0x1aff982d, /// 0x10
			 0x9a286e27, /// 0x14
			 0x4b6b6255, /// 0x18
			 0xa726cee4, /// 0x1c
			 0x3b65e89b, /// 0x20
			 0xd56fa1ec, /// 0x24
			 0xf02bb4b9, /// 0x28
			 0x10e87ce7, /// 0x2c
			 0xd2fafa2b, /// 0x30
			 0xb88555b4, /// 0x34
			 0x285ca9e8, /// 0x38
			 0x82f34526, /// 0x3c
			 0x4731576c, /// 0x40
			 0x875c3762, /// 0x44
			 0xd6b725f3, /// 0x48
			 0x4f7fde66, /// 0x4c
			 0x8b2b1d92, /// 0x50
			 0x2fc23399, /// 0x54
			 0x5e945375, /// 0x58
			 0xf48a693d, /// 0x5c
			 0x0fb808b6, /// 0x60
			 0x09e197be, /// 0x64
			 0xf6eac360, /// 0x68
			 0x494045cb, /// 0x6c
			 0x60f7f736, /// 0x70
			 0x736bd062, /// 0x74
			 0x5bf9b425, /// 0x78
			 0x79319a7a, /// 0x7c
			 0x7b3d3cdc, /// 0x80
			 0x20abc05a, /// 0x84
			 0xc1595ff9, /// 0x88
			 0xfca2b2f7, /// 0x8c
			 0xbb3ece4f, /// 0x90
			 0x36819b76, /// 0x94
			 0x45246366, /// 0x98
			 0x6b8648a6, /// 0x9c
			 0x4e7cead5, /// 0xa0
			 0x730cb9ca, /// 0xa4
			 0x5e0195ab, /// 0xa8
			 0xd192ee12, /// 0xac
			 0x937c0c0f, /// 0xb0
			 0x4eb80422, /// 0xb4
			 0x56f6b429, /// 0xb8
			 0x3b06f717, /// 0xbc
			 0x7429328c, /// 0xc0
			 0x31f8bf53, /// 0xc4
			 0x8c377e26, /// 0xc8
			 0x9484d26a, /// 0xcc
			 0x90f77ee8, /// 0xd0
			 0x19f196d9, /// 0xd4
			 0xf5e2e369, /// 0xd8
			 0xb9be3f7b, /// 0xdc
			 0x68548fa3, /// 0xe0
			 0xb8f0171d, /// 0xe4
			 0x4250f5c1, /// 0xe8
			 0x40a0abfb, /// 0xec
			 0xdb7a566c, /// 0xf0
			 0x8ca08d7f, /// 0xf4
			 0x1000f649, /// 0xf8
			 0x439559a6, /// 0xfc
			 0x4eec3ae7, /// 0x100
			 0xd75d9b20, /// 0x104
			 0xb5722ccd, /// 0x108
			 0xf1ba128c, /// 0x10c
			 0x384e7c50, /// 0x110
			 0xa2ce82e8, /// 0x114
			 0xbb6579fe, /// 0x118
			 0x12e36041, /// 0x11c
			 0x0b49587c, /// 0x120
			 0x09a721f2, /// 0x124
			 0x881a77a7, /// 0x128
			 0x9f35fd76, /// 0x12c
			 0x352683a7, /// 0x130
			 0x0df9c080, /// 0x134
			 0x7439013c, /// 0x138
			 0x7ca59047, /// 0x13c
			 0x60ab6864, /// 0x140
			 0x07d68905, /// 0x144
			 0x47aceab8, /// 0x148
			 0x3903b9be, /// 0x14c
			 0x550b1d87, /// 0x150
			 0x603f5180, /// 0x154
			 0x8bbd0cff, /// 0x158
			 0xce55a425, /// 0x15c
			 0xedbf53d0, /// 0x160
			 0xdb615427, /// 0x164
			 0x705e8f8b, /// 0x168
			 0xf5fc770d, /// 0x16c
			 0x697c6f0f, /// 0x170
			 0xe8a8ee38, /// 0x174
			 0xfeb0e3c1, /// 0x178
			 0xa16df59d, /// 0x17c
			 0x1fdbc269, /// 0x180
			 0x8ac61b49, /// 0x184
			 0x9b2543b2, /// 0x188
			 0x8c879e0b, /// 0x18c
			 0x053febe6, /// 0x190
			 0xf3fd1343, /// 0x194
			 0x40da731c, /// 0x198
			 0x35c58bfb, /// 0x19c
			 0xb9892114, /// 0x1a0
			 0xf9ade21e, /// 0x1a4
			 0x2c0a2834, /// 0x1a8
			 0x62e30bbb, /// 0x1ac
			 0x9349889e, /// 0x1b0
			 0x657430bd, /// 0x1b4
			 0x6eca7cca, /// 0x1b8
			 0x26ec6a8d, /// 0x1bc
			 0xd9a4dc5c, /// 0x1c0
			 0x626caa0f, /// 0x1c4
			 0x78c4ac83, /// 0x1c8
			 0x32e1036f, /// 0x1cc
			 0xc9062e5c, /// 0x1d0
			 0xcb7f60f1, /// 0x1d4
			 0x95e97af7, /// 0x1d8
			 0xc81074fe, /// 0x1dc
			 0x4efb5f74, /// 0x1e0
			 0x31286581, /// 0x1e4
			 0xda099f5a, /// 0x1e8
			 0xc6e079d2, /// 0x1ec
			 0xff59d734, /// 0x1f0
			 0xf453896b, /// 0x1f4
			 0x69479f35, /// 0x1f8
			 0x883d2053, /// 0x1fc
			 0x24d23542, /// 0x200
			 0x11929ce5, /// 0x204
			 0x03e5f54d, /// 0x208
			 0xed26fad5, /// 0x20c
			 0xde9c1aa8, /// 0x210
			 0x6077ff20, /// 0x214
			 0x1827fa4d, /// 0x218
			 0x5787f1cb, /// 0x21c
			 0xcf7c8d72, /// 0x220
			 0x8d60ebf4, /// 0x224
			 0xd32a2375, /// 0x228
			 0x8205cbab, /// 0x22c
			 0x7ab6e29b, /// 0x230
			 0x18b4e115, /// 0x234
			 0xc745d521, /// 0x238
			 0xde7a0029, /// 0x23c
			 0x63630f82, /// 0x240
			 0x0450b859, /// 0x244
			 0xd575f21b, /// 0x248
			 0x335b9e26, /// 0x24c
			 0x10d5823a, /// 0x250
			 0xe59fce49, /// 0x254
			 0xd2464cda, /// 0x258
			 0x42372815, /// 0x25c
			 0xa2c11166, /// 0x260
			 0xbc2aeef7, /// 0x264
			 0x71340c66, /// 0x268
			 0x0898dcb9, /// 0x26c
			 0x94c58326, /// 0x270
			 0xaaebfc3e, /// 0x274
			 0xee0a56a7, /// 0x278
			 0x85f2aef4, /// 0x27c
			 0xc92a9dd0, /// 0x280
			 0xc5bf88f7, /// 0x284
			 0xf3e63a3c, /// 0x288
			 0xd09b7116, /// 0x28c
			 0x9d42be12, /// 0x290
			 0x07cc8736, /// 0x294
			 0xb7dd6fc0, /// 0x298
			 0xf339200f, /// 0x29c
			 0x48984682, /// 0x2a0
			 0x43ec0ec6, /// 0x2a4
			 0x5b0ce9a6, /// 0x2a8
			 0x1133532b, /// 0x2ac
			 0x09adbc6e, /// 0x2b0
			 0x8f190451, /// 0x2b4
			 0xe8a94478, /// 0x2b8
			 0x1f8d6be4, /// 0x2bc
			 0x17ff7ee6, /// 0x2c0
			 0x4459952b, /// 0x2c4
			 0xa29dede3, /// 0x2c8
			 0x852f0501, /// 0x2cc
			 0xe746d407, /// 0x2d0
			 0xc7a93b1c, /// 0x2d4
			 0x3b449193, /// 0x2d8
			 0x03cf7a24, /// 0x2dc
			 0xf62f9690, /// 0x2e0
			 0x0c9f1659, /// 0x2e4
			 0xa4807683, /// 0x2e8
			 0x5e5ca858, /// 0x2ec
			 0x66304564, /// 0x2f0
			 0x725cb57a, /// 0x2f4
			 0xf808f9b6, /// 0x2f8
			 0x9c4509fc, /// 0x2fc
			 0xdc0316b3, /// 0x300
			 0xe7a4078a, /// 0x304
			 0xa2fec3b6, /// 0x308
			 0x6c45ab99, /// 0x30c
			 0xd2b10b6f, /// 0x310
			 0x8b57c12b, /// 0x314
			 0x37123c47, /// 0x318
			 0xde0b9851, /// 0x31c
			 0xd66459a5, /// 0x320
			 0x3722f3b7, /// 0x324
			 0x8ec788ca, /// 0x328
			 0xc8898502, /// 0x32c
			 0x1e449b11, /// 0x330
			 0x743caf3e, /// 0x334
			 0x349a45ff, /// 0x338
			 0x40771e64, /// 0x33c
			 0x6f26c268, /// 0x340
			 0x86710db0, /// 0x344
			 0x2b753feb, /// 0x348
			 0x190f8ed8, /// 0x34c
			 0xc1a5d0c0, /// 0x350
			 0x81a3b1de, /// 0x354
			 0x4a11acfa, /// 0x358
			 0xb11143b6, /// 0x35c
			 0x7483ea31, /// 0x360
			 0xb50f2072, /// 0x364
			 0x4b145429, /// 0x368
			 0xe58fc0fb, /// 0x36c
			 0x574ba2f8, /// 0x370
			 0x9b4c67d7, /// 0x374
			 0xef1e23ee, /// 0x378
			 0x0d4adcc6, /// 0x37c
			 0x5dc3a6dc, /// 0x380
			 0xa583cdb4, /// 0x384
			 0x996a6ea2, /// 0x388
			 0x7a734021, /// 0x38c
			 0x11e96b7d, /// 0x390
			 0xffad8071, /// 0x394
			 0xf2e6fc40, /// 0x398
			 0xd0ae51f4, /// 0x39c
			 0xdaffa875, /// 0x3a0
			 0xe9f0c3b8, /// 0x3a4
			 0xc3fa6303, /// 0x3a8
			 0x8a4620ce, /// 0x3ac
			 0xcca58d48, /// 0x3b0
			 0x9ea26e11, /// 0x3b4
			 0xa3586ff8, /// 0x3b8
			 0xa6b3458c, /// 0x3bc
			 0x1687a80c, /// 0x3c0
			 0xf1b3e310, /// 0x3c4
			 0xd983a6e2, /// 0x3c8
			 0xe782c0fb, /// 0x3cc
			 0x2bd65887, /// 0x3d0
			 0x09d21a3f, /// 0x3d4
			 0x18dce066, /// 0x3d8
			 0xfa38f3f3, /// 0x3dc
			 0x23b0ebb7, /// 0x3e0
			 0xbf6c56ae, /// 0x3e4
			 0xef1527ed, /// 0x3e8
			 0x86265554, /// 0x3ec
			 0xbed9bf2a, /// 0x3f0
			 0xcc57a9cf, /// 0x3f4
			 0x5f762a72, /// 0x3f8
			 0xb3d7aa6d, /// 0x3fc
			 0x93a26281, /// 0x400
			 0x71d8efc2, /// 0x404
			 0x86eb235b, /// 0x408
			 0x6966c182, /// 0x40c
			 0x38d90acd, /// 0x410
			 0xd3897d85, /// 0x414
			 0x64704261, /// 0x418
			 0x20bc7a2d, /// 0x41c
			 0xb4ac1b76, /// 0x420
			 0x577458c6, /// 0x424
			 0xd8bbc7c2, /// 0x428
			 0x03c4eec3, /// 0x42c
			 0xde7a2ffd, /// 0x430
			 0x4eaf98f3, /// 0x434
			 0x546ce5f5, /// 0x438
			 0xe84432f6, /// 0x43c
			 0x8f1a2cae, /// 0x440
			 0x607c867f, /// 0x444
			 0xedd5a4e9, /// 0x448
			 0x46a70f2d, /// 0x44c
			 0x251a890e, /// 0x450
			 0xb31f3ca6, /// 0x454
			 0xa9e858d4, /// 0x458
			 0xd6587ca4, /// 0x45c
			 0x8b89852c, /// 0x460
			 0x8d30e9a0, /// 0x464
			 0xc0164688, /// 0x468
			 0xcb635351, /// 0x46c
			 0xd14d9807, /// 0x470
			 0x10c96812, /// 0x474
			 0xd4ab59a8, /// 0x478
			 0x3a4eaf8f, /// 0x47c
			 0x2ae0828e, /// 0x480
			 0x9d83ccce, /// 0x484
			 0xcea32a36, /// 0x488
			 0x001040a5, /// 0x48c
			 0xd73ad0de, /// 0x490
			 0x76aa47bf, /// 0x494
			 0x471cfa9e, /// 0x498
			 0x079447fc, /// 0x49c
			 0x55ff1ffc, /// 0x4a0
			 0xf3977e00, /// 0x4a4
			 0x70cf9065, /// 0x4a8
			 0x6ab5b51a, /// 0x4ac
			 0xc0badb89, /// 0x4b0
			 0xf217a52b, /// 0x4b4
			 0xd594a9e7, /// 0x4b8
			 0xc8e88d74, /// 0x4bc
			 0x7aeb4fe9, /// 0x4c0
			 0x39654ab7, /// 0x4c4
			 0x7463fd79, /// 0x4c8
			 0x8b4448bb, /// 0x4cc
			 0xdfc53c57, /// 0x4d0
			 0x5daaea93, /// 0x4d4
			 0x8c99d1e5, /// 0x4d8
			 0xb9bc90cf, /// 0x4dc
			 0x50a2e541, /// 0x4e0
			 0x756f6e5a, /// 0x4e4
			 0x4e91bb6b, /// 0x4e8
			 0xfc7012a1, /// 0x4ec
			 0xdcecc508, /// 0x4f0
			 0x5bc7a97b, /// 0x4f4
			 0x3cd734db, /// 0x4f8
			 0x6560b73f, /// 0x4fc
			 0xddfdc0d8, /// 0x500
			 0x6e56277c, /// 0x504
			 0x7811fa95, /// 0x508
			 0x43082c21, /// 0x50c
			 0x47073824, /// 0x510
			 0xb78dceb3, /// 0x514
			 0x41f89d14, /// 0x518
			 0xf3c2c017, /// 0x51c
			 0x490cefcd, /// 0x520
			 0xd1b9251b, /// 0x524
			 0x4b93477e, /// 0x528
			 0xa16e31c3, /// 0x52c
			 0x56aad6e8, /// 0x530
			 0x534536a7, /// 0x534
			 0x401f922d, /// 0x538
			 0x13db2fc4, /// 0x53c
			 0xc0c48c01, /// 0x540
			 0x2a222272, /// 0x544
			 0xfa5639e0, /// 0x548
			 0x074eae26, /// 0x54c
			 0xd93d5096, /// 0x550
			 0x1e683189, /// 0x554
			 0xb2cd30cc, /// 0x558
			 0x0623e92c, /// 0x55c
			 0x99667528, /// 0x560
			 0x97d689a9, /// 0x564
			 0xb0de9122, /// 0x568
			 0x8807eda7, /// 0x56c
			 0x6f36391b, /// 0x570
			 0xd9009ab5, /// 0x574
			 0x319e779a, /// 0x578
			 0xe15162b5, /// 0x57c
			 0x6125905e, /// 0x580
			 0x9a2b88be, /// 0x584
			 0x2a13e630, /// 0x588
			 0x88ccab7d, /// 0x58c
			 0x55d1d364, /// 0x590
			 0x0d127ccc, /// 0x594
			 0xea873e8e, /// 0x598
			 0xaaa35b65, /// 0x59c
			 0xb8e8d3ab, /// 0x5a0
			 0x6bc187c7, /// 0x5a4
			 0x3d614a48, /// 0x5a8
			 0x4bf60cfc, /// 0x5ac
			 0x9826fae2, /// 0x5b0
			 0xb51ffd40, /// 0x5b4
			 0xe3b08910, /// 0x5b8
			 0xd255708f, /// 0x5bc
			 0x45caddaa, /// 0x5c0
			 0x798524db, /// 0x5c4
			 0x8a5e576d, /// 0x5c8
			 0xa12e6655, /// 0x5cc
			 0x901b55e7, /// 0x5d0
			 0x8b9fd69e, /// 0x5d4
			 0x77ae14f5, /// 0x5d8
			 0x50b10725, /// 0x5dc
			 0x0de442ce, /// 0x5e0
			 0x78961cc0, /// 0x5e4
			 0xff4e98dd, /// 0x5e8
			 0xbec9f0ca, /// 0x5ec
			 0xebbf1668, /// 0x5f0
			 0x9592bad8, /// 0x5f4
			 0xdf586d8a, /// 0x5f8
			 0x6c934330, /// 0x5fc
			 0xf6e9349f, /// 0x600
			 0x2505ee51, /// 0x604
			 0xa6b29500, /// 0x608
			 0x3057a3b6, /// 0x60c
			 0x70b9abe2, /// 0x610
			 0x8cd336f4, /// 0x614
			 0x3ca7dfad, /// 0x618
			 0x995d1a9f, /// 0x61c
			 0x23be6a95, /// 0x620
			 0xb712b9d0, /// 0x624
			 0xae8c0577, /// 0x628
			 0x5e7728c5, /// 0x62c
			 0x850d04e8, /// 0x630
			 0xea2aee8c, /// 0x634
			 0x3526b21c, /// 0x638
			 0x3c3a18bf, /// 0x63c
			 0x2d119169, /// 0x640
			 0xe4c27eb4, /// 0x644
			 0xeb5099b8, /// 0x648
			 0x9add0811, /// 0x64c
			 0xb3d954ff, /// 0x650
			 0x7c845c6a, /// 0x654
			 0x09f2c2f5, /// 0x658
			 0x9857a6a9, /// 0x65c
			 0x9b38c93e, /// 0x660
			 0x038c9787, /// 0x664
			 0xaba9d781, /// 0x668
			 0x3bc78f4a, /// 0x66c
			 0x9165fd7c, /// 0x670
			 0xc4feb1bf, /// 0x674
			 0xb8b32547, /// 0x678
			 0x3e8e86e1, /// 0x67c
			 0x46b5033b, /// 0x680
			 0xa2486c6c, /// 0x684
			 0x552dffbe, /// 0x688
			 0x460a2c22, /// 0x68c
			 0x8aa044db, /// 0x690
			 0xb4e6658c, /// 0x694
			 0x3c8bbb64, /// 0x698
			 0x5696e9b0, /// 0x69c
			 0x967bc541, /// 0x6a0
			 0x371b3b12, /// 0x6a4
			 0x4f27c043, /// 0x6a8
			 0x1abd4dc4, /// 0x6ac
			 0xf59fc184, /// 0x6b0
			 0xf50e7481, /// 0x6b4
			 0xb767d76c, /// 0x6b8
			 0x7ec50722, /// 0x6bc
			 0x16782436, /// 0x6c0
			 0x972bbfec, /// 0x6c4
			 0xe239396e, /// 0x6c8
			 0x0ee98bc7, /// 0x6cc
			 0x6e79c1d4, /// 0x6d0
			 0x75a63e54, /// 0x6d4
			 0x9ef56404, /// 0x6d8
			 0x211ff52d, /// 0x6dc
			 0xc75b669e, /// 0x6e0
			 0x8728161a, /// 0x6e4
			 0x8b95acfd, /// 0x6e8
			 0x9b88b316, /// 0x6ec
			 0x50c4dc36, /// 0x6f0
			 0xaedcc529, /// 0x6f4
			 0xc011bf45, /// 0x6f8
			 0x36586a36, /// 0x6fc
			 0x0173a846, /// 0x700
			 0xe91becf3, /// 0x704
			 0x247f216a, /// 0x708
			 0x25093e61, /// 0x70c
			 0xfe4d3dcd, /// 0x710
			 0xc66858c7, /// 0x714
			 0xb08342bf, /// 0x718
			 0x28bc8b38, /// 0x71c
			 0xd549d4de, /// 0x720
			 0xeaf4ae9c, /// 0x724
			 0xbfafcca1, /// 0x728
			 0x789a0e82, /// 0x72c
			 0xae420c5d, /// 0x730
			 0xa8e79a78, /// 0x734
			 0xf0129ebf, /// 0x738
			 0x9e12b213, /// 0x73c
			 0x3de0d01f, /// 0x740
			 0x836da63b, /// 0x744
			 0x8a49e7c9, /// 0x748
			 0xc004af7e, /// 0x74c
			 0x538da8c6, /// 0x750
			 0x3c040066, /// 0x754
			 0x5b9f70bb, /// 0x758
			 0x09c1d8dc, /// 0x75c
			 0x8386eb77, /// 0x760
			 0x5678a5f5, /// 0x764
			 0xdba22572, /// 0x768
			 0x6d953e15, /// 0x76c
			 0x826f3d7d, /// 0x770
			 0x4efe2ab5, /// 0x774
			 0xd916bd2b, /// 0x778
			 0xdbea5116, /// 0x77c
			 0x209163dd, /// 0x780
			 0xf1f0cdcf, /// 0x784
			 0xa7f89188, /// 0x788
			 0xe099b216, /// 0x78c
			 0x1393106d, /// 0x790
			 0x32e9265e, /// 0x794
			 0xbc6970d3, /// 0x798
			 0x686be09e, /// 0x79c
			 0x1c98b4ba, /// 0x7a0
			 0x239fc4b5, /// 0x7a4
			 0x48261bd3, /// 0x7a8
			 0x20b58ddb, /// 0x7ac
			 0xa0eb423d, /// 0x7b0
			 0x524c14ff, /// 0x7b4
			 0x353f7725, /// 0x7b8
			 0x1851d282, /// 0x7bc
			 0xf08f62af, /// 0x7c0
			 0xbe988cb1, /// 0x7c4
			 0xf4d6edea, /// 0x7c8
			 0x7cdb9449, /// 0x7cc
			 0x6203a732, /// 0x7d0
			 0x9867beac, /// 0x7d4
			 0x1cb6abe4, /// 0x7d8
			 0x0f963b59, /// 0x7dc
			 0x5762d885, /// 0x7e0
			 0xd5c9fa81, /// 0x7e4
			 0xb79c7996, /// 0x7e8
			 0xd053cc8c, /// 0x7ec
			 0x0e5e566c, /// 0x7f0
			 0xeb07f35e, /// 0x7f4
			 0xde491332, /// 0x7f8
			 0xf83b19d0, /// 0x7fc
			 0x471731e6, /// 0x800
			 0x56787b55, /// 0x804
			 0xdf0b60bf, /// 0x808
			 0xba15fe1c, /// 0x80c
			 0x46ddf63e, /// 0x810
			 0x3965932c, /// 0x814
			 0x3b8df128, /// 0x818
			 0x5c171895, /// 0x81c
			 0xe64c2430, /// 0x820
			 0x4de18ef9, /// 0x824
			 0xc029fb8b, /// 0x828
			 0x093e0918, /// 0x82c
			 0x8a2ef65c, /// 0x830
			 0xe052ea00, /// 0x834
			 0x31ddc3fc, /// 0x838
			 0x022a14ea, /// 0x83c
			 0x57b207ae, /// 0x840
			 0x5f8ce0e9, /// 0x844
			 0x285c37a0, /// 0x848
			 0xdabeeff2, /// 0x84c
			 0x1230de7e, /// 0x850
			 0x6ea3b789, /// 0x854
			 0xe2aefd0e, /// 0x858
			 0xbcdda020, /// 0x85c
			 0x7eb3cbcc, /// 0x860
			 0x402ec2da, /// 0x864
			 0x3f34ace5, /// 0x868
			 0x448b407f, /// 0x86c
			 0xe8b2d0fb, /// 0x870
			 0x2c9a6720, /// 0x874
			 0x65585527, /// 0x878
			 0xe0ecaa78, /// 0x87c
			 0x28b5c58a, /// 0x880
			 0x253b50b3, /// 0x884
			 0x2474fd7b, /// 0x888
			 0x1ea647bb, /// 0x88c
			 0x2c4570c1, /// 0x890
			 0x2cbcb7e1, /// 0x894
			 0xbcb14356, /// 0x898
			 0x06a9ffe7, /// 0x89c
			 0x95140c5a, /// 0x8a0
			 0x003187d2, /// 0x8a4
			 0x7a9f3a8b, /// 0x8a8
			 0xc4573c96, /// 0x8ac
			 0x748e549f, /// 0x8b0
			 0x49ca7dde, /// 0x8b4
			 0x864bd64e, /// 0x8b8
			 0xb6e396b0, /// 0x8bc
			 0xe4c26e56, /// 0x8c0
			 0x587cb397, /// 0x8c4
			 0x21a122d0, /// 0x8c8
			 0x92fedcdd, /// 0x8cc
			 0x32bae8b3, /// 0x8d0
			 0xf9024cb8, /// 0x8d4
			 0x5efd6012, /// 0x8d8
			 0xcd653ff2, /// 0x8dc
			 0x47ef8404, /// 0x8e0
			 0xfe3429e5, /// 0x8e4
			 0x4fe0fa1c, /// 0x8e8
			 0x9e4beacf, /// 0x8ec
			 0x307686bc, /// 0x8f0
			 0xf35f6c60, /// 0x8f4
			 0x9c1dd997, /// 0x8f8
			 0xfd3ccad4, /// 0x8fc
			 0xb4e69cc9, /// 0x900
			 0x37001610, /// 0x904
			 0x2aca3612, /// 0x908
			 0x943b6c97, /// 0x90c
			 0xd05cd06b, /// 0x910
			 0xdc4eb1be, /// 0x914
			 0xe9fb9f46, /// 0x918
			 0x3ed552b3, /// 0x91c
			 0xe63915ea, /// 0x920
			 0x8f8f7750, /// 0x924
			 0x4f54497c, /// 0x928
			 0xc9d45b24, /// 0x92c
			 0x6e33c637, /// 0x930
			 0x54122747, /// 0x934
			 0x5121e94b, /// 0x938
			 0x6f06a43b, /// 0x93c
			 0x7829c07a, /// 0x940
			 0xff60e5ac, /// 0x944
			 0x9c974e9b, /// 0x948
			 0x83cc2038, /// 0x94c
			 0x62f85893, /// 0x950
			 0x7fb271f1, /// 0x954
			 0x7f1c4207, /// 0x958
			 0x5c9d3804, /// 0x95c
			 0x7001808c, /// 0x960
			 0x559ad6d7, /// 0x964
			 0x330c4d75, /// 0x968
			 0xc81c46de, /// 0x96c
			 0x61c0c6f8, /// 0x970
			 0x4633b1a8, /// 0x974
			 0xbca64160, /// 0x978
			 0x7259218e, /// 0x97c
			 0xbf40e3af, /// 0x980
			 0xde94e871, /// 0x984
			 0xb3226eb4, /// 0x988
			 0x4deb7414, /// 0x98c
			 0x639bfb76, /// 0x990
			 0x41960e8b, /// 0x994
			 0xcc940140, /// 0x998
			 0x6a30f998, /// 0x99c
			 0x0e8c65c1, /// 0x9a0
			 0x16e5c295, /// 0x9a4
			 0x4b04c7b5, /// 0x9a8
			 0xa6fa4553, /// 0x9ac
			 0xcb4ed235, /// 0x9b0
			 0x5a14f6ab, /// 0x9b4
			 0xe1381511, /// 0x9b8
			 0xc7ed0f7d, /// 0x9bc
			 0xddf626bb, /// 0x9c0
			 0xbb5626a5, /// 0x9c4
			 0xdd86b84a, /// 0x9c8
			 0xec191c76, /// 0x9cc
			 0x0a0c750f, /// 0x9d0
			 0xbb21e0e9, /// 0x9d4
			 0xa0a955d4, /// 0x9d8
			 0x23431d99, /// 0x9dc
			 0x8b2ae8f4, /// 0x9e0
			 0x278ef895, /// 0x9e4
			 0x6cda84d8, /// 0x9e8
			 0xdfe3212d, /// 0x9ec
			 0xc8fd7f60, /// 0x9f0
			 0x48acb943, /// 0x9f4
			 0xa8683f1a, /// 0x9f8
			 0xd15d7b6e, /// 0x9fc
			 0x9c26f7e3, /// 0xa00
			 0xf585fd8b, /// 0xa04
			 0x1ab86d1b, /// 0xa08
			 0x7e81dc7b, /// 0xa0c
			 0x29be36b1, /// 0xa10
			 0x5f75a843, /// 0xa14
			 0x0c6f3111, /// 0xa18
			 0x91833af7, /// 0xa1c
			 0x28bdda75, /// 0xa20
			 0xd3c937d2, /// 0xa24
			 0xf392c606, /// 0xa28
			 0x95420196, /// 0xa2c
			 0xe3aa35d8, /// 0xa30
			 0x4ea7fb46, /// 0xa34
			 0xdffa48ec, /// 0xa38
			 0xdd2a379f, /// 0xa3c
			 0x64aee57c, /// 0xa40
			 0x43711938, /// 0xa44
			 0x1bffd21b, /// 0xa48
			 0x9c10b9c3, /// 0xa4c
			 0x13c1875d, /// 0xa50
			 0x4a3589eb, /// 0xa54
			 0x4258783f, /// 0xa58
			 0xe4b874d3, /// 0xa5c
			 0x8961c558, /// 0xa60
			 0x8e9e54cf, /// 0xa64
			 0x9b49b5e5, /// 0xa68
			 0xa95941d3, /// 0xa6c
			 0x0e638a2c, /// 0xa70
			 0x8cc7bfdc, /// 0xa74
			 0xb09f979c, /// 0xa78
			 0xc05c30ae, /// 0xa7c
			 0x00caae2d, /// 0xa80
			 0x237dc08f, /// 0xa84
			 0xf2694254, /// 0xa88
			 0x186d7ff0, /// 0xa8c
			 0x70e457c1, /// 0xa90
			 0xed2a796f, /// 0xa94
			 0x3ad905cf, /// 0xa98
			 0x531ce961, /// 0xa9c
			 0xccf0632d, /// 0xaa0
			 0xd2e6b486, /// 0xaa4
			 0x7bd8331f, /// 0xaa8
			 0x626cea99, /// 0xaac
			 0xce9e823d, /// 0xab0
			 0x28cc6aaa, /// 0xab4
			 0xd7042c17, /// 0xab8
			 0x45adda10, /// 0xabc
			 0x7b225dca, /// 0xac0
			 0xd70d746f, /// 0xac4
			 0x35a5fc08, /// 0xac8
			 0xf4976487, /// 0xacc
			 0xa10f47fc, /// 0xad0
			 0x1d966ea4, /// 0xad4
			 0xf2739d03, /// 0xad8
			 0xe1a06c63, /// 0xadc
			 0x4ba7ebd1, /// 0xae0
			 0x5fbfcc74, /// 0xae4
			 0x0adcf47c, /// 0xae8
			 0x4bcf8571, /// 0xaec
			 0xcbc4ba15, /// 0xaf0
			 0x79beff57, /// 0xaf4
			 0x63ea576b, /// 0xaf8
			 0x0bbe1700, /// 0xafc
			 0xb1817ced, /// 0xb00
			 0x2104c88e, /// 0xb04
			 0x4cc1e28d, /// 0xb08
			 0x37eaa8bc, /// 0xb0c
			 0x98ce3167, /// 0xb10
			 0xcbdc96fb, /// 0xb14
			 0x2f901952, /// 0xb18
			 0x75cfdfd9, /// 0xb1c
			 0x237a23f0, /// 0xb20
			 0x939c3fad, /// 0xb24
			 0x65ec8973, /// 0xb28
			 0xc07447e4, /// 0xb2c
			 0xe343caf5, /// 0xb30
			 0xf0e73d97, /// 0xb34
			 0x7d6b496c, /// 0xb38
			 0x9a04efc0, /// 0xb3c
			 0x886cfca2, /// 0xb40
			 0x439302a4, /// 0xb44
			 0x286018d8, /// 0xb48
			 0x03e7b69c, /// 0xb4c
			 0x37525ebd, /// 0xb50
			 0xcb319573, /// 0xb54
			 0x083c077e, /// 0xb58
			 0x22ee9235, /// 0xb5c
			 0x17f62b56, /// 0xb60
			 0xbed59d72, /// 0xb64
			 0xa2d0636f, /// 0xb68
			 0xfd224282, /// 0xb6c
			 0x0cd78622, /// 0xb70
			 0xe3200f41, /// 0xb74
			 0xfc09796d, /// 0xb78
			 0x6222e349, /// 0xb7c
			 0xb594991a, /// 0xb80
			 0x6878cac0, /// 0xb84
			 0x53e792ab, /// 0xb88
			 0x9b352a27, /// 0xb8c
			 0x03286f0f, /// 0xb90
			 0x36422548, /// 0xb94
			 0xf1ea606e, /// 0xb98
			 0x143927c9, /// 0xb9c
			 0x12c04b06, /// 0xba0
			 0xbcd4f34a, /// 0xba4
			 0xc7bcb9be, /// 0xba8
			 0xc9393bc1, /// 0xbac
			 0x85555b5a, /// 0xbb0
			 0x44e4a067, /// 0xbb4
			 0x4691fa22, /// 0xbb8
			 0x5be4a6ba, /// 0xbbc
			 0xffb10568, /// 0xbc0
			 0x73882f52, /// 0xbc4
			 0x374b6391, /// 0xbc8
			 0x502a2f55, /// 0xbcc
			 0x2fa37f19, /// 0xbd0
			 0xa6eb0245, /// 0xbd4
			 0x13bac5d5, /// 0xbd8
			 0x97cb7591, /// 0xbdc
			 0x3c00409d, /// 0xbe0
			 0x0ddde39d, /// 0xbe4
			 0x711dba70, /// 0xbe8
			 0x54a74ff4, /// 0xbec
			 0x3354bd65, /// 0xbf0
			 0x54810586, /// 0xbf4
			 0xaaf3d2d0, /// 0xbf8
			 0x4d7c2320, /// 0xbfc
			 0xdcac085e, /// 0xc00
			 0x1e2ee082, /// 0xc04
			 0x6ddc368b, /// 0xc08
			 0x5d479d36, /// 0xc0c
			 0x01b5277b, /// 0xc10
			 0x0fe316bd, /// 0xc14
			 0xe4c6b6c8, /// 0xc18
			 0xadd833c5, /// 0xc1c
			 0xb2f3fbe3, /// 0xc20
			 0xb3446543, /// 0xc24
			 0x4db2b4e5, /// 0xc28
			 0x1db97b1a, /// 0xc2c
			 0x81a4ec22, /// 0xc30
			 0x39a9e18d, /// 0xc34
			 0x1f804139, /// 0xc38
			 0x7afa6859, /// 0xc3c
			 0xd61bd53c, /// 0xc40
			 0x714145fa, /// 0xc44
			 0xe2a42cfc, /// 0xc48
			 0x1754efa0, /// 0xc4c
			 0x3a530759, /// 0xc50
			 0x7f4addda, /// 0xc54
			 0x4bfe98ee, /// 0xc58
			 0x1f1222ae, /// 0xc5c
			 0xbec12304, /// 0xc60
			 0x73cf89cd, /// 0xc64
			 0x3068c6f3, /// 0xc68
			 0x3ac02277, /// 0xc6c
			 0x5ea44648, /// 0xc70
			 0xac9e7cfb, /// 0xc74
			 0x43d49094, /// 0xc78
			 0x8c7681d6, /// 0xc7c
			 0x5142704a, /// 0xc80
			 0x060a9dfc, /// 0xc84
			 0xfd4e4de0, /// 0xc88
			 0xe84aed08, /// 0xc8c
			 0xebb6d66e, /// 0xc90
			 0x13ac7488, /// 0xc94
			 0xeded5cf6, /// 0xc98
			 0x990eaa44, /// 0xc9c
			 0xff9e25bc, /// 0xca0
			 0x2b5825f7, /// 0xca4
			 0xc3f6a348, /// 0xca8
			 0xa8ae796a, /// 0xcac
			 0x5465f04c, /// 0xcb0
			 0xab71f4f4, /// 0xcb4
			 0xb334bc31, /// 0xcb8
			 0x5592ea28, /// 0xcbc
			 0xc56221ed, /// 0xcc0
			 0x7e57ada9, /// 0xcc4
			 0x97d75b0f, /// 0xcc8
			 0xa5cfae5d, /// 0xccc
			 0x22e10ccd, /// 0xcd0
			 0xdbcf0f09, /// 0xcd4
			 0x17fdfb2a, /// 0xcd8
			 0x40cff889, /// 0xcdc
			 0x96e208c7, /// 0xce0
			 0xb5ba3577, /// 0xce4
			 0x94289747, /// 0xce8
			 0x5ce86989, /// 0xcec
			 0xc8145a8b, /// 0xcf0
			 0x8a9d0627, /// 0xcf4
			 0x0fb4a881, /// 0xcf8
			 0xb8100aae, /// 0xcfc
			 0xd28d28dc, /// 0xd00
			 0x78983d99, /// 0xd04
			 0xaec5d08a, /// 0xd08
			 0x45038cbb, /// 0xd0c
			 0x07f52a80, /// 0xd10
			 0x0a45a28f, /// 0xd14
			 0x8ae8c9d5, /// 0xd18
			 0x1ab2e3ba, /// 0xd1c
			 0xae4c0875, /// 0xd20
			 0xf2ba0511, /// 0xd24
			 0x9c39d691, /// 0xd28
			 0x5e9dff63, /// 0xd2c
			 0x9dccbe95, /// 0xd30
			 0x801fc762, /// 0xd34
			 0x6b9449d9, /// 0xd38
			 0x0d369ee6, /// 0xd3c
			 0xc67ec92a, /// 0xd40
			 0xb54afdb9, /// 0xd44
			 0x57d692f7, /// 0xd48
			 0xa9e319b2, /// 0xd4c
			 0x8b99b20a, /// 0xd50
			 0x5f0a2682, /// 0xd54
			 0x22b77e4a, /// 0xd58
			 0x197fcdc5, /// 0xd5c
			 0x3306c25c, /// 0xd60
			 0x83dd5fc5, /// 0xd64
			 0x03e2d586, /// 0xd68
			 0x77d40516, /// 0xd6c
			 0xf170a169, /// 0xd70
			 0x18df9161, /// 0xd74
			 0x48ec9c08, /// 0xd78
			 0x4c1b5f14, /// 0xd7c
			 0xe72787b5, /// 0xd80
			 0x37f45230, /// 0xd84
			 0x811a733d, /// 0xd88
			 0x2a38656b, /// 0xd8c
			 0x591db7a5, /// 0xd90
			 0xdc0145a5, /// 0xd94
			 0xb43a4add, /// 0xd98
			 0xcf77daef, /// 0xd9c
			 0xc7215ee3, /// 0xda0
			 0x46af8763, /// 0xda4
			 0x9cffac5f, /// 0xda8
			 0x8428f7dd, /// 0xdac
			 0x60ff6319, /// 0xdb0
			 0x88f33362, /// 0xdb4
			 0x83be9670, /// 0xdb8
			 0x39818d7f, /// 0xdbc
			 0x27b0179a, /// 0xdc0
			 0xf83af27b, /// 0xdc4
			 0x110024d1, /// 0xdc8
			 0x6071ce9c, /// 0xdcc
			 0x859447cc, /// 0xdd0
			 0x21816847, /// 0xdd4
			 0xdd6e55a4, /// 0xdd8
			 0xb207a2b8, /// 0xddc
			 0x094d4581, /// 0xde0
			 0x16425623, /// 0xde4
			 0x1fb2f0c6, /// 0xde8
			 0x7ecf6c22, /// 0xdec
			 0xd153a1ee, /// 0xdf0
			 0x115a7355, /// 0xdf4
			 0x2410eb06, /// 0xdf8
			 0x0901de05, /// 0xdfc
			 0x5882322c, /// 0xe00
			 0xb7c1490a, /// 0xe04
			 0x5a55518b, /// 0xe08
			 0x1fddc257, /// 0xe0c
			 0xf9963b27, /// 0xe10
			 0x33064565, /// 0xe14
			 0x5e5305f6, /// 0xe18
			 0xcf499595, /// 0xe1c
			 0xc8f2429a, /// 0xe20
			 0xa724b7c6, /// 0xe24
			 0x1f03b0d0, /// 0xe28
			 0x5a95cd08, /// 0xe2c
			 0x2cbcd822, /// 0xe30
			 0xbc9b0072, /// 0xe34
			 0xb826ea62, /// 0xe38
			 0xeaa4aa8e, /// 0xe3c
			 0x6f26acac, /// 0xe40
			 0x870126de, /// 0xe44
			 0x355cfbf1, /// 0xe48
			 0x857006ed, /// 0xe4c
			 0x357f7929, /// 0xe50
			 0x334d8d36, /// 0xe54
			 0xf56b469f, /// 0xe58
			 0x8bd42dbf, /// 0xe5c
			 0x6829200c, /// 0xe60
			 0xafa47aa8, /// 0xe64
			 0x5d2ee4fd, /// 0xe68
			 0x5fc29028, /// 0xe6c
			 0xa79e9545, /// 0xe70
			 0xebd8fd19, /// 0xe74
			 0x19fad88f, /// 0xe78
			 0xe34eb250, /// 0xe7c
			 0x89371a9b, /// 0xe80
			 0x42c9fe7d, /// 0xe84
			 0xacc31241, /// 0xe88
			 0x57ca8f8f, /// 0xe8c
			 0x387867f0, /// 0xe90
			 0x5e18dec4, /// 0xe94
			 0x4f28aa3e, /// 0xe98
			 0xb308ec50, /// 0xe9c
			 0xe4a109b8, /// 0xea0
			 0xe6ebb8ca, /// 0xea4
			 0x49ee1a77, /// 0xea8
			 0x6be415ac, /// 0xeac
			 0xe3a210cc, /// 0xeb0
			 0x1afc756a, /// 0xeb4
			 0xeb6d57f6, /// 0xeb8
			 0xccf3d6c7, /// 0xebc
			 0x34848bf4, /// 0xec0
			 0x5ca25383, /// 0xec4
			 0x09c897a9, /// 0xec8
			 0x444e069b, /// 0xecc
			 0x68dfdaf1, /// 0xed0
			 0xc5b1a741, /// 0xed4
			 0x825c4a4d, /// 0xed8
			 0x14475411, /// 0xedc
			 0x5b2169e7, /// 0xee0
			 0xf9267c74, /// 0xee4
			 0xec9316d9, /// 0xee8
			 0xcacc3c6b, /// 0xeec
			 0x3d8b9af0, /// 0xef0
			 0x72aad9b9, /// 0xef4
			 0xc13777dc, /// 0xef8
			 0xdd71e585, /// 0xefc
			 0x6bac9996, /// 0xf00
			 0x8e231a88, /// 0xf04
			 0x7a45464c, /// 0xf08
			 0xdec65d3b, /// 0xf0c
			 0xbd860b67, /// 0xf10
			 0x0f5b3fe1, /// 0xf14
			 0xb1454c99, /// 0xf18
			 0xad30c054, /// 0xf1c
			 0x8376b82b, /// 0xf20
			 0x66531ccd, /// 0xf24
			 0xb842934b, /// 0xf28
			 0xc6409ee8, /// 0xf2c
			 0x7afc8f0c, /// 0xf30
			 0xd0fc64f3, /// 0xf34
			 0xcaa4a797, /// 0xf38
			 0xc5756f49, /// 0xf3c
			 0x8a0cfa13, /// 0xf40
			 0xe9dc0b55, /// 0xf44
			 0x0685f421, /// 0xf48
			 0x5a60a2fd, /// 0xf4c
			 0x98591f3f, /// 0xf50
			 0xd434f044, /// 0xf54
			 0x101e2bb8, /// 0xf58
			 0xa588b9c4, /// 0xf5c
			 0xa896c805, /// 0xf60
			 0x69d73cf2, /// 0xf64
			 0xafdd5336, /// 0xf68
			 0xb7a54fb3, /// 0xf6c
			 0x4f7437b6, /// 0xf70
			 0x3f3aa648, /// 0xf74
			 0x76d30c3a, /// 0xf78
			 0x3c7fb9eb, /// 0xf7c
			 0xbba4c031, /// 0xf80
			 0xc35ff681, /// 0xf84
			 0xe9fefa5f, /// 0xf88
			 0x56f7afec, /// 0xf8c
			 0xe9081a22, /// 0xf90
			 0x00b486cd, /// 0xf94
			 0xe8a6668c, /// 0xf98
			 0xd176f000, /// 0xf9c
			 0xa67dcd32, /// 0xfa0
			 0xa48a3ddd, /// 0xfa4
			 0xaa55a0ec, /// 0xfa8
			 0x0f0e054e, /// 0xfac
			 0xaad2dd60, /// 0xfb0
			 0xb3cebbde, /// 0xfb4
			 0x8674b484, /// 0xfb8
			 0xaeb0ab70, /// 0xfbc
			 0x98650322, /// 0xfc0
			 0x47e8d4da, /// 0xfc4
			 0xe4bc73e3, /// 0xfc8
			 0x22468e3c, /// 0xfcc
			 0xf2b15a70, /// 0xfd0
			 0xbe92ddfc, /// 0xfd4
			 0x1ef5eb36, /// 0xfd8
			 0x8b8c5f38, /// 0xfdc
			 0x2e8e0294, /// 0xfe0
			 0xf8ff5b23, /// 0xfe4
			 0x749b6711, /// 0xfe8
			 0x66ca37c6, /// 0xfec
			 0x56502b31, /// 0xff0
			 0x7355c196, /// 0xff4
			 0x78ce8fca, /// 0xff8
			 0xb7acd273 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0000c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00010
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00018
			 0x33333333,                                                  // 4 random values                             /// 0001c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00020
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00028
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0002c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00030
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00038
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00040
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0004c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00054
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0005c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00060
			 0xffc00001,                                                  // -signaling NaN                              /// 00064
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00068
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x00011111,                                                  // 9.7958E-41                                  /// 00074
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0007c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xffc00001,                                                  // -signaling NaN                              /// 00084
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00094
			 0x33333333,                                                  // 4 random values                             /// 00098
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0009c
			 0x0c600000,                                                  // Leading 1s:                                 /// 000a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 000b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00108
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0010c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00114
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x00000000,                                                  // zero                                        /// 0011c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00128
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0012c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00134
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00138
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x80000000,                                                  // -zero                                       /// 00148
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0014c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00154
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00158
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0015c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0c780000,                                                  // Leading 1s:                                 /// 00164
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0016c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00170
			 0x0e000003,                                                  // Trailing 1s:                                /// 00174
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00178
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0017c
			 0x33333333,                                                  // 4 random values                             /// 00180
			 0x4b000000,                                                  // 8388608.0                                   /// 00184
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00188
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0018c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00198
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00200
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00208
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00210
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x00011111,                                                  // 9.7958E-41                                  /// 00218
			 0xbf028f5c,                                                  // -0.51                                       /// 0021c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00220
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00224
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x3f028f5c,                                                  // 0.51                                        /// 00230
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00234
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00238
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0023c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00240
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00244
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00248
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0024c
			 0xffc00001,                                                  // -signaling NaN                              /// 00250
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00254
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00258
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0025c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00264
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00268
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0026c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0027c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00280
			 0x0e000001,                                                  // Trailing 1s:                                /// 00284
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00290
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00294
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00298
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0029c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002cc
			 0xff800000,                                                  // -inf                                        /// 002d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002e4
			 0x00000000,                                                  // zero                                        /// 002e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f4
			 0xbf028f5c,                                                  // -0.51                                       /// 002f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00300
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00304
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0030c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00310
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00320
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00324
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00330
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00334
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00338
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0033c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00344
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00354
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0035c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00360
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00364
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00368
			 0x0c780000,                                                  // Leading 1s:                                 /// 0036c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00374
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0037c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00380
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00388
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0038c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00394
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00398
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0039c
			 0x55555555,                                                  // 4 random values                             /// 003a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00400
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00404
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0041c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00420
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00424
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00434
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00438
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0043c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00440
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00448
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0044c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00450
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00454
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0045c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00460
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00464
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00468
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00470
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0e000003,                                                  // Trailing 1s:                                /// 00478
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00480
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0048c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x3f028f5c,                                                  // 0.51                                        /// 00494
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0049c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004a8
			 0x3f028f5c,                                                  // 0.51                                        /// 004ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0xff800000,                                                  // -inf                                        /// 004c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 004e0
			 0xcb000000,                                                  // -8388608.0                                  /// 004e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00510
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00514
			 0x00011111,                                                  // 9.7958E-41                                  /// 00518
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0051c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00520
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0052c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00534
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00538
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0053c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00548
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0054c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00554
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00558
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0055c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00560
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00564
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00568
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0056c
			 0xbf028f5c,                                                  // -0.51                                       /// 00570
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0057c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00580
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0xbf028f5c,                                                  // -0.51                                       /// 00588
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00594
			 0x3f028f5c,                                                  // 0.51                                        /// 00598
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005b8
			 0x7fc00001,                                                  // signaling NaN                               /// 005bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005e0
			 0xbf028f5c,                                                  // -0.51                                       /// 005e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 005f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00600
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00608
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0060c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00610
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0061c
			 0x4b000000,                                                  // 8388608.0                                   /// 00620
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00630
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00634
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00638
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0063c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00640
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00644
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x55555555,                                                  // 4 random values                             /// 0064c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00650
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00654
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00658
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0065c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00664
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0066c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00674
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00678
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0067c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00680
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00684
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00688
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0068c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0069c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a0
			 0xcb000000,                                                  // -8388608.0                                  /// 006a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006b4
			 0x7f800000,                                                  // inf                                         /// 006b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006d0
			 0xffc00001,                                                  // -signaling NaN                              /// 006d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00000000,                                                  // zero                                        /// 006fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00700
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00704
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00714
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00718
			 0x7f800000,                                                  // inf                                         /// 0071c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00724
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0072c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00734
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00740
			 0x3f028f5c,                                                  // 0.51                                        /// 00744
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0074c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00750
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x4b000000,                                                  // 8388608.0                                   /// 00758
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00760
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00764
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00768
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0076c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00770
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00774
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0077c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00788
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0078c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00790
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00794
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0079c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x7fc00001,                                                  // signaling NaN                               /// 007b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x80000000,                                                  // -zero                                       /// 007c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00800
			 0x7f800000,                                                  // inf                                         /// 00804
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00808
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0080c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00814
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0081c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00820
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00824
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00828
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00830
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00834
			 0x0e000007,                                                  // Trailing 1s:                                /// 00838
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00840
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00844
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00848
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00850
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00858
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0085c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00868
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0086c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00870
			 0x0c780000,                                                  // Leading 1s:                                 /// 00874
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00878
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0087c
			 0x80000000,                                                  // -zero                                       /// 00880
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0089c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008a8
			 0x7fc00001,                                                  // signaling NaN                               /// 008ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xcb000000,                                                  // -8388608.0                                  /// 008b8
			 0x7fc00001,                                                  // signaling NaN                               /// 008bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d0
			 0x3f028f5c,                                                  // 0.51                                        /// 008d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00900
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00904
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0c600000,                                                  // Leading 1s:                                 /// 0090c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00910
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x0e000007,                                                  // Trailing 1s:                                /// 00918
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0091c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00920
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x00000000,                                                  // zero                                        /// 00928
			 0x7f800000,                                                  // inf                                         /// 0092c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00930
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00934
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00938
			 0x7f800000,                                                  // inf                                         /// 0093c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00940
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00944
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00011111,                                                  // 9.7958E-41                                  /// 0094c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00950
			 0x0e000001,                                                  // Trailing 1s:                                /// 00954
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00958
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0095c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00960
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00964
			 0x0c700000,                                                  // Leading 1s:                                 /// 00968
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0096c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00970
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00974
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00980
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00990
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009ec
			 0x33333333,                                                  // 4 random values                             /// 009f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a24
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80000000,                                                  // -zero                                       /// 00aa0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00aac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ab0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ab8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00abc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ac0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ac4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x55555555,                                                  // 4 random values                             /// 00acc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00adc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00af4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00afc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b34
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0xcb000000,                                                  // -8388608.0                                  /// 00b54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b74
			 0xbf028f5c,                                                  // -0.51                                       /// 00b78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00000000,                                                  // zero                                        /// 00b80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b88
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b8c
			 0x55555555,                                                  // 4 random values                             /// 00b90
			 0x7f800000,                                                  // inf                                         /// 00b94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ba4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ba8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bf8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c14
			 0xbf028f5c,                                                  // -0.51                                       /// 00c18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c28
			 0xbf028f5c,                                                  // -0.51                                       /// 00c2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x3f028f5c,                                                  // 0.51                                        /// 00c98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ccc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cdc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ce0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cf8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cfc
			 0x4b000000,                                                  // 8388608.0                                   /// 00d00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00da8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dbc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ddc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00de0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0xff800000,                                                  // -inf                                        /// 00df4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00df8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dfc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x80000000,                                                  // -zero                                       /// 00e50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e80
			 0x7fc00001,                                                  // signaling NaN                               /// 00e84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x55555555,                                                  // 4 random values                             /// 00e98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ea8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ebc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ec0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ec4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ecc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ed0
			 0x80000000,                                                  // -zero                                       /// 00ed4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ed8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00edc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ee0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x55555555,                                                  // 4 random values                             /// 00efc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0xcb000000,                                                  // -8388608.0                                  /// 00f1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f2c
			 0x00000000,                                                  // zero                                        /// 00f30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f80
			 0x7fc00001,                                                  // signaling NaN                               /// 00f84
			 0x7fc00001,                                                  // signaling NaN                               /// 00f88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x33333333,                                                  // 4 random values                             /// 00f94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ff4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff8
			 0x00000008                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x000000fc,
			 0x00000000,
			 0x000001ec,
			 0x000005dc,
			 0x00000354,
			 0x00000330,
			 0x00000408,
			 0x000003c0,

			 /// vpu register f2
			 0x41900000,
			 0x41b80000,
			 0x42000000,
			 0x41c00000,
			 0x41c80000,
			 0x41200000,
			 0x41300000,
			 0x42000000,

			 /// vpu register f3
			 0x41d00000,
			 0x3f800000,
			 0x40800000,
			 0x41400000,
			 0x41300000,
			 0x41e00000,
			 0x41100000,
			 0x41b80000,

			 /// vpu register f4
			 0x41c00000,
			 0x41400000,
			 0x41400000,
			 0x41d80000,
			 0x41200000,
			 0x41700000,
			 0x41880000,
			 0x41f80000,

			 /// vpu register f5
			 0x41a80000,
			 0x41b80000,
			 0x40a00000,
			 0x41500000,
			 0x41200000,
			 0x42000000,
			 0x41500000,
			 0x41d00000,

			 /// vpu register f6
			 0x41980000,
			 0x41900000,
			 0x41500000,
			 0x41300000,
			 0x41000000,
			 0x41300000,
			 0x42000000,
			 0x41f00000,

			 /// vpu register f7
			 0x41980000,
			 0x41700000,
			 0x41980000,
			 0x41d00000,
			 0x41d00000,
			 0x3f800000,
			 0x41d80000,
			 0x40000000,

			 /// vpu register f8
			 0x41f80000,
			 0x41d00000,
			 0x41000000,
			 0x41a00000,
			 0x40c00000,
			 0x41d80000,
			 0x41980000,
			 0x41c00000,

			 /// vpu register f9
			 0x40000000,
			 0x41300000,
			 0x41c00000,
			 0x41a00000,
			 0x41e80000,
			 0x41400000,
			 0x40400000,
			 0x40800000,

			 /// vpu register f10
			 0x3f800000,
			 0x42000000,
			 0x41e80000,
			 0x41900000,
			 0x41e80000,
			 0x41e00000,
			 0x41c00000,
			 0x41500000,

			 /// vpu register f11
			 0x41d00000,
			 0x41d00000,
			 0x41d00000,
			 0x42000000,
			 0x41400000,
			 0x41900000,
			 0x41100000,
			 0x41900000,

			 /// vpu register f12
			 0x40e00000,
			 0x41900000,
			 0x41d00000,
			 0x41b80000,
			 0x41d00000,
			 0x41a00000,
			 0x40000000,
			 0x41500000,

			 /// vpu register f13
			 0x41300000,
			 0x40c00000,
			 0x41000000,
			 0x41000000,
			 0x41000000,
			 0x41d80000,
			 0x41b80000,
			 0x40400000,

			 /// vpu register f14
			 0x41900000,
			 0x42000000,
			 0x41200000,
			 0x41e80000,
			 0x41f00000,
			 0x41a80000,
			 0x40400000,
			 0x41800000,

			 /// vpu register f15
			 0x41d00000,
			 0x40c00000,
			 0x41700000,
			 0x41e00000,
			 0x40800000,
			 0x41600000,
			 0x41600000,
			 0x41600000,

			 /// vpu register f16
			 0x41a00000,
			 0x41f80000,
			 0x41300000,
			 0x41f00000,
			 0x41700000,
			 0x41100000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f17
			 0x41880000,
			 0x41900000,
			 0x41700000,
			 0x40800000,
			 0x41b80000,
			 0x41000000,
			 0x41400000,
			 0x41400000,

			 /// vpu register f18
			 0x41e00000,
			 0x41600000,
			 0x41300000,
			 0x3f800000,
			 0x41b80000,
			 0x41f00000,
			 0x41d80000,
			 0x41e80000,

			 /// vpu register f19
			 0x41300000,
			 0x41e80000,
			 0x41880000,
			 0x40c00000,
			 0x41880000,
			 0x41000000,
			 0x40400000,
			 0x41a00000,

			 /// vpu register f20
			 0x41c80000,
			 0x41980000,
			 0x41c00000,
			 0x40e00000,
			 0x41300000,
			 0x40000000,
			 0x40800000,
			 0x40400000,

			 /// vpu register f21
			 0x40c00000,
			 0x41000000,
			 0x41880000,
			 0x41300000,
			 0x41000000,
			 0x41a00000,
			 0x41d80000,
			 0x41600000,

			 /// vpu register f22
			 0x40c00000,
			 0x41100000,
			 0x41c80000,
			 0x41a00000,
			 0x41a00000,
			 0x41a80000,
			 0x41700000,
			 0x42000000,

			 /// vpu register f23
			 0x41600000,
			 0x41c00000,
			 0x41000000,
			 0x40400000,
			 0x40e00000,
			 0x41a00000,
			 0x41e00000,
			 0x41d80000,

			 /// vpu register f24
			 0x41d80000,
			 0x40a00000,
			 0x41f80000,
			 0x41f80000,
			 0x41f00000,
			 0x41600000,
			 0x41a00000,
			 0x40e00000,

			 /// vpu register f25
			 0x41700000,
			 0x41c80000,
			 0x40000000,
			 0x41c00000,
			 0x41980000,
			 0x41800000,
			 0x41700000,
			 0x41c80000,

			 /// vpu register f26
			 0x41e80000,
			 0x41400000,
			 0x41b00000,
			 0x40e00000,
			 0x41b80000,
			 0x41200000,
			 0x41880000,
			 0x41e80000,

			 /// vpu register f27
			 0x41880000,
			 0x41200000,
			 0x41800000,
			 0x41500000,
			 0x41d80000,
			 0x41a80000,
			 0x41880000,
			 0x41e00000,

			 /// vpu register f28
			 0x41b80000,
			 0x41980000,
			 0x41300000,
			 0x42000000,
			 0x41400000,
			 0x40c00000,
			 0x40800000,
			 0x41880000,

			 /// vpu register f29
			 0x41c80000,
			 0x41d00000,
			 0x41100000,
			 0x41b00000,
			 0x41a00000,
			 0x41f80000,
			 0x3f800000,
			 0x41f80000,

			 /// vpu register f30
			 0x41c00000,
			 0x41b00000,
			 0x41300000,
			 0x41700000,
			 0x41400000,
			 0x41a80000,
			 0x41e00000,
			 0x41f80000,

			 /// vpu register f31
			 0x41980000,
			 0x41b00000,
			 0x41900000,
			 0x41a00000,
			 0x41600000,
			 0x41400000,
			 0x41a00000,
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
		"fgbl.ps f13, f1 (x13)\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f11, f1 (x15)\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f13, f1 (x15)\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f28, f1 (x11)\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f7, f1 (x15)\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f22, f1 (x12)\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f27, f1 (x11)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f7, f1 (x14)\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f16, f1 (x14)\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f11, f1 (x11)\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f4, f1 (x12)\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f5, f1 (x13)\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x13)\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f19, f1 (x11)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f6, f1 (x12)\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f20, f1 (x14)\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f16, f1 (x15)\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f9, f1 (x14)\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f24, f1 (x13)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f20, f1 (x11)\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f16, f1 (x15)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f27, f1 (x11)\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f3, f1 (x11)\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f17, f1 (x12)\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f10, f1 (x12)\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f24, f1 (x11)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f23, f1 (x14)\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x11)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f18, f1 (x12)\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x14)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f23, f1 (x11)\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f11, f1 (x13)\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f7, f1 (x15)\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f3, f1 (x12)\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f8, f1 (x12)\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f21, f1 (x12)\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f18, f1 (x15)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f18, f1 (x15)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f15, f1 (x14)\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f20, f1 (x13)\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f15, f1 (x11)\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x11)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f16, f1 (x13)\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f6, f1 (x11)\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f12, f1 (x11)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f11, f1 (x13)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f14, f1 (x11)\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f17, f1 (x12)\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f10, f1 (x12)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f3, f1 (x15)\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f4, f1 (x13)\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f28, f1 (x12)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f6, f1 (x12)\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f3, f1 (x15)\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f18, f1 (x15)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f10, f1 (x15)\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f15, f1 (x12)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x15)\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f17, f1 (x13)\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f10, f1 (x13)\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x13)\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f11, f1 (x11)\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f22, f1 (x13)\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f28, f1 (x12)\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f5, f1 (x14)\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f28, f1 (x14)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f17, f1 (x11)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f5, f1 (x13)\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f26, f1 (x12)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f30, f1 (x12)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f8, f1 (x15)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x12)\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f28, f1 (x11)\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f4, f1 (x14)\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f26, f1 (x15)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f17, f1 (x11)\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f19, f1 (x12)\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f20, f1 (x13)\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f12, f1 (x11)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f2, f1 (x11)\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f23, f1 (x14)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f3, f1 (x14)\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f28, f1 (x14)\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f25, f1 (x13)\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f15, f1 (x13)\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f17, f1 (x13)\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f9, f1 (x15)\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f14, f1 (x15)\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f16, f1 (x14)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f25, f1 (x15)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f22, f1 (x11)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f27, f1 (x11)\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f6, f1 (x11)\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f22, f1 (x13)\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f29, f1 (x12)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f13, f1 (x11)\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f24, f1 (x12)\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f16, f1 (x14)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgbl.ps f19, f1 (x12)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
