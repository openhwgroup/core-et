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
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00000
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00008
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0000c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00010
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00014
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00024
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00030
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00034
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00038
			 0x0e000001,                                                  // Trailing 1s:                                /// 0003c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00040
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0004c
			 0xff800000,                                                  // -inf                                        /// 00050
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00058
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0005c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00064
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00070
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00078
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0007c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00084
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00088
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0008c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00090
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00094
			 0x0c600000,                                                  // Leading 1s:                                 /// 00098
			 0xcb000000,                                                  // -8388608.0                                  /// 0009c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x4b000000,                                                  // 8388608.0                                   /// 000cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d4
			 0xff800000,                                                  // -inf                                        /// 000d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000e4
			 0xbf028f5c,                                                  // -0.51                                       /// 000e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00100
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00108
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0010c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00110
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00114
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00118
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0011c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00128
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00130
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00134
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00138
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00140
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00144
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00148
			 0x3f028f5c,                                                  // 0.51                                        /// 0014c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00158
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00160
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00164
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00168
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00170
			 0xcb000000,                                                  // -8388608.0                                  /// 00174
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00178
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00188
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00190
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00194
			 0x00011111,                                                  // 9.7958E-41                                  /// 00198
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0019c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00200
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00204
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0020c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00214
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00218
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00224
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00228
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00230
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00234
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0023c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00240
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00248
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0024c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00254
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0xffc00001,                                                  // -signaling NaN                              /// 0025c
			 0x3f028f5c,                                                  // 0.51                                        /// 00260
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00264
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0026c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00270
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0027c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00280
			 0x7fc00001,                                                  // signaling NaN                               /// 00284
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00290
			 0xffc00001,                                                  // -signaling NaN                              /// 00294
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002fc
			 0x33333333,                                                  // 4 random values                             /// 00300
			 0x0e000007,                                                  // Trailing 1s:                                /// 00304
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00308
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00310
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00318
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00320
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00324
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00328
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00334
			 0xff800000,                                                  // -inf                                        /// 00338
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0033c
			 0x7fc00001,                                                  // signaling NaN                               /// 00340
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0034c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00350
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00354
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00358
			 0x0c700000,                                                  // Leading 1s:                                 /// 0035c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00368
			 0x33333333,                                                  // 4 random values                             /// 0036c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00374
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00380
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00384
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00388
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0038c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00394
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00398
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80000000,                                                  // -zero                                       /// 003a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00400
			 0x7f800000,                                                  // inf                                         /// 00404
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00408
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0040c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00414
			 0x00011111,                                                  // 9.7958E-41                                  /// 00418
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00428
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00430
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00438
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0043c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00444
			 0x4b000000,                                                  // 8388608.0                                   /// 00448
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0044c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00450
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00454
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c700000,                                                  // Leading 1s:                                 /// 0045c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00460
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00468
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0046c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00470
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00478
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00480
			 0x33333333,                                                  // 4 random values                             /// 00484
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00488
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00490
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0049c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x7fc00001,                                                  // signaling NaN                               /// 004a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d4
			 0xcb000000,                                                  // -8388608.0                                  /// 004d8
			 0xbf028f5c,                                                  // -0.51                                       /// 004dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0xff800000,                                                  // -inf                                        /// 004f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f4
			 0x80000000,                                                  // -zero                                       /// 004f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00500
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00504
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0050c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00510
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00514
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00518
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00528
			 0x0c700000,                                                  // Leading 1s:                                 /// 0052c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00530
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0053c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00540
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00544
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0054c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00550
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00554
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0055c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00560
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00564
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00574
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00578
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0057c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00584
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00588
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0058c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00590
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00598
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0059c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005ac
			 0x7fc00001,                                                  // signaling NaN                               /// 005b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005e8
			 0x55555555,                                                  // 4 random values                             /// 005ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00604
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0060c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00610
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00614
			 0x0e000001,                                                  // Trailing 1s:                                /// 00618
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00620
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00628
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00630
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00638
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0063c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00640
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00644
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00648
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0064c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00650
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00658
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0065c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00660
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0066c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00670
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00684
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00688
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0068c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00690
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00698
			 0x7fc00001,                                                  // signaling NaN                               /// 0069c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e4
			 0x4b000000,                                                  // 8388608.0                                   /// 006e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00700
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00704
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0070c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00714
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00718
			 0x55555555,                                                  // 4 random values                             /// 0071c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00720
			 0x0e000003,                                                  // Trailing 1s:                                /// 00724
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00728
			 0x0c400000,                                                  // Leading 1s:                                 /// 0072c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00730
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00734
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x55555555,                                                  // 4 random values                             /// 00740
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00744
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00748
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0074c
			 0xbf028f5c,                                                  // -0.51                                       /// 00750
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00754
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00758
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0075c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00760
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00764
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0076c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00770
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00774
			 0x3f028f5c,                                                  // 0.51                                        /// 00778
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00780
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00784
			 0xff800000,                                                  // -inf                                        /// 00788
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0078c
			 0x7fc00001,                                                  // signaling NaN                               /// 00790
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00794
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00798
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x55555555,                                                  // 4 random values                             /// 007a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f4
			 0x33333333,                                                  // 4 random values                             /// 007f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00804
			 0x00011111,                                                  // 9.7958E-41                                  /// 00808
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0080c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00810
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00818
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00820
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00824
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00828
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00834
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x7fc00001,                                                  // signaling NaN                               /// 00840
			 0x55555555,                                                  // 4 random values                             /// 00844
			 0x0c400000,                                                  // Leading 1s:                                 /// 00848
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00858
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0085c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00868
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00874
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00000000,                                                  // zero                                        /// 0087c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00884
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00888
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00894
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0089c
			 0x0c780000,                                                  // Leading 1s:                                 /// 008a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x55555555,                                                  // 4 random values                             /// 00900
			 0x0e000003,                                                  // Trailing 1s:                                /// 00904
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00908
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0090c
			 0x7fc00001,                                                  // signaling NaN                               /// 00910
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00914
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00918
			 0x55555555,                                                  // 4 random values                             /// 0091c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00924
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00928
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e000007,                                                  // Trailing 1s:                                /// 00934
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00940
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00944
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00948
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0094c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00950
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00954
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00958
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00960
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00964
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00968
			 0x80000000,                                                  // -zero                                       /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00000000,                                                  // zero                                        /// 00978
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x55555555,                                                  // 4 random values                             /// 00980
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0098c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00990
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00994
			 0x80000000,                                                  // -zero                                       /// 00998
			 0x3f028f5c,                                                  // 0.51                                        /// 0099c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x4b000000,                                                  // 8388608.0                                   /// 009ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x80000000,                                                  // -zero                                       /// 009f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x55555555,                                                  // 4 random values                             /// 00a18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aa0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ab4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ab8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00abc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ac0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ac8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00adc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ae0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ae4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ae8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00af0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00af4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00afc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b04
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b84
			 0xbf028f5c,                                                  // -0.51                                       /// 00b88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ba4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bb4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0xff800000,                                                  // -inf                                        /// 00be8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bf8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c0c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c1c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0xff800000,                                                  // -inf                                        /// 00c28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c34
			 0x3f028f5c,                                                  // 0.51                                        /// 00c38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c84
			 0x80000000,                                                  // -zero                                       /// 00c88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca8
			 0x00000000,                                                  // zero                                        /// 00cac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x55555555,                                                  // 4 random values                             /// 00cb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cb8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cd8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cdc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x55555555,                                                  // 4 random values                             /// 00ce4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cf0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000000,                                                  // zero                                        /// 00d1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00da0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00da8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00db0
			 0xff800000,                                                  // -inf                                        /// 00db4
			 0x7f800000,                                                  // inf                                         /// 00db8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dbc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00de0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00de8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00df4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dfc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x55555555,                                                  // 4 random values                             /// 00e10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80000000,                                                  // -zero                                       /// 00e5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e68
			 0xffc00001,                                                  // -signaling NaN                              /// 00e6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xbf028f5c,                                                  // -0.51                                       /// 00e74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e78
			 0xcb000000,                                                  // -8388608.0                                  /// 00e7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e94
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e9c
			 0x80000000,                                                  // -zero                                       /// 00ea0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ec8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ecc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ed0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ed8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00edc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0xff800000,                                                  // -inf                                        /// 00ee4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ee8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0xff800000,                                                  // -inf                                        /// 00ef0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ef8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00efc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f14
			 0x80000000,                                                  // -zero                                       /// 00f18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f84
			 0xbf028f5c,                                                  // -0.51                                       /// 00f88
			 0x7fc00001,                                                  // signaling NaN                               /// 00f8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fa0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fb4
			 0x7f800000,                                                  // inf                                         /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80000000,                                                  // -zero                                       /// 00fc0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x7f800001 // SNaN                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00001,                                                  // -signaling NaN                              /// 00000
			 0x00011111,                                                  // 9.7958E-41                                  /// 00004
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00008
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00010
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00018
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0001c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00020
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00030
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00034
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00038
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0003c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00040
			 0xff800000,                                                  // -inf                                        /// 00044
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0004c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00058
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00060
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0006c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00070
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00074
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00078
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0007c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00080
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00084
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00088
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00090
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00094
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00098
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00100
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00108
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0010c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00110
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00114
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00118
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0011c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00120
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00124
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00128
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00138
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0013c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00140
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00144
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0014c
			 0x4b000000,                                                  // 8388608.0                                   /// 00150
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00154
			 0x3f028f5c,                                                  // 0.51                                        /// 00158
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0015c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00160
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00170
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00174
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00178
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00180
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00184
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00188
			 0x0e000007,                                                  // Trailing 1s:                                /// 0018c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00194
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0019c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001bc
			 0x80000000,                                                  // -zero                                       /// 001c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00200
			 0xff800000,                                                  // -inf                                        /// 00204
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00208
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00210
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00214
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0021c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00224
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00228
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0022c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00230
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00234
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00238
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00240
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00244
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00248
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0024c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00250
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00254
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00258
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0025c
			 0xcb000000,                                                  // -8388608.0                                  /// 00260
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00264
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0026c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00278
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00284
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00288
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00290
			 0x3f028f5c,                                                  // 0.51                                        /// 00294
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e000003,                                                  // Trailing 1s:                                /// 002a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x33333333,                                                  // 4 random values                             /// 002d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00300
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00304
			 0x0e000001,                                                  // Trailing 1s:                                /// 00308
			 0x80011111,                                                  // -9.7958E-41                                 /// 0030c
			 0x7fc00001,                                                  // signaling NaN                               /// 00310
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00314
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00320
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00324
			 0x00011111,                                                  // 9.7958E-41                                  /// 00328
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00334
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0033c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00340
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00344
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00348
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80000000,                                                  // -zero                                       /// 00350
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e000003,                                                  // Trailing 1s:                                /// 0035c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00368
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0036c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00370
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00374
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00378
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0037c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00380
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00011111,                                                  // 9.7958E-41                                  /// 00388
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00390
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00394
			 0xffc00001,                                                  // -signaling NaN                              /// 00398
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0039c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00400
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00404
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0040c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00410
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0041c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00424
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00428
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0042c
			 0xffc00001,                                                  // -signaling NaN                              /// 00430
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x7fc00001,                                                  // signaling NaN                               /// 00438
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0043c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00444
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00450
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0045c
			 0x3f028f5c,                                                  // 0.51                                        /// 00460
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00464
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00468
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0046c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00478
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00480
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00484
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00488
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00498
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004ac
			 0x55555555,                                                  // 4 random values                             /// 004b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x4b000000,                                                  // 8388608.0                                   /// 004c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004c4
			 0xff800000,                                                  // -inf                                        /// 004c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00500
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00504
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00510
			 0x0e000003,                                                  // Trailing 1s:                                /// 00514
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00518
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00520
			 0x00011111,                                                  // 9.7958E-41                                  /// 00524
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0052c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00530
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80011111,                                                  // -9.7958E-41                                 /// 00538
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00540
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00544
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00548
			 0x7f800000,                                                  // inf                                         /// 0054c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0xcb000000,                                                  // -8388608.0                                  /// 00554
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00558
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0055c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00560
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00568
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0056c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00574
			 0x0e000007,                                                  // Trailing 1s:                                /// 00578
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0057c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00588
			 0x7f800000,                                                  // inf                                         /// 0058c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00594
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00598
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00600
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00608
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0060c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00614
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00618
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0061c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00620
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00624
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00628
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0062c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00630
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00634
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00638
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00648
			 0xcb000000,                                                  // -8388608.0                                  /// 0064c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x0e000001,                                                  // Trailing 1s:                                /// 00654
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00658
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00660
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00668
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x00000000,                                                  // zero                                        /// 00674
			 0xff800000,                                                  // -inf                                        /// 00678
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00680
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00684
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00688
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00694
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00698
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0c400000,                                                  // Leading 1s:                                 /// 006a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0xcb000000,                                                  // -8388608.0                                  /// 006b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00704
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0070c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00714
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00718
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0071c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00720
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00000000,                                                  // zero                                        /// 00728
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0072c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00734
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00740
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00754
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0075c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00760
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00764
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00768
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0076c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00770
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00774
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0077c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00784
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00788
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00794
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00804
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00810
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00818
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0081c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00820
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00828
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0082c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00830
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00838
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0083c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00840
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00848
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00854
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00858
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0085c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00860
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00864
			 0x00011111,                                                  // 9.7958E-41                                  /// 00868
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0086c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00870
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00874
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00878
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0087c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00880
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00884
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00888
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00890
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00898
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00900
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00904
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00908
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00910
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00914
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0091c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00924
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00928
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0092c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00944
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00948
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00958
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0095c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00960
			 0x3f028f5c,                                                  // 0.51                                        /// 00964
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00968
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00970
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00974
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00978
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0097c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00980
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00984
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00988
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0098c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00990
			 0x0e000003,                                                  // Trailing 1s:                                /// 00994
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0099c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009b0
			 0x7fc00001,                                                  // signaling NaN                               /// 009b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c4
			 0x55555555,                                                  // 4 random values                             /// 009c8
			 0xffc00001,                                                  // -signaling NaN                              /// 009cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0xff800000,                                                  // -inf                                        /// 00a00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a58
			 0xcb000000,                                                  // -8388608.0                                  /// 00a5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ab8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00abc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ac0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac8
			 0xcb000000,                                                  // -8388608.0                                  /// 00acc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ad0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ad4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ae0
			 0x33333333,                                                  // 4 random values                             /// 00ae4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x7f800000,                                                  // inf                                         /// 00af0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00af4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00af8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b04
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b18
			 0x55555555,                                                  // 4 random values                             /// 00b1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b4c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x7f800000,                                                  // inf                                         /// 00b54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b5c
			 0xff800000,                                                  // -inf                                        /// 00b60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00be0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00be4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bf4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x4b000000,                                                  // 8388608.0                                   /// 00c08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c1c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c80
			 0xff800000,                                                  // -inf                                        /// 00c84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ca4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb0
			 0x33333333,                                                  // 4 random values                             /// 00cb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ccc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cdc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ce0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cf0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cfc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d08
			 0xffc00001,                                                  // -signaling NaN                              /// 00d0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d14
			 0x4b000000,                                                  // 8388608.0                                   /// 00d18
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d30
			 0x3f028f5c,                                                  // 0.51                                        /// 00d34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d58
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d68
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x4b000000,                                                  // 8388608.0                                   /// 00d7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00db0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dc4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dc8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00df0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00df8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e0c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e14
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00000000,                                                  // zero                                        /// 00e28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e34
			 0xffc00001,                                                  // -signaling NaN                              /// 00e38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0xffc00001,                                                  // -signaling NaN                              /// 00e58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e5c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e8c
			 0xff800000,                                                  // -inf                                        /// 00e90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ea0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ea8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb4
			 0x33333333,                                                  // 4 random values                             /// 00eb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ebc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ec0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ef0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ef8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00efc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x33333333,                                                  // 4 random values                             /// 00f70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fe0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x00000080                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000001,                                                  // Trailing 1s:                                /// 00000
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00004
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00008
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0000c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00010
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00014
			 0x33333333,                                                  // 4 random values                             /// 00018
			 0x0c780000,                                                  // Leading 1s:                                 /// 0001c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00024
			 0x80011111,                                                  // -9.7958E-41                                 /// 00028
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00038
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00040
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00048
			 0xbf028f5c,                                                  // -0.51                                       /// 0004c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00050
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00054
			 0x0c780000,                                                  // Leading 1s:                                 /// 00058
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0005c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00060
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00064
			 0x7f800000,                                                  // inf                                         /// 00068
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0006c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00070
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00074
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0007c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00080
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00084
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00088
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0008c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00090
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00094
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00098
			 0x0c400000,                                                  // Leading 1s:                                 /// 0009c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x55555555,                                                  // 4 random values                             /// 000d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xff800000,                                                  // -inf                                        /// 000f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00100
			 0x0c600000,                                                  // Leading 1s:                                 /// 00104
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00108
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0010c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00114
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00118
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0011c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00124
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00128
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00130
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00134
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00138
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0013c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00140
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00148
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0014c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00150
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00158
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00160
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00164
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00168
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0016c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00170
			 0x0c700000,                                                  // Leading 1s:                                 /// 00174
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00178
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00180
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00184
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00188
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0018c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00190
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00194
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00198
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0019c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b0
			 0x80000000,                                                  // -zero                                       /// 001b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 001c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00200
			 0x4b000000,                                                  // 8388608.0                                   /// 00204
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00208
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0020c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00210
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00214
			 0x7fc00001,                                                  // signaling NaN                               /// 00218
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0021c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00220
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0022c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00230
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00234
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00238
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00240
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00244
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x55555555,                                                  // 4 random values                             /// 0024c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00250
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0c700000,                                                  // Leading 1s:                                 /// 0025c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00260
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00268
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0026c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00278
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0027c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00280
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0028c
			 0x00000000,                                                  // zero                                        /// 00290
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00298
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0029c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0xff800000,                                                  // -inf                                        /// 002d4
			 0xcb000000,                                                  // -8388608.0                                  /// 002d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00000000,                                                  // zero                                        /// 002fc
			 0x80000000,                                                  // -zero                                       /// 00300
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0030c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00310
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0xff800000,                                                  // -inf                                        /// 00318
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0031c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00320
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00324
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0032c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00330
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00334
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0033c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00344
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00350
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00354
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00358
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0035c
			 0xffc00001,                                                  // -signaling NaN                              /// 00360
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00364
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00368
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0036c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00370
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00374
			 0x0c400000,                                                  // Leading 1s:                                 /// 00378
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0037c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0038c
			 0x4b000000,                                                  // 8388608.0                                   /// 00390
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00394
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00398
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0039c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x7f800000,                                                  // inf                                         /// 003ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d4
			 0x80000000,                                                  // -zero                                       /// 003d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00400
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00404
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00408
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00410
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00414
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00418
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0041c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00424
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00430
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00434
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00440
			 0x4b000000,                                                  // 8388608.0                                   /// 00444
			 0x80000000,                                                  // -zero                                       /// 00448
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0044c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x4b000000,                                                  // 8388608.0                                   /// 00458
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00460
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00474
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0047c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00484
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00488
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0048c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00494
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00498
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0049c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7fc00001,                                                  // signaling NaN                               /// 004e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00508
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00510
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00514
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00518
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00524
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00528
			 0x0e000001,                                                  // Trailing 1s:                                /// 0052c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00534
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00538
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00540
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00550
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00554
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00558
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0055c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00560
			 0xbf028f5c,                                                  // -0.51                                       /// 00564
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00568
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00570
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00574
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00578
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0057c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00580
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00584
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00588
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0058c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00590
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0059c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80000000,                                                  // -zero                                       /// 005e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00600
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00604
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0060c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00610
			 0xbf028f5c,                                                  // -0.51                                       /// 00614
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00618
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00620
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00624
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00628
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80000000,                                                  // -zero                                       /// 00630
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00634
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00644
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00658
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0065c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00660
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00664
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00668
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0066c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00674
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00678
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0067c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00680
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00684
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0068c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00694
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00698
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0069c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e4
			 0x55555555,                                                  // 4 random values                             /// 006e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00700
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00710
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00718
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0071c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00720
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00724
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00728
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00730
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00734
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00740
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00744
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00748
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0074c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00750
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00758
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0075c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00764
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00770
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00774
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00778
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00784
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0078c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00790
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00798
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007a4
			 0x7f800000,                                                  // inf                                         /// 007a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0xbf028f5c,                                                  // -0.51                                       /// 007d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007e4
			 0x7fc00001,                                                  // signaling NaN                               /// 007e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00800
			 0x00000000,                                                  // zero                                        /// 00804
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00808
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0080c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00810
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00814
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0081c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00820
			 0x55555555,                                                  // 4 random values                             /// 00824
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00828
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00830
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00848
			 0x80011111,                                                  // -9.7958E-41                                 /// 0084c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00850
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00854
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0085c
			 0x80000000,                                                  // -zero                                       /// 00860
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00864
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00868
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00870
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00878
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0087c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00880
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00884
			 0x7f800000,                                                  // inf                                         /// 00888
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00890
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00894
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00898
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 008c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00904
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00908
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00914
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00918
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00928
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00938
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0093c
			 0x55555555,                                                  // 4 random values                             /// 00940
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00944
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00948
			 0x00011111,                                                  // 9.7958E-41                                  /// 0094c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00950
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00954
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00958
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0095c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00960
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00964
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00968
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0096c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00970
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00974
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00980
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00984
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00988
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0098c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00990
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0099c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c0
			 0xbf028f5c,                                                  // -0.51                                       /// 009c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a54
			 0xbf028f5c,                                                  // -0.51                                       /// 00a58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a90
			 0x80000000,                                                  // -zero                                       /// 00a94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aa0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ab4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ab8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00abc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ac0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ac4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac8
			 0x3f028f5c,                                                  // 0.51                                        /// 00acc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ad0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00adc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ae0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ae4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0xff800000,                                                  // -inf                                        /// 00aec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00af0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00af4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00af8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b08
			 0x33333333,                                                  // 4 random values                             /// 00b0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b24
			 0x55555555,                                                  // 4 random values                             /// 00b28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00ba0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ba4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ba8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bb0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bb4
			 0x7f800000,                                                  // inf                                         /// 00bb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bbc
			 0xffc00001,                                                  // -signaling NaN                              /// 00bc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00be0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bfc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c10
			 0xbf028f5c,                                                  // -0.51                                       /// 00c14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c40
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ca0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ccc
			 0x55555555,                                                  // 4 random values                             /// 00cd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x55555555,                                                  // 4 random values                             /// 00ce8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cf0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cf4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cf8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cfc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xffc00001,                                                  // -signaling NaN                              /// 00d14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x3f028f5c,                                                  // 0.51                                        /// 00d4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d90
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00da8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0xcb000000,                                                  // -8388608.0                                  /// 00db4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dc4
			 0xffc00001,                                                  // -signaling NaN                              /// 00dc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00de8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df0
			 0x80000000,                                                  // -zero                                       /// 00df4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00df8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ea0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ea4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eb8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ec0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ed0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ee4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ef0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80000000,                                                  // -zero                                       /// 00f20
			 0xcb000000,                                                  // -8388608.0                                  /// 00f24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x55555555,                                                  // 4 random values                             /// 00f94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fa0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fa4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fe0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ff0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff8
			 0x80000002 // min subnorm + 1 ulp                           // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x36f97cd7, /// 0x0
			 0x856c12f4, /// 0x4
			 0x43d876f7, /// 0x8
			 0xd8aa8fad, /// 0xc
			 0xa028fa33, /// 0x10
			 0x23c78548, /// 0x14
			 0xbf43338b, /// 0x18
			 0xc5e3e751, /// 0x1c
			 0xaa1d8d47, /// 0x20
			 0x0e73ee89, /// 0x24
			 0x00102323, /// 0x28
			 0xbcda5ff4, /// 0x2c
			 0x70ed77b7, /// 0x30
			 0xa51200cb, /// 0x34
			 0x9d4f0ac1, /// 0x38
			 0xac725d9c, /// 0x3c
			 0xb4515421, /// 0x40
			 0x96c70888, /// 0x44
			 0x4a202466, /// 0x48
			 0xe93e6c44, /// 0x4c
			 0x748e3c38, /// 0x50
			 0x02e756f8, /// 0x54
			 0xf871bad5, /// 0x58
			 0x71d94387, /// 0x5c
			 0xa9b7ae17, /// 0x60
			 0x202560d1, /// 0x64
			 0xf7e568fa, /// 0x68
			 0x00884f62, /// 0x6c
			 0xb87345c5, /// 0x70
			 0x14cacf2b, /// 0x74
			 0xfc65d5d1, /// 0x78
			 0xdaa3c33b, /// 0x7c
			 0x872e1224, /// 0x80
			 0xa0cf3802, /// 0x84
			 0xcb537578, /// 0x88
			 0xcc50b030, /// 0x8c
			 0xeda5a292, /// 0x90
			 0x4148c980, /// 0x94
			 0x3f1339f7, /// 0x98
			 0x72b8e07a, /// 0x9c
			 0xd0632c33, /// 0xa0
			 0xf2da6572, /// 0xa4
			 0xfc50d921, /// 0xa8
			 0x4a0c0114, /// 0xac
			 0xc4dcd78f, /// 0xb0
			 0x096a904d, /// 0xb4
			 0x03c7a7b5, /// 0xb8
			 0x4b71a314, /// 0xbc
			 0x7171c385, /// 0xc0
			 0xa5c1c6a4, /// 0xc4
			 0x35d0d915, /// 0xc8
			 0x594de5a4, /// 0xcc
			 0x0e49feb6, /// 0xd0
			 0x0669f762, /// 0xd4
			 0x61ababad, /// 0xd8
			 0xd58decd8, /// 0xdc
			 0xac2b5302, /// 0xe0
			 0xd580800b, /// 0xe4
			 0x2de9bbb9, /// 0xe8
			 0x0cbec663, /// 0xec
			 0x4830fd2e, /// 0xf0
			 0x87572bcc, /// 0xf4
			 0x8e80dcd2, /// 0xf8
			 0x132c1b73, /// 0xfc
			 0x38e063ac, /// 0x100
			 0x56405279, /// 0x104
			 0x2280af12, /// 0x108
			 0x9886ba63, /// 0x10c
			 0x259bb0c7, /// 0x110
			 0x0850866a, /// 0x114
			 0x06b244ac, /// 0x118
			 0x9e2c9c42, /// 0x11c
			 0x980bf9f6, /// 0x120
			 0xc6fa3e62, /// 0x124
			 0xc1ab9ff9, /// 0x128
			 0xdc899750, /// 0x12c
			 0x18c1b1fa, /// 0x130
			 0x7d049871, /// 0x134
			 0x699e7773, /// 0x138
			 0x59276104, /// 0x13c
			 0xe0762b1e, /// 0x140
			 0x645c37ca, /// 0x144
			 0xe7d0adc0, /// 0x148
			 0x96a93ab1, /// 0x14c
			 0x63489834, /// 0x150
			 0x5ae7be57, /// 0x154
			 0x3e7fa836, /// 0x158
			 0xa2c140f6, /// 0x15c
			 0x6be20987, /// 0x160
			 0x407f0c4a, /// 0x164
			 0xbcf10b58, /// 0x168
			 0xc891291d, /// 0x16c
			 0x6738d525, /// 0x170
			 0x78c84ff1, /// 0x174
			 0x5471883e, /// 0x178
			 0xd9247752, /// 0x17c
			 0x77288388, /// 0x180
			 0xca19aa22, /// 0x184
			 0xa0a3ec2f, /// 0x188
			 0xb335d8c3, /// 0x18c
			 0x5d1fe7f6, /// 0x190
			 0xb2a449a7, /// 0x194
			 0xe1160277, /// 0x198
			 0x60686cfa, /// 0x19c
			 0xcc711e26, /// 0x1a0
			 0x1a1cc8e8, /// 0x1a4
			 0x2fc6afc4, /// 0x1a8
			 0x4cf04c8c, /// 0x1ac
			 0x0d974ae8, /// 0x1b0
			 0xb2de7628, /// 0x1b4
			 0x9802305f, /// 0x1b8
			 0x29258569, /// 0x1bc
			 0xeab8f658, /// 0x1c0
			 0xe935fd40, /// 0x1c4
			 0xe6a5023f, /// 0x1c8
			 0x7ca044c3, /// 0x1cc
			 0xd5fc63c1, /// 0x1d0
			 0x943121a0, /// 0x1d4
			 0x929b739b, /// 0x1d8
			 0x8420958b, /// 0x1dc
			 0x61dc030d, /// 0x1e0
			 0xff4f823c, /// 0x1e4
			 0x70fade1e, /// 0x1e8
			 0x9e8fd652, /// 0x1ec
			 0x9929c1b8, /// 0x1f0
			 0xf3b7b6c3, /// 0x1f4
			 0xca6e95e4, /// 0x1f8
			 0x878d589a, /// 0x1fc
			 0x08e0133d, /// 0x200
			 0xf9ec54e9, /// 0x204
			 0x6838fee5, /// 0x208
			 0x653052ec, /// 0x20c
			 0x72947fa8, /// 0x210
			 0xe6572036, /// 0x214
			 0x7836ade5, /// 0x218
			 0x4680de76, /// 0x21c
			 0xbcd96f59, /// 0x220
			 0x147f9857, /// 0x224
			 0x83954e08, /// 0x228
			 0x98863a69, /// 0x22c
			 0x1717969f, /// 0x230
			 0xae296680, /// 0x234
			 0x6f944112, /// 0x238
			 0x9c1c0b3c, /// 0x23c
			 0x527cbb31, /// 0x240
			 0xccf45dd4, /// 0x244
			 0xad26bbd8, /// 0x248
			 0x1c699d6b, /// 0x24c
			 0xaf6ca149, /// 0x250
			 0xf553159f, /// 0x254
			 0x161c6d75, /// 0x258
			 0xc3308646, /// 0x25c
			 0xeb3e24ce, /// 0x260
			 0xf1ab1357, /// 0x264
			 0x09134375, /// 0x268
			 0x95403c07, /// 0x26c
			 0x434c5172, /// 0x270
			 0xd5b902de, /// 0x274
			 0xa3edef05, /// 0x278
			 0x81c387bb, /// 0x27c
			 0xb4bbccfa, /// 0x280
			 0x7067afca, /// 0x284
			 0xde6dafda, /// 0x288
			 0x09a2ea5d, /// 0x28c
			 0x7c28bd91, /// 0x290
			 0x10d3a603, /// 0x294
			 0xbf6d9869, /// 0x298
			 0x57950f6e, /// 0x29c
			 0xcccf475a, /// 0x2a0
			 0x33848de1, /// 0x2a4
			 0x505dba19, /// 0x2a8
			 0x1a0ab314, /// 0x2ac
			 0x0761f0cd, /// 0x2b0
			 0xa5488954, /// 0x2b4
			 0xa923cbf7, /// 0x2b8
			 0x889cd97d, /// 0x2bc
			 0x4178976e, /// 0x2c0
			 0xe9facbc9, /// 0x2c4
			 0x2d49d63b, /// 0x2c8
			 0x8a7284c6, /// 0x2cc
			 0x466b5b55, /// 0x2d0
			 0x33e86b1c, /// 0x2d4
			 0xfc8ea7b8, /// 0x2d8
			 0xa8bd6ef0, /// 0x2dc
			 0x74388df5, /// 0x2e0
			 0x6b1f6ed2, /// 0x2e4
			 0x1e6e6671, /// 0x2e8
			 0xcfb95a38, /// 0x2ec
			 0x8544a55b, /// 0x2f0
			 0x18279432, /// 0x2f4
			 0xeab3178b, /// 0x2f8
			 0x458922d6, /// 0x2fc
			 0x066dcccc, /// 0x300
			 0x41e43a2a, /// 0x304
			 0x40ac1b73, /// 0x308
			 0x4c0ed9e9, /// 0x30c
			 0xaeb2138c, /// 0x310
			 0x9009c733, /// 0x314
			 0xdef42659, /// 0x318
			 0x0bcbfe3c, /// 0x31c
			 0x605e6234, /// 0x320
			 0x69043829, /// 0x324
			 0xf19f0e14, /// 0x328
			 0x51deb9ba, /// 0x32c
			 0xc4325105, /// 0x330
			 0x07c0ca66, /// 0x334
			 0x54406dfb, /// 0x338
			 0x41b8bc55, /// 0x33c
			 0x423df771, /// 0x340
			 0xb33dc212, /// 0x344
			 0xde70ddbe, /// 0x348
			 0x75a8da4b, /// 0x34c
			 0x0d19eef8, /// 0x350
			 0x6a545143, /// 0x354
			 0x07ac7c69, /// 0x358
			 0x2e63529c, /// 0x35c
			 0x4dc45c3c, /// 0x360
			 0x944ff111, /// 0x364
			 0xf704b9f1, /// 0x368
			 0x7e2dea0f, /// 0x36c
			 0x111063ac, /// 0x370
			 0x9afed881, /// 0x374
			 0x10357595, /// 0x378
			 0x8404d35f, /// 0x37c
			 0xa838ab53, /// 0x380
			 0xc9bb7bdf, /// 0x384
			 0x118b92c1, /// 0x388
			 0x1d9d65d3, /// 0x38c
			 0x819236f2, /// 0x390
			 0xf62270c8, /// 0x394
			 0x5a7607ab, /// 0x398
			 0x8a254136, /// 0x39c
			 0xe18e1ff5, /// 0x3a0
			 0x28a81626, /// 0x3a4
			 0x266c6bb0, /// 0x3a8
			 0x393b8483, /// 0x3ac
			 0x50b37c42, /// 0x3b0
			 0xe2d85984, /// 0x3b4
			 0x1bbfadf1, /// 0x3b8
			 0xef643696, /// 0x3bc
			 0x4c6f576f, /// 0x3c0
			 0x2b5d8f8d, /// 0x3c4
			 0xe7edf001, /// 0x3c8
			 0xa46580f4, /// 0x3cc
			 0xc5e46bd3, /// 0x3d0
			 0xcf972183, /// 0x3d4
			 0x43de6603, /// 0x3d8
			 0xde4ba17a, /// 0x3dc
			 0xeb99648e, /// 0x3e0
			 0x45c04e28, /// 0x3e4
			 0x6ea872d5, /// 0x3e8
			 0xb089dfe9, /// 0x3ec
			 0xa85d633f, /// 0x3f0
			 0x732c803e, /// 0x3f4
			 0x993d29bb, /// 0x3f8
			 0x631de67a, /// 0x3fc
			 0x5cb95f37, /// 0x400
			 0x9dd675db, /// 0x404
			 0x0521617c, /// 0x408
			 0xc404a9d8, /// 0x40c
			 0xfabfd71b, /// 0x410
			 0x88783755, /// 0x414
			 0x4eb2cace, /// 0x418
			 0x3f677bac, /// 0x41c
			 0xce4eba66, /// 0x420
			 0xd00bc826, /// 0x424
			 0xa301fd5b, /// 0x428
			 0xf6da46e7, /// 0x42c
			 0x699dc0da, /// 0x430
			 0x615938df, /// 0x434
			 0x6b97132f, /// 0x438
			 0x00b91528, /// 0x43c
			 0xccd2d102, /// 0x440
			 0x86fcfbdd, /// 0x444
			 0x63643bf4, /// 0x448
			 0x43ca0ef4, /// 0x44c
			 0xc22841e3, /// 0x450
			 0xd71025df, /// 0x454
			 0x1b22d92a, /// 0x458
			 0x34791c23, /// 0x45c
			 0xc2b42302, /// 0x460
			 0x9c9b5899, /// 0x464
			 0x079ec11a, /// 0x468
			 0x17d804f7, /// 0x46c
			 0x9aa32ee5, /// 0x470
			 0x0ad57267, /// 0x474
			 0x0fb83879, /// 0x478
			 0x6d66573a, /// 0x47c
			 0x274fde82, /// 0x480
			 0x664caf9f, /// 0x484
			 0x409fc415, /// 0x488
			 0x71aa981d, /// 0x48c
			 0x61bdfdb5, /// 0x490
			 0x2ec4119f, /// 0x494
			 0x6b411384, /// 0x498
			 0x388d9626, /// 0x49c
			 0xa1a5dd5f, /// 0x4a0
			 0x77808131, /// 0x4a4
			 0x65777597, /// 0x4a8
			 0xa54df230, /// 0x4ac
			 0xade8bac5, /// 0x4b0
			 0x93d40947, /// 0x4b4
			 0xb21bf464, /// 0x4b8
			 0x7f64cdc8, /// 0x4bc
			 0xc98ff993, /// 0x4c0
			 0xa25227a3, /// 0x4c4
			 0x3ad5300a, /// 0x4c8
			 0x2741e0a0, /// 0x4cc
			 0x23aeabef, /// 0x4d0
			 0x5e1b1731, /// 0x4d4
			 0xff66ae1a, /// 0x4d8
			 0x17978c72, /// 0x4dc
			 0x4cc149d2, /// 0x4e0
			 0x10361b3c, /// 0x4e4
			 0xafd9d0d6, /// 0x4e8
			 0x46c305bd, /// 0x4ec
			 0xb9e2262f, /// 0x4f0
			 0xdad32879, /// 0x4f4
			 0x4a411ec6, /// 0x4f8
			 0xecb7a4f5, /// 0x4fc
			 0xf1d7c133, /// 0x500
			 0xa3317972, /// 0x504
			 0x369748f4, /// 0x508
			 0x504bc13f, /// 0x50c
			 0x10a6b238, /// 0x510
			 0xf70b83e0, /// 0x514
			 0x2ab8320b, /// 0x518
			 0x07e6a431, /// 0x51c
			 0x16c8e2e6, /// 0x520
			 0x2b5783d2, /// 0x524
			 0xd6e5bafc, /// 0x528
			 0xfdcb97a1, /// 0x52c
			 0xdeb6fefb, /// 0x530
			 0x46ddac38, /// 0x534
			 0xf46458be, /// 0x538
			 0x08caafad, /// 0x53c
			 0x796c83e7, /// 0x540
			 0x95e42408, /// 0x544
			 0x87c894a8, /// 0x548
			 0xfea4cfbf, /// 0x54c
			 0x9c65bf04, /// 0x550
			 0x418f3c9c, /// 0x554
			 0xf1b5d656, /// 0x558
			 0x475c926c, /// 0x55c
			 0x0a4e2b33, /// 0x560
			 0x6ea6a437, /// 0x564
			 0x4586aaac, /// 0x568
			 0xf4d9a044, /// 0x56c
			 0xadb92e50, /// 0x570
			 0xca97af6e, /// 0x574
			 0xa8991464, /// 0x578
			 0xf8ee2eb2, /// 0x57c
			 0xdb0979a6, /// 0x580
			 0x092ca3b1, /// 0x584
			 0xab7d4a08, /// 0x588
			 0x5fff488a, /// 0x58c
			 0x8453c345, /// 0x590
			 0xaaa8fe82, /// 0x594
			 0x9a41db1f, /// 0x598
			 0xc116b6d5, /// 0x59c
			 0x8aff72e8, /// 0x5a0
			 0x31237db8, /// 0x5a4
			 0x178ce41d, /// 0x5a8
			 0x0f0f37f1, /// 0x5ac
			 0x63d38932, /// 0x5b0
			 0x7fb65613, /// 0x5b4
			 0x766f701d, /// 0x5b8
			 0x59271086, /// 0x5bc
			 0xc29fc7ba, /// 0x5c0
			 0x9b6f599b, /// 0x5c4
			 0xf0a57722, /// 0x5c8
			 0xd6d52022, /// 0x5cc
			 0x038e4c2f, /// 0x5d0
			 0x879e7d4a, /// 0x5d4
			 0x1e7fdb57, /// 0x5d8
			 0x46593fc6, /// 0x5dc
			 0x8de284af, /// 0x5e0
			 0x89293d05, /// 0x5e4
			 0x65562093, /// 0x5e8
			 0xe9e2af28, /// 0x5ec
			 0x7b4656f0, /// 0x5f0
			 0x755b9e63, /// 0x5f4
			 0xdf9e54ae, /// 0x5f8
			 0x7a5d4a7e, /// 0x5fc
			 0x21db3bec, /// 0x600
			 0xe5a94e44, /// 0x604
			 0x27662e65, /// 0x608
			 0xf206057f, /// 0x60c
			 0x2a97bb67, /// 0x610
			 0xde1f5249, /// 0x614
			 0xa5bb4add, /// 0x618
			 0x1759ae93, /// 0x61c
			 0xc85ffeaa, /// 0x620
			 0xe5de8d13, /// 0x624
			 0xfc496c4b, /// 0x628
			 0x9249ce6f, /// 0x62c
			 0x12f89d24, /// 0x630
			 0xd201cd67, /// 0x634
			 0x62aae332, /// 0x638
			 0xa3dee5fb, /// 0x63c
			 0x566cdd35, /// 0x640
			 0xe29b3a17, /// 0x644
			 0x6d07a205, /// 0x648
			 0x66b7d592, /// 0x64c
			 0x0550e3b0, /// 0x650
			 0x65cdd935, /// 0x654
			 0x0eb45476, /// 0x658
			 0x0b6b91b5, /// 0x65c
			 0x7c722a92, /// 0x660
			 0xcf0a06a1, /// 0x664
			 0xc0ff2115, /// 0x668
			 0x896d435e, /// 0x66c
			 0xcdd50725, /// 0x670
			 0x2d2a5e8f, /// 0x674
			 0x5971aae7, /// 0x678
			 0x9c78e983, /// 0x67c
			 0x4ed0d131, /// 0x680
			 0xfeda6ef5, /// 0x684
			 0x3867d8b9, /// 0x688
			 0x50d2e405, /// 0x68c
			 0xd41cf32c, /// 0x690
			 0x4fedf50c, /// 0x694
			 0x40b513cc, /// 0x698
			 0x4167c6c9, /// 0x69c
			 0xc83fdfe7, /// 0x6a0
			 0x91bca3e0, /// 0x6a4
			 0x01b4042e, /// 0x6a8
			 0x8e44ae97, /// 0x6ac
			 0x60e9a593, /// 0x6b0
			 0x1e2de0ef, /// 0x6b4
			 0x580a8279, /// 0x6b8
			 0xc4fb01be, /// 0x6bc
			 0x3621ce73, /// 0x6c0
			 0xddb43cc2, /// 0x6c4
			 0x171652bf, /// 0x6c8
			 0x28012640, /// 0x6cc
			 0xe321f194, /// 0x6d0
			 0xcbf3fd68, /// 0x6d4
			 0xca84eec4, /// 0x6d8
			 0x75de6f48, /// 0x6dc
			 0x046cd4b6, /// 0x6e0
			 0x7bcfc1d5, /// 0x6e4
			 0x1275c7d6, /// 0x6e8
			 0xe6795ddf, /// 0x6ec
			 0x0b0b7d19, /// 0x6f0
			 0xf84d3f23, /// 0x6f4
			 0xf7207ce1, /// 0x6f8
			 0xe7d03e3e, /// 0x6fc
			 0xda29f646, /// 0x700
			 0xde90328a, /// 0x704
			 0x9a1f6425, /// 0x708
			 0x48b35813, /// 0x70c
			 0x1fc3e46c, /// 0x710
			 0x803dcebd, /// 0x714
			 0xad16ae83, /// 0x718
			 0x5befe40a, /// 0x71c
			 0xbc4de8ec, /// 0x720
			 0xe2cd68a6, /// 0x724
			 0x3f3e9551, /// 0x728
			 0x998330eb, /// 0x72c
			 0x534235d5, /// 0x730
			 0x4d7fb906, /// 0x734
			 0xe524d4f6, /// 0x738
			 0x8a9c9918, /// 0x73c
			 0x9e1e2979, /// 0x740
			 0xff2c0a60, /// 0x744
			 0x9f6816e8, /// 0x748
			 0x70c47e77, /// 0x74c
			 0xf98c51b3, /// 0x750
			 0xcfc10a0f, /// 0x754
			 0xf2bf5eca, /// 0x758
			 0x6ac973f5, /// 0x75c
			 0xc989a70e, /// 0x760
			 0xa2fff9c2, /// 0x764
			 0xd6ee61fb, /// 0x768
			 0x326e7849, /// 0x76c
			 0x6f1e6ae3, /// 0x770
			 0xdea33f63, /// 0x774
			 0x66dd5eed, /// 0x778
			 0x008a2f07, /// 0x77c
			 0x388355ad, /// 0x780
			 0xb77d76fd, /// 0x784
			 0x5cac6120, /// 0x788
			 0x26407298, /// 0x78c
			 0x5d5f4379, /// 0x790
			 0x98b3c78f, /// 0x794
			 0xfa08ea1a, /// 0x798
			 0x488ff3ea, /// 0x79c
			 0x31ac2cbe, /// 0x7a0
			 0x832ab484, /// 0x7a4
			 0x11c855da, /// 0x7a8
			 0xcc3eb741, /// 0x7ac
			 0xcc9c31d9, /// 0x7b0
			 0x9d765574, /// 0x7b4
			 0xf6980829, /// 0x7b8
			 0x0dd57f29, /// 0x7bc
			 0xae415f2f, /// 0x7c0
			 0x8d811910, /// 0x7c4
			 0x7c242019, /// 0x7c8
			 0xb0fe41f2, /// 0x7cc
			 0x15b51f68, /// 0x7d0
			 0x98cf0668, /// 0x7d4
			 0xa45c7cba, /// 0x7d8
			 0xf92b0166, /// 0x7dc
			 0x4d0ff7ab, /// 0x7e0
			 0x91935630, /// 0x7e4
			 0xa2b0c8b9, /// 0x7e8
			 0x5bd5dd1b, /// 0x7ec
			 0x1797db1a, /// 0x7f0
			 0xd92eef13, /// 0x7f4
			 0x23c38274, /// 0x7f8
			 0xba5655ff, /// 0x7fc
			 0x0f3c6b57, /// 0x800
			 0xf1345923, /// 0x804
			 0xa996de72, /// 0x808
			 0x9b70e4d4, /// 0x80c
			 0x31711b7e, /// 0x810
			 0xf58a5a03, /// 0x814
			 0x8d56582b, /// 0x818
			 0x9e9172f1, /// 0x81c
			 0x9774c8ab, /// 0x820
			 0xd6cac74e, /// 0x824
			 0x74b5e67b, /// 0x828
			 0x527a6351, /// 0x82c
			 0x8ba9db16, /// 0x830
			 0xbcc505c8, /// 0x834
			 0xabb21509, /// 0x838
			 0xd6a9ac1e, /// 0x83c
			 0x9da2a33d, /// 0x840
			 0x737c19b7, /// 0x844
			 0x80242ac6, /// 0x848
			 0x91ce729e, /// 0x84c
			 0xc9e22e29, /// 0x850
			 0x4c844a20, /// 0x854
			 0x8de1b5e4, /// 0x858
			 0x6941a6cd, /// 0x85c
			 0x95c590f7, /// 0x860
			 0xe59b6b24, /// 0x864
			 0x99a93201, /// 0x868
			 0x380b54d1, /// 0x86c
			 0x4052107f, /// 0x870
			 0x749d85d6, /// 0x874
			 0x9698a740, /// 0x878
			 0xd8209aad, /// 0x87c
			 0x41cd3e27, /// 0x880
			 0x6d03721f, /// 0x884
			 0x208e25dc, /// 0x888
			 0xa1077b68, /// 0x88c
			 0xd583dd33, /// 0x890
			 0xea311758, /// 0x894
			 0x12f99c8f, /// 0x898
			 0x82229a39, /// 0x89c
			 0x8d4db21d, /// 0x8a0
			 0xed6bf5da, /// 0x8a4
			 0x9a66b710, /// 0x8a8
			 0x89bd2ef0, /// 0x8ac
			 0xecdd0810, /// 0x8b0
			 0x226833e3, /// 0x8b4
			 0x5c2a8771, /// 0x8b8
			 0x81f4e5f1, /// 0x8bc
			 0x01e7e682, /// 0x8c0
			 0x9d99eec7, /// 0x8c4
			 0x23a95cfa, /// 0x8c8
			 0xc34f6cfb, /// 0x8cc
			 0xb9866969, /// 0x8d0
			 0x88309a59, /// 0x8d4
			 0x9311a87c, /// 0x8d8
			 0x132f5736, /// 0x8dc
			 0x0168ef25, /// 0x8e0
			 0xe51495cd, /// 0x8e4
			 0x8ca136af, /// 0x8e8
			 0xed24a388, /// 0x8ec
			 0x36faffef, /// 0x8f0
			 0xdbaab7d6, /// 0x8f4
			 0x0ce1fd36, /// 0x8f8
			 0xd96aee99, /// 0x8fc
			 0x74691432, /// 0x900
			 0x12382725, /// 0x904
			 0x82043456, /// 0x908
			 0x73453554, /// 0x90c
			 0x25bdcf63, /// 0x910
			 0x016ee943, /// 0x914
			 0xeb1a92c6, /// 0x918
			 0xcec67cc0, /// 0x91c
			 0x7a53b406, /// 0x920
			 0x798a7cba, /// 0x924
			 0xd776d779, /// 0x928
			 0xa6640ae9, /// 0x92c
			 0x3ca5ede5, /// 0x930
			 0x773a4274, /// 0x934
			 0xe00ebab5, /// 0x938
			 0xccc7e4f0, /// 0x93c
			 0x501b8b7b, /// 0x940
			 0xec87a757, /// 0x944
			 0x2b4387b4, /// 0x948
			 0x9d9dab5d, /// 0x94c
			 0x69ff54b1, /// 0x950
			 0x6793fdd3, /// 0x954
			 0x49efa08a, /// 0x958
			 0x40c460af, /// 0x95c
			 0x5438b05d, /// 0x960
			 0x31b6d0ad, /// 0x964
			 0x2d54d28e, /// 0x968
			 0xb543527f, /// 0x96c
			 0x6d4777a2, /// 0x970
			 0xe854356d, /// 0x974
			 0x01f087e8, /// 0x978
			 0x045e2334, /// 0x97c
			 0x1548cb83, /// 0x980
			 0xef06dee5, /// 0x984
			 0xc6d4104b, /// 0x988
			 0xc4e99901, /// 0x98c
			 0x1e2c0d3f, /// 0x990
			 0x6a5a42f1, /// 0x994
			 0x57963718, /// 0x998
			 0x39f7e343, /// 0x99c
			 0x25bb0324, /// 0x9a0
			 0xf571dede, /// 0x9a4
			 0x29d98538, /// 0x9a8
			 0x7ba3da74, /// 0x9ac
			 0x42461db0, /// 0x9b0
			 0x94523dcb, /// 0x9b4
			 0x8bc2bdc9, /// 0x9b8
			 0xf8e1a157, /// 0x9bc
			 0xd1a330f5, /// 0x9c0
			 0x482bf1b0, /// 0x9c4
			 0x92a219a5, /// 0x9c8
			 0x8ccf8333, /// 0x9cc
			 0xf44a9210, /// 0x9d0
			 0x563513bc, /// 0x9d4
			 0x1738c6d7, /// 0x9d8
			 0x737b5e39, /// 0x9dc
			 0xab5f5193, /// 0x9e0
			 0x76b6e5b6, /// 0x9e4
			 0x08c7f671, /// 0x9e8
			 0x3958465c, /// 0x9ec
			 0xf586211f, /// 0x9f0
			 0x0e522f64, /// 0x9f4
			 0xdb11b9c1, /// 0x9f8
			 0x1b873bf3, /// 0x9fc
			 0x1ef02e00, /// 0xa00
			 0x3e2285bb, /// 0xa04
			 0xbd2a31ea, /// 0xa08
			 0x2a081c67, /// 0xa0c
			 0xeaebc70e, /// 0xa10
			 0x77f206a2, /// 0xa14
			 0x5fa0f23b, /// 0xa18
			 0x9eebb025, /// 0xa1c
			 0x6dc24cf0, /// 0xa20
			 0x4d157ec9, /// 0xa24
			 0xa86d1ee4, /// 0xa28
			 0x8851a800, /// 0xa2c
			 0xd9644f4d, /// 0xa30
			 0x63a13188, /// 0xa34
			 0x45ca4bb4, /// 0xa38
			 0x30ac36aa, /// 0xa3c
			 0x29088b0d, /// 0xa40
			 0xa84157d1, /// 0xa44
			 0xa25c0e23, /// 0xa48
			 0xb124df5c, /// 0xa4c
			 0xe5c50d2f, /// 0xa50
			 0x721900dc, /// 0xa54
			 0xe172741f, /// 0xa58
			 0x972720ba, /// 0xa5c
			 0xaa90e9f8, /// 0xa60
			 0xf586fa80, /// 0xa64
			 0x75cfd2df, /// 0xa68
			 0x3071ee2e, /// 0xa6c
			 0xe27a011b, /// 0xa70
			 0xde00fce2, /// 0xa74
			 0x758d7bc1, /// 0xa78
			 0x822b16f4, /// 0xa7c
			 0x30da68e8, /// 0xa80
			 0xc524493f, /// 0xa84
			 0x00c90c1e, /// 0xa88
			 0xb0c24a30, /// 0xa8c
			 0xe6d69102, /// 0xa90
			 0xc82e9a0c, /// 0xa94
			 0x26f63308, /// 0xa98
			 0xccd79f97, /// 0xa9c
			 0x35cbb75e, /// 0xaa0
			 0x6c870281, /// 0xaa4
			 0x90324bf6, /// 0xaa8
			 0x91c4db81, /// 0xaac
			 0x84f829e0, /// 0xab0
			 0x57d429f2, /// 0xab4
			 0xa446471d, /// 0xab8
			 0x41434dc0, /// 0xabc
			 0x14da1a6f, /// 0xac0
			 0x6cae5393, /// 0xac4
			 0x38321d09, /// 0xac8
			 0x80848a66, /// 0xacc
			 0x8d7f6e68, /// 0xad0
			 0x5b7bea19, /// 0xad4
			 0x34a0668e, /// 0xad8
			 0x0e523852, /// 0xadc
			 0xfc0ba555, /// 0xae0
			 0x084edb52, /// 0xae4
			 0x0b4ad7cc, /// 0xae8
			 0x0654c5c9, /// 0xaec
			 0x7346fe0d, /// 0xaf0
			 0xa8febbf9, /// 0xaf4
			 0xb1e849ab, /// 0xaf8
			 0x5bc59608, /// 0xafc
			 0x6c18ed59, /// 0xb00
			 0x5fffa0ad, /// 0xb04
			 0x393f4b65, /// 0xb08
			 0x896786fd, /// 0xb0c
			 0xdc8a6f3c, /// 0xb10
			 0xa0a534d6, /// 0xb14
			 0xb7ce8686, /// 0xb18
			 0xefc03ae6, /// 0xb1c
			 0x2faa70ea, /// 0xb20
			 0x9d0134f6, /// 0xb24
			 0xc5e9f6ea, /// 0xb28
			 0x077eb808, /// 0xb2c
			 0x1cff4e78, /// 0xb30
			 0x0518ed39, /// 0xb34
			 0xca85464b, /// 0xb38
			 0x3234e5bb, /// 0xb3c
			 0x92838083, /// 0xb40
			 0xcd2d36a8, /// 0xb44
			 0xc44bfc5f, /// 0xb48
			 0xfec61b8e, /// 0xb4c
			 0x88dd148b, /// 0xb50
			 0xc200140e, /// 0xb54
			 0x1a24c030, /// 0xb58
			 0x47787ffa, /// 0xb5c
			 0x2e742b08, /// 0xb60
			 0x7ff5ae9e, /// 0xb64
			 0xe55ff401, /// 0xb68
			 0x66b077fa, /// 0xb6c
			 0x2429d279, /// 0xb70
			 0x5806f576, /// 0xb74
			 0x9facb39b, /// 0xb78
			 0x27daa90f, /// 0xb7c
			 0x0958034b, /// 0xb80
			 0x163d2ce6, /// 0xb84
			 0x405f0b7f, /// 0xb88
			 0xde40711f, /// 0xb8c
			 0xc0dbd362, /// 0xb90
			 0x274fafbb, /// 0xb94
			 0xcfedef33, /// 0xb98
			 0xe1512e73, /// 0xb9c
			 0x8691f1ff, /// 0xba0
			 0x64d4e4ce, /// 0xba4
			 0x0025411c, /// 0xba8
			 0x187c4044, /// 0xbac
			 0xf853e907, /// 0xbb0
			 0x518a0d3e, /// 0xbb4
			 0xebce84c1, /// 0xbb8
			 0x7cc9c838, /// 0xbbc
			 0x91c59127, /// 0xbc0
			 0xd61ddc2f, /// 0xbc4
			 0xcedc1ffd, /// 0xbc8
			 0x54d235ed, /// 0xbcc
			 0xe5bf79ef, /// 0xbd0
			 0x56b1aafa, /// 0xbd4
			 0x7582c2c8, /// 0xbd8
			 0xf62b0170, /// 0xbdc
			 0x423e57a5, /// 0xbe0
			 0xe2c1ca80, /// 0xbe4
			 0xd126319e, /// 0xbe8
			 0x341b39ad, /// 0xbec
			 0x347a0e8e, /// 0xbf0
			 0x708d8a5d, /// 0xbf4
			 0xfcefe325, /// 0xbf8
			 0x196200c8, /// 0xbfc
			 0x055ef58f, /// 0xc00
			 0x9335ad62, /// 0xc04
			 0xc61ff858, /// 0xc08
			 0x0ef55258, /// 0xc0c
			 0x258cc467, /// 0xc10
			 0x48a8b3bc, /// 0xc14
			 0x9410cc72, /// 0xc18
			 0xe453aec6, /// 0xc1c
			 0x2ebb5e48, /// 0xc20
			 0x51c1d8c8, /// 0xc24
			 0xd84fddbd, /// 0xc28
			 0x4c8d0d63, /// 0xc2c
			 0xd5ad9115, /// 0xc30
			 0x2e99c24b, /// 0xc34
			 0x4f1be06d, /// 0xc38
			 0xcec4e314, /// 0xc3c
			 0x0c51a7d0, /// 0xc40
			 0xa8db7abf, /// 0xc44
			 0xe2525cda, /// 0xc48
			 0xdc153c70, /// 0xc4c
			 0xe9dbc32a, /// 0xc50
			 0x755b0b60, /// 0xc54
			 0x3bcfbe5f, /// 0xc58
			 0xb4e52e80, /// 0xc5c
			 0xd3e312db, /// 0xc60
			 0x2442ec02, /// 0xc64
			 0x1c147305, /// 0xc68
			 0x168b0601, /// 0xc6c
			 0x1c2ef1c5, /// 0xc70
			 0x09d04e5f, /// 0xc74
			 0xf0e099a1, /// 0xc78
			 0x33f4d53a, /// 0xc7c
			 0x295c7236, /// 0xc80
			 0x0f198d9c, /// 0xc84
			 0xf848e511, /// 0xc88
			 0x5d6be255, /// 0xc8c
			 0xd3384fb3, /// 0xc90
			 0xc7720582, /// 0xc94
			 0x4176714b, /// 0xc98
			 0x0e529769, /// 0xc9c
			 0x40700e05, /// 0xca0
			 0xb1147c68, /// 0xca4
			 0x7be685b7, /// 0xca8
			 0x627a999d, /// 0xcac
			 0x0b68c5a3, /// 0xcb0
			 0x1d307307, /// 0xcb4
			 0x2af58f3a, /// 0xcb8
			 0xe4f91ee4, /// 0xcbc
			 0xcd257fd2, /// 0xcc0
			 0x458ac3bb, /// 0xcc4
			 0x30faf070, /// 0xcc8
			 0x0b15a626, /// 0xccc
			 0xde6e9729, /// 0xcd0
			 0xd2a8499d, /// 0xcd4
			 0x7d3c27e7, /// 0xcd8
			 0x5c19f869, /// 0xcdc
			 0xb281291b, /// 0xce0
			 0x68f04be0, /// 0xce4
			 0x12a7fcb0, /// 0xce8
			 0x35c6df21, /// 0xcec
			 0x1f67d8f6, /// 0xcf0
			 0x4c75d96e, /// 0xcf4
			 0xfbcabd3a, /// 0xcf8
			 0x117ed736, /// 0xcfc
			 0x167694bb, /// 0xd00
			 0x2ee1f927, /// 0xd04
			 0x78ef91e6, /// 0xd08
			 0xf785e6d7, /// 0xd0c
			 0x456ae4fc, /// 0xd10
			 0xd769c348, /// 0xd14
			 0xcc0d1864, /// 0xd18
			 0x45828e74, /// 0xd1c
			 0x6819b8d2, /// 0xd20
			 0xeb3ba033, /// 0xd24
			 0x9d177f40, /// 0xd28
			 0x875d2557, /// 0xd2c
			 0x7264c351, /// 0xd30
			 0xa285833b, /// 0xd34
			 0xd4474e06, /// 0xd38
			 0x85da5f5c, /// 0xd3c
			 0x13ab612a, /// 0xd40
			 0x52cb8730, /// 0xd44
			 0xd660d037, /// 0xd48
			 0x6c8443f2, /// 0xd4c
			 0x89136051, /// 0xd50
			 0x0f792aea, /// 0xd54
			 0x46884b9f, /// 0xd58
			 0xd06e271b, /// 0xd5c
			 0xfd7880a5, /// 0xd60
			 0x8943f519, /// 0xd64
			 0xc4f4f755, /// 0xd68
			 0xfc3bebf1, /// 0xd6c
			 0x85008d17, /// 0xd70
			 0xe5f4229b, /// 0xd74
			 0x32f7a1d9, /// 0xd78
			 0x1a6ceff0, /// 0xd7c
			 0x9cd81540, /// 0xd80
			 0x0a9878cc, /// 0xd84
			 0xfa70df80, /// 0xd88
			 0x566c2364, /// 0xd8c
			 0xc847ccf9, /// 0xd90
			 0x9f776083, /// 0xd94
			 0x6c7a2427, /// 0xd98
			 0xe8c236de, /// 0xd9c
			 0xdefbb032, /// 0xda0
			 0x23e20190, /// 0xda4
			 0xfb7770df, /// 0xda8
			 0x895de647, /// 0xdac
			 0x522b0c53, /// 0xdb0
			 0xcb80aad7, /// 0xdb4
			 0xbeca90b5, /// 0xdb8
			 0x3e86ba8f, /// 0xdbc
			 0x2747f508, /// 0xdc0
			 0x7fd437c4, /// 0xdc4
			 0x3bf0db9a, /// 0xdc8
			 0x9d622807, /// 0xdcc
			 0xff15ecb9, /// 0xdd0
			 0x75f395f9, /// 0xdd4
			 0x13394e0a, /// 0xdd8
			 0x42521f1a, /// 0xddc
			 0xfb87d326, /// 0xde0
			 0xe62355eb, /// 0xde4
			 0xb8606ec4, /// 0xde8
			 0xa7254971, /// 0xdec
			 0xebd2c3e8, /// 0xdf0
			 0x5bea2b5f, /// 0xdf4
			 0xb17da067, /// 0xdf8
			 0xe0e228f3, /// 0xdfc
			 0x74b1dc01, /// 0xe00
			 0x5980c061, /// 0xe04
			 0x97db0747, /// 0xe08
			 0xb2a2744c, /// 0xe0c
			 0x176f40c0, /// 0xe10
			 0x6ecdd4cc, /// 0xe14
			 0x6b8b9b79, /// 0xe18
			 0x966a72fb, /// 0xe1c
			 0x397dd513, /// 0xe20
			 0xb84830f3, /// 0xe24
			 0xfe60b1bb, /// 0xe28
			 0x486609c2, /// 0xe2c
			 0x0712cda5, /// 0xe30
			 0x211c48c3, /// 0xe34
			 0x7e8f9044, /// 0xe38
			 0x2594cdb9, /// 0xe3c
			 0x49c90e3c, /// 0xe40
			 0x1b08f16c, /// 0xe44
			 0x0e838487, /// 0xe48
			 0x642f93f8, /// 0xe4c
			 0xf5959f98, /// 0xe50
			 0xf48652cd, /// 0xe54
			 0xf5f716cf, /// 0xe58
			 0x2da4e4e7, /// 0xe5c
			 0xf694118b, /// 0xe60
			 0x92f33c3d, /// 0xe64
			 0x6857e3f9, /// 0xe68
			 0x34039383, /// 0xe6c
			 0xe1f32e8c, /// 0xe70
			 0x63875ede, /// 0xe74
			 0xe90af5e6, /// 0xe78
			 0x8c3bb10f, /// 0xe7c
			 0xf26b6dcf, /// 0xe80
			 0x6dbfc0aa, /// 0xe84
			 0x456379fc, /// 0xe88
			 0xe445cf3b, /// 0xe8c
			 0x5d17a498, /// 0xe90
			 0x5c21187d, /// 0xe94
			 0x2958de06, /// 0xe98
			 0xf2efacca, /// 0xe9c
			 0xf7912aad, /// 0xea0
			 0x7f217922, /// 0xea4
			 0x0ee2160b, /// 0xea8
			 0x4da427c1, /// 0xeac
			 0x90850f39, /// 0xeb0
			 0xace4a1d8, /// 0xeb4
			 0x129a55c5, /// 0xeb8
			 0xaaa05646, /// 0xebc
			 0x516e9750, /// 0xec0
			 0x9ad0c247, /// 0xec4
			 0xb7515411, /// 0xec8
			 0xd4dfd525, /// 0xecc
			 0x3abf0344, /// 0xed0
			 0xbda679ef, /// 0xed4
			 0xdcbc5b86, /// 0xed8
			 0x1166c6cf, /// 0xedc
			 0x89acf279, /// 0xee0
			 0x8983c712, /// 0xee4
			 0x9fdb1cb2, /// 0xee8
			 0x48468659, /// 0xeec
			 0x9e94ee6c, /// 0xef0
			 0x956602cf, /// 0xef4
			 0xf42b9a3f, /// 0xef8
			 0x042130cf, /// 0xefc
			 0x7bac0269, /// 0xf00
			 0xf6d6e2c9, /// 0xf04
			 0xd5dc7f51, /// 0xf08
			 0xf37ccabf, /// 0xf0c
			 0xd85611b5, /// 0xf10
			 0x4d0a7281, /// 0xf14
			 0x652aeb49, /// 0xf18
			 0x1f79b36f, /// 0xf1c
			 0x95cc6c1d, /// 0xf20
			 0xe7d1d296, /// 0xf24
			 0x35bfbd1f, /// 0xf28
			 0xf40ce74f, /// 0xf2c
			 0x3ef8299e, /// 0xf30
			 0x3b74df45, /// 0xf34
			 0xd5cbffdd, /// 0xf38
			 0xb3289948, /// 0xf3c
			 0xfefa7f43, /// 0xf40
			 0xee3939a2, /// 0xf44
			 0x9d1f25e3, /// 0xf48
			 0x0f10c688, /// 0xf4c
			 0x335a76a4, /// 0xf50
			 0x51461e7c, /// 0xf54
			 0xacc6423f, /// 0xf58
			 0x7b4a5783, /// 0xf5c
			 0x2bb24f2d, /// 0xf60
			 0xa59b0ed1, /// 0xf64
			 0x754bbd32, /// 0xf68
			 0xa49d5582, /// 0xf6c
			 0x1abfb1c6, /// 0xf70
			 0x76e3341a, /// 0xf74
			 0x4ae0f1c7, /// 0xf78
			 0x738569c9, /// 0xf7c
			 0x61260fbd, /// 0xf80
			 0x533061d3, /// 0xf84
			 0x2d7dac90, /// 0xf88
			 0x93f85737, /// 0xf8c
			 0x6299730b, /// 0xf90
			 0xaafa1db8, /// 0xf94
			 0xb3f52752, /// 0xf98
			 0x3e176e38, /// 0xf9c
			 0x0d8564ea, /// 0xfa0
			 0x883af2ae, /// 0xfa4
			 0x2ecd6a47, /// 0xfa8
			 0xb3e9c87b, /// 0xfac
			 0xb758b4f1, /// 0xfb0
			 0xd571458c, /// 0xfb4
			 0x2243db80, /// 0xfb8
			 0x8fb0df49, /// 0xfbc
			 0xecec095b, /// 0xfc0
			 0xa3985b17, /// 0xfc4
			 0xa2777105, /// 0xfc8
			 0xd4631e78, /// 0xfcc
			 0xf4371c8f, /// 0xfd0
			 0x6d61a05d, /// 0xfd4
			 0x68fd2f38, /// 0xfd8
			 0x74f9c4f2, /// 0xfdc
			 0x0e7c5638, /// 0xfe0
			 0x76fbcd36, /// 0xfe4
			 0xb8538746, /// 0xfe8
			 0xaef9c305, /// 0xfec
			 0xb63fe8da, /// 0xff0
			 0x0e7b3dd1, /// 0xff4
			 0x66a35145, /// 0xff8
			 0xa8ac8b7d /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xcea03423, /// 0x0
			 0xf263c969, /// 0x4
			 0x95ef8f71, /// 0x8
			 0x33b76771, /// 0xc
			 0xe24209a3, /// 0x10
			 0x09a75951, /// 0x14
			 0x1587b965, /// 0x18
			 0x1a38f270, /// 0x1c
			 0x751744d2, /// 0x20
			 0x53d357fe, /// 0x24
			 0x7a19eb8f, /// 0x28
			 0xc0b26253, /// 0x2c
			 0xde50fab8, /// 0x30
			 0x6cf63426, /// 0x34
			 0xfedf0718, /// 0x38
			 0x50d1fbb0, /// 0x3c
			 0xbd5e9cbf, /// 0x40
			 0xcac23917, /// 0x44
			 0xad51b710, /// 0x48
			 0x6f8e4952, /// 0x4c
			 0x29ee5d53, /// 0x50
			 0x82ffce57, /// 0x54
			 0x0cb0056c, /// 0x58
			 0x5693b14b, /// 0x5c
			 0x62135176, /// 0x60
			 0xdca24e6f, /// 0x64
			 0xb36c71c1, /// 0x68
			 0x3459d26d, /// 0x6c
			 0xf97956d7, /// 0x70
			 0x977a583d, /// 0x74
			 0x807dd028, /// 0x78
			 0x463e7bdb, /// 0x7c
			 0xfc67f67d, /// 0x80
			 0x4ae3c20f, /// 0x84
			 0x7c9b630c, /// 0x88
			 0x4a478870, /// 0x8c
			 0x604b557d, /// 0x90
			 0xbda16bd5, /// 0x94
			 0xd0a9e8fd, /// 0x98
			 0x18239594, /// 0x9c
			 0x7ad39913, /// 0xa0
			 0xed55f2a0, /// 0xa4
			 0x569bfc47, /// 0xa8
			 0xaf276c36, /// 0xac
			 0x7c4d9aa5, /// 0xb0
			 0xa71040ab, /// 0xb4
			 0x1c587851, /// 0xb8
			 0xd0ff38ae, /// 0xbc
			 0x768fbc47, /// 0xc0
			 0x5424d7b0, /// 0xc4
			 0x18ffc231, /// 0xc8
			 0x258d4954, /// 0xcc
			 0xa767e792, /// 0xd0
			 0x1a2a726c, /// 0xd4
			 0x27fcd21b, /// 0xd8
			 0x2cec01be, /// 0xdc
			 0xa59c8190, /// 0xe0
			 0x75dba10c, /// 0xe4
			 0x0184530c, /// 0xe8
			 0x29c1fcd6, /// 0xec
			 0x48c761bd, /// 0xf0
			 0x601e844a, /// 0xf4
			 0xac68a6f6, /// 0xf8
			 0x6775738a, /// 0xfc
			 0xe5d74174, /// 0x100
			 0x3f26305f, /// 0x104
			 0x65514cd4, /// 0x108
			 0x2f00c768, /// 0x10c
			 0xb7de8f15, /// 0x110
			 0x6b890fcc, /// 0x114
			 0xaedc8996, /// 0x118
			 0x42ad5df0, /// 0x11c
			 0x4e6ed518, /// 0x120
			 0x07f10d12, /// 0x124
			 0xc44ef56f, /// 0x128
			 0x52fb70b0, /// 0x12c
			 0x8641a8de, /// 0x130
			 0xef5c8706, /// 0x134
			 0x47eaf345, /// 0x138
			 0xfb0cd1c0, /// 0x13c
			 0x3f0bb0d4, /// 0x140
			 0x05218a82, /// 0x144
			 0xe2914f84, /// 0x148
			 0x07597cd5, /// 0x14c
			 0x2d4e300d, /// 0x150
			 0xf4ffef63, /// 0x154
			 0x7266d891, /// 0x158
			 0x7d92c367, /// 0x15c
			 0x03800571, /// 0x160
			 0x8cfb3f6a, /// 0x164
			 0xa87ca683, /// 0x168
			 0x9f280832, /// 0x16c
			 0x872829a1, /// 0x170
			 0x0a169c16, /// 0x174
			 0xdf8e2f0f, /// 0x178
			 0x7c9def3c, /// 0x17c
			 0x663982d7, /// 0x180
			 0xa5410b80, /// 0x184
			 0x2fa3e4af, /// 0x188
			 0xe44daf45, /// 0x18c
			 0xf947ddf7, /// 0x190
			 0x1bf5c896, /// 0x194
			 0x1bde344d, /// 0x198
			 0x2b53689c, /// 0x19c
			 0xee921c7f, /// 0x1a0
			 0x2d75d8d9, /// 0x1a4
			 0xaa971e70, /// 0x1a8
			 0xf34ad50f, /// 0x1ac
			 0xac01d9fb, /// 0x1b0
			 0x96b06b27, /// 0x1b4
			 0x754a1db4, /// 0x1b8
			 0x565ff3ca, /// 0x1bc
			 0x94cd0930, /// 0x1c0
			 0x11a42ab9, /// 0x1c4
			 0xb8985454, /// 0x1c8
			 0x0519c375, /// 0x1cc
			 0x301bde6e, /// 0x1d0
			 0x0caaabcb, /// 0x1d4
			 0x8e97566c, /// 0x1d8
			 0x98cb6157, /// 0x1dc
			 0xdb45713e, /// 0x1e0
			 0x2a4c7238, /// 0x1e4
			 0xaa8e390e, /// 0x1e8
			 0x5c0abc4c, /// 0x1ec
			 0x1cbc3ba3, /// 0x1f0
			 0x329c0530, /// 0x1f4
			 0x7e85a25c, /// 0x1f8
			 0xdce8e78c, /// 0x1fc
			 0x60bccb99, /// 0x200
			 0x76faf52b, /// 0x204
			 0x5f30c488, /// 0x208
			 0x258e9da6, /// 0x20c
			 0x13cf5651, /// 0x210
			 0x812e150b, /// 0x214
			 0x67669e91, /// 0x218
			 0xfee2348c, /// 0x21c
			 0xa982dc13, /// 0x220
			 0xc115a700, /// 0x224
			 0x3f281e7a, /// 0x228
			 0x4e40e60f, /// 0x22c
			 0xc3ce276c, /// 0x230
			 0xa8ae8804, /// 0x234
			 0xc30c2019, /// 0x238
			 0xff5e4795, /// 0x23c
			 0xbbef811a, /// 0x240
			 0x8d77ea70, /// 0x244
			 0x1f6ff94f, /// 0x248
			 0xe4de56ea, /// 0x24c
			 0x93b46dec, /// 0x250
			 0x52ac6bea, /// 0x254
			 0x79c54eb2, /// 0x258
			 0xa256fc96, /// 0x25c
			 0xcd280eaf, /// 0x260
			 0x2a9cc209, /// 0x264
			 0x0f0e68de, /// 0x268
			 0x423d2128, /// 0x26c
			 0xb5ce4652, /// 0x270
			 0x54f06bb1, /// 0x274
			 0x080da66d, /// 0x278
			 0xcf378cc6, /// 0x27c
			 0xb3218aec, /// 0x280
			 0x5f51c748, /// 0x284
			 0x12d8a3a2, /// 0x288
			 0x2bc36949, /// 0x28c
			 0x64a26296, /// 0x290
			 0x5d2961ea, /// 0x294
			 0x85398861, /// 0x298
			 0x69c988ed, /// 0x29c
			 0x8fa37351, /// 0x2a0
			 0xbda3e985, /// 0x2a4
			 0xd2617575, /// 0x2a8
			 0x308fa2cb, /// 0x2ac
			 0x1eeb2f98, /// 0x2b0
			 0xe63e3516, /// 0x2b4
			 0x22af180a, /// 0x2b8
			 0xe0c1c3a9, /// 0x2bc
			 0xa582e053, /// 0x2c0
			 0x19c5f218, /// 0x2c4
			 0xaf0fd8d6, /// 0x2c8
			 0xfc1e9e5c, /// 0x2cc
			 0x5a870afc, /// 0x2d0
			 0x1cb20d8b, /// 0x2d4
			 0x5ed0efb1, /// 0x2d8
			 0x9a14e6fb, /// 0x2dc
			 0xfa5ac7bc, /// 0x2e0
			 0x3ed4490f, /// 0x2e4
			 0x1a062f57, /// 0x2e8
			 0x92e5e2f9, /// 0x2ec
			 0x41a7cb6f, /// 0x2f0
			 0x7ce58a5e, /// 0x2f4
			 0x9bfc51f5, /// 0x2f8
			 0xae17899a, /// 0x2fc
			 0x6d639e74, /// 0x300
			 0x535ba828, /// 0x304
			 0x28bd2b77, /// 0x308
			 0xde398329, /// 0x30c
			 0x22ea6c43, /// 0x310
			 0x3ee8d54f, /// 0x314
			 0x9c989b3b, /// 0x318
			 0xc4bd1c05, /// 0x31c
			 0x0d2bb4bd, /// 0x320
			 0xf393ab49, /// 0x324
			 0x2c28a72d, /// 0x328
			 0xcb2f91da, /// 0x32c
			 0xe0dfa498, /// 0x330
			 0x4294ae25, /// 0x334
			 0x7e308459, /// 0x338
			 0x96d683bf, /// 0x33c
			 0x811be5f4, /// 0x340
			 0x2e5ade0f, /// 0x344
			 0x29e06e2e, /// 0x348
			 0x60bc389a, /// 0x34c
			 0xfa1f7a0e, /// 0x350
			 0x39b0c43d, /// 0x354
			 0xf546dd03, /// 0x358
			 0x61277654, /// 0x35c
			 0x1abc6c85, /// 0x360
			 0xa7bc4974, /// 0x364
			 0xf0198cb3, /// 0x368
			 0x587bb522, /// 0x36c
			 0x1ef65cb3, /// 0x370
			 0x5ba04c80, /// 0x374
			 0xc012d602, /// 0x378
			 0x60a043e0, /// 0x37c
			 0x2d629c31, /// 0x380
			 0x4c4d5fe1, /// 0x384
			 0x4c75c58f, /// 0x388
			 0x6ab6b7e5, /// 0x38c
			 0x0b9e8186, /// 0x390
			 0xc1c77c81, /// 0x394
			 0x14ab289a, /// 0x398
			 0xb8ad0a6e, /// 0x39c
			 0x2497cbef, /// 0x3a0
			 0xa3022246, /// 0x3a4
			 0xfd35d38b, /// 0x3a8
			 0x9f3b67fb, /// 0x3ac
			 0x6097c5bb, /// 0x3b0
			 0x4f15f029, /// 0x3b4
			 0x491188a0, /// 0x3b8
			 0xbe063c2f, /// 0x3bc
			 0xbfe32cb0, /// 0x3c0
			 0xfa783978, /// 0x3c4
			 0x42d06ebe, /// 0x3c8
			 0xfee01800, /// 0x3cc
			 0xaf5f7c95, /// 0x3d0
			 0xf812bc77, /// 0x3d4
			 0x54f01673, /// 0x3d8
			 0x2150d6f6, /// 0x3dc
			 0x40fa8361, /// 0x3e0
			 0x984426b5, /// 0x3e4
			 0x29a7bae6, /// 0x3e8
			 0x5e11c35c, /// 0x3ec
			 0xf3c408d6, /// 0x3f0
			 0xbe265f4d, /// 0x3f4
			 0x627e27b2, /// 0x3f8
			 0x3a950775, /// 0x3fc
			 0xdb0b97c0, /// 0x400
			 0xa13a5a34, /// 0x404
			 0x7e3a02a8, /// 0x408
			 0x494fa414, /// 0x40c
			 0x2d104f32, /// 0x410
			 0xb8c9b810, /// 0x414
			 0xcedc42ab, /// 0x418
			 0xcbab8d82, /// 0x41c
			 0x39ffcabd, /// 0x420
			 0xed355228, /// 0x424
			 0x5fdc6be9, /// 0x428
			 0xc9df398d, /// 0x42c
			 0xb76ec569, /// 0x430
			 0x2d135ffd, /// 0x434
			 0x3abd0aef, /// 0x438
			 0x7d16a306, /// 0x43c
			 0x3d851361, /// 0x440
			 0xdf9616f9, /// 0x444
			 0xb0ed0cc7, /// 0x448
			 0xf2ed8cd0, /// 0x44c
			 0x0ad38846, /// 0x450
			 0x3b96e169, /// 0x454
			 0xd00ebb9b, /// 0x458
			 0x84112dec, /// 0x45c
			 0x50e002fb, /// 0x460
			 0xa2d7a6a3, /// 0x464
			 0xfecc2b2f, /// 0x468
			 0xed23aa86, /// 0x46c
			 0x0952f1b5, /// 0x470
			 0x01e687a3, /// 0x474
			 0x54f92ef6, /// 0x478
			 0x5dcfa8c4, /// 0x47c
			 0x090b39f1, /// 0x480
			 0xaaba5f64, /// 0x484
			 0xed7c54d4, /// 0x488
			 0x54fdd0e7, /// 0x48c
			 0x28e29040, /// 0x490
			 0x39c1b9cd, /// 0x494
			 0xbdb96237, /// 0x498
			 0xa6cfa699, /// 0x49c
			 0x8ccf67ee, /// 0x4a0
			 0x58fa83cf, /// 0x4a4
			 0x1b72f444, /// 0x4a8
			 0x7510c041, /// 0x4ac
			 0x0c626d65, /// 0x4b0
			 0x2fdf74dc, /// 0x4b4
			 0x9400b207, /// 0x4b8
			 0xe4c40442, /// 0x4bc
			 0x8e47ac7a, /// 0x4c0
			 0xe3224cb5, /// 0x4c4
			 0x79fbfd28, /// 0x4c8
			 0xf35d5b09, /// 0x4cc
			 0xb54596e5, /// 0x4d0
			 0x628f9af9, /// 0x4d4
			 0x1756aac3, /// 0x4d8
			 0xbfaeb1d9, /// 0x4dc
			 0x85b787ac, /// 0x4e0
			 0x4c80fec3, /// 0x4e4
			 0x6473232b, /// 0x4e8
			 0x40564b0b, /// 0x4ec
			 0xb1527c82, /// 0x4f0
			 0xbf96d7b9, /// 0x4f4
			 0x0007ed06, /// 0x4f8
			 0xae611009, /// 0x4fc
			 0x55f5d8aa, /// 0x500
			 0xa4b19db7, /// 0x504
			 0x932b319c, /// 0x508
			 0x391203ed, /// 0x50c
			 0x4b413f18, /// 0x510
			 0x6871cffc, /// 0x514
			 0x30d4dd63, /// 0x518
			 0xee6b7325, /// 0x51c
			 0x62360811, /// 0x520
			 0x282741eb, /// 0x524
			 0xfb2b50b8, /// 0x528
			 0x11b3f808, /// 0x52c
			 0xc25502e6, /// 0x530
			 0xa4a8df0f, /// 0x534
			 0xfb683f83, /// 0x538
			 0x20c423fc, /// 0x53c
			 0x716b8922, /// 0x540
			 0xaa983666, /// 0x544
			 0x2ceb94fb, /// 0x548
			 0xcd7828cf, /// 0x54c
			 0xffef6874, /// 0x550
			 0x1c8532d8, /// 0x554
			 0xd0abefaf, /// 0x558
			 0x09d019ca, /// 0x55c
			 0xa544d15d, /// 0x560
			 0x0124e53b, /// 0x564
			 0xa528e7c9, /// 0x568
			 0x462954d5, /// 0x56c
			 0x699e6922, /// 0x570
			 0xc622dc19, /// 0x574
			 0x1e43e433, /// 0x578
			 0x72b7b314, /// 0x57c
			 0x3923286a, /// 0x580
			 0x33a2a0bc, /// 0x584
			 0xc81faaed, /// 0x588
			 0xe4f5a03b, /// 0x58c
			 0x0161c9d4, /// 0x590
			 0xec7a389e, /// 0x594
			 0x79f7e0f8, /// 0x598
			 0xc48974bc, /// 0x59c
			 0x408e85b0, /// 0x5a0
			 0x0659cfde, /// 0x5a4
			 0x58499f23, /// 0x5a8
			 0xe4889ae8, /// 0x5ac
			 0x2029a261, /// 0x5b0
			 0x819eff05, /// 0x5b4
			 0x5dc120b3, /// 0x5b8
			 0xb1438382, /// 0x5bc
			 0x6d380ab2, /// 0x5c0
			 0x6f99e8e0, /// 0x5c4
			 0x370b155e, /// 0x5c8
			 0x39b6a5bb, /// 0x5cc
			 0x3152a355, /// 0x5d0
			 0xd8555306, /// 0x5d4
			 0xb3e28861, /// 0x5d8
			 0x8671070f, /// 0x5dc
			 0xe1a7968a, /// 0x5e0
			 0x5ae57b43, /// 0x5e4
			 0x7b4ea132, /// 0x5e8
			 0xeb2b58e7, /// 0x5ec
			 0x4a847cce, /// 0x5f0
			 0xfd6b66c4, /// 0x5f4
			 0xd03ea739, /// 0x5f8
			 0x99142f20, /// 0x5fc
			 0x0b58159a, /// 0x600
			 0x7fcec2ad, /// 0x604
			 0x8c0e0300, /// 0x608
			 0x99e810db, /// 0x60c
			 0x90c9c9fd, /// 0x610
			 0x38829aaf, /// 0x614
			 0x5eaf268b, /// 0x618
			 0xc27f88f1, /// 0x61c
			 0xf1eae8f7, /// 0x620
			 0xeb90476a, /// 0x624
			 0x78b4acda, /// 0x628
			 0x112c36d1, /// 0x62c
			 0xc00d2622, /// 0x630
			 0x6c600a01, /// 0x634
			 0xaef97d94, /// 0x638
			 0x7ef05ac2, /// 0x63c
			 0x6dde0a94, /// 0x640
			 0x3228c5a4, /// 0x644
			 0x7ecaecd8, /// 0x648
			 0xe6f4733b, /// 0x64c
			 0xb7bda28a, /// 0x650
			 0x1b1fc09c, /// 0x654
			 0x5a459888, /// 0x658
			 0x2a3818b1, /// 0x65c
			 0xeea97636, /// 0x660
			 0xd2d459bd, /// 0x664
			 0xbb681745, /// 0x668
			 0xcb5fbc30, /// 0x66c
			 0x773858ff, /// 0x670
			 0x11c3e4ea, /// 0x674
			 0xf91a2a18, /// 0x678
			 0xe248b49a, /// 0x67c
			 0x31823f98, /// 0x680
			 0x1a599f45, /// 0x684
			 0x79bcb172, /// 0x688
			 0x64c321eb, /// 0x68c
			 0x78e70bbd, /// 0x690
			 0x951ddcbf, /// 0x694
			 0x41980c19, /// 0x698
			 0xb1290f7b, /// 0x69c
			 0xf42e0a16, /// 0x6a0
			 0xe8b34214, /// 0x6a4
			 0x29d7d3dc, /// 0x6a8
			 0x50cdd88b, /// 0x6ac
			 0x9c096f90, /// 0x6b0
			 0xe9428bb0, /// 0x6b4
			 0xfcdc0ffd, /// 0x6b8
			 0x0df8b5f4, /// 0x6bc
			 0xf7c65a38, /// 0x6c0
			 0xe57f7680, /// 0x6c4
			 0xddebca74, /// 0x6c8
			 0x1c8e0495, /// 0x6cc
			 0x0889cf97, /// 0x6d0
			 0x0bb745da, /// 0x6d4
			 0x47c9ee90, /// 0x6d8
			 0xdf1a520a, /// 0x6dc
			 0xcfb07bd9, /// 0x6e0
			 0x8d71c5d8, /// 0x6e4
			 0x80fd67aa, /// 0x6e8
			 0x498f5150, /// 0x6ec
			 0xdfd3865e, /// 0x6f0
			 0x9cb49d59, /// 0x6f4
			 0xc67dbb3c, /// 0x6f8
			 0x9a8abeaa, /// 0x6fc
			 0x698b720b, /// 0x700
			 0x7ea7459d, /// 0x704
			 0x58bde4f9, /// 0x708
			 0xea848183, /// 0x70c
			 0xa82ec0ce, /// 0x710
			 0x5fb2b9e0, /// 0x714
			 0xd5dc1a08, /// 0x718
			 0x7c3bd60e, /// 0x71c
			 0x13c60496, /// 0x720
			 0xc5c07090, /// 0x724
			 0xb3b53a06, /// 0x728
			 0x8b38aa32, /// 0x72c
			 0x6181d8b3, /// 0x730
			 0xe555307f, /// 0x734
			 0x442d6d83, /// 0x738
			 0x667c0562, /// 0x73c
			 0x0531d368, /// 0x740
			 0x5d45c4b3, /// 0x744
			 0xa6bf484b, /// 0x748
			 0x70dd76bd, /// 0x74c
			 0x8ea97ef2, /// 0x750
			 0x9c147f9b, /// 0x754
			 0x8fa44b8e, /// 0x758
			 0x10a92d82, /// 0x75c
			 0x66bec563, /// 0x760
			 0x7568bc1e, /// 0x764
			 0x0984c13d, /// 0x768
			 0x0a1da484, /// 0x76c
			 0xe5535f0f, /// 0x770
			 0x6b2d56ba, /// 0x774
			 0x12c7fb6e, /// 0x778
			 0xb83e17e9, /// 0x77c
			 0xddeef6ac, /// 0x780
			 0xc3b8d8d0, /// 0x784
			 0xbfe1cd65, /// 0x788
			 0xe2dd6dd0, /// 0x78c
			 0x279932ae, /// 0x790
			 0x8a19b543, /// 0x794
			 0xf0982700, /// 0x798
			 0x67bcf6dc, /// 0x79c
			 0x55e247b0, /// 0x7a0
			 0x2d21f10d, /// 0x7a4
			 0x4d8e4a56, /// 0x7a8
			 0x00089539, /// 0x7ac
			 0x1c2aac64, /// 0x7b0
			 0xb4767ac9, /// 0x7b4
			 0xf53bdb8e, /// 0x7b8
			 0x4738c39d, /// 0x7bc
			 0xd1987f56, /// 0x7c0
			 0x14d71482, /// 0x7c4
			 0x7cd49cd2, /// 0x7c8
			 0xfa633b00, /// 0x7cc
			 0x93d456ed, /// 0x7d0
			 0xe09a62b2, /// 0x7d4
			 0x17fba8f5, /// 0x7d8
			 0x3ef3b9eb, /// 0x7dc
			 0x7043830c, /// 0x7e0
			 0x65d188d4, /// 0x7e4
			 0x98ffdc70, /// 0x7e8
			 0x5093a7c6, /// 0x7ec
			 0x207789ab, /// 0x7f0
			 0x6b02e8d1, /// 0x7f4
			 0x1e205425, /// 0x7f8
			 0xeb117611, /// 0x7fc
			 0x31faf949, /// 0x800
			 0xaa547f86, /// 0x804
			 0xed8770c9, /// 0x808
			 0x8eeba98e, /// 0x80c
			 0x77833d96, /// 0x810
			 0x960ac176, /// 0x814
			 0xdea4c283, /// 0x818
			 0x3f27a35c, /// 0x81c
			 0xfdd30103, /// 0x820
			 0xbf0c616d, /// 0x824
			 0x95492ed4, /// 0x828
			 0x700c2238, /// 0x82c
			 0x7458111b, /// 0x830
			 0x957f66f3, /// 0x834
			 0x863a3f4d, /// 0x838
			 0xab5d4ec6, /// 0x83c
			 0xa9066b80, /// 0x840
			 0x3b1620eb, /// 0x844
			 0x3ac551a7, /// 0x848
			 0x700b18d9, /// 0x84c
			 0xe0f8145f, /// 0x850
			 0x675868dc, /// 0x854
			 0x67d100e7, /// 0x858
			 0x1c39c7ea, /// 0x85c
			 0x35caca35, /// 0x860
			 0x65ad8a00, /// 0x864
			 0x8a8f55b9, /// 0x868
			 0xfd985df9, /// 0x86c
			 0x7e5970ff, /// 0x870
			 0xc88ad013, /// 0x874
			 0x0eccd86f, /// 0x878
			 0x9fc9f1df, /// 0x87c
			 0x15de9114, /// 0x880
			 0x5428d6fd, /// 0x884
			 0xaa6fbbcd, /// 0x888
			 0x8bbbb302, /// 0x88c
			 0x9c7a5bb8, /// 0x890
			 0x1207b88d, /// 0x894
			 0x6ae3f8b1, /// 0x898
			 0xe83ce06f, /// 0x89c
			 0x8ce58907, /// 0x8a0
			 0x1e76a6d5, /// 0x8a4
			 0xb68f2bfd, /// 0x8a8
			 0xdca83b60, /// 0x8ac
			 0x90f04c36, /// 0x8b0
			 0x73b56cb0, /// 0x8b4
			 0xe4cd014c, /// 0x8b8
			 0x671ef3f8, /// 0x8bc
			 0xd09918e3, /// 0x8c0
			 0xad6cc5d6, /// 0x8c4
			 0x5e86e14e, /// 0x8c8
			 0x3665650b, /// 0x8cc
			 0xeabd74f4, /// 0x8d0
			 0x69ade9b2, /// 0x8d4
			 0x412fb35e, /// 0x8d8
			 0x8b8cd15b, /// 0x8dc
			 0x3d384585, /// 0x8e0
			 0xb8cd084e, /// 0x8e4
			 0x44da5771, /// 0x8e8
			 0x39cf2cf2, /// 0x8ec
			 0xea10c93c, /// 0x8f0
			 0x56a3ccf3, /// 0x8f4
			 0x8d9832ac, /// 0x8f8
			 0xcdf78c63, /// 0x8fc
			 0x346c4cab, /// 0x900
			 0x4fbff93e, /// 0x904
			 0x44aec9b8, /// 0x908
			 0xe9ddf53b, /// 0x90c
			 0x92dcb816, /// 0x910
			 0x0a66bf6e, /// 0x914
			 0x22204c3a, /// 0x918
			 0xe008f407, /// 0x91c
			 0x253af646, /// 0x920
			 0x860aa85f, /// 0x924
			 0x07a25cb8, /// 0x928
			 0x7ed32f5e, /// 0x92c
			 0xf08ad151, /// 0x930
			 0xf3907189, /// 0x934
			 0x4d7fd53c, /// 0x938
			 0xa495e37e, /// 0x93c
			 0xe1ab8aa1, /// 0x940
			 0x7805db00, /// 0x944
			 0x9445e1dd, /// 0x948
			 0xd458aad8, /// 0x94c
			 0xe78b0cfc, /// 0x950
			 0xe9c97bb7, /// 0x954
			 0xa61a2553, /// 0x958
			 0xc038b921, /// 0x95c
			 0xd24e8156, /// 0x960
			 0x4f79f807, /// 0x964
			 0x720d263e, /// 0x968
			 0x09d6a51c, /// 0x96c
			 0x04c49539, /// 0x970
			 0x9714c7e8, /// 0x974
			 0x567c7542, /// 0x978
			 0x62ffa084, /// 0x97c
			 0x1593bb0e, /// 0x980
			 0xe85b4dea, /// 0x984
			 0x8047d1a9, /// 0x988
			 0x915360bc, /// 0x98c
			 0xeeec8e34, /// 0x990
			 0xeba56b24, /// 0x994
			 0x3383568f, /// 0x998
			 0x20ada3ad, /// 0x99c
			 0xeaf8b48d, /// 0x9a0
			 0x9ef3ea51, /// 0x9a4
			 0xbc1ab787, /// 0x9a8
			 0xcb32e7a0, /// 0x9ac
			 0x01cbb839, /// 0x9b0
			 0xf2183351, /// 0x9b4
			 0x18c24508, /// 0x9b8
			 0x4fe3b80e, /// 0x9bc
			 0x4846928d, /// 0x9c0
			 0x2be656b5, /// 0x9c4
			 0x4e67785b, /// 0x9c8
			 0x1b3b26ca, /// 0x9cc
			 0xb2f757c3, /// 0x9d0
			 0xddce4633, /// 0x9d4
			 0x534c2475, /// 0x9d8
			 0x38204bf4, /// 0x9dc
			 0x0c2d5080, /// 0x9e0
			 0x40ae0864, /// 0x9e4
			 0x02f72cea, /// 0x9e8
			 0x05edbb09, /// 0x9ec
			 0xcf6c4b9f, /// 0x9f0
			 0x88a7f46b, /// 0x9f4
			 0x57ce1093, /// 0x9f8
			 0x67254c34, /// 0x9fc
			 0xdad770e4, /// 0xa00
			 0xfb710d5c, /// 0xa04
			 0x23317fec, /// 0xa08
			 0xe87ca3ee, /// 0xa0c
			 0x5fc54ee2, /// 0xa10
			 0x6b026211, /// 0xa14
			 0x52cfce56, /// 0xa18
			 0x9e9061bb, /// 0xa1c
			 0x2a58cfda, /// 0xa20
			 0xe1bc62d8, /// 0xa24
			 0xb5a0dffa, /// 0xa28
			 0x0b188196, /// 0xa2c
			 0x9519443a, /// 0xa30
			 0x92453544, /// 0xa34
			 0x052e00d1, /// 0xa38
			 0x077451c0, /// 0xa3c
			 0xfa5fcc9d, /// 0xa40
			 0x480334ba, /// 0xa44
			 0xc7151a2c, /// 0xa48
			 0x075c905a, /// 0xa4c
			 0xe41449e8, /// 0xa50
			 0x61920a95, /// 0xa54
			 0x1b7a01c2, /// 0xa58
			 0x99a01013, /// 0xa5c
			 0xe0a47bd8, /// 0xa60
			 0x764c4c37, /// 0xa64
			 0x1133c787, /// 0xa68
			 0x8a5355ce, /// 0xa6c
			 0xab6c3888, /// 0xa70
			 0x5fb2a916, /// 0xa74
			 0x82651445, /// 0xa78
			 0x086171e1, /// 0xa7c
			 0x3d48829a, /// 0xa80
			 0xbd550164, /// 0xa84
			 0x92b5a4a1, /// 0xa88
			 0x4b8abdcc, /// 0xa8c
			 0xc1bdee4b, /// 0xa90
			 0x022419ba, /// 0xa94
			 0x11a05f5a, /// 0xa98
			 0xdefc35e5, /// 0xa9c
			 0x4cc5290e, /// 0xaa0
			 0xd79177c3, /// 0xaa4
			 0x0f088b49, /// 0xaa8
			 0x83d30e0f, /// 0xaac
			 0xae3bfc3a, /// 0xab0
			 0x69d5f9bc, /// 0xab4
			 0xd6e8d55b, /// 0xab8
			 0x87d2c0f9, /// 0xabc
			 0x6cab8bd2, /// 0xac0
			 0x89a8985e, /// 0xac4
			 0xf1a32b9d, /// 0xac8
			 0x4a33fe3a, /// 0xacc
			 0xca0a272a, /// 0xad0
			 0x0a54b8ef, /// 0xad4
			 0x4ea9bd99, /// 0xad8
			 0xeafab7ba, /// 0xadc
			 0x4dff7227, /// 0xae0
			 0x04531a21, /// 0xae4
			 0x533a9189, /// 0xae8
			 0xde08c247, /// 0xaec
			 0x6cf401bd, /// 0xaf0
			 0x4d96383a, /// 0xaf4
			 0x095b9dd1, /// 0xaf8
			 0xe667485d, /// 0xafc
			 0x26961293, /// 0xb00
			 0x04986210, /// 0xb04
			 0x9917f6f7, /// 0xb08
			 0x631d3ed3, /// 0xb0c
			 0x7c907ac5, /// 0xb10
			 0x34e26499, /// 0xb14
			 0x17e83204, /// 0xb18
			 0xd83f2c16, /// 0xb1c
			 0xf4f05485, /// 0xb20
			 0x5077c328, /// 0xb24
			 0xc4819d9f, /// 0xb28
			 0x6354270d, /// 0xb2c
			 0x9a1f2dce, /// 0xb30
			 0xb3f9d710, /// 0xb34
			 0xfc003edb, /// 0xb38
			 0x8a820f0b, /// 0xb3c
			 0x5de7a512, /// 0xb40
			 0x67b2a555, /// 0xb44
			 0xe213d17b, /// 0xb48
			 0x1b41630e, /// 0xb4c
			 0xf433f0c8, /// 0xb50
			 0x83ff4728, /// 0xb54
			 0x644e86c3, /// 0xb58
			 0x1f658185, /// 0xb5c
			 0xab1e54b6, /// 0xb60
			 0xaa4f1995, /// 0xb64
			 0x281cac78, /// 0xb68
			 0x855abdb8, /// 0xb6c
			 0xa59f5deb, /// 0xb70
			 0x1b8c1f47, /// 0xb74
			 0x255412b1, /// 0xb78
			 0x8d5b22cc, /// 0xb7c
			 0xc9dfff5c, /// 0xb80
			 0x486152d4, /// 0xb84
			 0xdcbef1d6, /// 0xb88
			 0x917ae30e, /// 0xb8c
			 0x9486412e, /// 0xb90
			 0x12426895, /// 0xb94
			 0xc2bae7c3, /// 0xb98
			 0x5dd0ade7, /// 0xb9c
			 0x3a8af6db, /// 0xba0
			 0xa6ea35c2, /// 0xba4
			 0xb9c43c18, /// 0xba8
			 0xc4e51c58, /// 0xbac
			 0x84582580, /// 0xbb0
			 0x452fcc6a, /// 0xbb4
			 0xa7541dee, /// 0xbb8
			 0x3bb1d17e, /// 0xbbc
			 0xb4c22d6d, /// 0xbc0
			 0xcb1c8cb4, /// 0xbc4
			 0xd57ec562, /// 0xbc8
			 0xd424e264, /// 0xbcc
			 0xc854418a, /// 0xbd0
			 0x2d09c523, /// 0xbd4
			 0x50f0074c, /// 0xbd8
			 0x437472dd, /// 0xbdc
			 0x3bd0be4a, /// 0xbe0
			 0xc121ba58, /// 0xbe4
			 0xe69b8899, /// 0xbe8
			 0x2ea31e18, /// 0xbec
			 0xfe410f94, /// 0xbf0
			 0xce62e54a, /// 0xbf4
			 0xa554a496, /// 0xbf8
			 0x5ce55106, /// 0xbfc
			 0xa6c33e8c, /// 0xc00
			 0xb576d272, /// 0xc04
			 0xae002572, /// 0xc08
			 0x1a452d0c, /// 0xc0c
			 0x47d9bffa, /// 0xc10
			 0x838741cf, /// 0xc14
			 0x4a564b4a, /// 0xc18
			 0x7de9d5a4, /// 0xc1c
			 0xd271bad0, /// 0xc20
			 0xb11b14c5, /// 0xc24
			 0xe6c4f44d, /// 0xc28
			 0x7e939534, /// 0xc2c
			 0xa11450a9, /// 0xc30
			 0xdc2887c9, /// 0xc34
			 0xc16df6fb, /// 0xc38
			 0xa1159673, /// 0xc3c
			 0x10e0b0a4, /// 0xc40
			 0x4ae5f1dd, /// 0xc44
			 0xcdeebdb6, /// 0xc48
			 0xf753960f, /// 0xc4c
			 0x52799bf9, /// 0xc50
			 0xe6e087bc, /// 0xc54
			 0x7ade4427, /// 0xc58
			 0xbeba45b3, /// 0xc5c
			 0x4ec55d11, /// 0xc60
			 0xf1a79139, /// 0xc64
			 0x27e7492f, /// 0xc68
			 0xee648b21, /// 0xc6c
			 0x301f60fe, /// 0xc70
			 0x7107477f, /// 0xc74
			 0xc80a78c0, /// 0xc78
			 0xd25f7b21, /// 0xc7c
			 0xea1750d1, /// 0xc80
			 0x0639ffc1, /// 0xc84
			 0xab1a4063, /// 0xc88
			 0xc8e5e48c, /// 0xc8c
			 0xda2184e4, /// 0xc90
			 0xe159fd80, /// 0xc94
			 0x6b31e353, /// 0xc98
			 0xd1199f9d, /// 0xc9c
			 0x91eee401, /// 0xca0
			 0x54fcbd2d, /// 0xca4
			 0x63a153c3, /// 0xca8
			 0xfddb8d48, /// 0xcac
			 0x521a5859, /// 0xcb0
			 0x0aa46704, /// 0xcb4
			 0x13e45c72, /// 0xcb8
			 0xa9794a59, /// 0xcbc
			 0xa262a360, /// 0xcc0
			 0xbc00cfdf, /// 0xcc4
			 0x529d03ea, /// 0xcc8
			 0x27917cda, /// 0xccc
			 0x14cd9daa, /// 0xcd0
			 0x98eba064, /// 0xcd4
			 0xb484e695, /// 0xcd8
			 0x2b42dcae, /// 0xcdc
			 0x9e46ee0b, /// 0xce0
			 0xe79a3c16, /// 0xce4
			 0x8db672a6, /// 0xce8
			 0x5e9cc55a, /// 0xcec
			 0x8002ab3a, /// 0xcf0
			 0x4b45d5f8, /// 0xcf4
			 0xc4e4fa6c, /// 0xcf8
			 0x8961d228, /// 0xcfc
			 0x6899c6b7, /// 0xd00
			 0xf2e1e099, /// 0xd04
			 0xc49b547c, /// 0xd08
			 0xec44ed47, /// 0xd0c
			 0xce850d7a, /// 0xd10
			 0x0e0ead39, /// 0xd14
			 0xc1ecee76, /// 0xd18
			 0x7444bfa0, /// 0xd1c
			 0xdb38f105, /// 0xd20
			 0x5fb13ece, /// 0xd24
			 0x7dbd8343, /// 0xd28
			 0xe461fb75, /// 0xd2c
			 0xb54fff21, /// 0xd30
			 0x5a8713c5, /// 0xd34
			 0xab0060ca, /// 0xd38
			 0x86ce95f4, /// 0xd3c
			 0x25ab5e23, /// 0xd40
			 0x382ccd3f, /// 0xd44
			 0xbb57f473, /// 0xd48
			 0xbc5c3f13, /// 0xd4c
			 0xb2457c87, /// 0xd50
			 0xa63c0c99, /// 0xd54
			 0x6c541901, /// 0xd58
			 0xdbe52ca8, /// 0xd5c
			 0xff2c8c67, /// 0xd60
			 0x0c289369, /// 0xd64
			 0x14f56dbc, /// 0xd68
			 0xa2772b38, /// 0xd6c
			 0xc2a487c4, /// 0xd70
			 0x4ecea44a, /// 0xd74
			 0x68574c2c, /// 0xd78
			 0x67ef7d7b, /// 0xd7c
			 0xfc9569ff, /// 0xd80
			 0x5a6e4ab5, /// 0xd84
			 0x60597850, /// 0xd88
			 0x19e8974e, /// 0xd8c
			 0x8da8fdeb, /// 0xd90
			 0x08535b92, /// 0xd94
			 0x709f2c89, /// 0xd98
			 0x4595b170, /// 0xd9c
			 0xbfdcf0bb, /// 0xda0
			 0x2ff9bce6, /// 0xda4
			 0xec0177e5, /// 0xda8
			 0x07f26d79, /// 0xdac
			 0x0ea9014e, /// 0xdb0
			 0x474e29dc, /// 0xdb4
			 0xb35c5b5c, /// 0xdb8
			 0x0c37a030, /// 0xdbc
			 0x3711330a, /// 0xdc0
			 0xfb033fa7, /// 0xdc4
			 0xb95abb7b, /// 0xdc8
			 0xb3f1d900, /// 0xdcc
			 0xb6437d6c, /// 0xdd0
			 0xb26521f5, /// 0xdd4
			 0xebae3827, /// 0xdd8
			 0x6e6b2fd3, /// 0xddc
			 0x83977a87, /// 0xde0
			 0xb95da563, /// 0xde4
			 0xecc83a8d, /// 0xde8
			 0x124c4898, /// 0xdec
			 0x6a06fe7a, /// 0xdf0
			 0x725f5a46, /// 0xdf4
			 0x602cd734, /// 0xdf8
			 0x8de88174, /// 0xdfc
			 0x2378b43a, /// 0xe00
			 0x4839cef6, /// 0xe04
			 0x7af36c5a, /// 0xe08
			 0x9e0b1980, /// 0xe0c
			 0x47b16295, /// 0xe10
			 0xe1fa302d, /// 0xe14
			 0x9db75de2, /// 0xe18
			 0x639bf347, /// 0xe1c
			 0x01a84ed1, /// 0xe20
			 0x94b930fe, /// 0xe24
			 0x199f1cce, /// 0xe28
			 0x8b809366, /// 0xe2c
			 0x556344bb, /// 0xe30
			 0x588a04a0, /// 0xe34
			 0xfc84846f, /// 0xe38
			 0x321a3570, /// 0xe3c
			 0x299a13f9, /// 0xe40
			 0x4f264818, /// 0xe44
			 0xd9da5de1, /// 0xe48
			 0xac8a8722, /// 0xe4c
			 0xd0b0ad09, /// 0xe50
			 0xd4ea9a84, /// 0xe54
			 0x4b58a4e9, /// 0xe58
			 0xdf5e2d04, /// 0xe5c
			 0xebc91a35, /// 0xe60
			 0x9dba429b, /// 0xe64
			 0x74db541e, /// 0xe68
			 0x5e235995, /// 0xe6c
			 0x35fa4a4a, /// 0xe70
			 0x938f2dbe, /// 0xe74
			 0xeafcef12, /// 0xe78
			 0x9f930795, /// 0xe7c
			 0xe874a38a, /// 0xe80
			 0x66466d2b, /// 0xe84
			 0x9fb1d8d1, /// 0xe88
			 0xfad12b15, /// 0xe8c
			 0xc7ac6f16, /// 0xe90
			 0x145a33f9, /// 0xe94
			 0x3a43f486, /// 0xe98
			 0xe9803864, /// 0xe9c
			 0x550ddb05, /// 0xea0
			 0xd9dc3b41, /// 0xea4
			 0x643348ae, /// 0xea8
			 0xbfa9052e, /// 0xeac
			 0x2f2f71d7, /// 0xeb0
			 0x2e4f0fad, /// 0xeb4
			 0xba58340e, /// 0xeb8
			 0xf9de22d4, /// 0xebc
			 0xe7e54fe5, /// 0xec0
			 0x1cd22e24, /// 0xec4
			 0x4532705a, /// 0xec8
			 0x0045e16a, /// 0xecc
			 0x0eafdd42, /// 0xed0
			 0xbdf00505, /// 0xed4
			 0x2297b494, /// 0xed8
			 0xace45288, /// 0xedc
			 0x72be3e0a, /// 0xee0
			 0xfa78da98, /// 0xee4
			 0x5dbd3e97, /// 0xee8
			 0x37714d03, /// 0xeec
			 0xdcd44efa, /// 0xef0
			 0x0102b6de, /// 0xef4
			 0xdde810ff, /// 0xef8
			 0x0fcefeef, /// 0xefc
			 0x01176ce7, /// 0xf00
			 0x89e272f0, /// 0xf04
			 0x302dc4fc, /// 0xf08
			 0x51cf88e8, /// 0xf0c
			 0xd14212d6, /// 0xf10
			 0x4aa070a6, /// 0xf14
			 0x8f4b041b, /// 0xf18
			 0x6370d37c, /// 0xf1c
			 0x1454d389, /// 0xf20
			 0x7964ee76, /// 0xf24
			 0x205c435c, /// 0xf28
			 0x1c299940, /// 0xf2c
			 0x1d4c0158, /// 0xf30
			 0x21d9c468, /// 0xf34
			 0xeb7c913f, /// 0xf38
			 0x10bfd201, /// 0xf3c
			 0x379d7678, /// 0xf40
			 0x4cd534e0, /// 0xf44
			 0x7f27c95b, /// 0xf48
			 0x77f6b93b, /// 0xf4c
			 0x8bcd0ba0, /// 0xf50
			 0xfcfdfa30, /// 0xf54
			 0x4bf62798, /// 0xf58
			 0x79718a6c, /// 0xf5c
			 0x908d16ab, /// 0xf60
			 0x689ca792, /// 0xf64
			 0x499ce863, /// 0xf68
			 0xc250f60a, /// 0xf6c
			 0xf0aa36f7, /// 0xf70
			 0xe2fec5e3, /// 0xf74
			 0xfe97099c, /// 0xf78
			 0xf2a67580, /// 0xf7c
			 0x872cb027, /// 0xf80
			 0xdcb8bdf3, /// 0xf84
			 0xa3bbf6e5, /// 0xf88
			 0xbae1bc17, /// 0xf8c
			 0xdccca9cb, /// 0xf90
			 0xf9290bce, /// 0xf94
			 0xa75f0759, /// 0xf98
			 0x179365bd, /// 0xf9c
			 0x111345f8, /// 0xfa0
			 0xb4d05296, /// 0xfa4
			 0x929af5fe, /// 0xfa8
			 0x614a0417, /// 0xfac
			 0x017f421e, /// 0xfb0
			 0xacdfe4a2, /// 0xfb4
			 0xc33569b6, /// 0xfb8
			 0x363507ce, /// 0xfbc
			 0x63e2ddf0, /// 0xfc0
			 0xfb8c8425, /// 0xfc4
			 0x81e319c7, /// 0xfc8
			 0xf2498d83, /// 0xfcc
			 0xfde4d2fa, /// 0xfd0
			 0x9a8d5049, /// 0xfd4
			 0x423017d7, /// 0xfd8
			 0xf6850c28, /// 0xfdc
			 0xa6d7e4d7, /// 0xfe0
			 0x16f6e267, /// 0xfe4
			 0x99759e46, /// 0xfe8
			 0x126e6c8e, /// 0xfec
			 0xf005b934, /// 0xff0
			 0x7cd5fd80, /// 0xff4
			 0x5da101a9, /// 0xff8
			 0x67f88244 /// last
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
			 0x00000598,
			 0x00000560,
			 0x00000108,
			 0x00000178,
			 0x000000b0,
			 0x0000019c,
			 0x00000520,
			 0x000002c8,

			 /// vpu register f2
			 0x40400000,
			 0x40a00000,
			 0x42000000,
			 0x41b80000,
			 0x41800000,
			 0x41800000,
			 0x40e00000,
			 0x41400000,

			 /// vpu register f3
			 0x40800000,
			 0x41c80000,
			 0x41b80000,
			 0x41e80000,
			 0x41900000,
			 0x41e00000,
			 0x41880000,
			 0x41800000,

			 /// vpu register f4
			 0x41e80000,
			 0x40c00000,
			 0x41d00000,
			 0x41800000,
			 0x41880000,
			 0x41000000,
			 0x41500000,
			 0x40400000,

			 /// vpu register f5
			 0x41880000,
			 0x40400000,
			 0x40400000,
			 0x41600000,
			 0x41200000,
			 0x41300000,
			 0x41980000,
			 0x41880000,

			 /// vpu register f6
			 0x41c00000,
			 0x3f800000,
			 0x41980000,
			 0x41b80000,
			 0x41e00000,
			 0x40400000,
			 0x41400000,
			 0x41c80000,

			 /// vpu register f7
			 0x41f00000,
			 0x41a80000,
			 0x41b00000,
			 0x40a00000,
			 0x40000000,
			 0x41500000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f8
			 0x41980000,
			 0x41400000,
			 0x41c00000,
			 0x41900000,
			 0x41300000,
			 0x41700000,
			 0x41800000,
			 0x41c80000,

			 /// vpu register f9
			 0x41e80000,
			 0x41300000,
			 0x41a80000,
			 0x41f80000,
			 0x41600000,
			 0x41d80000,
			 0x41a80000,
			 0x41900000,

			 /// vpu register f10
			 0x41b80000,
			 0x41880000,
			 0x41700000,
			 0x41b80000,
			 0x41c00000,
			 0x41000000,
			 0x41800000,
			 0x41a80000,

			 /// vpu register f11
			 0x40c00000,
			 0x41f00000,
			 0x41200000,
			 0x41600000,
			 0x41a80000,
			 0x40400000,
			 0x41980000,
			 0x41d00000,

			 /// vpu register f12
			 0x41d80000,
			 0x41f80000,
			 0x41400000,
			 0x41f80000,
			 0x41980000,
			 0x41d80000,
			 0x41c00000,
			 0x40800000,

			 /// vpu register f13
			 0x40400000,
			 0x40c00000,
			 0x41900000,
			 0x41980000,
			 0x41900000,
			 0x41980000,
			 0x41e80000,
			 0x41200000,

			 /// vpu register f14
			 0x41980000,
			 0x41700000,
			 0x41900000,
			 0x41d80000,
			 0x40400000,
			 0x41a80000,
			 0x41880000,
			 0x41b00000,

			 /// vpu register f15
			 0x42000000,
			 0x41700000,
			 0x40400000,
			 0x41400000,
			 0x41880000,
			 0x41400000,
			 0x41200000,
			 0x41700000,

			 /// vpu register f16
			 0x41300000,
			 0x41b00000,
			 0x41a00000,
			 0x41900000,
			 0x41f80000,
			 0x41100000,
			 0x41a00000,
			 0x41000000,

			 /// vpu register f17
			 0x40c00000,
			 0x40800000,
			 0x41d80000,
			 0x41500000,
			 0x41f00000,
			 0x41400000,
			 0x41b00000,
			 0x41f00000,

			 /// vpu register f18
			 0x41c00000,
			 0x41600000,
			 0x41b80000,
			 0x41100000,
			 0x3f800000,
			 0x41300000,
			 0x41880000,
			 0x41000000,

			 /// vpu register f19
			 0x41900000,
			 0x40400000,
			 0x40800000,
			 0x41880000,
			 0x41900000,
			 0x40000000,
			 0x41800000,
			 0x41200000,

			 /// vpu register f20
			 0x41500000,
			 0x40c00000,
			 0x41000000,
			 0x41000000,
			 0x41880000,
			 0x41e00000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f21
			 0x41300000,
			 0x41f80000,
			 0x41300000,
			 0x41a00000,
			 0x41a00000,
			 0x40e00000,
			 0x41600000,
			 0x41600000,

			 /// vpu register f22
			 0x42000000,
			 0x40e00000,
			 0x41300000,
			 0x41d00000,
			 0x41980000,
			 0x40800000,
			 0x40c00000,
			 0x41f80000,

			 /// vpu register f23
			 0x42000000,
			 0x41100000,
			 0x41c00000,
			 0x41500000,
			 0x41980000,
			 0x41d80000,
			 0x41900000,
			 0x41f00000,

			 /// vpu register f24
			 0x41600000,
			 0x41400000,
			 0x41600000,
			 0x40a00000,
			 0x41980000,
			 0x41500000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f25
			 0x41200000,
			 0x42000000,
			 0x41900000,
			 0x41c00000,
			 0x41d80000,
			 0x41100000,
			 0x40400000,
			 0x41d00000,

			 /// vpu register f26
			 0x41a00000,
			 0x40000000,
			 0x41880000,
			 0x40000000,
			 0x41900000,
			 0x41200000,
			 0x41a00000,
			 0x41300000,

			 /// vpu register f27
			 0x41f80000,
			 0x3f800000,
			 0x41e80000,
			 0x41e80000,
			 0x41c00000,
			 0x41b80000,
			 0x41c80000,
			 0x41600000,

			 /// vpu register f28
			 0x41400000,
			 0x41c80000,
			 0x40000000,
			 0x41b00000,
			 0x41f80000,
			 0x40c00000,
			 0x40000000,
			 0x41200000,

			 /// vpu register f29
			 0x41980000,
			 0x41e00000,
			 0x41400000,
			 0x41c80000,
			 0x40a00000,
			 0x41c00000,
			 0x41e80000,
			 0x41d80000,

			 /// vpu register f30
			 0x40c00000,
			 0x41500000,
			 0x41b00000,
			 0x40a00000,
			 0x41900000,
			 0x40a00000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f31
			 0x41900000,
			 0x3f800000,
			 0x41600000,
			 0x41880000,
			 0x41f80000,
			 0x41f00000,
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
		"fscw.ps f26, f1 (x13)\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f5, f1 (x14)\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f6, f1 (x13)\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f23, f1 (x15)\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x12)\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f7, f1 (x14)\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f20, f1 (x12)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x15)\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f2, f1 (x13)\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f1, f1 (x14)\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f8, f1 (x12)\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f11, f1 (x11)\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f1, f1 (x11)\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f26, f1 (x14)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f16, f1 (x11)\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f7, f1 (x12)\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f9, f1 (x11)\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f24, f1 (x13)\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f18, f1 (x13)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f1, f1 (x14)\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f30, f1 (x13)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f17, f1 (x13)\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f18, f1 (x12)\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f5, f1 (x13)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f18, f1 (x12)\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x11)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f26, f1 (x15)\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f9, f1 (x11)\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x11)\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f16, f1 (x11)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f10, f1 (x12)\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f13, f1 (x14)\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f20, f1 (x12)\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x13)\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f23, f1 (x15)\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f16, f1 (x11)\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f14, f1 (x11)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f12, f1 (x15)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f27, f1 (x14)\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f26, f1 (x15)\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f11, f1 (x12)\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f30, f1 (x11)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f4, f1 (x13)\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f6, f1 (x15)\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f13, f1 (x13)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f23, f1 (x12)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f0, f1 (x13)\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f25, f1 (x14)\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f14, f1 (x13)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f25, f1 (x13)\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f0, f1 (x13)\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f26, f1 (x15)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f26, f1 (x14)\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f4, f1 (x12)\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f4, f1 (x11)\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f23, f1 (x15)\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f15, f1 (x11)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f16, f1 (x13)\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f7, f1 (x15)\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f25, f1 (x13)\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f5, f1 (x12)\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f27, f1 (x12)\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f19, f1 (x15)\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f24, f1 (x15)\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f17, f1 (x11)\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f17, f1 (x15)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f0, f1 (x12)\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f15, f1 (x11)\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f23, f1 (x11)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f26, f1 (x14)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f5, f1 (x14)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f20, f1 (x15)\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f19, f1 (x13)\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f16, f1 (x13)\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f8, f1 (x14)\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f12, f1 (x12)\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x15)\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f2, f1 (x11)\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f11, f1 (x15)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f16, f1 (x13)\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f10, f1 (x14)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f8, f1 (x13)\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f20, f1 (x12)\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f4, f1 (x15)\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f1, f1 (x12)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f14, f1 (x13)\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f15, f1 (x14)\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f0, f1 (x11)\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f16, f1 (x15)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f4, f1 (x11)\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f4, f1 (x11)\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f28, f1 (x12)\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f19, f1 (x14)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x14)\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f19, f1 (x12)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f15, f1 (x14)\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f21, f1 (x12)\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f13, f1 (x13)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscw.ps f22, f1 (x15)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
