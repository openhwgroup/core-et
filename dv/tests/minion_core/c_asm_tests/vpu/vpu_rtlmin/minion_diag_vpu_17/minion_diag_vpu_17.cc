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
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x33333333,                                                  // 4 random values                             /// 00004
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00008
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00010
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00018
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0001c
			 0xffc00001,                                                  // -signaling NaN                              /// 00020
			 0x80000000,                                                  // -zero                                       /// 00024
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00028
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0002c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00030
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00038
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0003c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00040
			 0x0e000003,                                                  // Trailing 1s:                                /// 00044
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0004c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x4b000000,                                                  // 8388608.0                                   /// 00054
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00058
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0005c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00060
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0006c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00070
			 0xcb000000,                                                  // -8388608.0                                  /// 00074
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00078
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0007c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00084
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00090
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00098
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00100
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00104
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00110
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00118
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00124
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00128
			 0x4b000000,                                                  // 8388608.0                                   /// 0012c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00140
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00148
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0014c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00150
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00154
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00158
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00160
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00164
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0016c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00174
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0017c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00180
			 0x00011111,                                                  // 9.7958E-41                                  /// 00184
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00188
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0018c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00190
			 0x55555555,                                                  // 4 random values                             /// 00194
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00198
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0019c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001f4
			 0xff800000,                                                  // -inf                                        /// 001f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00200
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00204
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00208
			 0xbf028f5c,                                                  // -0.51                                       /// 0020c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00210
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00218
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00228
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0022c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00230
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00238
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0023c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00240
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00248
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0025c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00260
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x4b000000,                                                  // 8388608.0                                   /// 00268
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0026c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x7f800000,                                                  // inf                                         /// 00274
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00278
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0027c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00280
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00284
			 0x00011111,                                                  // 9.7958E-41                                  /// 00288
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00290
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00294
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00298
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0029c
			 0x7fc00001,                                                  // signaling NaN                               /// 002a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00300
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00308
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0030c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x3f028f5c,                                                  // 0.51                                        /// 00314
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00318
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00324
			 0x00000000,                                                  // zero                                        /// 00328
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0032c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00330
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00338
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00340
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00348
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0034c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00354
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00360
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00364
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00370
			 0x80011111,                                                  // -9.7958E-41                                 /// 00374
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00378
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00380
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00388
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0038c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00390
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00398
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0039c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x7fc00001,                                                  // signaling NaN                               /// 003cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0xffc00001,                                                  // -signaling NaN                              /// 003d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003e0
			 0x3f028f5c,                                                  // 0.51                                        /// 003e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00400
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00408
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00418
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0041c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00428
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00430
			 0x80000000,                                                  // -zero                                       /// 00434
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0043c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00440
			 0x0c780000,                                                  // Leading 1s:                                 /// 00444
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00448
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00458
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0045c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00464
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00468
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00470
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00478
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0047c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00484
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0048c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00490
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00494
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00498
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004e4
			 0x00000000,                                                  // zero                                        /// 004e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00500
			 0x0c400000,                                                  // Leading 1s:                                 /// 00504
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00508
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0050c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00510
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00514
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00520
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0052c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00530
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00538
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00544
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00548
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0054c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00550
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00554
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00558
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0055c
			 0x33333333,                                                  // 4 random values                             /// 00560
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00564
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00568
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0056c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00570
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00574
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00578
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00588
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0058c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00590
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00594
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00598
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0059c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00600
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00604
			 0x33333333,                                                  // 4 random values                             /// 00608
			 0x00000000,                                                  // zero                                        /// 0060c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00610
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00618
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0061c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00624
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00628
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00630
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00634
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00638
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0063c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00640
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00644
			 0x3f028f5c,                                                  // 0.51                                        /// 00648
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0064c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00650
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00658
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0065c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00660
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80000000,                                                  // -zero                                       /// 00674
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00678
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x55555555,                                                  // 4 random values                             /// 00684
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0e000003,                                                  // Trailing 1s:                                /// 0068c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00690
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00694
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0069c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00700
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0070c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80011111,                                                  // -9.7958E-41                                 /// 00714
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00728
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0072c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00730
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x55555555,                                                  // 4 random values                             /// 00738
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0073c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00740
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00748
			 0x3f028f5c,                                                  // 0.51                                        /// 0074c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00750
			 0x7f800000,                                                  // inf                                         /// 00754
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00760
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00764
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00768
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00770
			 0xffc00001,                                                  // -signaling NaN                              /// 00774
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00778
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00780
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00788
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00790
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00794
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00798
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0079c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007dc
			 0x55555555,                                                  // 4 random values                             /// 007e0
			 0x80000000,                                                  // -zero                                       /// 007e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00800
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00804
			 0x33333333,                                                  // 4 random values                             /// 00808
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00810
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00814
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00818
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0081c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00820
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00824
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00828
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0082c
			 0x33333333,                                                  // 4 random values                             /// 00830
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00834
			 0xffc00001,                                                  // -signaling NaN                              /// 00838
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0083c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00840
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0e000007,                                                  // Trailing 1s:                                /// 00848
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0084c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00850
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00854
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00858
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0085c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00864
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00868
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00870
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00874
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00878
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00880
			 0x7f800000,                                                  // inf                                         /// 00884
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0088c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0089c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d8
			 0xff800000,                                                  // -inf                                        /// 008dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00900
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00904
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0092c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00930
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00934
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x3f028f5c,                                                  // 0.51                                        /// 0093c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00944
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00948
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0094c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00954
			 0x0c400000,                                                  // Leading 1s:                                 /// 00958
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0095c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00960
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00968
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00974
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0097c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00984
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00988
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00990
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00998
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009ec
			 0x7fc00001,                                                  // signaling NaN                               /// 009f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ab0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00abc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ac0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00acc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ae8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00af4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b28
			 0xcb000000,                                                  // -8388608.0                                  /// 00b2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b9c
			 0x80000000,                                                  // -zero                                       /// 00ba0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bb8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bbc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bc8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00be0
			 0x7f800000,                                                  // inf                                         /// 00be4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bec
			 0xbf028f5c,                                                  // -0.51                                       /// 00bf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bf8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c78
			 0x7f800000,                                                  // inf                                         /// 00c7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ca8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cdc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ce0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ce4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cfc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d00
			 0x7f800000,                                                  // inf                                         /// 00d04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d08
			 0x80000000,                                                  // -zero                                       /// 00d0c
			 0x33333333,                                                  // 4 random values                             /// 00d10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00da0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00da8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00db4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dcc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00de0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00de4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00df0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0xbf028f5c,                                                  // -0.51                                       /// 00e28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e94
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e9c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ea0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ebc
			 0x7f800000,                                                  // inf                                         /// 00ec0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ec4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ec8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ecc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ed0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ed8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00edc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ef8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00efc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0xffc00001,                                                  // -signaling NaN                              /// 00f04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f14
			 0x33333333,                                                  // 4 random values                             /// 00f18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f20
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f38
			 0x33333333,                                                  // 4 random values                             /// 00f3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f64
			 0xbf028f5c,                                                  // -0.51                                       /// 00f68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x80000000,                                                  // -zero                                       /// 00f8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fa0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fa4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x33333333,                                                  // 4 random values                             /// 00fb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fc8
			 0x55555555,                                                  // 4 random values                             /// 00fcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fe0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x80800002                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00000
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00004
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00008
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0xbf028f5c,                                                  // -0.51                                       /// 00010
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00014
			 0x7fc00001,                                                  // signaling NaN                               /// 00018
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0001c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00020
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00024
			 0x80011111,                                                  // -9.7958E-41                                 /// 00028
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x80011111,                                                  // -9.7958E-41                                 /// 00038
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0003c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00040
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00048
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00050
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00058
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0005c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00060
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00064
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00068
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00070
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00074
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00080
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00088
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00094
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0009c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000f0
			 0x00000000,                                                  // zero                                        /// 000f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00100
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e000007,                                                  // Trailing 1s:                                /// 00108
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00114
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00118
			 0x33333333,                                                  // 4 random values                             /// 0011c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00124
			 0x0e000003,                                                  // Trailing 1s:                                /// 00128
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0013c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00144
			 0x4b000000,                                                  // 8388608.0                                   /// 00148
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0014c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00150
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00158
			 0xcb000000,                                                  // -8388608.0                                  /// 0015c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00160
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00164
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00168
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0016c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00174
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00178
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00180
			 0x80000000,                                                  // -zero                                       /// 00184
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0018c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00190
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00198
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0019c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x4b000000,                                                  // 8388608.0                                   /// 001f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00200
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00204
			 0x0c600000,                                                  // Leading 1s:                                 /// 00208
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00210
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00214
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00218
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0021c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00220
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00224
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00228
			 0xff800000,                                                  // -inf                                        /// 0022c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00230
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00234
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0023c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00240
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0024c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00250
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00260
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00264
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00268
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0026c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00274
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0xcb000000,                                                  // -8388608.0                                  /// 00288
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00290
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00294
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002d4
			 0x80000000,                                                  // -zero                                       /// 002d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x3f028f5c,                                                  // 0.51                                        /// 002f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0030c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00310
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0031c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00324
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0xbf028f5c,                                                  // -0.51                                       /// 0032c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00330
			 0x0e000001,                                                  // Trailing 1s:                                /// 00334
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00338
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0033c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00344
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00348
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0034c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00354
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00358
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0035c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00364
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00374
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0037c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00384
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0038c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00390
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00394
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00398
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0039c
			 0x33333333,                                                  // 4 random values                             /// 003a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003ec
			 0x3f028f5c,                                                  // 0.51                                        /// 003f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00400
			 0x4b000000,                                                  // 8388608.0                                   /// 00404
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00408
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0040c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00410
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00418
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0041c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00420
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00424
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0042c
			 0x33333333,                                                  // 4 random values                             /// 00430
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00434
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00438
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0043c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00440
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00448
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0044c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00458
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00460
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00470
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00474
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80011111,                                                  // -9.7958E-41                                 /// 0047c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00480
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00484
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0048c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0c600000,                                                  // Leading 1s:                                 /// 00498
			 0x0e000003,                                                  // Trailing 1s:                                /// 0049c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0xffc00001,                                                  // -signaling NaN                              /// 004e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00514
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00518
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0051c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00524
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00528
			 0x00011111,                                                  // 9.7958E-41                                  /// 0052c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x0e000003,                                                  // Trailing 1s:                                /// 00538
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00540
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00548
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0054c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00558
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0055c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00560
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00564
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00574
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00578
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0057c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00584
			 0x7f800000,                                                  // inf                                         /// 00588
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0058c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00590
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00594
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00598
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00608
			 0x0c400000,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00610
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00614
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00618
			 0xffc00001,                                                  // -signaling NaN                              /// 0061c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00620
			 0x7f800000,                                                  // inf                                         /// 00624
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00628
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0062c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00630
			 0x7f800000,                                                  // inf                                         /// 00634
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00638
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0063c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00640
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00644
			 0x00011111,                                                  // 9.7958E-41                                  /// 00648
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00664
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0066c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00674
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00678
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0067c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00684
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00688
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0068c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00694
			 0x4b000000,                                                  // 8388608.0                                   /// 00698
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0069c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x7fc00001,                                                  // signaling NaN                               /// 006b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0xffc00001,                                                  // -signaling NaN                              /// 006c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d0
			 0x7fc00001,                                                  // signaling NaN                               /// 006d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00700
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0070c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00710
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00720
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00724
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00728
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0072c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00734
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00738
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00740
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00748
			 0x00000000,                                                  // zero                                        /// 0074c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00754
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00758
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0075c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00760
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00764
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0076c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00774
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00778
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00780
			 0x4b000000,                                                  // 8388608.0                                   /// 00784
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00788
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00790
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00794
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00798
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0079c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0xff800000,                                                  // -inf                                        /// 007b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e4
			 0x33333333,                                                  // 4 random values                             /// 007e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00808
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00011111,                                                  // 9.7958E-41                                  /// 00818
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0081c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00820
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00824
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00828
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x7fc00001,                                                  // signaling NaN                               /// 00830
			 0xff800000,                                                  // -inf                                        /// 00834
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00838
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c700000,                                                  // Leading 1s:                                 /// 00844
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00848
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0084c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00858
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0085c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00860
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00864
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00868
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00870
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00874
			 0xbf028f5c,                                                  // -0.51                                       /// 00878
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0087c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00888
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0088c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00890
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00894
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00898
			 0x80011111,                                                  // -9.7958E-41                                 /// 0089c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008b4
			 0x3f028f5c,                                                  // 0.51                                        /// 008b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x4b000000,                                                  // 8388608.0                                   /// 008fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0xcb000000,                                                  // -8388608.0                                  /// 00904
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00908
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0090c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00910
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00914
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00918
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0091c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00920
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00924
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00928
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0092c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00930
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00934
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00938
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0093c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00940
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0094c
			 0x3f028f5c,                                                  // 0.51                                        /// 00950
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00954
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00958
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0095c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00968
			 0x0e000003,                                                  // Trailing 1s:                                /// 0096c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00970
			 0x00011111,                                                  // 9.7958E-41                                  /// 00974
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00978
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0097c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00980
			 0x55555555,                                                  // 4 random values                             /// 00984
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00988
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0098c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00990
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00994
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0099c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a8
			 0xbf028f5c,                                                  // -0.51                                       /// 009ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f8
			 0xcb000000,                                                  // -8388608.0                                  /// 009fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0xff800000,                                                  // -inf                                        /// 00a98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ab4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ac8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ad4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00adc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x00000000,                                                  // zero                                        /// 00ae8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00af0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00af8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00afc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b50
			 0x55555555,                                                  // 4 random values                             /// 00b54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80000000,                                                  // -zero                                       /// 00b90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0xff800000,                                                  // -inf                                        /// 00ba0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ba4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bcc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bf4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c10
			 0x7f800000,                                                  // inf                                         /// 00c14
			 0x3f028f5c,                                                  // 0.51                                        /// 00c18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c68
			 0x80000000,                                                  // -zero                                       /// 00c6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c74
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c80
			 0xcb000000,                                                  // -8388608.0                                  /// 00c84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cac
			 0x00000000,                                                  // zero                                        /// 00cb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xff800000,                                                  // -inf                                        /// 00cb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cc4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cc8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ccc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cdc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ce4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cf4
			 0x55555555,                                                  // 4 random values                             /// 00cf8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80000000,                                                  // -zero                                       /// 00d60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d64
			 0xcb000000,                                                  // -8388608.0                                  /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00da4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00de0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00de4
			 0xff800000,                                                  // -inf                                        /// 00de8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00df0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dfc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0xffc00001,                                                  // -signaling NaN                              /// 00e08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e40
			 0xcb000000,                                                  // -8388608.0                                  /// 00e44
			 0x4b000000,                                                  // 8388608.0                                   /// 00e48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ea4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eb0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ecc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ed4
			 0x33333333,                                                  // 4 random values                             /// 00ed8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x00000000,                                                  // zero                                        /// 00ee8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ef0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00efc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f1c
			 0x33333333,                                                  // 4 random values                             /// 00f20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fa8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fc0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fe4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ff4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ff8
			 0x0c7f0000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x4dfe8a74, /// 0x0
			 0x9bcaece8, /// 0x4
			 0xe9cc2d9e, /// 0x8
			 0x2751574d, /// 0xc
			 0x044bdd32, /// 0x10
			 0x6feafdce, /// 0x14
			 0xb083106d, /// 0x18
			 0x82dea90d, /// 0x1c
			 0xf58dd745, /// 0x20
			 0xe0b5f4c7, /// 0x24
			 0x331c4f96, /// 0x28
			 0x29543085, /// 0x2c
			 0xb615cb66, /// 0x30
			 0x6eeac4d7, /// 0x34
			 0x11902879, /// 0x38
			 0x00fc4b97, /// 0x3c
			 0xc37d4cf4, /// 0x40
			 0x9d0d5e68, /// 0x44
			 0x994b70da, /// 0x48
			 0x0e5c5e0c, /// 0x4c
			 0x2ba081ed, /// 0x50
			 0x86c3255d, /// 0x54
			 0x10e03c33, /// 0x58
			 0xce63159c, /// 0x5c
			 0xd504f211, /// 0x60
			 0xb373e971, /// 0x64
			 0x950122ae, /// 0x68
			 0x7e3b132d, /// 0x6c
			 0x1b221e2f, /// 0x70
			 0xd09ead47, /// 0x74
			 0x0418d05f, /// 0x78
			 0x23c5835b, /// 0x7c
			 0x4e484cba, /// 0x80
			 0x94504ebc, /// 0x84
			 0x2dd87251, /// 0x88
			 0x1932e6d7, /// 0x8c
			 0x9bc9b9ee, /// 0x90
			 0xc68c11f5, /// 0x94
			 0x1099b00e, /// 0x98
			 0x58bd54e4, /// 0x9c
			 0x515f93d5, /// 0xa0
			 0xc8f9cb35, /// 0xa4
			 0x4cae03f7, /// 0xa8
			 0xf808b5d6, /// 0xac
			 0x390aa772, /// 0xb0
			 0xea1d1409, /// 0xb4
			 0xb57a3ba3, /// 0xb8
			 0x36f5cf0f, /// 0xbc
			 0xc6b697b8, /// 0xc0
			 0x92c397a2, /// 0xc4
			 0x0912e413, /// 0xc8
			 0x67cb17a5, /// 0xcc
			 0x5cdcf489, /// 0xd0
			 0x4f57985b, /// 0xd4
			 0xb5f79d69, /// 0xd8
			 0x401aaa6d, /// 0xdc
			 0xc2ed54bb, /// 0xe0
			 0xb34c86b9, /// 0xe4
			 0xa2a8a420, /// 0xe8
			 0x6d81c8b3, /// 0xec
			 0x3f3b197a, /// 0xf0
			 0x6e2974b5, /// 0xf4
			 0x8c98fcf9, /// 0xf8
			 0x0dd80e49, /// 0xfc
			 0x5e405317, /// 0x100
			 0xf78ccc1a, /// 0x104
			 0xed060b1d, /// 0x108
			 0xa1fc5e20, /// 0x10c
			 0x67646818, /// 0x110
			 0x2caedf6e, /// 0x114
			 0x5ed814d4, /// 0x118
			 0x84b7ad5d, /// 0x11c
			 0x350ab543, /// 0x120
			 0x1b0dcc5c, /// 0x124
			 0xaa8ca735, /// 0x128
			 0xf5c6c3b0, /// 0x12c
			 0x20b97569, /// 0x130
			 0x503e003a, /// 0x134
			 0x73d03074, /// 0x138
			 0x27ab1dba, /// 0x13c
			 0x71202b40, /// 0x140
			 0xc8b86496, /// 0x144
			 0x74e6c50d, /// 0x148
			 0xec14d88b, /// 0x14c
			 0x05bb28dc, /// 0x150
			 0x0aec945b, /// 0x154
			 0x6363a3fc, /// 0x158
			 0x0f7c9edc, /// 0x15c
			 0xa656f344, /// 0x160
			 0xc36b61f8, /// 0x164
			 0x58690c24, /// 0x168
			 0x445bd768, /// 0x16c
			 0x00d67af2, /// 0x170
			 0x19347e91, /// 0x174
			 0x44f8de77, /// 0x178
			 0xe81c245d, /// 0x17c
			 0xb6cbfbde, /// 0x180
			 0x9eb133c6, /// 0x184
			 0xece07852, /// 0x188
			 0xe2257731, /// 0x18c
			 0x46d7b798, /// 0x190
			 0x8bf9ffa0, /// 0x194
			 0xcc53bafd, /// 0x198
			 0x47e2891c, /// 0x19c
			 0x94f52a54, /// 0x1a0
			 0xf3305f80, /// 0x1a4
			 0xdff7b285, /// 0x1a8
			 0xf7ccf755, /// 0x1ac
			 0x815ee96e, /// 0x1b0
			 0x171985d1, /// 0x1b4
			 0xdb1c7a96, /// 0x1b8
			 0x260a96d1, /// 0x1bc
			 0x3e498f5f, /// 0x1c0
			 0x72772e03, /// 0x1c4
			 0xfc8e2b94, /// 0x1c8
			 0x163b5ee0, /// 0x1cc
			 0xa933f49a, /// 0x1d0
			 0x678cbddb, /// 0x1d4
			 0x3220abde, /// 0x1d8
			 0xf4015b4a, /// 0x1dc
			 0x5446722f, /// 0x1e0
			 0x8dc98a01, /// 0x1e4
			 0x0f96379e, /// 0x1e8
			 0xf2b13a3b, /// 0x1ec
			 0x632499de, /// 0x1f0
			 0xcf2e70c5, /// 0x1f4
			 0x2acd4126, /// 0x1f8
			 0x65bcf5a6, /// 0x1fc
			 0x1eb79d31, /// 0x200
			 0x3f834fcd, /// 0x204
			 0xea884bde, /// 0x208
			 0x0133a703, /// 0x20c
			 0x2dc29eef, /// 0x210
			 0xd7ca0750, /// 0x214
			 0xdfb782b3, /// 0x218
			 0x63f20bf8, /// 0x21c
			 0x317b0729, /// 0x220
			 0xe03db788, /// 0x224
			 0x1f4e1f42, /// 0x228
			 0x7bae7ec5, /// 0x22c
			 0xbbf87460, /// 0x230
			 0xb377caf3, /// 0x234
			 0x03c6ecce, /// 0x238
			 0xa63f6b73, /// 0x23c
			 0xd38eb63c, /// 0x240
			 0xbdfd7852, /// 0x244
			 0xd9db9364, /// 0x248
			 0xca29171e, /// 0x24c
			 0x82de05c1, /// 0x250
			 0xb080aa39, /// 0x254
			 0xd87dd4b4, /// 0x258
			 0x0bbd2641, /// 0x25c
			 0xe7ddf8e9, /// 0x260
			 0x09d41cc1, /// 0x264
			 0xd96d1988, /// 0x268
			 0x321fe879, /// 0x26c
			 0x9ff95f6b, /// 0x270
			 0xf2c2b459, /// 0x274
			 0x81c58e4b, /// 0x278
			 0x0271e371, /// 0x27c
			 0xd7299097, /// 0x280
			 0x5d89bc25, /// 0x284
			 0x53eeb4ba, /// 0x288
			 0x050a183a, /// 0x28c
			 0x2608296f, /// 0x290
			 0x308ea11c, /// 0x294
			 0x5617fd8b, /// 0x298
			 0x92ace349, /// 0x29c
			 0x4a560033, /// 0x2a0
			 0xc9aa43f9, /// 0x2a4
			 0xddd7f870, /// 0x2a8
			 0x80569fbb, /// 0x2ac
			 0x403c9ed6, /// 0x2b0
			 0x257c83ee, /// 0x2b4
			 0x35444221, /// 0x2b8
			 0x099897fb, /// 0x2bc
			 0xc5984567, /// 0x2c0
			 0x86e83b41, /// 0x2c4
			 0xe41360ef, /// 0x2c8
			 0xac5c0cdf, /// 0x2cc
			 0x1cca8e53, /// 0x2d0
			 0x1abd056c, /// 0x2d4
			 0x68070f9a, /// 0x2d8
			 0x2c064c5a, /// 0x2dc
			 0xd359c9fe, /// 0x2e0
			 0x5812a5a9, /// 0x2e4
			 0x020152d7, /// 0x2e8
			 0x6f4a88e8, /// 0x2ec
			 0x6ff51970, /// 0x2f0
			 0x2a3690d4, /// 0x2f4
			 0x47300f8f, /// 0x2f8
			 0x96f6a11a, /// 0x2fc
			 0x861d185c, /// 0x300
			 0x0ba984c3, /// 0x304
			 0x0a336d52, /// 0x308
			 0xa228dc2f, /// 0x30c
			 0xc892a0a4, /// 0x310
			 0x37935adf, /// 0x314
			 0x9ed2f3e3, /// 0x318
			 0xe1ee4499, /// 0x31c
			 0x352bac81, /// 0x320
			 0x4c6cead2, /// 0x324
			 0x9e26ff7a, /// 0x328
			 0xb78e0ab3, /// 0x32c
			 0xf22a759b, /// 0x330
			 0xd55d8d3e, /// 0x334
			 0x0fa36d33, /// 0x338
			 0x8da37a49, /// 0x33c
			 0x896b2a62, /// 0x340
			 0x18f25a93, /// 0x344
			 0x3b4b27bb, /// 0x348
			 0x4c8db975, /// 0x34c
			 0x1554115d, /// 0x350
			 0xa40d5b2a, /// 0x354
			 0x856cb3be, /// 0x358
			 0xc3190b98, /// 0x35c
			 0xb8a1d214, /// 0x360
			 0x65811481, /// 0x364
			 0xb798da80, /// 0x368
			 0xe0a8cbff, /// 0x36c
			 0x39a21496, /// 0x370
			 0xd4bfef02, /// 0x374
			 0xe6b4c039, /// 0x378
			 0x8ae424bd, /// 0x37c
			 0xfa43a129, /// 0x380
			 0xdb69f87a, /// 0x384
			 0xc3fe04b0, /// 0x388
			 0xbc4234a7, /// 0x38c
			 0x9d73ec1d, /// 0x390
			 0x6e944d5c, /// 0x394
			 0x8eb7cf5e, /// 0x398
			 0xd30f8643, /// 0x39c
			 0x77680150, /// 0x3a0
			 0xf7438572, /// 0x3a4
			 0x3693c797, /// 0x3a8
			 0x92a283f1, /// 0x3ac
			 0x9178fe86, /// 0x3b0
			 0x90acb599, /// 0x3b4
			 0x55820d27, /// 0x3b8
			 0x307d98d1, /// 0x3bc
			 0x1346592c, /// 0x3c0
			 0xafa3c9ca, /// 0x3c4
			 0xd1205301, /// 0x3c8
			 0xa80a3b9c, /// 0x3cc
			 0xbaec6ae4, /// 0x3d0
			 0xae286f1f, /// 0x3d4
			 0xa0be1324, /// 0x3d8
			 0x655043ed, /// 0x3dc
			 0xe31c5534, /// 0x3e0
			 0x2f831a20, /// 0x3e4
			 0xe271d51b, /// 0x3e8
			 0x7f73e438, /// 0x3ec
			 0x3426b21a, /// 0x3f0
			 0xdccd739b, /// 0x3f4
			 0x858f0803, /// 0x3f8
			 0x947452a1, /// 0x3fc
			 0xe9f6b882, /// 0x400
			 0x1ecc976c, /// 0x404
			 0xb33803c9, /// 0x408
			 0xa159becf, /// 0x40c
			 0xd1b57c8b, /// 0x410
			 0xa0c18255, /// 0x414
			 0xfb0de139, /// 0x418
			 0x9a82de9b, /// 0x41c
			 0xc116e113, /// 0x420
			 0x865df2ec, /// 0x424
			 0x45d028db, /// 0x428
			 0xd92e30a0, /// 0x42c
			 0x57385b2f, /// 0x430
			 0x849bbc61, /// 0x434
			 0xdf982cd0, /// 0x438
			 0x7ff0bbd8, /// 0x43c
			 0x48e458b6, /// 0x440
			 0xb076eb28, /// 0x444
			 0x6a946aab, /// 0x448
			 0x4363c19d, /// 0x44c
			 0x53c8ca63, /// 0x450
			 0xe2656228, /// 0x454
			 0xcf2516da, /// 0x458
			 0x4be80a9a, /// 0x45c
			 0x88ba45a7, /// 0x460
			 0xcac96a0a, /// 0x464
			 0xb8991daa, /// 0x468
			 0x9db86b28, /// 0x46c
			 0x17fdb2cb, /// 0x470
			 0x7aed2055, /// 0x474
			 0xaf4421b3, /// 0x478
			 0xfc5879ed, /// 0x47c
			 0x7497660d, /// 0x480
			 0x6f757c27, /// 0x484
			 0x226f775d, /// 0x488
			 0xec345d8e, /// 0x48c
			 0xabf04691, /// 0x490
			 0x67b6305f, /// 0x494
			 0x3de578c0, /// 0x498
			 0xf85c846e, /// 0x49c
			 0x1cb65e04, /// 0x4a0
			 0xe6bb2ae9, /// 0x4a4
			 0x838e596c, /// 0x4a8
			 0x5d3777e9, /// 0x4ac
			 0x1cc9f7e8, /// 0x4b0
			 0x2fc33f26, /// 0x4b4
			 0x0c007df0, /// 0x4b8
			 0xa210e20f, /// 0x4bc
			 0xbc2222f1, /// 0x4c0
			 0x9ffb2d75, /// 0x4c4
			 0xe13c2a93, /// 0x4c8
			 0x5d6e7241, /// 0x4cc
			 0x8f4f487f, /// 0x4d0
			 0xe8138efc, /// 0x4d4
			 0x28143b84, /// 0x4d8
			 0x18c32408, /// 0x4dc
			 0x458d5f2c, /// 0x4e0
			 0x40a3c71a, /// 0x4e4
			 0x48e5d8c6, /// 0x4e8
			 0xc03194ab, /// 0x4ec
			 0xc5cc98d9, /// 0x4f0
			 0xa033b15b, /// 0x4f4
			 0x4b225d66, /// 0x4f8
			 0x88877dae, /// 0x4fc
			 0x9e1ec271, /// 0x500
			 0x564d8cbe, /// 0x504
			 0xa3203241, /// 0x508
			 0x555f2729, /// 0x50c
			 0x7ed46ae0, /// 0x510
			 0x59b2103b, /// 0x514
			 0x76a064bf, /// 0x518
			 0x7c4897da, /// 0x51c
			 0x0b33b92b, /// 0x520
			 0xfde73a35, /// 0x524
			 0xf47e2753, /// 0x528
			 0xbd467361, /// 0x52c
			 0x8260d763, /// 0x530
			 0x36d6e0d9, /// 0x534
			 0x737e54a2, /// 0x538
			 0xa3c843da, /// 0x53c
			 0x4647abea, /// 0x540
			 0x6127bf75, /// 0x544
			 0x937e80f6, /// 0x548
			 0x8ff767e7, /// 0x54c
			 0xe9b6c277, /// 0x550
			 0xf843e0f6, /// 0x554
			 0xe954d6c2, /// 0x558
			 0x08264a92, /// 0x55c
			 0xc6bbf16c, /// 0x560
			 0xd8f5e1a4, /// 0x564
			 0xb9a9bd75, /// 0x568
			 0x8aae7f9a, /// 0x56c
			 0x942a41be, /// 0x570
			 0xfb2378a5, /// 0x574
			 0xc963c172, /// 0x578
			 0x8d91da85, /// 0x57c
			 0x7ff30af9, /// 0x580
			 0x28b98ab1, /// 0x584
			 0xf81cb6dd, /// 0x588
			 0xe99889e3, /// 0x58c
			 0x4118c95d, /// 0x590
			 0xe082b82f, /// 0x594
			 0x663ae656, /// 0x598
			 0xdf8dd7b4, /// 0x59c
			 0x762f5fac, /// 0x5a0
			 0x245f3a39, /// 0x5a4
			 0x0b8a8d82, /// 0x5a8
			 0x43a8c305, /// 0x5ac
			 0x10b6f3aa, /// 0x5b0
			 0xd52fde4d, /// 0x5b4
			 0x13f3726c, /// 0x5b8
			 0xc370fdcc, /// 0x5bc
			 0x08c2e876, /// 0x5c0
			 0x8160acdc, /// 0x5c4
			 0xf7fce2fd, /// 0x5c8
			 0xa9c92241, /// 0x5cc
			 0x170f2185, /// 0x5d0
			 0xe8ee1308, /// 0x5d4
			 0xf5e5b9e6, /// 0x5d8
			 0x12a208dc, /// 0x5dc
			 0x8f79e268, /// 0x5e0
			 0xdb877b7e, /// 0x5e4
			 0x5e36f924, /// 0x5e8
			 0x430ad965, /// 0x5ec
			 0x4e6838e6, /// 0x5f0
			 0x09f3cbe4, /// 0x5f4
			 0x136d1634, /// 0x5f8
			 0x3a4cbb77, /// 0x5fc
			 0x7441adf9, /// 0x600
			 0x0a2f58a9, /// 0x604
			 0x4d357ab6, /// 0x608
			 0xfa79c8d8, /// 0x60c
			 0x335f0094, /// 0x610
			 0x9c034cb3, /// 0x614
			 0x0a1f7382, /// 0x618
			 0xe7129f75, /// 0x61c
			 0x06ac9dee, /// 0x620
			 0x0b506311, /// 0x624
			 0xcb4454b0, /// 0x628
			 0xa103686d, /// 0x62c
			 0x838ba664, /// 0x630
			 0x6334abd4, /// 0x634
			 0xf3e1e303, /// 0x638
			 0xbd1ca0fd, /// 0x63c
			 0x93b39884, /// 0x640
			 0x8571b4ac, /// 0x644
			 0x91c4f204, /// 0x648
			 0x24a52398, /// 0x64c
			 0xddbc2ceb, /// 0x650
			 0xd9f343ff, /// 0x654
			 0x468bc273, /// 0x658
			 0x106838f4, /// 0x65c
			 0xefd1e5d0, /// 0x660
			 0x13158d6c, /// 0x664
			 0x5dea26e9, /// 0x668
			 0x617b8779, /// 0x66c
			 0x9d711e9c, /// 0x670
			 0x0bea3be7, /// 0x674
			 0x7e65ba82, /// 0x678
			 0xf7a263f4, /// 0x67c
			 0xa4b08857, /// 0x680
			 0x34a152bd, /// 0x684
			 0x8cb3b14b, /// 0x688
			 0x1abe157c, /// 0x68c
			 0x806fe575, /// 0x690
			 0xbd66028e, /// 0x694
			 0xa5c3b543, /// 0x698
			 0x21cc9853, /// 0x69c
			 0x7daa6ec8, /// 0x6a0
			 0x44c63b8c, /// 0x6a4
			 0xda939f60, /// 0x6a8
			 0x89473142, /// 0x6ac
			 0xb77ec8f4, /// 0x6b0
			 0xce052c1c, /// 0x6b4
			 0x8518ad9a, /// 0x6b8
			 0x2b4c2aca, /// 0x6bc
			 0xd9ef4d48, /// 0x6c0
			 0xe35b1fb4, /// 0x6c4
			 0x1462b5c8, /// 0x6c8
			 0x461b519a, /// 0x6cc
			 0xc23bcce6, /// 0x6d0
			 0x14540e85, /// 0x6d4
			 0x214e859b, /// 0x6d8
			 0x5b21c8b0, /// 0x6dc
			 0x06ca7008, /// 0x6e0
			 0xf284464c, /// 0x6e4
			 0x0d7b469b, /// 0x6e8
			 0xbc096417, /// 0x6ec
			 0x280999e3, /// 0x6f0
			 0xed160459, /// 0x6f4
			 0xc5243b83, /// 0x6f8
			 0xc4b2082d, /// 0x6fc
			 0x48927e01, /// 0x700
			 0xcae41a34, /// 0x704
			 0x2ba3e751, /// 0x708
			 0xaa64e274, /// 0x70c
			 0x793d6a7b, /// 0x710
			 0x126aa3f0, /// 0x714
			 0x50ed5d1b, /// 0x718
			 0x7a2dabb7, /// 0x71c
			 0x4d8f5620, /// 0x720
			 0xa66b1d1f, /// 0x724
			 0x52c6c954, /// 0x728
			 0x26066085, /// 0x72c
			 0xfa965bdd, /// 0x730
			 0x707840db, /// 0x734
			 0xf5f295e5, /// 0x738
			 0xc9df7bab, /// 0x73c
			 0x6961d74f, /// 0x740
			 0x84a94158, /// 0x744
			 0x7c7bdbc5, /// 0x748
			 0x289cf6f8, /// 0x74c
			 0xd03bea22, /// 0x750
			 0xb696a918, /// 0x754
			 0x3cf7fa74, /// 0x758
			 0xfcf212f1, /// 0x75c
			 0x4c3fa1b7, /// 0x760
			 0x1f7cb9fe, /// 0x764
			 0x113ad848, /// 0x768
			 0xb697766d, /// 0x76c
			 0x48ce5763, /// 0x770
			 0x1fd25525, /// 0x774
			 0xe95603fc, /// 0x778
			 0xc779b18c, /// 0x77c
			 0x6301b812, /// 0x780
			 0xa92e1528, /// 0x784
			 0x6a5a3601, /// 0x788
			 0xbdf73de3, /// 0x78c
			 0xcf1f85b5, /// 0x790
			 0xef4f8462, /// 0x794
			 0x2da47b19, /// 0x798
			 0x9a4284c4, /// 0x79c
			 0x210f606a, /// 0x7a0
			 0xff1be7a6, /// 0x7a4
			 0xc177fb5f, /// 0x7a8
			 0x0ce6bf61, /// 0x7ac
			 0x2e8e7b92, /// 0x7b0
			 0x1b93b19f, /// 0x7b4
			 0x83fb19d1, /// 0x7b8
			 0x3d55e444, /// 0x7bc
			 0x4e66d52c, /// 0x7c0
			 0x55f20415, /// 0x7c4
			 0x82a89acd, /// 0x7c8
			 0x603b31c4, /// 0x7cc
			 0x81d04a3b, /// 0x7d0
			 0x71a87df6, /// 0x7d4
			 0x22cfbce9, /// 0x7d8
			 0xef2acf05, /// 0x7dc
			 0x5453738a, /// 0x7e0
			 0x1445a641, /// 0x7e4
			 0xd6021e1d, /// 0x7e8
			 0xb2b81378, /// 0x7ec
			 0x43d4fc4d, /// 0x7f0
			 0xba0a5e96, /// 0x7f4
			 0x16175d76, /// 0x7f8
			 0x556a0d21, /// 0x7fc
			 0xf581ab2c, /// 0x800
			 0x8fcf42bf, /// 0x804
			 0x07c8efbc, /// 0x808
			 0xe0adf6d6, /// 0x80c
			 0x45f82606, /// 0x810
			 0xb4a800b2, /// 0x814
			 0xf08e769c, /// 0x818
			 0x310a930b, /// 0x81c
			 0x0f718138, /// 0x820
			 0xd4c80c1e, /// 0x824
			 0x661089f5, /// 0x828
			 0x5d7356c3, /// 0x82c
			 0x0aac5311, /// 0x830
			 0x1f92a9e7, /// 0x834
			 0xc5bb1cdd, /// 0x838
			 0x0e68bf83, /// 0x83c
			 0xff2a9b95, /// 0x840
			 0x54090398, /// 0x844
			 0xa1a0251a, /// 0x848
			 0x4e10275c, /// 0x84c
			 0x1d5f3365, /// 0x850
			 0xf177d84a, /// 0x854
			 0x6f4c82b0, /// 0x858
			 0x8401e728, /// 0x85c
			 0x47eb8169, /// 0x860
			 0x25e33483, /// 0x864
			 0x18f6c0b4, /// 0x868
			 0xe31deb49, /// 0x86c
			 0xc62d00e1, /// 0x870
			 0xeba262f6, /// 0x874
			 0xf6e0e11c, /// 0x878
			 0xec200712, /// 0x87c
			 0x1b2d66b2, /// 0x880
			 0x97606e9a, /// 0x884
			 0x2c41c856, /// 0x888
			 0xa7138cef, /// 0x88c
			 0x492bd258, /// 0x890
			 0x008cdc64, /// 0x894
			 0x3acb515e, /// 0x898
			 0x3de11db8, /// 0x89c
			 0xa2f00003, /// 0x8a0
			 0x6ec91e04, /// 0x8a4
			 0xc999f079, /// 0x8a8
			 0x4e77fa7a, /// 0x8ac
			 0x982547e3, /// 0x8b0
			 0x535c1154, /// 0x8b4
			 0x686e75d9, /// 0x8b8
			 0x099b0cb0, /// 0x8bc
			 0x305be3f7, /// 0x8c0
			 0xd7133a03, /// 0x8c4
			 0x712394cf, /// 0x8c8
			 0xedf9d2cf, /// 0x8cc
			 0xeb2c131c, /// 0x8d0
			 0x83f34d28, /// 0x8d4
			 0x65943844, /// 0x8d8
			 0x170fbbdd, /// 0x8dc
			 0xd91099c2, /// 0x8e0
			 0x1391f467, /// 0x8e4
			 0xd370dd82, /// 0x8e8
			 0x716ed74a, /// 0x8ec
			 0xff4e6e46, /// 0x8f0
			 0x7d650d7a, /// 0x8f4
			 0x86aadc88, /// 0x8f8
			 0x6270c1e4, /// 0x8fc
			 0xb2b6079d, /// 0x900
			 0xf98f72a9, /// 0x904
			 0xa4dbba83, /// 0x908
			 0x2ec60e87, /// 0x90c
			 0xf8694cb5, /// 0x910
			 0x4189d5a5, /// 0x914
			 0x64c60b3e, /// 0x918
			 0x1a642394, /// 0x91c
			 0x1e0ac9c3, /// 0x920
			 0x5f4126c8, /// 0x924
			 0x07d7b46d, /// 0x928
			 0x748a39ea, /// 0x92c
			 0xc7d18fdd, /// 0x930
			 0x200f13d4, /// 0x934
			 0x88fb5f8c, /// 0x938
			 0xdec3253d, /// 0x93c
			 0x0360c867, /// 0x940
			 0xb0d9cc9c, /// 0x944
			 0x6e562a2e, /// 0x948
			 0x5025d642, /// 0x94c
			 0xf23322a6, /// 0x950
			 0xd115f27f, /// 0x954
			 0x7d0f1858, /// 0x958
			 0xabdd4c7a, /// 0x95c
			 0x0a8d77c2, /// 0x960
			 0x9a764f34, /// 0x964
			 0x640b34c2, /// 0x968
			 0xc74eb038, /// 0x96c
			 0xab536ad6, /// 0x970
			 0x17057d2e, /// 0x974
			 0xba2fbc16, /// 0x978
			 0x5f8935d9, /// 0x97c
			 0x27ffbd99, /// 0x980
			 0xd2c44f3b, /// 0x984
			 0x54847f6a, /// 0x988
			 0x43dd9978, /// 0x98c
			 0xdeb00b3f, /// 0x990
			 0xd78a4e14, /// 0x994
			 0x3d930a30, /// 0x998
			 0xeee18845, /// 0x99c
			 0x901311e5, /// 0x9a0
			 0x64914e10, /// 0x9a4
			 0x890c055c, /// 0x9a8
			 0x7a542bb0, /// 0x9ac
			 0x819eaded, /// 0x9b0
			 0x642b24a3, /// 0x9b4
			 0x0271b4b8, /// 0x9b8
			 0x47d42347, /// 0x9bc
			 0x11d151f1, /// 0x9c0
			 0xf5903dc0, /// 0x9c4
			 0xbd697970, /// 0x9c8
			 0xcf8c23a7, /// 0x9cc
			 0x8e506eb0, /// 0x9d0
			 0x826b60b4, /// 0x9d4
			 0xf935a0d0, /// 0x9d8
			 0xaa6d8e71, /// 0x9dc
			 0x49c1787d, /// 0x9e0
			 0x28b3de0f, /// 0x9e4
			 0xebf67a28, /// 0x9e8
			 0x0be8d74a, /// 0x9ec
			 0x8a22727b, /// 0x9f0
			 0x9bc7994f, /// 0x9f4
			 0x4f30b457, /// 0x9f8
			 0x36e2f970, /// 0x9fc
			 0xee9f7aca, /// 0xa00
			 0x24c2265e, /// 0xa04
			 0x0d734abb, /// 0xa08
			 0x8d990deb, /// 0xa0c
			 0x6eb1e1c9, /// 0xa10
			 0x9734f63c, /// 0xa14
			 0xa3615f4c, /// 0xa18
			 0xc4835c7e, /// 0xa1c
			 0x2f25a28c, /// 0xa20
			 0xc365c1fc, /// 0xa24
			 0x1d20612d, /// 0xa28
			 0xdc1e03c6, /// 0xa2c
			 0x59fafb52, /// 0xa30
			 0x1295d93d, /// 0xa34
			 0xfb2a1492, /// 0xa38
			 0xaa319ec3, /// 0xa3c
			 0xc9bfcd9b, /// 0xa40
			 0x37f4892c, /// 0xa44
			 0x2689eaa6, /// 0xa48
			 0x1c040872, /// 0xa4c
			 0x7eb1dcd3, /// 0xa50
			 0xd0abf0e7, /// 0xa54
			 0x6df12260, /// 0xa58
			 0xf627085e, /// 0xa5c
			 0x47cf4ad7, /// 0xa60
			 0x179221ff, /// 0xa64
			 0xd3a65d3b, /// 0xa68
			 0x68fd5ea0, /// 0xa6c
			 0x371d797b, /// 0xa70
			 0x8dcec34c, /// 0xa74
			 0xfce85aa9, /// 0xa78
			 0x4c7c5433, /// 0xa7c
			 0x5954d32c, /// 0xa80
			 0xa82bb945, /// 0xa84
			 0x4d6dc5dd, /// 0xa88
			 0x5faa7f9b, /// 0xa8c
			 0x201d2c78, /// 0xa90
			 0x6714b7a3, /// 0xa94
			 0xb368dd2f, /// 0xa98
			 0xb0ef60fe, /// 0xa9c
			 0x57a688ef, /// 0xaa0
			 0x2e912d74, /// 0xaa4
			 0xbc450412, /// 0xaa8
			 0x545b7d19, /// 0xaac
			 0xe7cc26e2, /// 0xab0
			 0xa4a39328, /// 0xab4
			 0x8ed36022, /// 0xab8
			 0xa0b79315, /// 0xabc
			 0x79f1b19d, /// 0xac0
			 0xf52b66b7, /// 0xac4
			 0x7d9ba053, /// 0xac8
			 0x65ae29fe, /// 0xacc
			 0x6230fda2, /// 0xad0
			 0xc384f8ba, /// 0xad4
			 0xdf1efd8e, /// 0xad8
			 0x5f82609a, /// 0xadc
			 0x191ba052, /// 0xae0
			 0xc4e26c85, /// 0xae4
			 0x8b89a1cc, /// 0xae8
			 0x6a79d1e0, /// 0xaec
			 0xc3754c05, /// 0xaf0
			 0x6d79df76, /// 0xaf4
			 0xae807281, /// 0xaf8
			 0x8d7f645a, /// 0xafc
			 0xda53d096, /// 0xb00
			 0x2ee36672, /// 0xb04
			 0x6e28b6e0, /// 0xb08
			 0x111bc860, /// 0xb0c
			 0xe8d575b9, /// 0xb10
			 0x9644e179, /// 0xb14
			 0x97350ab4, /// 0xb18
			 0xca774e43, /// 0xb1c
			 0xba86ad9f, /// 0xb20
			 0x0a6d1e96, /// 0xb24
			 0x4d68dafa, /// 0xb28
			 0x40cea906, /// 0xb2c
			 0xdf43e2f8, /// 0xb30
			 0x57c4a74e, /// 0xb34
			 0xddd3d90e, /// 0xb38
			 0xfd5d0618, /// 0xb3c
			 0xe93bd220, /// 0xb40
			 0xa3a271a0, /// 0xb44
			 0x0ac444b9, /// 0xb48
			 0x4b053b2b, /// 0xb4c
			 0x8b22beb9, /// 0xb50
			 0x2394b11b, /// 0xb54
			 0xafe7526e, /// 0xb58
			 0x8f5dcd4f, /// 0xb5c
			 0x7ae42c09, /// 0xb60
			 0x380edcb4, /// 0xb64
			 0xe47b0942, /// 0xb68
			 0xc09cff6b, /// 0xb6c
			 0x1be2fcd2, /// 0xb70
			 0xf414159e, /// 0xb74
			 0xbe225c09, /// 0xb78
			 0xa22cd07a, /// 0xb7c
			 0x389fab92, /// 0xb80
			 0x710b8dce, /// 0xb84
			 0xd008903c, /// 0xb88
			 0x206d4ec9, /// 0xb8c
			 0x2658ba0f, /// 0xb90
			 0x70303a6a, /// 0xb94
			 0x5648bc1f, /// 0xb98
			 0x588b270d, /// 0xb9c
			 0xf43b17c1, /// 0xba0
			 0xb9a7142d, /// 0xba4
			 0x32f18d6e, /// 0xba8
			 0xf97bc105, /// 0xbac
			 0x86475dec, /// 0xbb0
			 0x4ae0388a, /// 0xbb4
			 0x8164462a, /// 0xbb8
			 0xa87137b3, /// 0xbbc
			 0x7f104578, /// 0xbc0
			 0xa6fb1142, /// 0xbc4
			 0xd48e1771, /// 0xbc8
			 0x27079bf9, /// 0xbcc
			 0x7e01f5c1, /// 0xbd0
			 0x79852739, /// 0xbd4
			 0x821531ef, /// 0xbd8
			 0x397f2e77, /// 0xbdc
			 0x1fa94c18, /// 0xbe0
			 0x4206a801, /// 0xbe4
			 0xb7c3e1ad, /// 0xbe8
			 0x67a8933c, /// 0xbec
			 0x8ae576af, /// 0xbf0
			 0x64cbbfd8, /// 0xbf4
			 0x20d3abe8, /// 0xbf8
			 0x3b0044dc, /// 0xbfc
			 0x8d534e23, /// 0xc00
			 0x3ae52b5e, /// 0xc04
			 0x9eb53849, /// 0xc08
			 0x38685afc, /// 0xc0c
			 0x139d9cd9, /// 0xc10
			 0x85f90680, /// 0xc14
			 0xa3c469bf, /// 0xc18
			 0x9248cf0b, /// 0xc1c
			 0xa00e2ab1, /// 0xc20
			 0x552fb9e5, /// 0xc24
			 0x5076c3fe, /// 0xc28
			 0xd23b6cfa, /// 0xc2c
			 0x2c75f5ca, /// 0xc30
			 0x915149f7, /// 0xc34
			 0x142b0470, /// 0xc38
			 0xf98ec019, /// 0xc3c
			 0xe1affa12, /// 0xc40
			 0x8eab8541, /// 0xc44
			 0x1d027340, /// 0xc48
			 0xd6131bbc, /// 0xc4c
			 0xd8d7ea8a, /// 0xc50
			 0xa93c3d1a, /// 0xc54
			 0xca90acf5, /// 0xc58
			 0xad045e60, /// 0xc5c
			 0xae96ddd9, /// 0xc60
			 0x74bc0dc0, /// 0xc64
			 0xca2ef39b, /// 0xc68
			 0x3731d33e, /// 0xc6c
			 0xfe6a5a47, /// 0xc70
			 0x2ab503f3, /// 0xc74
			 0xfcccb9e5, /// 0xc78
			 0x0557d06a, /// 0xc7c
			 0x42b97a71, /// 0xc80
			 0xb249f958, /// 0xc84
			 0x6e4c7cf5, /// 0xc88
			 0x7ecde431, /// 0xc8c
			 0x509c66a4, /// 0xc90
			 0x06bb3337, /// 0xc94
			 0xa149bd71, /// 0xc98
			 0x093218db, /// 0xc9c
			 0x0140301f, /// 0xca0
			 0x1014748e, /// 0xca4
			 0x6e4388e8, /// 0xca8
			 0x1d877d3f, /// 0xcac
			 0x915d810f, /// 0xcb0
			 0xe7c73cef, /// 0xcb4
			 0xc8a134ff, /// 0xcb8
			 0x17671225, /// 0xcbc
			 0x0e3b1c3c, /// 0xcc0
			 0xd671894f, /// 0xcc4
			 0x474f5dd8, /// 0xcc8
			 0xd4d12d91, /// 0xccc
			 0x6aa722ed, /// 0xcd0
			 0x1ad9eebd, /// 0xcd4
			 0xf7b79b2f, /// 0xcd8
			 0xcd20c942, /// 0xcdc
			 0xd1f472f3, /// 0xce0
			 0xb5c264bf, /// 0xce4
			 0xe3ed16f8, /// 0xce8
			 0xfcf88d90, /// 0xcec
			 0xeded90a0, /// 0xcf0
			 0x8baab41d, /// 0xcf4
			 0x8a5ddda5, /// 0xcf8
			 0x904d8a27, /// 0xcfc
			 0xa06d7633, /// 0xd00
			 0xd1a95c3f, /// 0xd04
			 0x6fc6e3e6, /// 0xd08
			 0x0c7f046c, /// 0xd0c
			 0x91f5cce9, /// 0xd10
			 0xb744d9fe, /// 0xd14
			 0xf0bd4727, /// 0xd18
			 0xab61b01f, /// 0xd1c
			 0xf5c3bcb9, /// 0xd20
			 0x2355ff8a, /// 0xd24
			 0x0d099fa5, /// 0xd28
			 0xf55d3df5, /// 0xd2c
			 0xa8f6f6dd, /// 0xd30
			 0xaa1aa567, /// 0xd34
			 0xcd02468e, /// 0xd38
			 0x63c3a8be, /// 0xd3c
			 0x39efeba6, /// 0xd40
			 0x91fb494d, /// 0xd44
			 0x92ef820b, /// 0xd48
			 0x608de0e6, /// 0xd4c
			 0xb4a6620b, /// 0xd50
			 0x19ef3da2, /// 0xd54
			 0x41f22e12, /// 0xd58
			 0xf0536320, /// 0xd5c
			 0xc1b7d196, /// 0xd60
			 0xe6a48b80, /// 0xd64
			 0xd7040548, /// 0xd68
			 0x3b5f0ce3, /// 0xd6c
			 0xb1bc184b, /// 0xd70
			 0x5a9f7ddf, /// 0xd74
			 0x899f2ea4, /// 0xd78
			 0x3218dd0c, /// 0xd7c
			 0xacf5b18c, /// 0xd80
			 0x1090a60b, /// 0xd84
			 0x6bbac6aa, /// 0xd88
			 0xaf4dbd8e, /// 0xd8c
			 0xc0c14e00, /// 0xd90
			 0x0b94ef8d, /// 0xd94
			 0xccf704eb, /// 0xd98
			 0x75b46d66, /// 0xd9c
			 0xc6e177e9, /// 0xda0
			 0x11a3876a, /// 0xda4
			 0x00f3355b, /// 0xda8
			 0xcb154bd6, /// 0xdac
			 0x0e5a604b, /// 0xdb0
			 0x1d907603, /// 0xdb4
			 0x6d05f4f1, /// 0xdb8
			 0x4d42e390, /// 0xdbc
			 0x60875f32, /// 0xdc0
			 0x4ad38caf, /// 0xdc4
			 0x60630849, /// 0xdc8
			 0xf93dc837, /// 0xdcc
			 0xa67c084a, /// 0xdd0
			 0xe20e0295, /// 0xdd4
			 0x50f9d05c, /// 0xdd8
			 0xb2473f7f, /// 0xddc
			 0x3b38abf3, /// 0xde0
			 0xd49ef172, /// 0xde4
			 0x3dbb470c, /// 0xde8
			 0x6c96a91f, /// 0xdec
			 0x01613c1d, /// 0xdf0
			 0xa042b459, /// 0xdf4
			 0xfd0b2982, /// 0xdf8
			 0xc40aa684, /// 0xdfc
			 0x8ca22177, /// 0xe00
			 0x29520c0d, /// 0xe04
			 0xe28636f2, /// 0xe08
			 0xd308b974, /// 0xe0c
			 0xb075b2e3, /// 0xe10
			 0x05fd449e, /// 0xe14
			 0xc220a7f1, /// 0xe18
			 0x582a9a00, /// 0xe1c
			 0x86eff57e, /// 0xe20
			 0x1c7476dd, /// 0xe24
			 0x55f11398, /// 0xe28
			 0x92e8000e, /// 0xe2c
			 0x156d4bdb, /// 0xe30
			 0x9d78b6fa, /// 0xe34
			 0x90aa9e8d, /// 0xe38
			 0xadfd36dd, /// 0xe3c
			 0x0c984712, /// 0xe40
			 0x99b206af, /// 0xe44
			 0xbf6b7be1, /// 0xe48
			 0x5156b3f8, /// 0xe4c
			 0x9a2423da, /// 0xe50
			 0x3a585fff, /// 0xe54
			 0x82982603, /// 0xe58
			 0xcb00fc21, /// 0xe5c
			 0xb4ed2a40, /// 0xe60
			 0x652d9a38, /// 0xe64
			 0x7bf210d0, /// 0xe68
			 0x431eb46c, /// 0xe6c
			 0x5d0a1ecd, /// 0xe70
			 0xa0ae7e27, /// 0xe74
			 0x440dfc62, /// 0xe78
			 0x29b71c8c, /// 0xe7c
			 0x641a6e5b, /// 0xe80
			 0xffbba1bf, /// 0xe84
			 0x20d01978, /// 0xe88
			 0x58b01930, /// 0xe8c
			 0x7061b9c8, /// 0xe90
			 0x505428f7, /// 0xe94
			 0xec78d532, /// 0xe98
			 0x79eb2b85, /// 0xe9c
			 0x687d6e55, /// 0xea0
			 0xd7be249f, /// 0xea4
			 0x7f14b078, /// 0xea8
			 0x76f37d82, /// 0xeac
			 0x63f6e9df, /// 0xeb0
			 0xcc4f642d, /// 0xeb4
			 0x830dbf4f, /// 0xeb8
			 0xf9a2fe4d, /// 0xebc
			 0x7564fafc, /// 0xec0
			 0xefa4f211, /// 0xec4
			 0x8ae489cb, /// 0xec8
			 0xac1126a4, /// 0xecc
			 0x30297dc6, /// 0xed0
			 0x06f7f34d, /// 0xed4
			 0x8e0d2940, /// 0xed8
			 0x3077471f, /// 0xedc
			 0xa59c1ed0, /// 0xee0
			 0x06c8440c, /// 0xee4
			 0x3235a31d, /// 0xee8
			 0xb21c07b0, /// 0xeec
			 0x2a17fbe8, /// 0xef0
			 0xb6d17f47, /// 0xef4
			 0xaf3edf8b, /// 0xef8
			 0x86d4d6b6, /// 0xefc
			 0x1f29bc52, /// 0xf00
			 0x401889a8, /// 0xf04
			 0xcc14e875, /// 0xf08
			 0x7d272c1d, /// 0xf0c
			 0xbf4be833, /// 0xf10
			 0x7a7c28ff, /// 0xf14
			 0x838b48fb, /// 0xf18
			 0x0235dedb, /// 0xf1c
			 0x82d3e7af, /// 0xf20
			 0x6dde48b7, /// 0xf24
			 0x3fb3da60, /// 0xf28
			 0xfe392992, /// 0xf2c
			 0xda01fb3b, /// 0xf30
			 0x23836cf6, /// 0xf34
			 0xdbcf09ab, /// 0xf38
			 0xb414ae16, /// 0xf3c
			 0x99460a22, /// 0xf40
			 0x6960131f, /// 0xf44
			 0x27c0091b, /// 0xf48
			 0x9f35e62b, /// 0xf4c
			 0xa40246f7, /// 0xf50
			 0xc52e8b16, /// 0xf54
			 0xf1028770, /// 0xf58
			 0xff865784, /// 0xf5c
			 0x4cdfa26b, /// 0xf60
			 0xf0b238ae, /// 0xf64
			 0x8a42b7bc, /// 0xf68
			 0x9bb56fdd, /// 0xf6c
			 0xbc7fd28d, /// 0xf70
			 0x07269ff0, /// 0xf74
			 0xe1c95b3d, /// 0xf78
			 0x54a58c30, /// 0xf7c
			 0xb5f9d630, /// 0xf80
			 0x86cf4efd, /// 0xf84
			 0xd4fbbd27, /// 0xf88
			 0x827da303, /// 0xf8c
			 0x08019f5e, /// 0xf90
			 0x1731e373, /// 0xf94
			 0x50b4d268, /// 0xf98
			 0xa85d0263, /// 0xf9c
			 0x7df34617, /// 0xfa0
			 0xbcb91391, /// 0xfa4
			 0x1d0ba1b0, /// 0xfa8
			 0x920023d8, /// 0xfac
			 0xe669a201, /// 0xfb0
			 0xd71ecc24, /// 0xfb4
			 0xcc447f11, /// 0xfb8
			 0x5fc2a815, /// 0xfbc
			 0xb2043ef7, /// 0xfc0
			 0xcca5b6d8, /// 0xfc4
			 0xa25e7dfa, /// 0xfc8
			 0x9ba10427, /// 0xfcc
			 0x449ef983, /// 0xfd0
			 0x1fa51b3d, /// 0xfd4
			 0x10111092, /// 0xfd8
			 0xcca3ba11, /// 0xfdc
			 0x71ffaf5c, /// 0xfe0
			 0xddca0e37, /// 0xfe4
			 0xbeaa558f, /// 0xfe8
			 0x5aa2c7ad, /// 0xfec
			 0x48daec79, /// 0xff0
			 0x38962b73, /// 0xff4
			 0x90f39114, /// 0xff8
			 0xa6d2594a /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00008
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00014
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00018
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00020
			 0x0c780000,                                                  // Leading 1s:                                 /// 00024
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00028
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0002c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x7fc00001,                                                  // signaling NaN                               /// 00034
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0xffc00001,                                                  // -signaling NaN                              /// 00044
			 0xcb000000,                                                  // -8388608.0                                  /// 00048
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00054
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00070
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00074
			 0x0c780000,                                                  // Leading 1s:                                 /// 00078
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0007c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00088
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0008c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00090
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00094
			 0xbf028f5c,                                                  // -0.51                                       /// 00098
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0009c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000cc
			 0x7fc00001,                                                  // signaling NaN                               /// 000d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x7f800000,                                                  // inf                                         /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x55555555,                                                  // 4 random values                             /// 000fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00100
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00104
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00108
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0010c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00114
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00120
			 0x0c780000,                                                  // Leading 1s:                                 /// 00124
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x3f028f5c,                                                  // 0.51                                        /// 00134
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0013c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00144
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00148
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0014c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00150
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00154
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0015c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00164
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00168
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00170
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00174
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0017c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00180
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0018c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00190
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00198
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0xffc00001,                                                  // -signaling NaN                              /// 001d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00200
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0c400000,                                                  // Leading 1s:                                 /// 00208
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0020c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00210
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00218
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0021c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00220
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00224
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00228
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0022c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00230
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0023c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00244
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00248
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0024c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00250
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00254
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00260
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00264
			 0x3f028f5c,                                                  // 0.51                                        /// 00268
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00270
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00278
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0027c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00280
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00284
			 0x7f800000,                                                  // inf                                         /// 00288
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0028c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0xffc00001,                                                  // -signaling NaN                              /// 00294
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00298
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0029c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002b4
			 0xff800000,                                                  // -inf                                        /// 002b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00304
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00308
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0030c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00314
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0031c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x0e000001,                                                  // Trailing 1s:                                /// 00324
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00334
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00338
			 0x80011111,                                                  // -9.7958E-41                                 /// 0033c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00348
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00354
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00358
			 0x0c780000,                                                  // Leading 1s:                                 /// 0035c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00360
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00368
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0036c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00370
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00380
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00384
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0038c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00398
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003c4
			 0xff800000,                                                  // -inf                                        /// 003c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003ec
			 0x3f028f5c,                                                  // 0.51                                        /// 003f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00400
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00404
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00408
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0040c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0041c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00420
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00424
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00428
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0042c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00434
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00438
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0043c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00440
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00444
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00450
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00458
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0045c
			 0x3f028f5c,                                                  // 0.51                                        /// 00460
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00464
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00470
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00474
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00478
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0047c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00480
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00484
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00488
			 0xbf028f5c,                                                  // -0.51                                       /// 0048c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00498
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0049c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004c4
			 0x7f800000,                                                  // inf                                         /// 004c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004f4
			 0x55555555,                                                  // 4 random values                             /// 004f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x0e000007,                                                  // Trailing 1s:                                /// 00504
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0050c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00510
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00514
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00518
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00524
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00528
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0052c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00534
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00538
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0053c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00540
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00544
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00548
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0054c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00554
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00558
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0055c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00564
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00578
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0057c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00580
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00588
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0058c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00590
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00594
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00598
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0059c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f4
			 0xbf028f5c,                                                  // -0.51                                       /// 005f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00600
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00604
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00608
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0060c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00610
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00614
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x80011111,                                                  // -9.7958E-41                                 /// 0061c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00624
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00628
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0062c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00630
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00634
			 0x0c700000,                                                  // Leading 1s:                                 /// 00638
			 0x0e000007,                                                  // Trailing 1s:                                /// 0063c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00640
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00644
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00648
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00650
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0065c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00664
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00668
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00670
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0068c
			 0x00000000,                                                  // zero                                        /// 00690
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00694
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00698
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0069c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80000000,                                                  // -zero                                       /// 006d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x4b000000,                                                  // 8388608.0                                   /// 006f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00700
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00704
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0070c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00710
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00714
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00718
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xbf028f5c,                                                  // -0.51                                       /// 00724
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00728
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0072c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00740
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00744
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00748
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0074c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00750
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0075c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00764
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0076c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00000000,                                                  // zero                                        /// 00774
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00778
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0077c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00780
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00784
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0078c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00790
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00794
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00798
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0079c
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007b0
			 0x4b000000,                                                  // 8388608.0                                   /// 007b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 007d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00011111,                                                  // 9.7958E-41                                  /// 00804
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0080c
			 0xffc00001,                                                  // -signaling NaN                              /// 00810
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0081c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00820
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00824
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00828
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0082c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00830
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00834
			 0x0c600000,                                                  // Leading 1s:                                 /// 00838
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0083c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00840
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00844
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00848
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0084c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00850
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0085c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00860
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00864
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0086c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00878
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00880
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00888
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0088c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00890
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00894
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00898
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008bc
			 0xffc00001,                                                  // -signaling NaN                              /// 008c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00900
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00904
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00908
			 0x0e000001,                                                  // Trailing 1s:                                /// 0090c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00910
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00918
			 0x3f028f5c,                                                  // 0.51                                        /// 0091c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x33333333,                                                  // 4 random values                             /// 00924
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0092c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00930
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00934
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00938
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0094c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00954
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00958
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0095c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00964
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00968
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00970
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00974
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00978
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0097c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00980
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00984
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00988
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x00000000,                                                  // zero                                        /// 00998
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x55555555,                                                  // 4 random values                             /// 009ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009c0
			 0xbf028f5c,                                                  // -0.51                                       /// 009c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a24
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a48
			 0x4b000000,                                                  // 8388608.0                                   /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0xbf028f5c,                                                  // -0.51                                       /// 00a68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x7fc00001,                                                  // signaling NaN                               /// 00a74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x55555555,                                                  // 4 random values                             /// 00aa4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ab0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ab8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00adc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ae0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ae8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x7fc00001,                                                  // signaling NaN                               /// 00af0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00afc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x55555555,                                                  // 4 random values                             /// 00b10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x7f800000,                                                  // inf                                         /// 00b28
			 0xbf028f5c,                                                  // -0.51                                       /// 00b2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ba8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bc4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bd4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0xcb000000,                                                  // -8388608.0                                  /// 00be4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00be8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bec
			 0x00000000,                                                  // zero                                        /// 00bf0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bfc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80000000,                                                  // -zero                                       /// 00c24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x55555555,                                                  // 4 random values                             /// 00c34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c38
			 0x7fc00001,                                                  // signaling NaN                               /// 00c3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x7fc00001,                                                  // signaling NaN                               /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xffc00001,                                                  // -signaling NaN                              /// 00c8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c98
			 0xffc00001,                                                  // -signaling NaN                              /// 00c9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ca8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x3f028f5c,                                                  // 0.51                                        /// 00cb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cdc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ce8
			 0x33333333,                                                  // 4 random values                             /// 00cec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x80000000,                                                  // -zero                                       /// 00d80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00da8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00db0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dcc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x7f800000,                                                  // inf                                         /// 00dd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x55555555,                                                  // 4 random values                             /// 00de0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00df8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x4b000000,                                                  // 8388608.0                                   /// 00e0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e20
			 0x80000000,                                                  // -zero                                       /// 00e24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e28
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e68
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x80000000,                                                  // -zero                                       /// 00e94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ea0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00eac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ebc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ec0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ecc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ed4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00edc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ee0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ee4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ef4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00efc
			 0x33333333,                                                  // 4 random values                             /// 00f00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f70
			 0x3f028f5c,                                                  // 0.51                                        /// 00f74
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fcc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fdc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fe4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ff0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ff4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ff8
			 0x0c7ffff0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00000
			 0x33333333,                                                  // 4 random values                             /// 00004
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00008
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0000c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00010
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0001c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00024
			 0x00011111,                                                  // 9.7958E-41                                  /// 00028
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0002c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00030
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00034
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00048
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0004c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00050
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00054
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0005c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00060
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00064
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00068
			 0x00011111,                                                  // 9.7958E-41                                  /// 0006c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00070
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00078
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0007c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00080
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00084
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 000d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f0
			 0x3f028f5c,                                                  // 0.51                                        /// 000f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00100
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00104
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0010c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0011c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00128
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0012c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00130
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00134
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00140
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00144
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0014c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00150
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x4b000000,                                                  // 8388608.0                                   /// 0015c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00160
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00170
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00174
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00180
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0xff800000,                                                  // -inf                                        /// 0018c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00194
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0e000003,                                                  // Trailing 1s:                                /// 0019c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001bc
			 0x80000000,                                                  // -zero                                       /// 001c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00204
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00210
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00214
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00218
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00220
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00224
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00228
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0022c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00230
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00238
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c600000,                                                  // Leading 1s:                                 /// 00248
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00250
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00258
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00260
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00270
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00278
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00280
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00284
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00288
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00294
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00298
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xbf028f5c,                                                  // -0.51                                       /// 002c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002dc
			 0xbf028f5c,                                                  // -0.51                                       /// 002e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ec
			 0xcb000000,                                                  // -8388608.0                                  /// 002f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0030c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00310
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00314
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00318
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0c780000,                                                  // Leading 1s:                                 /// 00324
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00328
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00330
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00334
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00340
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00344
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00350
			 0xff800000,                                                  // -inf                                        /// 00354
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00360
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00364
			 0x80011111,                                                  // -9.7958E-41                                 /// 00368
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0036c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00370
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00374
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x33333333,                                                  // 4 random values                             /// 0037c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00384
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00388
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00390
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0039c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00400
			 0xbf028f5c,                                                  // -0.51                                       /// 00404
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0040c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00410
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00418
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80011111,                                                  // -9.7958E-41                                 /// 00424
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0042c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00430
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00434
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00438
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00440
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00444
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0044c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00450
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00454
			 0x80011111,                                                  // -9.7958E-41                                 /// 00458
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00460
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00464
			 0x0c700000,                                                  // Leading 1s:                                 /// 00468
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0046c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00470
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00478
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00480
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00488
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0048c
			 0x33333333,                                                  // 4 random values                             /// 00490
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0049c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x55555555,                                                  // 4 random values                             /// 004a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7fc00001,                                                  // signaling NaN                               /// 004e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004f4
			 0x4b000000,                                                  // 8388608.0                                   /// 004f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00508
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00510
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00518
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00520
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00528
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0052c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00530
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00534
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00538
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0053c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00540
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00544
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00550
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00554
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00558
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00560
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00564
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00568
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0056c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00570
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00578
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0057c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00580
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00584
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00590
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x80000000,                                                  // -zero                                       /// 00598
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0059c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005ac
			 0x55555555,                                                  // 4 random values                             /// 005b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x80000000,                                                  // -zero                                       /// 005b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00000000,                                                  // zero                                        /// 005c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x55555555,                                                  // 4 random values                             /// 005e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00600
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0060c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00614
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00618
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00620
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00624
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00628
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0062c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00634
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0063c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00640
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00644
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00648
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0064c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00658
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00660
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00664
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00670
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00674
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00680
			 0xcb000000,                                                  // -8388608.0                                  /// 00684
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00688
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0068c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00690
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x3f028f5c,                                                  // 0.51                                        /// 006c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d0
			 0x3f028f5c,                                                  // 0.51                                        /// 006d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e0
			 0x55555555,                                                  // 4 random values                             /// 006e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00708
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0070c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00710
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x80011111,                                                  // -9.7958E-41                                 /// 00718
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0071c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0072c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00730
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00734
			 0x0c400000,                                                  // Leading 1s:                                 /// 00738
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0073c
			 0x80000000,                                                  // -zero                                       /// 00740
			 0x7f800000,                                                  // inf                                         /// 00744
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00748
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00750
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00754
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00758
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00760
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00764
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0c400000,                                                  // Leading 1s:                                 /// 0076c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00770
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00774
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00784
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00788
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0078c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00790
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007d8
			 0x7fc00001,                                                  // signaling NaN                               /// 007dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00804
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x7fc00001,                                                  // signaling NaN                               /// 00814
			 0x80011111,                                                  // -9.7958E-41                                 /// 00818
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0081c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00820
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00824
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00828
			 0x0e000001,                                                  // Trailing 1s:                                /// 0082c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00834
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0083c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00840
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00844
			 0x0c600000,                                                  // Leading 1s:                                 /// 00848
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x55555555,                                                  // 4 random values                             /// 00850
			 0x80000000,                                                  // -zero                                       /// 00854
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0085c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00860
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0086c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00870
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00874
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00878
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0087c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00884
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00888
			 0x55555555,                                                  // 4 random values                             /// 0088c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00894
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x7fc00001,                                                  // signaling NaN                               /// 0089c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008b8
			 0x4b000000,                                                  // 8388608.0                                   /// 008bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e8
			 0x55555555,                                                  // 4 random values                             /// 008ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00900
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00904
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00910
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00914
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00918
			 0x0e000003,                                                  // Trailing 1s:                                /// 0091c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00924
			 0xffc00001,                                                  // -signaling NaN                              /// 00928
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00930
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00934
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00938
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0093c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00944
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00948
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00950
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00954
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0095c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00964
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00968
			 0x0c780000,                                                  // Leading 1s:                                 /// 0096c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00974
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0097c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00988
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0098c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00994
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00998
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0099c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009b0
			 0xff800000,                                                  // -inf                                        /// 009b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009c4
			 0x55555555,                                                  // 4 random values                             /// 009c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009f0
			 0x55555555,                                                  // 4 random values                             /// 009f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a54
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ab4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ab8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00abc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ad8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00adc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00afc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0xbf028f5c,                                                  // -0.51                                       /// 00b5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00be4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00be8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bf4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c88
			 0xff800000,                                                  // -inf                                        /// 00c8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ca8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ce8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cf4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d40
			 0x3f028f5c,                                                  // 0.51                                        /// 00d44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d60
			 0x3f028f5c,                                                  // 0.51                                        /// 00d64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d74
			 0x7f800000,                                                  // inf                                         /// 00d78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d8c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d94
			 0x33333333,                                                  // 4 random values                             /// 00d98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00da8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80000000,                                                  // -zero                                       /// 00dd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ddc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00de4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00df0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e00
			 0x7fc00001,                                                  // signaling NaN                               /// 00e04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e0c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e48
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e68
			 0x80000000,                                                  // -zero                                       /// 00e6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e78
			 0x55555555,                                                  // 4 random values                             /// 00e7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x33333333,                                                  // 4 random values                             /// 00ea0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ea8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eac
			 0xbf028f5c,                                                  // -0.51                                       /// 00eb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00eb4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eb8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ebc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ec0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x7f800000,                                                  // inf                                         /// 00ecc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ed0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ed8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00edc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ee0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ee4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ee8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00efc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fa8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fc4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fe8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ff0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ff8
			 0x0c7ffc00                                                  // Leading 1s:                                 /// last
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
			 0x000000f8,
			 0x000005f0,
			 0x00000728,
			 0x00000114,
			 0x00000410,
			 0x0000039c,
			 0x00000708,
			 0x00000080,

			 /// vpu register f2
			 0x40a00000,
			 0x41980000,
			 0x41200000,
			 0x40000000,
			 0x41e80000,
			 0x41880000,
			 0x41f00000,
			 0x41f00000,

			 /// vpu register f3
			 0x40a00000,
			 0x41a00000,
			 0x41a80000,
			 0x41e80000,
			 0x42000000,
			 0x41c80000,
			 0x3f800000,
			 0x40400000,

			 /// vpu register f4
			 0x41600000,
			 0x41e00000,
			 0x41100000,
			 0x41100000,
			 0x41600000,
			 0x41700000,
			 0x41f00000,
			 0x41e80000,

			 /// vpu register f5
			 0x41f80000,
			 0x41900000,
			 0x41700000,
			 0x41f80000,
			 0x41600000,
			 0x40a00000,
			 0x40c00000,
			 0x41600000,

			 /// vpu register f6
			 0x41e80000,
			 0x41b80000,
			 0x41000000,
			 0x41980000,
			 0x40c00000,
			 0x40400000,
			 0x3f800000,
			 0x41700000,

			 /// vpu register f7
			 0x40800000,
			 0x41c00000,
			 0x41880000,
			 0x41880000,
			 0x41b80000,
			 0x40800000,
			 0x41000000,
			 0x41f00000,

			 /// vpu register f8
			 0x41b00000,
			 0x41100000,
			 0x41880000,
			 0x41a80000,
			 0x41a80000,
			 0x41600000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f9
			 0x3f800000,
			 0x41f80000,
			 0x41400000,
			 0x41d00000,
			 0x42000000,
			 0x41100000,
			 0x41200000,
			 0x41200000,

			 /// vpu register f10
			 0x40800000,
			 0x41a00000,
			 0x40c00000,
			 0x40000000,
			 0x41d80000,
			 0x41b80000,
			 0x41100000,
			 0x41000000,

			 /// vpu register f11
			 0x41e00000,
			 0x41600000,
			 0x41400000,
			 0x41c00000,
			 0x41600000,
			 0x41d00000,
			 0x40400000,
			 0x41100000,

			 /// vpu register f12
			 0x41200000,
			 0x41e80000,
			 0x41e00000,
			 0x40400000,
			 0x41400000,
			 0x41b00000,
			 0x41200000,
			 0x41800000,

			 /// vpu register f13
			 0x41300000,
			 0x41b80000,
			 0x41900000,
			 0x41200000,
			 0x41700000,
			 0x40400000,
			 0x40e00000,
			 0x41d00000,

			 /// vpu register f14
			 0x41800000,
			 0x40a00000,
			 0x41100000,
			 0x41200000,
			 0x41500000,
			 0x3f800000,
			 0x41700000,
			 0x41500000,

			 /// vpu register f15
			 0x40e00000,
			 0x41700000,
			 0x41b80000,
			 0x41300000,
			 0x41b80000,
			 0x41700000,
			 0x41f00000,
			 0x41d00000,

			 /// vpu register f16
			 0x41f80000,
			 0x40e00000,
			 0x40800000,
			 0x41200000,
			 0x40400000,
			 0x41f00000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f17
			 0x41c80000,
			 0x41880000,
			 0x40400000,
			 0x41500000,
			 0x42000000,
			 0x40a00000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f18
			 0x41b00000,
			 0x41600000,
			 0x41100000,
			 0x3f800000,
			 0x42000000,
			 0x41500000,
			 0x40e00000,
			 0x41600000,

			 /// vpu register f19
			 0x40a00000,
			 0x41d80000,
			 0x41880000,
			 0x41c00000,
			 0x41c00000,
			 0x41100000,
			 0x41f80000,
			 0x41d80000,

			 /// vpu register f20
			 0x41b00000,
			 0x41900000,
			 0x41d00000,
			 0x40e00000,
			 0x40400000,
			 0x41200000,
			 0x40800000,
			 0x3f800000,

			 /// vpu register f21
			 0x40c00000,
			 0x41c00000,
			 0x40c00000,
			 0x41500000,
			 0x41600000,
			 0x41000000,
			 0x40c00000,
			 0x41b80000,

			 /// vpu register f22
			 0x41300000,
			 0x41c00000,
			 0x3f800000,
			 0x3f800000,
			 0x40800000,
			 0x41200000,
			 0x40e00000,
			 0x3f800000,

			 /// vpu register f23
			 0x41d00000,
			 0x41e80000,
			 0x41c80000,
			 0x41d00000,
			 0x40000000,
			 0x40800000,
			 0x41c00000,
			 0x41b00000,

			 /// vpu register f24
			 0x40000000,
			 0x41b80000,
			 0x41500000,
			 0x40800000,
			 0x40800000,
			 0x40e00000,
			 0x41800000,
			 0x41000000,

			 /// vpu register f25
			 0x42000000,
			 0x41e00000,
			 0x41c80000,
			 0x41b00000,
			 0x41300000,
			 0x41a80000,
			 0x41d80000,
			 0x41400000,

			 /// vpu register f26
			 0x41f80000,
			 0x41d00000,
			 0x41200000,
			 0x41a80000,
			 0x41700000,
			 0x41b00000,
			 0x41f80000,
			 0x40e00000,

			 /// vpu register f27
			 0x40e00000,
			 0x41900000,
			 0x41000000,
			 0x41000000,
			 0x41b00000,
			 0x40000000,
			 0x41e80000,
			 0x41400000,

			 /// vpu register f28
			 0x41000000,
			 0x41900000,
			 0x41b00000,
			 0x40e00000,
			 0x40c00000,
			 0x41d80000,
			 0x40a00000,
			 0x41e80000,

			 /// vpu register f29
			 0x42000000,
			 0x3f800000,
			 0x41000000,
			 0x40000000,
			 0x41200000,
			 0x41a80000,
			 0x41100000,
			 0x41e80000,

			 /// vpu register f30
			 0x41a80000,
			 0x41a00000,
			 0x3f800000,
			 0x41880000,
			 0x41980000,
			 0x41200000,
			 0x41f80000,
			 0x42000000,

			 /// vpu register f31
			 0x41b80000,
			 0x41300000,
			 0x41880000,
			 0x41100000,
			 0x41b80000,
			 0x41900000,
			 0x41600000,
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
		"nop\n"
		"fswl.ps f4, (x12)\n"                                 ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x13)\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f30, (x13)\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f25, (x15)\n"                                ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f13, (x13)\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f18, (x14)\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f18, (x14)\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f2, (x12)\n"                                 ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f29, (x12)\n"                                ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f25, (x11)\n"                                ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f6, (x11)\n"                                 ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f12, (x14)\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x13)\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f4, (x12)\n"                                 ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f0, (x12)\n"                                 ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f24, (x11)\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f28, (x13)\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f29, (x12)\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f12, (x15)\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f27, (x15)\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f29, (x13)\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f0, (x14)\n"                                 ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f13, (x15)\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f10, (x14)\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x14)\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f20, (x11)\n"                                ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f10, (x12)\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f27, (x12)\n"                                ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f22, (x15)\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f14, (x15)\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x13)\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f12, (x14)\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f26, (x12)\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f19, (x15)\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x13)\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f27, (x11)\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f26, (x14)\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f28, (x15)\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f11, (x13)\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f11, (x11)\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f18, (x11)\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f21, (x14)\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f8, (x14)\n"                                 ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f24, (x15)\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f21, (x14)\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f27, (x14)\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f5, (x15)\n"                                 ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f7, (x11)\n"                                 ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f9, (x11)\n"                                 ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f7, (x13)\n"                                 ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f28, (x12)\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f24, (x14)\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f22, (x15)\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f16, (x15)\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f5, (x13)\n"                                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f19, (x14)\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f6, (x11)\n"                                 ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f5, (x15)\n"                                 ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f22, (x13)\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x14)\n"                                ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f8, (x11)\n"                                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f30, (x11)\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f23, (x13)\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f16, (x15)\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f6, (x15)\n"                                 ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f26, (x15)\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f27, (x11)\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f21, (x12)\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f6, (x11)\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f6, (x14)\n"                                 ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f11, (x13)\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f3, (x14)\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f25, (x15)\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f22, (x11)\n"                                ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f6, (x13)\n"                                 ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f7, (x11)\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f2, (x14)\n"                                 ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f19, (x13)\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f13, (x12)\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f22, (x12)\n"                                ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f22, (x15)\n"                                ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f22, (x14)\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f7, (x12)\n"                                 ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f6, (x12)\n"                                 ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f12, (x13)\n"                                ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f30, (x12)\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f29, (x13)\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f28, (x15)\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f2, (x13)\n"                                 ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f13, (x13)\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f23, (x11)\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f0, (x11)\n"                                 ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x12)\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f25, (x12)\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f25, (x14)\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f19, (x11)\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f27, (x12)\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f17, (x15)\n"                                ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswl.ps f10, (x12)\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
