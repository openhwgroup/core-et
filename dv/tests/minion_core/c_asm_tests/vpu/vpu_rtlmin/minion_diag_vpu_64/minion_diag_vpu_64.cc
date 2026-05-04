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
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00000
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00008
			 0x0e000007,                                                  // Trailing 1s:                                /// 0000c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00010
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00014
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00018
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00020
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00028
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00030
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00038
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0003c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0004c
			 0xffc00001,                                                  // -signaling NaN                              /// 00050
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00064
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00068
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00070
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fc00001,                                                  // signaling NaN                               /// 00078
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00080
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00088
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0008c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00090
			 0x7fc00001,                                                  // signaling NaN                               /// 00094
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x0e000001,                                                  // Trailing 1s:                                /// 000a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xbf028f5c,                                                  // -0.51                                       /// 000d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00100
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00110
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00118
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0011c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00124
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00128
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0012c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00134
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00140
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00144
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0014c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00154
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00158
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x4b000000,                                                  // 8388608.0                                   /// 00160
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00164
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00168
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0016c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00170
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00174
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00178
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0017c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00180
			 0x80000000,                                                  // -zero                                       /// 00184
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0018c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0xff800000,                                                  // -inf                                        /// 00194
			 0x00011111,                                                  // 9.7958E-41                                  /// 00198
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0019c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001ec
			 0x80000000,                                                  // -zero                                       /// 001f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00204
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00208
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0020c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00210
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00214
			 0x33333333,                                                  // 4 random values                             /// 00218
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0021c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c600000,                                                  // Leading 1s:                                 /// 00228
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0022c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00230
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00234
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x33333333,                                                  // 4 random values                             /// 0023c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00240
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0024c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00250
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00254
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00258
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0025c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00264
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00268
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00274
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00284
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00288
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0028c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00290
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x3f028f5c,                                                  // 0.51                                        /// 00298
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0029c
			 0x3f028f5c,                                                  // 0.51                                        /// 002a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 002d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00300
			 0x00011111,                                                  // 9.7958E-41                                  /// 00304
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00308
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0030c
			 0xff800000,                                                  // -inf                                        /// 00310
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00314
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00318
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0031c
			 0xbf028f5c,                                                  // -0.51                                       /// 00320
			 0x0c600000,                                                  // Leading 1s:                                 /// 00324
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00328
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00330
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00338
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0033c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00340
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00344
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00348
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0034c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00350
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00360
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00364
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00368
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0036c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00370
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x33333333,                                                  // 4 random values                             /// 00378
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x0e000007,                                                  // Trailing 1s:                                /// 00384
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00388
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0038c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00390
			 0x0e000007,                                                  // Trailing 1s:                                /// 00394
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0039c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003ac
			 0xbf028f5c,                                                  // -0.51                                       /// 003b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x4b000000,                                                  // 8388608.0                                   /// 003c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x80000000,                                                  // -zero                                       /// 00404
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00408
			 0x00011111,                                                  // 9.7958E-41                                  /// 0040c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00414
			 0x00000000,                                                  // zero                                        /// 00418
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0041c
			 0xcb000000,                                                  // -8388608.0                                  /// 00420
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00430
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00438
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0044c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00450
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00454
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00458
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00464
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00468
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0046c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00474
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00478
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0047c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00480
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00484
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00488
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00490
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00494
			 0x7f800000,                                                  // inf                                         /// 00498
			 0x0c600000,                                                  // Leading 1s:                                 /// 0049c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x33333333,                                                  // 4 random values                             /// 004a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x55555555,                                                  // 4 random values                             /// 004b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0xffc00001,                                                  // -signaling NaN                              /// 004c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004ec
			 0x3f028f5c,                                                  // 0.51                                        /// 004f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00500
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00504
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00508
			 0xff800000,                                                  // -inf                                        /// 0050c
			 0x7fc00001,                                                  // signaling NaN                               /// 00510
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00514
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0051c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00520
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c600000,                                                  // Leading 1s:                                 /// 00528
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00534
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0e000001,                                                  // Trailing 1s:                                /// 0053c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00540
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00548
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0054c
			 0x7f800000,                                                  // inf                                         /// 00550
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00554
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00558
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0056c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00570
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00574
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0057c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00580
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00584
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00588
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00590
			 0x4b000000,                                                  // 8388608.0                                   /// 00594
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00598
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0059c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00604
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0060c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00614
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00618
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00620
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00624
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00628
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00640
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00644
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00648
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00654
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00658
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0065c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0066c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00674
			 0x00000000,                                                  // zero                                        /// 00678
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0067c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0068c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00690
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00694
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0c400000,                                                  // Leading 1s:                                 /// 0069c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0xff800000,                                                  // -inf                                        /// 006f0
			 0xffc00001,                                                  // -signaling NaN                              /// 006f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00700
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00704
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0070c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00710
			 0xbf028f5c,                                                  // -0.51                                       /// 00714
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x7fc00001,                                                  // signaling NaN                               /// 00720
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00724
			 0x33333333,                                                  // 4 random values                             /// 00728
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0072c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00730
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00734
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00738
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00740
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00744
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00748
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00750
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00754
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00758
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0075c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x4b000000,                                                  // 8388608.0                                   /// 00768
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00770
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x3f028f5c,                                                  // 0.51                                        /// 00778
			 0x0e000001,                                                  // Trailing 1s:                                /// 0077c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00784
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00788
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0078c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00790
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00794
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00798
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0079c
			 0x4b000000,                                                  // 8388608.0                                   /// 007a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007cc
			 0x33333333,                                                  // 4 random values                             /// 007d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00800
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00804
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00808
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0080c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00820
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00824
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00828
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0082c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00830
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00834
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00840
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00844
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00848
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00850
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00854
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00858
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0085c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00860
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00868
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0086c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00870
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00880
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00884
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0088c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00890
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00894
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00898
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0089c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x7f800000,                                                  // inf                                         /// 008ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00900
			 0x7f800000,                                                  // inf                                         /// 00904
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00910
			 0x0c600000,                                                  // Leading 1s:                                 /// 00914
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00920
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00924
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0092c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x55555555,                                                  // 4 random values                             /// 0093c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00940
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0094c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00950
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00958
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00960
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00964
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00968
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0096c
			 0x80000000,                                                  // -zero                                       /// 00970
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00978
			 0xbf028f5c,                                                  // -0.51                                       /// 0097c
			 0xcb000000,                                                  // -8388608.0                                  /// 00980
			 0x33333333,                                                  // 4 random values                             /// 00984
			 0x0c400000,                                                  // Leading 1s:                                 /// 00988
			 0x80011111,                                                  // -9.7958E-41                                 /// 0098c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00990
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00994
			 0xffc00001,                                                  // -signaling NaN                              /// 00998
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0099c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009dc
			 0x55555555,                                                  // 4 random values                             /// 009e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a10
			 0x33333333,                                                  // 4 random values                             /// 00a14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a60
			 0x7f800000,                                                  // inf                                         /// 00a64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0xffc00001,                                                  // -signaling NaN                              /// 00a7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aa4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ab4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ac0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00acc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ad8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00adc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ae4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ae8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00af0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00af4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b00
			 0xff800000,                                                  // -inf                                        /// 00b04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b0c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x7f800000,                                                  // inf                                         /// 00b40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ba0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80000000,                                                  // -zero                                       /// 00ba8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bcc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00be0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00be8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bf0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bfc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x33333333,                                                  // 4 random values                             /// 00c68
			 0x3f028f5c,                                                  // 0.51                                        /// 00c6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7f800000,                                                  // inf                                         /// 00c8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ca8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cc4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ccc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cd4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cdc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x55555555,                                                  // 4 random values                             /// 00ce4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cf0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cfc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d3c
			 0x00000000,                                                  // zero                                        /// 00d40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d78
			 0x4b000000,                                                  // 8388608.0                                   /// 00d7c
			 0x7f800000,                                                  // inf                                         /// 00d80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d94
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00da0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da4
			 0x33333333,                                                  // 4 random values                             /// 00da8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00db4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00db8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dcc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00de8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00df8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e00
			 0x4b000000,                                                  // 8388608.0                                   /// 00e04
			 0x00000000,                                                  // zero                                        /// 00e08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e80
			 0xffc00001,                                                  // -signaling NaN                              /// 00e84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x55555555,                                                  // 4 random values                             /// 00eb0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eb4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ec0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ec8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ed4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ed8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00edc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ee8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ef0
			 0x33333333,                                                  // 4 random values                             /// 00ef4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ef8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f04
			 0x3f028f5c,                                                  // 0.51                                        /// 00f08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f3c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f7c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f80
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0xffc00001,                                                  // -signaling NaN                              /// 00f8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fbc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fc4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fcc
			 0x7f800000,                                                  // inf                                         /// 00fd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fe4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fe8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x7f800000,                                                  // inf                                         /// 00ff4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80000000                                                  // -zero                                       /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00000
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00004
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00008
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00010
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00014
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00018
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0001c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00024
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00028
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0002c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00034
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0xffc00001,                                                  // -signaling NaN                              /// 00044
			 0x0c400000,                                                  // Leading 1s:                                 /// 00048
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0004c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00050
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00054
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00058
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0005c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x7f800000,                                                  // inf                                         /// 00064
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00068
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0006c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00070
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00074
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00078
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0007c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00090
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0009c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000a0
			 0xcb000000,                                                  // -8388608.0                                  /// 000a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x7f800000,                                                  // inf                                         /// 000ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b4
			 0xff800000,                                                  // -inf                                        /// 000b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000c8
			 0xbf028f5c,                                                  // -0.51                                       /// 000cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00100
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00108
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00110
			 0x4b000000,                                                  // 8388608.0                                   /// 00114
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00118
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00130
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0xffc00001,                                                  // -signaling NaN                              /// 0013c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00140
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00144
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0014c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00150
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00154
			 0xffc00001,                                                  // -signaling NaN                              /// 00158
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00168
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00170
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0017c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00184
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00188
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00190
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00194
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00198
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0019c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0020c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00210
			 0x0c700000,                                                  // Leading 1s:                                 /// 00214
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00220
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00224
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00228
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00230
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00234
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00238
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0023c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00240
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00244
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00248
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0024c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00250
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00254
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00258
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00260
			 0x00011111,                                                  // 9.7958E-41                                  /// 00264
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00268
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00270
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0027c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00280
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00288
			 0x7f800000,                                                  // inf                                         /// 0028c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00294
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00298
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x4b000000,                                                  // 8388608.0                                   /// 002c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ec
			 0x7fc00001,                                                  // signaling NaN                               /// 002f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0xbf028f5c,                                                  // -0.51                                       /// 00304
			 0x55555555,                                                  // 4 random values                             /// 00308
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0030c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00310
			 0x4b000000,                                                  // 8388608.0                                   /// 00314
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00318
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00320
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00324
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00330
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00334
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00338
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00340
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00344
			 0x0e000007,                                                  // Trailing 1s:                                /// 00348
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0034c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00350
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00354
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00358
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00360
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00364
			 0x7f800000,                                                  // inf                                         /// 00368
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0036c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00374
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00378
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0037c
			 0x33333333,                                                  // 4 random values                             /// 00380
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00384
			 0x0e000001,                                                  // Trailing 1s:                                /// 00388
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0038c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00390
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00394
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0039c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b4
			 0x3f028f5c,                                                  // 0.51                                        /// 003b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00400
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0xcb000000,                                                  // -8388608.0                                  /// 0040c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0041c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00428
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00430
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00438
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00448
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00450
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00454
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00458
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x0c400000,                                                  // Leading 1s:                                 /// 00464
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00468
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00470
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0xbf028f5c,                                                  // -0.51                                       /// 00480
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00484
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00488
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0048c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00494
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0049c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00500
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00504
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00508
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0050c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00510
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00514
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00518
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00520
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00528
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0052c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00530
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00534
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00538
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00540
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00548
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0c600000,                                                  // Leading 1s:                                 /// 00554
			 0x80011111,                                                  // -9.7958E-41                                 /// 00558
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00560
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00564
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0056c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00570
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00574
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00578
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0xcb000000,                                                  // -8388608.0                                  /// 00588
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0058c
			 0xbf028f5c,                                                  // -0.51                                       /// 00590
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00598
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xff800000,                                                  // -inf                                        /// 005cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00600
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00608
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0060c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00610
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00618
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0061c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00620
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0e000007,                                                  // Trailing 1s:                                /// 00628
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00634
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0063c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00640
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00644
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00648
			 0x7fc00001,                                                  // signaling NaN                               /// 0064c
			 0x00000000,                                                  // zero                                        /// 00650
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00660
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00664
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00668
			 0xff800000,                                                  // -inf                                        /// 0066c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00670
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00674
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0067c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00680
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00684
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00688
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00690
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00694
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00698
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00700
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00704
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00710
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00714
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00718
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00720
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00728
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0072c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00730
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x33333333,                                                  // 4 random values                             /// 0073c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00748
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0074c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00750
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00758
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0075c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00760
			 0x80011111,                                                  // -9.7958E-41                                 /// 00764
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00770
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0077c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00788
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00790
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00794
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00798
			 0x00011111,                                                  // 9.7958E-41                                  /// 0079c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00800
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0xbf028f5c,                                                  // -0.51                                       /// 0080c
			 0xbf028f5c,                                                  // -0.51                                       /// 00810
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00814
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00818
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00820
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00828
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00838
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00840
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00844
			 0xff800000,                                                  // -inf                                        /// 00848
			 0x0e000007,                                                  // Trailing 1s:                                /// 0084c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x55555555,                                                  // 4 random values                             /// 00864
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0086c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00874
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00878
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00880
			 0x0e000001,                                                  // Trailing 1s:                                /// 00884
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0088c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00890
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00000000,                                                  // zero                                        /// 00898
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0089c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008a4
			 0x7fc00001,                                                  // signaling NaN                               /// 008a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x33333333,                                                  // 4 random values                             /// 008b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00900
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00904
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00908
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00910
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00914
			 0x55555555,                                                  // 4 random values                             /// 00918
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00924
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0092c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00930
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0093c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00940
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00944
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0c600000,                                                  // Leading 1s:                                 /// 00954
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0095c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00968
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0096c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x80011111,                                                  // -9.7958E-41                                 /// 00974
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00978
			 0x4b000000,                                                  // 8388608.0                                   /// 0097c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00980
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00984
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00988
			 0x00011111,                                                  // 9.7958E-41                                  /// 0098c
			 0x7fc00001,                                                  // signaling NaN                               /// 00990
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00994
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0099c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009b0
			 0xff800000,                                                  // -inf                                        /// 009b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a08
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a78
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aa4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ab4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x4b000000,                                                  // 8388608.0                                   /// 00abc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00adc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ae8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00af0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00af4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b00
			 0xcb000000,                                                  // -8388608.0                                  /// 00b04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b0c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b70
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ba0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00bd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00be0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00be4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bec
			 0x7fc00001,                                                  // signaling NaN                               /// 00bf0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bf4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c34
			 0x7fc00001,                                                  // signaling NaN                               /// 00c38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c68
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cb0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cd8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cdc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ce4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cfc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d24
			 0xffc00001,                                                  // -signaling NaN                              /// 00d28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d54
			 0xffc00001,                                                  // -signaling NaN                              /// 00d58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d6c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dac
			 0x00000000,                                                  // zero                                        /// 00db0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00db8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ddc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00de0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00df0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e00
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e18
			 0x80000000,                                                  // -zero                                       /// 00e1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7f800000,                                                  // inf                                         /// 00e6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e84
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea4
			 0x7f800000,                                                  // inf                                         /// 00ea8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ec0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ec4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ecc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ed4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00edc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x7fc00001,                                                  // signaling NaN                               /// 00f3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0xff800000,                                                  // -inf                                        /// 00f5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f64
			 0xffc00001,                                                  // -signaling NaN                              /// 00f68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x80000000,                                                  // -zero                                       /// 00f74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fa0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fd4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fe4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fe8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00002000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00010
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0001c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00020
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x33333333,                                                  // 4 random values                             /// 00030
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00034
			 0x00011111,                                                  // 9.7958E-41                                  /// 00038
			 0x0c400000,                                                  // Leading 1s:                                 /// 0003c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00048
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0004c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00054
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00058
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00060
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00064
			 0x0c400000,                                                  // Leading 1s:                                 /// 00068
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0006c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00070
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00074
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00080
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00084
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00088
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0008c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00090
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00094
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00098
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0009c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b8
			 0x33333333,                                                  // 4 random values                             /// 000bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000cc
			 0x55555555,                                                  // 4 random values                             /// 000d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000e8
			 0x7f800000,                                                  // inf                                         /// 000ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f0
			 0x3f028f5c,                                                  // 0.51                                        /// 000f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00104
			 0xffc00001,                                                  // -signaling NaN                              /// 00108
			 0x0e000007,                                                  // Trailing 1s:                                /// 0010c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00110
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00114
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00118
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00120
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00128
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0012c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00134
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00138
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00140
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00150
			 0x0e000007,                                                  // Trailing 1s:                                /// 00154
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00158
			 0x0c700000,                                                  // Leading 1s:                                 /// 0015c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00164
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00168
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0016c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00170
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00178
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0017c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00180
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0018c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00190
			 0x80011111,                                                  // -9.7958E-41                                 /// 00194
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00200
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00204
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00208
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x00011111,                                                  // 9.7958E-41                                  /// 00218
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0021c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00224
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00228
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0022c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0xffc00001,                                                  // -signaling NaN                              /// 00238
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0023c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00240
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00244
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00254
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0xcb000000,                                                  // -8388608.0                                  /// 0025c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00264
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x00000000,                                                  // zero                                        /// 0026c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00278
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0027c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00288
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0028c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0029c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00300
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00304
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00308
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00320
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0032c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0033c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00344
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00348
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0034c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00354
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00358
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0035c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00360
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00364
			 0x80000000,                                                  // -zero                                       /// 00368
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0036c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00370
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00374
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00378
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0xff800000,                                                  // -inf                                        /// 00388
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0038c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00390
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00398
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0039c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00400
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00404
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00408
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00420
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00430
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00434
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00438
			 0x7f800000,                                                  // inf                                         /// 0043c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00440
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00448
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0045c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00468
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0046c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00470
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00474
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0047c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00480
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00484
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00488
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0048c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00490
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00494
			 0x33333333,                                                  // 4 random values                             /// 00498
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0049c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 004b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004bc
			 0x33333333,                                                  // 4 random values                             /// 004c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x7f800000,                                                  // inf                                         /// 004ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00500
			 0x00011111,                                                  // 9.7958E-41                                  /// 00504
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0050c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x55555555,                                                  // 4 random values                             /// 0051c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00520
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00524
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x0c700000,                                                  // Leading 1s:                                 /// 0052c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00530
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00534
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00538
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00540
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00550
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00554
			 0xffc00001,                                                  // -signaling NaN                              /// 00558
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00564
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00568
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0056c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00574
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00578
			 0xff800000,                                                  // -inf                                        /// 0057c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00580
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00588
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0058c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00594
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005b8
			 0xcb000000,                                                  // -8388608.0                                  /// 005bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0xffc00001,                                                  // -signaling NaN                              /// 005d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00604
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00608
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0060c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00614
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0061c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00620
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00624
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0xff800000,                                                  // -inf                                        /// 0062c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00634
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0xff800000,                                                  // -inf                                        /// 00640
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00644
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00654
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00658
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0065c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00660
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00668
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0066c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00670
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00674
			 0x55555555,                                                  // 4 random values                             /// 00678
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00680
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00688
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00690
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00694
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00698
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006e0
			 0x33333333,                                                  // 4 random values                             /// 006e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00704
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00708
			 0x0e000003,                                                  // Trailing 1s:                                /// 0070c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00714
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00718
			 0xcb000000,                                                  // -8388608.0                                  /// 0071c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00734
			 0x0c700000,                                                  // Leading 1s:                                 /// 00738
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0073c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00740
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00744
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0074c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00750
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00758
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0075c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0076c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00774
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00778
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0077c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00780
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00784
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00794
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00798
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0079c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0xff800000,                                                  // -inf                                        /// 007c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007f0
			 0xbf028f5c,                                                  // -0.51                                       /// 007f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00800
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00804
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00808
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0080c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00814
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0081c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00820
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00824
			 0x4b000000,                                                  // 8388608.0                                   /// 00828
			 0x80000000,                                                  // -zero                                       /// 0082c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00834
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00844
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00848
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0xff800000,                                                  // -inf                                        /// 00854
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00868
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00870
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00878
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00880
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00884
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00888
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00894
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00898
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008c0
			 0x80000000,                                                  // -zero                                       /// 008c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c8
			 0xff800000,                                                  // -inf                                        /// 008cc
			 0x33333333,                                                  // 4 random values                             /// 008d0
			 0x00000000,                                                  // zero                                        /// 008d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00900
			 0x00011111,                                                  // 9.7958E-41                                  /// 00904
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00908
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0090c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00918
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0091c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00920
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00924
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0092c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00930
			 0xbf028f5c,                                                  // -0.51                                       /// 00934
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0093c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00940
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00944
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00948
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00950
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00964
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0xbf028f5c,                                                  // -0.51                                       /// 00974
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00978
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0c600000,                                                  // Leading 1s:                                 /// 00984
			 0x7fc00001,                                                  // signaling NaN                               /// 00988
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00990
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00998
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a04
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x3f028f5c,                                                  // 0.51                                        /// 00a28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x33333333,                                                  // 4 random values                             /// 00a60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aa8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ab0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00abc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ac0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ac4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00acc
			 0xff800000,                                                  // -inf                                        /// 00ad0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ad4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ae0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ae4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00af0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00af4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00af8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00afc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x33333333,                                                  // 4 random values                             /// 00b4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b70
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b90
			 0x80000000,                                                  // -zero                                       /// 00b94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bb8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bc0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be4
			 0x7f800000,                                                  // inf                                         /// 00be8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c10
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ca0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ca4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb0
			 0xff800000,                                                  // -inf                                        /// 00cb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc4
			 0x80000000,                                                  // -zero                                       /// 00cc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ccc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00000000,                                                  // zero                                        /// 00cd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cdc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ce8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cf8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d00
			 0xbf028f5c,                                                  // -0.51                                       /// 00d04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d44
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d50
			 0x4b000000,                                                  // 8388608.0                                   /// 00d54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00da0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dac
			 0x3f028f5c,                                                  // 0.51                                        /// 00db0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00db4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00db8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dbc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00de0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e24
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e38
			 0x4b000000,                                                  // 8388608.0                                   /// 00e3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80000000,                                                  // -zero                                       /// 00e48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e8c
			 0x7f800000,                                                  // inf                                         /// 00e90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e98
			 0x7fc00001,                                                  // signaling NaN                               /// 00e9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ea4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eac
			 0x80000000,                                                  // -zero                                       /// 00eb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eb8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ecc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ed4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed8
			 0x3f028f5c,                                                  // 0.51                                        /// 00edc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ee4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ee8
			 0x3f028f5c,                                                  // 0.51                                        /// 00eec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ef8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00efc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f2c
			 0x80000000,                                                  // -zero                                       /// 00f30
			 0x33333333,                                                  // 4 random values                             /// 00f34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f68
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x55555555,                                                  // 4 random values                             /// 00f70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fa8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fc4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ff0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ff4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00800000                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xe55c5520, /// 0x0
			 0xce01fac7, /// 0x4
			 0x4f29721e, /// 0x8
			 0x40519d31, /// 0xc
			 0x76423080, /// 0x10
			 0x92ddbbc0, /// 0x14
			 0x1f71f9b3, /// 0x18
			 0xd1b7a3e1, /// 0x1c
			 0xe02615d1, /// 0x20
			 0xe9bf5175, /// 0x24
			 0x9e5f1779, /// 0x28
			 0x72481474, /// 0x2c
			 0xe209f4f8, /// 0x30
			 0x57858504, /// 0x34
			 0xfd6bc37a, /// 0x38
			 0x27c0ac01, /// 0x3c
			 0x3f8b2fe6, /// 0x40
			 0x0b526c60, /// 0x44
			 0x84c2ab6b, /// 0x48
			 0xa4843825, /// 0x4c
			 0xeaf8ec66, /// 0x50
			 0xddfc35e1, /// 0x54
			 0x3fc61456, /// 0x58
			 0x761064ee, /// 0x5c
			 0x10bc2f96, /// 0x60
			 0x74e9e395, /// 0x64
			 0xf5533010, /// 0x68
			 0x8b84cdfe, /// 0x6c
			 0x29d8f1e1, /// 0x70
			 0x750e589d, /// 0x74
			 0x7ea166f8, /// 0x78
			 0xe93ca163, /// 0x7c
			 0x0ae3e7e8, /// 0x80
			 0xcb2e4817, /// 0x84
			 0xfe176b90, /// 0x88
			 0x3824992d, /// 0x8c
			 0xa63ef6e0, /// 0x90
			 0x73e0870d, /// 0x94
			 0x28f33b41, /// 0x98
			 0x173c8ee1, /// 0x9c
			 0xb64203d6, /// 0xa0
			 0x525c65ae, /// 0xa4
			 0x830622f7, /// 0xa8
			 0x02c2533e, /// 0xac
			 0x308c8cea, /// 0xb0
			 0x48892a2e, /// 0xb4
			 0xe50452c8, /// 0xb8
			 0x6b2a268d, /// 0xbc
			 0x39abac83, /// 0xc0
			 0xe9909ae6, /// 0xc4
			 0x26f2cd86, /// 0xc8
			 0xabf571ba, /// 0xcc
			 0xe4e8838e, /// 0xd0
			 0xcc629737, /// 0xd4
			 0xbd4649e9, /// 0xd8
			 0xf898724d, /// 0xdc
			 0xbc1cd694, /// 0xe0
			 0x847d64fe, /// 0xe4
			 0xf7729e75, /// 0xe8
			 0x7f08dda8, /// 0xec
			 0x6a0d7b64, /// 0xf0
			 0x7d5055c6, /// 0xf4
			 0xec88e1e4, /// 0xf8
			 0x25f3de8b, /// 0xfc
			 0xb8707439, /// 0x100
			 0xa72205a2, /// 0x104
			 0xfd49d818, /// 0x108
			 0xa3b60715, /// 0x10c
			 0x40ecc150, /// 0x110
			 0x0ccc3693, /// 0x114
			 0x85206bbe, /// 0x118
			 0x637d4700, /// 0x11c
			 0x10655e4d, /// 0x120
			 0x7d536c9e, /// 0x124
			 0x003f6038, /// 0x128
			 0x4c1eb6e9, /// 0x12c
			 0xb2dc6900, /// 0x130
			 0xa09a6c69, /// 0x134
			 0xb14a4dc7, /// 0x138
			 0x62167892, /// 0x13c
			 0x03160c72, /// 0x140
			 0x1db2d4b6, /// 0x144
			 0x15ff5164, /// 0x148
			 0x630f694f, /// 0x14c
			 0x42e72422, /// 0x150
			 0x44b1f9b9, /// 0x154
			 0x66cf6e3b, /// 0x158
			 0x9d877288, /// 0x15c
			 0xd8d40e8d, /// 0x160
			 0x09602b78, /// 0x164
			 0xe29d7b5f, /// 0x168
			 0x50d6be5e, /// 0x16c
			 0x09ce20b7, /// 0x170
			 0x8a57531f, /// 0x174
			 0xaf505b60, /// 0x178
			 0xc1156189, /// 0x17c
			 0x7255561b, /// 0x180
			 0xb680308b, /// 0x184
			 0xb60e19e3, /// 0x188
			 0xbdfffb23, /// 0x18c
			 0x7f9b1e1d, /// 0x190
			 0x4cdb9f81, /// 0x194
			 0xacb7c991, /// 0x198
			 0xe3a6a7b5, /// 0x19c
			 0x6e9c3b57, /// 0x1a0
			 0xee58c250, /// 0x1a4
			 0xa922c57f, /// 0x1a8
			 0x766736fd, /// 0x1ac
			 0x9c8acaf8, /// 0x1b0
			 0x4670e88d, /// 0x1b4
			 0x4e536473, /// 0x1b8
			 0x66940908, /// 0x1bc
			 0x3993a12f, /// 0x1c0
			 0x8e80b161, /// 0x1c4
			 0xd9a2af3f, /// 0x1c8
			 0xa92eeb2b, /// 0x1cc
			 0x04384434, /// 0x1d0
			 0x5ce6605c, /// 0x1d4
			 0xaf21e6c5, /// 0x1d8
			 0x01c6506c, /// 0x1dc
			 0xb4962aa4, /// 0x1e0
			 0x7c1c7e90, /// 0x1e4
			 0x80092436, /// 0x1e8
			 0x109961b2, /// 0x1ec
			 0x6477c855, /// 0x1f0
			 0xe8c2f774, /// 0x1f4
			 0xc380912a, /// 0x1f8
			 0x3c2f0479, /// 0x1fc
			 0xa4f06e1e, /// 0x200
			 0xa2c5b066, /// 0x204
			 0x30befead, /// 0x208
			 0x114334ce, /// 0x20c
			 0x77c47ef8, /// 0x210
			 0xd62377d9, /// 0x214
			 0x52b331aa, /// 0x218
			 0x38d61482, /// 0x21c
			 0xa837c7b5, /// 0x220
			 0xfde9581c, /// 0x224
			 0x332fbd29, /// 0x228
			 0x7fd8e3af, /// 0x22c
			 0x168b18ba, /// 0x230
			 0x01c141ec, /// 0x234
			 0xbaec968d, /// 0x238
			 0x01cd0839, /// 0x23c
			 0xaa516b9b, /// 0x240
			 0x2868ea3c, /// 0x244
			 0x548e8e4c, /// 0x248
			 0x052535a2, /// 0x24c
			 0x40df4052, /// 0x250
			 0xc23aa1ae, /// 0x254
			 0x9102d7e0, /// 0x258
			 0x78bec0ca, /// 0x25c
			 0xae9de025, /// 0x260
			 0x8717a3b7, /// 0x264
			 0x4628e3c4, /// 0x268
			 0xea883bbe, /// 0x26c
			 0xcf38692a, /// 0x270
			 0x625f4fd6, /// 0x274
			 0x6475baa0, /// 0x278
			 0x7128548e, /// 0x27c
			 0x7cc1f152, /// 0x280
			 0xb57bc0a0, /// 0x284
			 0x57f11041, /// 0x288
			 0x1a6e1532, /// 0x28c
			 0xe2e0f405, /// 0x290
			 0xe2324f55, /// 0x294
			 0xc3d6eba3, /// 0x298
			 0x371e7f3c, /// 0x29c
			 0x2691f38e, /// 0x2a0
			 0x76b214af, /// 0x2a4
			 0x36aa0a82, /// 0x2a8
			 0xd2e0239a, /// 0x2ac
			 0x67b928e5, /// 0x2b0
			 0xd440f3af, /// 0x2b4
			 0x28a572ba, /// 0x2b8
			 0x9fd780d5, /// 0x2bc
			 0x611b492e, /// 0x2c0
			 0x1c80223f, /// 0x2c4
			 0x59aa7c47, /// 0x2c8
			 0xd006f7e8, /// 0x2cc
			 0xa7828e64, /// 0x2d0
			 0xcc6b02fa, /// 0x2d4
			 0x08f8138e, /// 0x2d8
			 0x967053b2, /// 0x2dc
			 0x3cabfac2, /// 0x2e0
			 0x57a085a9, /// 0x2e4
			 0x56c987cb, /// 0x2e8
			 0x710c625d, /// 0x2ec
			 0x4a8dde8e, /// 0x2f0
			 0x4bf92dcb, /// 0x2f4
			 0xd66a906d, /// 0x2f8
			 0x5f813031, /// 0x2fc
			 0x67c11d81, /// 0x300
			 0xb0148a80, /// 0x304
			 0x627d6c07, /// 0x308
			 0x40445fb8, /// 0x30c
			 0xb5b472d6, /// 0x310
			 0x5347629e, /// 0x314
			 0x2b2f5959, /// 0x318
			 0xfbabda20, /// 0x31c
			 0xa60fbc7b, /// 0x320
			 0xba4f01d1, /// 0x324
			 0x2d6847e0, /// 0x328
			 0x230bb009, /// 0x32c
			 0x143773ba, /// 0x330
			 0x4a392a2d, /// 0x334
			 0xb22ebe79, /// 0x338
			 0xfbcdf36a, /// 0x33c
			 0xa50129c4, /// 0x340
			 0x9488cfc6, /// 0x344
			 0x4d7a91b3, /// 0x348
			 0xe2df17bc, /// 0x34c
			 0x77851ad5, /// 0x350
			 0x40b62e42, /// 0x354
			 0x940f7d36, /// 0x358
			 0xe5503e24, /// 0x35c
			 0x4800d7f7, /// 0x360
			 0x61ab34d8, /// 0x364
			 0x5d5cc51f, /// 0x368
			 0x11dca133, /// 0x36c
			 0xecdfb3ec, /// 0x370
			 0xdb115daf, /// 0x374
			 0xe51fd9a1, /// 0x378
			 0x389f6d14, /// 0x37c
			 0xf8605710, /// 0x380
			 0x677dd374, /// 0x384
			 0x0dea9bbe, /// 0x388
			 0xbdd0827c, /// 0x38c
			 0x08d4a67a, /// 0x390
			 0xfdb06d09, /// 0x394
			 0x01eddd06, /// 0x398
			 0xb757799d, /// 0x39c
			 0x08ece0cc, /// 0x3a0
			 0xc6e5de94, /// 0x3a4
			 0x1877b0ca, /// 0x3a8
			 0x06c5ff92, /// 0x3ac
			 0x27dd3228, /// 0x3b0
			 0x77309e26, /// 0x3b4
			 0xa10bc385, /// 0x3b8
			 0xb3ead9a1, /// 0x3bc
			 0x80e39392, /// 0x3c0
			 0x65a79eae, /// 0x3c4
			 0x472b8662, /// 0x3c8
			 0xf9012aaa, /// 0x3cc
			 0x889cea45, /// 0x3d0
			 0xa3506717, /// 0x3d4
			 0x1ad44219, /// 0x3d8
			 0xd1673f4c, /// 0x3dc
			 0x5c5d6ec7, /// 0x3e0
			 0xc8f7d782, /// 0x3e4
			 0x545d0341, /// 0x3e8
			 0xe0a5c998, /// 0x3ec
			 0xa88e8f35, /// 0x3f0
			 0x91b32131, /// 0x3f4
			 0x4627b8b2, /// 0x3f8
			 0x52a33bf9, /// 0x3fc
			 0xdce5a8f1, /// 0x400
			 0x6a7e4540, /// 0x404
			 0xb937fa6d, /// 0x408
			 0x1a38ec48, /// 0x40c
			 0xf013c73b, /// 0x410
			 0x60564906, /// 0x414
			 0x0e2cb89f, /// 0x418
			 0xb7b36137, /// 0x41c
			 0xf1975c24, /// 0x420
			 0xce07454f, /// 0x424
			 0x64bb883e, /// 0x428
			 0xbb0a1331, /// 0x42c
			 0x9b62db2f, /// 0x430
			 0x4a358bd9, /// 0x434
			 0x1e2d2c30, /// 0x438
			 0xeb6fac73, /// 0x43c
			 0x3d306e5e, /// 0x440
			 0xa9f556c9, /// 0x444
			 0x2f577658, /// 0x448
			 0x6ef36b12, /// 0x44c
			 0x4c4783a3, /// 0x450
			 0x4dc30824, /// 0x454
			 0x4fc7f5ee, /// 0x458
			 0xee9dffe2, /// 0x45c
			 0xf7a4a9fe, /// 0x460
			 0x09103386, /// 0x464
			 0x5b451fb6, /// 0x468
			 0x7db767a6, /// 0x46c
			 0xed99915c, /// 0x470
			 0x868386e8, /// 0x474
			 0x747f3b13, /// 0x478
			 0x23a69359, /// 0x47c
			 0x365ea063, /// 0x480
			 0xcd2b195b, /// 0x484
			 0x27e854ed, /// 0x488
			 0x03a9d5f1, /// 0x48c
			 0xddee268d, /// 0x490
			 0x7a69d070, /// 0x494
			 0xaf9ef7ec, /// 0x498
			 0x48a7e00e, /// 0x49c
			 0x5344c7db, /// 0x4a0
			 0x347b59d0, /// 0x4a4
			 0x38be4ef5, /// 0x4a8
			 0x6856bf84, /// 0x4ac
			 0x964ce18d, /// 0x4b0
			 0xe402198e, /// 0x4b4
			 0xe5220551, /// 0x4b8
			 0xdcbd6fec, /// 0x4bc
			 0x030184d6, /// 0x4c0
			 0x099dd7be, /// 0x4c4
			 0xc1426e7b, /// 0x4c8
			 0xfb40b069, /// 0x4cc
			 0x17865184, /// 0x4d0
			 0x6186be94, /// 0x4d4
			 0x69ed7c00, /// 0x4d8
			 0x050f32a4, /// 0x4dc
			 0x2ff0d192, /// 0x4e0
			 0x3089a4e3, /// 0x4e4
			 0x1c70b3f9, /// 0x4e8
			 0xf7739206, /// 0x4ec
			 0xd52d823e, /// 0x4f0
			 0x9d96f31d, /// 0x4f4
			 0xa5bfc8b4, /// 0x4f8
			 0x8308f8a4, /// 0x4fc
			 0xffe91755, /// 0x500
			 0x149423d6, /// 0x504
			 0xe131e2fc, /// 0x508
			 0x451abd4b, /// 0x50c
			 0x774dbbaa, /// 0x510
			 0xc2b6c17e, /// 0x514
			 0xb3466adc, /// 0x518
			 0x68896bfc, /// 0x51c
			 0x3f9a8f90, /// 0x520
			 0x7d8b11ea, /// 0x524
			 0x422ce978, /// 0x528
			 0xd6b55229, /// 0x52c
			 0x60bb2bf3, /// 0x530
			 0x7906d178, /// 0x534
			 0x726f7038, /// 0x538
			 0x47e73c7e, /// 0x53c
			 0x9b4ddbb1, /// 0x540
			 0xfd2a51dc, /// 0x544
			 0xf71b58c8, /// 0x548
			 0x28be5811, /// 0x54c
			 0x3d69e321, /// 0x550
			 0x4804469b, /// 0x554
			 0x569ae30f, /// 0x558
			 0x439a45f8, /// 0x55c
			 0x6b4b42ff, /// 0x560
			 0x8695240f, /// 0x564
			 0x3a199f43, /// 0x568
			 0x33bb8ec7, /// 0x56c
			 0xd80afac3, /// 0x570
			 0x0450ff18, /// 0x574
			 0x5737108f, /// 0x578
			 0xb78b0443, /// 0x57c
			 0x0682e0a0, /// 0x580
			 0x679e9be0, /// 0x584
			 0x46fcf6eb, /// 0x588
			 0x51b89040, /// 0x58c
			 0xed054143, /// 0x590
			 0xc3edc6ee, /// 0x594
			 0x353e4973, /// 0x598
			 0x0185aba5, /// 0x59c
			 0x89d6b0dd, /// 0x5a0
			 0x2a678c32, /// 0x5a4
			 0x643df92b, /// 0x5a8
			 0x834decb7, /// 0x5ac
			 0x45f02fc4, /// 0x5b0
			 0xf3e38c19, /// 0x5b4
			 0x8fd795da, /// 0x5b8
			 0x5eab6fba, /// 0x5bc
			 0x8b1e486e, /// 0x5c0
			 0x3cf2a5be, /// 0x5c4
			 0xf723381f, /// 0x5c8
			 0x5c51ceb8, /// 0x5cc
			 0x37ebe161, /// 0x5d0
			 0x41a7598a, /// 0x5d4
			 0xd04d8692, /// 0x5d8
			 0x699ec2cf, /// 0x5dc
			 0xadb3251d, /// 0x5e0
			 0x797b3d97, /// 0x5e4
			 0x11e84fa0, /// 0x5e8
			 0xf54d19a3, /// 0x5ec
			 0x2f6b01f2, /// 0x5f0
			 0xe55f1b1c, /// 0x5f4
			 0x24009c13, /// 0x5f8
			 0xa9cee2f2, /// 0x5fc
			 0x95054126, /// 0x600
			 0x7eee929c, /// 0x604
			 0xb58f091b, /// 0x608
			 0xe935c5f6, /// 0x60c
			 0xdec7334d, /// 0x610
			 0x4c122a36, /// 0x614
			 0x41edd621, /// 0x618
			 0xd6e735c2, /// 0x61c
			 0xb10183f4, /// 0x620
			 0x837fc585, /// 0x624
			 0x2194aceb, /// 0x628
			 0x2dbd14fa, /// 0x62c
			 0x47401a96, /// 0x630
			 0x8ff0cffe, /// 0x634
			 0xfc9d11fc, /// 0x638
			 0xb5fa5579, /// 0x63c
			 0x79858dc6, /// 0x640
			 0x40129a36, /// 0x644
			 0xd71e44d2, /// 0x648
			 0x864464e0, /// 0x64c
			 0x6bebe348, /// 0x650
			 0x1b9b7d2a, /// 0x654
			 0x043d14e4, /// 0x658
			 0x0f168ad1, /// 0x65c
			 0x8ae841ac, /// 0x660
			 0xb62667bd, /// 0x664
			 0xa6ccb486, /// 0x668
			 0x6290a607, /// 0x66c
			 0x6ed60b0c, /// 0x670
			 0xf4f402b8, /// 0x674
			 0x14739d1a, /// 0x678
			 0xe66d6a85, /// 0x67c
			 0xc14d7987, /// 0x680
			 0x94e39c31, /// 0x684
			 0xe9ee2a3c, /// 0x688
			 0xae8b189d, /// 0x68c
			 0x966edde9, /// 0x690
			 0x2def764f, /// 0x694
			 0xbbe1a241, /// 0x698
			 0x062f5e4b, /// 0x69c
			 0xfcba4bdc, /// 0x6a0
			 0xe7682755, /// 0x6a4
			 0xd47db471, /// 0x6a8
			 0xcccd104d, /// 0x6ac
			 0x0e8f5e6b, /// 0x6b0
			 0x6b5a60d3, /// 0x6b4
			 0xfef64467, /// 0x6b8
			 0x6152b128, /// 0x6bc
			 0xabf20110, /// 0x6c0
			 0x6c659429, /// 0x6c4
			 0x71163a5d, /// 0x6c8
			 0xed60b76d, /// 0x6cc
			 0x8d481b77, /// 0x6d0
			 0xd945f104, /// 0x6d4
			 0xca665eed, /// 0x6d8
			 0x1c541e57, /// 0x6dc
			 0xf972302d, /// 0x6e0
			 0xa3a45027, /// 0x6e4
			 0x7abfeed3, /// 0x6e8
			 0xd2baef10, /// 0x6ec
			 0xe1d3ee92, /// 0x6f0
			 0x692aac52, /// 0x6f4
			 0x5763703d, /// 0x6f8
			 0x538c6c47, /// 0x6fc
			 0xb1dd834d, /// 0x700
			 0xdf6e74c3, /// 0x704
			 0x42f88f01, /// 0x708
			 0xede472e6, /// 0x70c
			 0xc9b456dc, /// 0x710
			 0x71b3b420, /// 0x714
			 0xc9eea110, /// 0x718
			 0x21ae8535, /// 0x71c
			 0x407dafb7, /// 0x720
			 0x13a38d04, /// 0x724
			 0x30a4843e, /// 0x728
			 0xaf82f522, /// 0x72c
			 0x39fbff65, /// 0x730
			 0x9842ce14, /// 0x734
			 0xe5cdac79, /// 0x738
			 0x4f022d60, /// 0x73c
			 0x408f70c8, /// 0x740
			 0xf7d30027, /// 0x744
			 0x088af42c, /// 0x748
			 0x2f41a0fe, /// 0x74c
			 0x819b8390, /// 0x750
			 0x3441d0cc, /// 0x754
			 0x3a548da0, /// 0x758
			 0xcf6a62e4, /// 0x75c
			 0x0002c3fa, /// 0x760
			 0xbfb14037, /// 0x764
			 0x213291b5, /// 0x768
			 0x7a52c4d5, /// 0x76c
			 0x0fe26090, /// 0x770
			 0x576a0b22, /// 0x774
			 0x862c753e, /// 0x778
			 0xb4b2a705, /// 0x77c
			 0xdaac61c6, /// 0x780
			 0xcac76407, /// 0x784
			 0xfd718274, /// 0x788
			 0x3f2dab53, /// 0x78c
			 0x45eb9607, /// 0x790
			 0x701f2feb, /// 0x794
			 0x4049f263, /// 0x798
			 0x2dbd2ed3, /// 0x79c
			 0xa2858740, /// 0x7a0
			 0x8bc7314e, /// 0x7a4
			 0x58752a76, /// 0x7a8
			 0x85203c1b, /// 0x7ac
			 0xf410c071, /// 0x7b0
			 0x5fed22ea, /// 0x7b4
			 0xde5cdcd2, /// 0x7b8
			 0xc0b83bea, /// 0x7bc
			 0xfc6162e7, /// 0x7c0
			 0xd5299cb8, /// 0x7c4
			 0x9a5a00ed, /// 0x7c8
			 0xa148fbbb, /// 0x7cc
			 0xcd3c2bbd, /// 0x7d0
			 0x818756f9, /// 0x7d4
			 0xcac14650, /// 0x7d8
			 0x90996522, /// 0x7dc
			 0xdfbe816c, /// 0x7e0
			 0xca257b07, /// 0x7e4
			 0x4dd00d5a, /// 0x7e8
			 0x3a290946, /// 0x7ec
			 0xa092203c, /// 0x7f0
			 0x4c3efdba, /// 0x7f4
			 0x4c7f3fbe, /// 0x7f8
			 0x1a649597, /// 0x7fc
			 0xbf8e488c, /// 0x800
			 0x47b113b1, /// 0x804
			 0x4ed61faf, /// 0x808
			 0xa44fe988, /// 0x80c
			 0x4b53ef74, /// 0x810
			 0x011907bf, /// 0x814
			 0x01ac929b, /// 0x818
			 0x7501fbcb, /// 0x81c
			 0xa9143dca, /// 0x820
			 0xd541a50a, /// 0x824
			 0xae494bd7, /// 0x828
			 0xabaaf679, /// 0x82c
			 0x3baafd5f, /// 0x830
			 0x80b6506e, /// 0x834
			 0xc91cfe3a, /// 0x838
			 0xe9947ef2, /// 0x83c
			 0x46e671b9, /// 0x840
			 0x6221f9b9, /// 0x844
			 0xfa31cddf, /// 0x848
			 0xaa4ccb1b, /// 0x84c
			 0x8f939165, /// 0x850
			 0x9a5e2a14, /// 0x854
			 0x273f4717, /// 0x858
			 0x30d8f52b, /// 0x85c
			 0xfa9f0643, /// 0x860
			 0x348cf250, /// 0x864
			 0x666d2172, /// 0x868
			 0x7b7a3515, /// 0x86c
			 0x72abac0e, /// 0x870
			 0x85cae545, /// 0x874
			 0xe63a1e34, /// 0x878
			 0xffd412ad, /// 0x87c
			 0x9373ca97, /// 0x880
			 0x1f34b8e2, /// 0x884
			 0x11a8c8d4, /// 0x888
			 0x82f8669e, /// 0x88c
			 0x4a694b61, /// 0x890
			 0xe6c7dd7f, /// 0x894
			 0x3d6cd168, /// 0x898
			 0xbcf75868, /// 0x89c
			 0x989d8272, /// 0x8a0
			 0x85780f7a, /// 0x8a4
			 0xab759b16, /// 0x8a8
			 0x66a069f2, /// 0x8ac
			 0xf8111480, /// 0x8b0
			 0xa44a6e33, /// 0x8b4
			 0xb419e1f9, /// 0x8b8
			 0x68599f86, /// 0x8bc
			 0x333094b7, /// 0x8c0
			 0xe943324c, /// 0x8c4
			 0x55f0caf4, /// 0x8c8
			 0xc81d474c, /// 0x8cc
			 0x5b302bc9, /// 0x8d0
			 0x7e985131, /// 0x8d4
			 0x4dd27239, /// 0x8d8
			 0xb72c6353, /// 0x8dc
			 0xfbd4d04e, /// 0x8e0
			 0xe0e8cff2, /// 0x8e4
			 0x6a8adabf, /// 0x8e8
			 0xac6d34ff, /// 0x8ec
			 0xf5e29112, /// 0x8f0
			 0xea245866, /// 0x8f4
			 0x6935ddd8, /// 0x8f8
			 0xdf9732c4, /// 0x8fc
			 0xc46a3ebf, /// 0x900
			 0x6813c13d, /// 0x904
			 0x41f08a89, /// 0x908
			 0xc4627393, /// 0x90c
			 0x31572052, /// 0x910
			 0xc61363ac, /// 0x914
			 0x13d714dd, /// 0x918
			 0xdb830978, /// 0x91c
			 0x23ea1462, /// 0x920
			 0x385ae9e1, /// 0x924
			 0xcc2551e1, /// 0x928
			 0x49e9c2f7, /// 0x92c
			 0x335eb52e, /// 0x930
			 0xfa35aded, /// 0x934
			 0x51e4430d, /// 0x938
			 0x491a90fb, /// 0x93c
			 0x882e3847, /// 0x940
			 0xaff16609, /// 0x944
			 0x0004a8ee, /// 0x948
			 0xbdd65256, /// 0x94c
			 0x7a79a05c, /// 0x950
			 0xa20a7675, /// 0x954
			 0xf1ed6570, /// 0x958
			 0x91d7dc6e, /// 0x95c
			 0xd8471167, /// 0x960
			 0xaa106a12, /// 0x964
			 0xa6513683, /// 0x968
			 0x16e2d7ba, /// 0x96c
			 0x6e0947ab, /// 0x970
			 0xa432c448, /// 0x974
			 0x347f4326, /// 0x978
			 0x5ce87da7, /// 0x97c
			 0x2f48a096, /// 0x980
			 0x4c0994c7, /// 0x984
			 0xbd97b7cf, /// 0x988
			 0x92b69cd1, /// 0x98c
			 0xe9a82fb5, /// 0x990
			 0xae496537, /// 0x994
			 0x66f42942, /// 0x998
			 0xbe0a58d2, /// 0x99c
			 0x165f1eb7, /// 0x9a0
			 0xd5f16322, /// 0x9a4
			 0xf9a1f62f, /// 0x9a8
			 0x25f95fb3, /// 0x9ac
			 0xe9e5ba97, /// 0x9b0
			 0x87f3f256, /// 0x9b4
			 0x3a69e243, /// 0x9b8
			 0x3c7ceb2e, /// 0x9bc
			 0x89d64201, /// 0x9c0
			 0x739e2fe7, /// 0x9c4
			 0x95142d16, /// 0x9c8
			 0x4521c94d, /// 0x9cc
			 0x1f174bc3, /// 0x9d0
			 0x9909a61e, /// 0x9d4
			 0x7cb59050, /// 0x9d8
			 0x7fcc11d9, /// 0x9dc
			 0x8351bab2, /// 0x9e0
			 0xe3849d9e, /// 0x9e4
			 0x61674b3b, /// 0x9e8
			 0x943a0c42, /// 0x9ec
			 0xf98b0dbf, /// 0x9f0
			 0x700bf228, /// 0x9f4
			 0xdb94b282, /// 0x9f8
			 0xd3c2b13f, /// 0x9fc
			 0x5f9a0721, /// 0xa00
			 0xfb86824c, /// 0xa04
			 0xeb640515, /// 0xa08
			 0xcdc6ca45, /// 0xa0c
			 0x25ccb9db, /// 0xa10
			 0xea1761fc, /// 0xa14
			 0xa64f6ba7, /// 0xa18
			 0x76ad8eac, /// 0xa1c
			 0x08fb09b8, /// 0xa20
			 0x3f5fca26, /// 0xa24
			 0x2bf8864d, /// 0xa28
			 0xebd0b717, /// 0xa2c
			 0xf93e1083, /// 0xa30
			 0xfa708ed9, /// 0xa34
			 0x554f7396, /// 0xa38
			 0x7495023a, /// 0xa3c
			 0x702949d7, /// 0xa40
			 0xcdd75a0c, /// 0xa44
			 0xcc2219a7, /// 0xa48
			 0xbfb870d5, /// 0xa4c
			 0x9b715679, /// 0xa50
			 0xaeec98df, /// 0xa54
			 0x3886eab1, /// 0xa58
			 0xf4f8b808, /// 0xa5c
			 0xf1d0ee92, /// 0xa60
			 0xc355eaef, /// 0xa64
			 0x221f0adf, /// 0xa68
			 0x8388c729, /// 0xa6c
			 0xa5b6954f, /// 0xa70
			 0xe64f45df, /// 0xa74
			 0x51920b3a, /// 0xa78
			 0x86e0318d, /// 0xa7c
			 0xccd371a9, /// 0xa80
			 0x3e6a386f, /// 0xa84
			 0x08464c38, /// 0xa88
			 0x2ef16347, /// 0xa8c
			 0x9816f082, /// 0xa90
			 0x17ac26ea, /// 0xa94
			 0x585f8c7d, /// 0xa98
			 0x6ff39d41, /// 0xa9c
			 0xa8127c23, /// 0xaa0
			 0xf773a36d, /// 0xaa4
			 0x9e113423, /// 0xaa8
			 0x6e0763db, /// 0xaac
			 0x36e12ba1, /// 0xab0
			 0x1e133858, /// 0xab4
			 0x1be52321, /// 0xab8
			 0x7168ef36, /// 0xabc
			 0xc49477f4, /// 0xac0
			 0x15e72e4a, /// 0xac4
			 0xa31facbd, /// 0xac8
			 0xc4fa5531, /// 0xacc
			 0xf3f60778, /// 0xad0
			 0xae1d0571, /// 0xad4
			 0xceb2c9c9, /// 0xad8
			 0xa659cbd3, /// 0xadc
			 0x4815726f, /// 0xae0
			 0x9935346c, /// 0xae4
			 0xfb39ff88, /// 0xae8
			 0x882fe3ff, /// 0xaec
			 0x0755d0a3, /// 0xaf0
			 0x87ba20fe, /// 0xaf4
			 0xcec2ddcf, /// 0xaf8
			 0x0f563c6a, /// 0xafc
			 0xbc7557bb, /// 0xb00
			 0x24afb2a5, /// 0xb04
			 0x7e2062e9, /// 0xb08
			 0xa35617bc, /// 0xb0c
			 0x51d041b5, /// 0xb10
			 0x76d0ff72, /// 0xb14
			 0x6475939a, /// 0xb18
			 0x118ecb7f, /// 0xb1c
			 0xba247499, /// 0xb20
			 0xe5276897, /// 0xb24
			 0x7df61642, /// 0xb28
			 0x2c0a6af8, /// 0xb2c
			 0xaf1ded32, /// 0xb30
			 0xcde61a50, /// 0xb34
			 0xb1ad2267, /// 0xb38
			 0x81a279b4, /// 0xb3c
			 0xb3029e56, /// 0xb40
			 0xa5e9d19c, /// 0xb44
			 0x9ddd617f, /// 0xb48
			 0xa9de23d0, /// 0xb4c
			 0x26922074, /// 0xb50
			 0x9e0483fe, /// 0xb54
			 0xddb5ef12, /// 0xb58
			 0xb8307e1f, /// 0xb5c
			 0xd1510d71, /// 0xb60
			 0x075eeeb4, /// 0xb64
			 0xaa75f63f, /// 0xb68
			 0x7af694bc, /// 0xb6c
			 0x2fd8173a, /// 0xb70
			 0x94e8dec8, /// 0xb74
			 0x27b9b8cf, /// 0xb78
			 0xc2686ec1, /// 0xb7c
			 0x3a921888, /// 0xb80
			 0x303c4cb9, /// 0xb84
			 0xf50e737a, /// 0xb88
			 0xdb0b1a54, /// 0xb8c
			 0xcfa049c5, /// 0xb90
			 0xad6239b5, /// 0xb94
			 0x465e20ef, /// 0xb98
			 0x26d1d94d, /// 0xb9c
			 0x041d227d, /// 0xba0
			 0xd97c144c, /// 0xba4
			 0xf4250c7e, /// 0xba8
			 0xa8c51311, /// 0xbac
			 0xd4a8b5cc, /// 0xbb0
			 0x616a4d3f, /// 0xbb4
			 0x74da331c, /// 0xbb8
			 0x5860561c, /// 0xbbc
			 0x3f00621c, /// 0xbc0
			 0xe0c7cc18, /// 0xbc4
			 0x351b66ed, /// 0xbc8
			 0x30782075, /// 0xbcc
			 0xc9596304, /// 0xbd0
			 0x3cff3a67, /// 0xbd4
			 0x85d4febc, /// 0xbd8
			 0x987a48a0, /// 0xbdc
			 0x4d4a54c8, /// 0xbe0
			 0x1d8b024f, /// 0xbe4
			 0xe4aff714, /// 0xbe8
			 0x578ba995, /// 0xbec
			 0x4b42d309, /// 0xbf0
			 0x1e87e6df, /// 0xbf4
			 0x63176c5f, /// 0xbf8
			 0xad754bc8, /// 0xbfc
			 0x72f77d08, /// 0xc00
			 0xe6b3dbfd, /// 0xc04
			 0xe4e9203e, /// 0xc08
			 0xed024408, /// 0xc0c
			 0x42f6feb8, /// 0xc10
			 0x9f6c00aa, /// 0xc14
			 0x14489d1c, /// 0xc18
			 0x76042c22, /// 0xc1c
			 0xd4442200, /// 0xc20
			 0xacc7c813, /// 0xc24
			 0xc7923f7c, /// 0xc28
			 0xafa39c97, /// 0xc2c
			 0x0e41bb13, /// 0xc30
			 0x8f807c4e, /// 0xc34
			 0xb6ab617f, /// 0xc38
			 0x62aa0616, /// 0xc3c
			 0xcb1ec2ff, /// 0xc40
			 0x6f42fb89, /// 0xc44
			 0xfdc965c1, /// 0xc48
			 0x6d634315, /// 0xc4c
			 0xac7985c5, /// 0xc50
			 0x0f6e94c4, /// 0xc54
			 0x3d1b18d3, /// 0xc58
			 0x52573a67, /// 0xc5c
			 0x146aa27a, /// 0xc60
			 0x3fdff09f, /// 0xc64
			 0xa3650729, /// 0xc68
			 0xdc488685, /// 0xc6c
			 0xdb327c00, /// 0xc70
			 0xd5f87b46, /// 0xc74
			 0x8b536403, /// 0xc78
			 0x4a95273f, /// 0xc7c
			 0xcc5740ee, /// 0xc80
			 0x363e37e4, /// 0xc84
			 0x11d0eb1e, /// 0xc88
			 0x728ce00b, /// 0xc8c
			 0x1389affa, /// 0xc90
			 0xb25d6c28, /// 0xc94
			 0x553e42a7, /// 0xc98
			 0x7f1d2016, /// 0xc9c
			 0x62a4beb3, /// 0xca0
			 0x82b591ba, /// 0xca4
			 0x2e5171b1, /// 0xca8
			 0x4250c6a3, /// 0xcac
			 0x2171f048, /// 0xcb0
			 0x2f26782b, /// 0xcb4
			 0x82b7fc4d, /// 0xcb8
			 0x6f9db01c, /// 0xcbc
			 0x2aeb3f1f, /// 0xcc0
			 0xa0b20c47, /// 0xcc4
			 0xb605259f, /// 0xcc8
			 0xd952a5e2, /// 0xccc
			 0x2bc8fa3a, /// 0xcd0
			 0x8e891cce, /// 0xcd4
			 0x547edb3c, /// 0xcd8
			 0x7209ee13, /// 0xcdc
			 0xd1e6a8d7, /// 0xce0
			 0x1a80bd93, /// 0xce4
			 0xd1f7ceb7, /// 0xce8
			 0x4736b091, /// 0xcec
			 0xbfe9a66c, /// 0xcf0
			 0x378e1bd9, /// 0xcf4
			 0x43ed76bc, /// 0xcf8
			 0x0e4ef2ef, /// 0xcfc
			 0xe3b57079, /// 0xd00
			 0xa1702f94, /// 0xd04
			 0x1fb11e22, /// 0xd08
			 0x3ea50357, /// 0xd0c
			 0xa6d9fb90, /// 0xd10
			 0x16083353, /// 0xd14
			 0x7e8709d5, /// 0xd18
			 0x6bb79fb0, /// 0xd1c
			 0xe519cdb5, /// 0xd20
			 0xe99ead0b, /// 0xd24
			 0x1ec962e1, /// 0xd28
			 0x7ff47b52, /// 0xd2c
			 0xf4d87f02, /// 0xd30
			 0xd962e767, /// 0xd34
			 0xd41dae87, /// 0xd38
			 0x6599282c, /// 0xd3c
			 0xabf9c9f7, /// 0xd40
			 0x0aec8742, /// 0xd44
			 0x7af9aad1, /// 0xd48
			 0x5bd402c6, /// 0xd4c
			 0x06eb57ae, /// 0xd50
			 0x75eea776, /// 0xd54
			 0x0d3a1a1a, /// 0xd58
			 0x977f2665, /// 0xd5c
			 0x430d4616, /// 0xd60
			 0x9302393b, /// 0xd64
			 0xd23162f0, /// 0xd68
			 0x9d229214, /// 0xd6c
			 0x21aa43cb, /// 0xd70
			 0x9947decc, /// 0xd74
			 0x277284c9, /// 0xd78
			 0x0f53f428, /// 0xd7c
			 0x1ec0f91b, /// 0xd80
			 0x797864dc, /// 0xd84
			 0x72fd096a, /// 0xd88
			 0x15af87c3, /// 0xd8c
			 0x00c9857c, /// 0xd90
			 0x55f45dfb, /// 0xd94
			 0xb15ba780, /// 0xd98
			 0x4cf3c79d, /// 0xd9c
			 0x2e809c46, /// 0xda0
			 0x92ca3654, /// 0xda4
			 0xc5cc83b5, /// 0xda8
			 0x95e20a71, /// 0xdac
			 0x00c909a5, /// 0xdb0
			 0xae1080c1, /// 0xdb4
			 0x6cb7ad03, /// 0xdb8
			 0x56ba6ac5, /// 0xdbc
			 0x7bf8d752, /// 0xdc0
			 0xa5d6548c, /// 0xdc4
			 0x0a344669, /// 0xdc8
			 0xab0f03c6, /// 0xdcc
			 0x65418079, /// 0xdd0
			 0xd6ba64b9, /// 0xdd4
			 0xe6a418c6, /// 0xdd8
			 0x2b48701f, /// 0xddc
			 0x390bf2e1, /// 0xde0
			 0x868339e6, /// 0xde4
			 0x38efbe96, /// 0xde8
			 0xe12c5fdc, /// 0xdec
			 0x9ace1c96, /// 0xdf0
			 0x57d7408b, /// 0xdf4
			 0x852725cf, /// 0xdf8
			 0x1b422977, /// 0xdfc
			 0x48922d9e, /// 0xe00
			 0x6a515146, /// 0xe04
			 0x4a3eb65d, /// 0xe08
			 0x6cffa18a, /// 0xe0c
			 0xda1358c0, /// 0xe10
			 0xc24fc18e, /// 0xe14
			 0xd4b82b92, /// 0xe18
			 0x236991a7, /// 0xe1c
			 0xa06bbc98, /// 0xe20
			 0x09c0fbc0, /// 0xe24
			 0xbfff82c2, /// 0xe28
			 0xadb41918, /// 0xe2c
			 0xce1fd5af, /// 0xe30
			 0x545a2b9f, /// 0xe34
			 0x13434cdd, /// 0xe38
			 0x96ea7e8a, /// 0xe3c
			 0x0956db92, /// 0xe40
			 0x3cb1735e, /// 0xe44
			 0x1868f0b0, /// 0xe48
			 0xa4953bd1, /// 0xe4c
			 0xa5664a6b, /// 0xe50
			 0x41153d0f, /// 0xe54
			 0x27a83c50, /// 0xe58
			 0xfe0b218f, /// 0xe5c
			 0xb1d6edea, /// 0xe60
			 0x997d4763, /// 0xe64
			 0xe806f226, /// 0xe68
			 0x79c0d81f, /// 0xe6c
			 0x5ef49479, /// 0xe70
			 0xc31992da, /// 0xe74
			 0x9790938e, /// 0xe78
			 0xf7396168, /// 0xe7c
			 0x1fec47de, /// 0xe80
			 0x598adaad, /// 0xe84
			 0x21ae9878, /// 0xe88
			 0xb786b0aa, /// 0xe8c
			 0x02cfc5ed, /// 0xe90
			 0x6210b875, /// 0xe94
			 0xeb00a4aa, /// 0xe98
			 0xa35f52f6, /// 0xe9c
			 0x2a098b8a, /// 0xea0
			 0x98ae03d9, /// 0xea4
			 0x4e2b032f, /// 0xea8
			 0x063aafa6, /// 0xeac
			 0xc26b4959, /// 0xeb0
			 0x43da484a, /// 0xeb4
			 0xbd75445e, /// 0xeb8
			 0xd86ed669, /// 0xebc
			 0xa56e08c0, /// 0xec0
			 0x323369df, /// 0xec4
			 0xa10d8e68, /// 0xec8
			 0x0adaa67f, /// 0xecc
			 0x3147714a, /// 0xed0
			 0xdd86ad07, /// 0xed4
			 0x6e4d8d27, /// 0xed8
			 0x8fa0cbe5, /// 0xedc
			 0x16dea6f7, /// 0xee0
			 0x1fde3f3c, /// 0xee4
			 0xb54b96c8, /// 0xee8
			 0x4f0421f2, /// 0xeec
			 0xd2733f27, /// 0xef0
			 0xdd96015e, /// 0xef4
			 0x3bde760c, /// 0xef8
			 0xa2b19ffd, /// 0xefc
			 0x7563fa90, /// 0xf00
			 0x6496fc27, /// 0xf04
			 0xfa5df51c, /// 0xf08
			 0xa909d7a6, /// 0xf0c
			 0xbbfdb2ba, /// 0xf10
			 0xf828a959, /// 0xf14
			 0x4b422755, /// 0xf18
			 0x34df8b90, /// 0xf1c
			 0x5aae1a28, /// 0xf20
			 0x62f0c46a, /// 0xf24
			 0xe3c7784e, /// 0xf28
			 0xf937951e, /// 0xf2c
			 0xc4d7eb91, /// 0xf30
			 0x0e792051, /// 0xf34
			 0x8afd8c45, /// 0xf38
			 0x3ffe2a34, /// 0xf3c
			 0xc9b05672, /// 0xf40
			 0x570bcefd, /// 0xf44
			 0x02541f6f, /// 0xf48
			 0x52149052, /// 0xf4c
			 0xfebb52b7, /// 0xf50
			 0xc45251b4, /// 0xf54
			 0x370dbc21, /// 0xf58
			 0x60344936, /// 0xf5c
			 0x5f443040, /// 0xf60
			 0x0c685b91, /// 0xf64
			 0x7a78f99f, /// 0xf68
			 0xf3dc060c, /// 0xf6c
			 0x9576116b, /// 0xf70
			 0x0851ba32, /// 0xf74
			 0xbaf5640d, /// 0xf78
			 0xda012bb3, /// 0xf7c
			 0xb1dc54c8, /// 0xf80
			 0xb0759c91, /// 0xf84
			 0x90d76b96, /// 0xf88
			 0x11aee6a9, /// 0xf8c
			 0xc3789fa2, /// 0xf90
			 0x54a1f879, /// 0xf94
			 0x67025c23, /// 0xf98
			 0x19f9aa9a, /// 0xf9c
			 0xaa9293c8, /// 0xfa0
			 0x8d552ef0, /// 0xfa4
			 0xa4ec6f69, /// 0xfa8
			 0x62b2770d, /// 0xfac
			 0xbabca2e1, /// 0xfb0
			 0x1d84a9e5, /// 0xfb4
			 0x409f3fbf, /// 0xfb8
			 0xae7ddc1f, /// 0xfbc
			 0xb59c8354, /// 0xfc0
			 0xffa46a0e, /// 0xfc4
			 0x52339729, /// 0xfc8
			 0x82ef744c, /// 0xfcc
			 0xa2b5f073, /// 0xfd0
			 0xacf4884d, /// 0xfd4
			 0x445b5b63, /// 0xfd8
			 0x80ca07e9, /// 0xfdc
			 0xcb75fa33, /// 0xfe0
			 0xfbd2c2cb, /// 0xfe4
			 0x39847a5f, /// 0xfe8
			 0x354d5bf6, /// 0xfec
			 0xb8f9247a, /// 0xff0
			 0xaacd626d, /// 0xff4
			 0x23fdcc0b, /// 0xff8
			 0xe3fe6842 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x574d620a, /// 0x0
			 0xe37fc610, /// 0x4
			 0x9ed15bbb, /// 0x8
			 0x08302902, /// 0xc
			 0x9e63e21c, /// 0x10
			 0xcb03415b, /// 0x14
			 0xe10e896a, /// 0x18
			 0x2e85ed19, /// 0x1c
			 0x19a038c5, /// 0x20
			 0xbbe24fba, /// 0x24
			 0xa14142e7, /// 0x28
			 0x475abc6f, /// 0x2c
			 0xe5068730, /// 0x30
			 0xea6bd62d, /// 0x34
			 0x8aa614a2, /// 0x38
			 0xbfc3614f, /// 0x3c
			 0xfd488b95, /// 0x40
			 0x0729564d, /// 0x44
			 0x0a7b4cdf, /// 0x48
			 0xdcb327de, /// 0x4c
			 0x2b2c2877, /// 0x50
			 0x1cbff26c, /// 0x54
			 0xd2c1cc0b, /// 0x58
			 0xdd089966, /// 0x5c
			 0x5a85cada, /// 0x60
			 0x2a1cc0dc, /// 0x64
			 0x285381be, /// 0x68
			 0x745e42cb, /// 0x6c
			 0x1c9aebca, /// 0x70
			 0xc40e457e, /// 0x74
			 0xff0c9108, /// 0x78
			 0x70b7023a, /// 0x7c
			 0xb7359eb3, /// 0x80
			 0xf57b048d, /// 0x84
			 0xfdfc66b8, /// 0x88
			 0xc469bbb4, /// 0x8c
			 0x85b41182, /// 0x90
			 0x297b5a8a, /// 0x94
			 0x1a111c0d, /// 0x98
			 0xc0a0262c, /// 0x9c
			 0x39f570bb, /// 0xa0
			 0x473211dc, /// 0xa4
			 0x249a390f, /// 0xa8
			 0xe1f3ce9e, /// 0xac
			 0x00acb2e0, /// 0xb0
			 0xe2a4c195, /// 0xb4
			 0x4ee4461a, /// 0xb8
			 0xa44c60b4, /// 0xbc
			 0x7e77f7de, /// 0xc0
			 0xfed2ca6f, /// 0xc4
			 0xf96c8017, /// 0xc8
			 0xc3949810, /// 0xcc
			 0xcc8bb485, /// 0xd0
			 0x9c9eac18, /// 0xd4
			 0x0b2c5cbe, /// 0xd8
			 0xbc95dc90, /// 0xdc
			 0x5228d8dc, /// 0xe0
			 0x177702cc, /// 0xe4
			 0x062ba73c, /// 0xe8
			 0x0aeff674, /// 0xec
			 0xe88a2372, /// 0xf0
			 0xbf20d498, /// 0xf4
			 0xb8269eb4, /// 0xf8
			 0x9b1bf426, /// 0xfc
			 0x2759e861, /// 0x100
			 0x49545cf4, /// 0x104
			 0x149797aa, /// 0x108
			 0x5f8ea25f, /// 0x10c
			 0x136613fe, /// 0x110
			 0xb8fe02cf, /// 0x114
			 0x33e2f2a1, /// 0x118
			 0xbe36a0ee, /// 0x11c
			 0x855a77e8, /// 0x120
			 0xa16efb2a, /// 0x124
			 0x9cc78f87, /// 0x128
			 0x7fc45d2a, /// 0x12c
			 0xff2a6512, /// 0x130
			 0xbefbf946, /// 0x134
			 0x9494aa24, /// 0x138
			 0x98b678e3, /// 0x13c
			 0xf68e6b3c, /// 0x140
			 0x7ff88c45, /// 0x144
			 0xf4a28769, /// 0x148
			 0x8f9145b4, /// 0x14c
			 0xcf9fa627, /// 0x150
			 0xcd3cf1dd, /// 0x154
			 0x72e02789, /// 0x158
			 0xad939421, /// 0x15c
			 0x3fec9d9c, /// 0x160
			 0x7e95c301, /// 0x164
			 0xa8c2f4e4, /// 0x168
			 0xa257b9a9, /// 0x16c
			 0x185700f8, /// 0x170
			 0x44298cc9, /// 0x174
			 0x608225d3, /// 0x178
			 0x8a843ef2, /// 0x17c
			 0x4d433c48, /// 0x180
			 0x64ad55b9, /// 0x184
			 0x5cd7e957, /// 0x188
			 0xcabb77d2, /// 0x18c
			 0x9fa0321b, /// 0x190
			 0x4c52ebad, /// 0x194
			 0x12844346, /// 0x198
			 0xfc8ed138, /// 0x19c
			 0x89389f9b, /// 0x1a0
			 0x028895b6, /// 0x1a4
			 0x6ef9a7ce, /// 0x1a8
			 0x64cf2a0e, /// 0x1ac
			 0x39c4bd90, /// 0x1b0
			 0x93590388, /// 0x1b4
			 0x5df8fc65, /// 0x1b8
			 0x8c3a6218, /// 0x1bc
			 0x07007573, /// 0x1c0
			 0x41260821, /// 0x1c4
			 0xe0c6ca25, /// 0x1c8
			 0x918779ec, /// 0x1cc
			 0x54dada19, /// 0x1d0
			 0xb6276cd7, /// 0x1d4
			 0x77daab19, /// 0x1d8
			 0xc3ed034f, /// 0x1dc
			 0xa0ccc493, /// 0x1e0
			 0xefc037c9, /// 0x1e4
			 0x699493da, /// 0x1e8
			 0x198ed39b, /// 0x1ec
			 0xd6f70b55, /// 0x1f0
			 0xb32f8147, /// 0x1f4
			 0xb46891bb, /// 0x1f8
			 0x810b8e81, /// 0x1fc
			 0xdda02135, /// 0x200
			 0x3cba6435, /// 0x204
			 0xf672cb6f, /// 0x208
			 0x7bad963b, /// 0x20c
			 0xd1510352, /// 0x210
			 0x6b5da935, /// 0x214
			 0xaa5daf05, /// 0x218
			 0x57bbfdb0, /// 0x21c
			 0x0154f94f, /// 0x220
			 0x16c76b9c, /// 0x224
			 0x9c87c7f9, /// 0x228
			 0xdad52859, /// 0x22c
			 0x8a35bc25, /// 0x230
			 0x22057271, /// 0x234
			 0xf3443437, /// 0x238
			 0x091a332f, /// 0x23c
			 0x6ee4da28, /// 0x240
			 0x4def9c12, /// 0x244
			 0xb556ce91, /// 0x248
			 0x4f2b3bdb, /// 0x24c
			 0x1e59033f, /// 0x250
			 0x2d8503e9, /// 0x254
			 0xa3773225, /// 0x258
			 0xbe2ce49b, /// 0x25c
			 0xd5d5574f, /// 0x260
			 0x6fae8305, /// 0x264
			 0xe3a1772d, /// 0x268
			 0x6767b9a0, /// 0x26c
			 0xdb99af7f, /// 0x270
			 0x4d70f195, /// 0x274
			 0x0377c1f7, /// 0x278
			 0xb00eacd5, /// 0x27c
			 0x3f9abe34, /// 0x280
			 0x341d0051, /// 0x284
			 0x0a1bfa23, /// 0x288
			 0xf3fd0c93, /// 0x28c
			 0xa8cf7bc1, /// 0x290
			 0x1c7b3781, /// 0x294
			 0xc3e878d3, /// 0x298
			 0x117d0741, /// 0x29c
			 0x6a768f59, /// 0x2a0
			 0x26b4e28d, /// 0x2a4
			 0x343056d9, /// 0x2a8
			 0x19a448be, /// 0x2ac
			 0xd8d2d7f6, /// 0x2b0
			 0xefba5685, /// 0x2b4
			 0x8e49987f, /// 0x2b8
			 0xdbdff402, /// 0x2bc
			 0xf77f3abb, /// 0x2c0
			 0xca5e4e6c, /// 0x2c4
			 0xe15261d4, /// 0x2c8
			 0x67de5102, /// 0x2cc
			 0x6249ae07, /// 0x2d0
			 0xd7588d06, /// 0x2d4
			 0xa35f7b9e, /// 0x2d8
			 0xf98a7a88, /// 0x2dc
			 0xec38f754, /// 0x2e0
			 0xde914029, /// 0x2e4
			 0xecd8564d, /// 0x2e8
			 0x25298136, /// 0x2ec
			 0x8e9ac84d, /// 0x2f0
			 0x62b580f9, /// 0x2f4
			 0xfef4a1be, /// 0x2f8
			 0x80744ced, /// 0x2fc
			 0xd8b98258, /// 0x300
			 0x022d8ae7, /// 0x304
			 0x64349b13, /// 0x308
			 0x9041a593, /// 0x30c
			 0x8062f4e0, /// 0x310
			 0xd2e7b82b, /// 0x314
			 0xb4080933, /// 0x318
			 0xefcbd78f, /// 0x31c
			 0x1ddec199, /// 0x320
			 0x1fbadf50, /// 0x324
			 0x718f12ba, /// 0x328
			 0xb3978b1c, /// 0x32c
			 0x16406e91, /// 0x330
			 0xa89d0e44, /// 0x334
			 0x9bc8d3fa, /// 0x338
			 0xffd899c2, /// 0x33c
			 0xd0b0ff7d, /// 0x340
			 0x9c13aa26, /// 0x344
			 0xfc57e73b, /// 0x348
			 0xd8076122, /// 0x34c
			 0xe4f29105, /// 0x350
			 0xd6a1df38, /// 0x354
			 0xb58a2463, /// 0x358
			 0x4c0cd69e, /// 0x35c
			 0x40ea041e, /// 0x360
			 0x1c7d47be, /// 0x364
			 0x4fdf0342, /// 0x368
			 0xfd691e39, /// 0x36c
			 0x66ed6a05, /// 0x370
			 0x61d91976, /// 0x374
			 0x1fb84d46, /// 0x378
			 0x7f88db00, /// 0x37c
			 0x922c32a3, /// 0x380
			 0x7f2bc65c, /// 0x384
			 0x7bf0ac56, /// 0x388
			 0x5b30d6a8, /// 0x38c
			 0x64f4983b, /// 0x390
			 0x12725600, /// 0x394
			 0x499a097c, /// 0x398
			 0xa479136b, /// 0x39c
			 0x4726c752, /// 0x3a0
			 0x65db317f, /// 0x3a4
			 0xc677702f, /// 0x3a8
			 0x76953469, /// 0x3ac
			 0x6b4e08b9, /// 0x3b0
			 0x56e9bc25, /// 0x3b4
			 0xb75bd789, /// 0x3b8
			 0xc7d13c7a, /// 0x3bc
			 0xdcf23b4a, /// 0x3c0
			 0x4d499de2, /// 0x3c4
			 0xc01db6ae, /// 0x3c8
			 0xcc88ae31, /// 0x3cc
			 0x95646e19, /// 0x3d0
			 0xbe984d4b, /// 0x3d4
			 0x1e7f7b18, /// 0x3d8
			 0x757c01ef, /// 0x3dc
			 0xb762b025, /// 0x3e0
			 0x2f75fa4a, /// 0x3e4
			 0xc6d88805, /// 0x3e8
			 0xd406b528, /// 0x3ec
			 0x6519a16f, /// 0x3f0
			 0x8af0167a, /// 0x3f4
			 0xe670155f, /// 0x3f8
			 0xaddb7c4a, /// 0x3fc
			 0x850de636, /// 0x400
			 0xf0ad3a06, /// 0x404
			 0x9f5869c9, /// 0x408
			 0x34e88e1f, /// 0x40c
			 0xf41d8ffc, /// 0x410
			 0x2b3012ec, /// 0x414
			 0xcdc2b167, /// 0x418
			 0xd637e7c9, /// 0x41c
			 0xb5a1a745, /// 0x420
			 0x635d2fdf, /// 0x424
			 0x7b42bfbf, /// 0x428
			 0x8c6e29e9, /// 0x42c
			 0xf9ff3875, /// 0x430
			 0x5665220e, /// 0x434
			 0x3a89b76b, /// 0x438
			 0xbfc80081, /// 0x43c
			 0x28aaef5b, /// 0x440
			 0x580b5f43, /// 0x444
			 0xf71f5181, /// 0x448
			 0x3748138a, /// 0x44c
			 0xf31f72f5, /// 0x450
			 0x75048f05, /// 0x454
			 0xe8797577, /// 0x458
			 0xd197ad1f, /// 0x45c
			 0xe28f8cc5, /// 0x460
			 0x599ee208, /// 0x464
			 0x300cb38b, /// 0x468
			 0xb2205c04, /// 0x46c
			 0xe2e00a95, /// 0x470
			 0x0dd7a18e, /// 0x474
			 0xd11a6b07, /// 0x478
			 0xfc9a21ef, /// 0x47c
			 0xc1c9e4ea, /// 0x480
			 0xf4e50371, /// 0x484
			 0x9ea5e660, /// 0x488
			 0x2929294c, /// 0x48c
			 0x9ea8d092, /// 0x490
			 0xf7b20260, /// 0x494
			 0xf17333ae, /// 0x498
			 0xeaa4b151, /// 0x49c
			 0x841a760e, /// 0x4a0
			 0xd93776d0, /// 0x4a4
			 0x8e3ebec1, /// 0x4a8
			 0x18051aab, /// 0x4ac
			 0x597731f6, /// 0x4b0
			 0x8daae686, /// 0x4b4
			 0x007c8d60, /// 0x4b8
			 0x29c06ee9, /// 0x4bc
			 0x16cf9500, /// 0x4c0
			 0x347dbb24, /// 0x4c4
			 0x25b332b7, /// 0x4c8
			 0xe71b8092, /// 0x4cc
			 0x66657b90, /// 0x4d0
			 0x2edad751, /// 0x4d4
			 0x264539c8, /// 0x4d8
			 0x3fa74472, /// 0x4dc
			 0x291b7eb1, /// 0x4e0
			 0x41571acd, /// 0x4e4
			 0x7058af23, /// 0x4e8
			 0xcccb9eae, /// 0x4ec
			 0x0fb3700a, /// 0x4f0
			 0xa02e21f6, /// 0x4f4
			 0xe705ed8d, /// 0x4f8
			 0x72968a82, /// 0x4fc
			 0xef2f42f0, /// 0x500
			 0xf007a4eb, /// 0x504
			 0x6a726846, /// 0x508
			 0x0bd6d435, /// 0x50c
			 0xa2763e3f, /// 0x510
			 0x701cf4e2, /// 0x514
			 0x842db6b9, /// 0x518
			 0xbf9d7b62, /// 0x51c
			 0x6b123f75, /// 0x520
			 0xe35abd81, /// 0x524
			 0x1dff9ad4, /// 0x528
			 0xf2caadec, /// 0x52c
			 0x4b4ed514, /// 0x530
			 0x85fb14b1, /// 0x534
			 0xccfca6b5, /// 0x538
			 0x50fbeea8, /// 0x53c
			 0x0ff595ff, /// 0x540
			 0x9b396b60, /// 0x544
			 0x7e35d559, /// 0x548
			 0x718dd519, /// 0x54c
			 0xed9ee083, /// 0x550
			 0x5e87988a, /// 0x554
			 0xac7bd8b6, /// 0x558
			 0x708d5454, /// 0x55c
			 0x2b98b36d, /// 0x560
			 0x3893ddd8, /// 0x564
			 0x39bacdc1, /// 0x568
			 0xfac37dda, /// 0x56c
			 0xeb84f6fd, /// 0x570
			 0xa6453cc4, /// 0x574
			 0xee64d0f1, /// 0x578
			 0x553f5336, /// 0x57c
			 0x402e0cd4, /// 0x580
			 0x9513c4ab, /// 0x584
			 0xa32f700f, /// 0x588
			 0x21a744ec, /// 0x58c
			 0xa1ba40a3, /// 0x590
			 0xae4eb720, /// 0x594
			 0xc3b49e65, /// 0x598
			 0x18ff2f77, /// 0x59c
			 0xf39db63d, /// 0x5a0
			 0x540686be, /// 0x5a4
			 0x9a5a2a13, /// 0x5a8
			 0x1346059c, /// 0x5ac
			 0xf46bb0c3, /// 0x5b0
			 0xeabcf46c, /// 0x5b4
			 0x16b46e0f, /// 0x5b8
			 0x9e045910, /// 0x5bc
			 0x8e90f042, /// 0x5c0
			 0xbdd59e2e, /// 0x5c4
			 0x26d58152, /// 0x5c8
			 0xb9756687, /// 0x5cc
			 0xfd2c1f43, /// 0x5d0
			 0xec42da94, /// 0x5d4
			 0xbe3541b9, /// 0x5d8
			 0x21bb18bd, /// 0x5dc
			 0x0bc37c9a, /// 0x5e0
			 0xd1afe3cb, /// 0x5e4
			 0xbb1c0342, /// 0x5e8
			 0x7bfbba78, /// 0x5ec
			 0xc2e39d66, /// 0x5f0
			 0x421a3331, /// 0x5f4
			 0xc5b9327e, /// 0x5f8
			 0x45723a80, /// 0x5fc
			 0x8baae946, /// 0x600
			 0x150b504f, /// 0x604
			 0xa5192e8e, /// 0x608
			 0xc0b2e128, /// 0x60c
			 0x0a8ae2c8, /// 0x610
			 0x025540ec, /// 0x614
			 0xd10ef930, /// 0x618
			 0x874429d0, /// 0x61c
			 0x5a115668, /// 0x620
			 0x081b51ac, /// 0x624
			 0x7675f189, /// 0x628
			 0x9ff3dfe3, /// 0x62c
			 0xc03c2884, /// 0x630
			 0x8d36f12d, /// 0x634
			 0x9325fb93, /// 0x638
			 0x66517205, /// 0x63c
			 0x24f0fccc, /// 0x640
			 0x970fe7f4, /// 0x644
			 0xc4fdad11, /// 0x648
			 0xb59bdc0e, /// 0x64c
			 0x2224c86f, /// 0x650
			 0xb45a417b, /// 0x654
			 0xa466ff90, /// 0x658
			 0xf9936301, /// 0x65c
			 0x3e1f079d, /// 0x660
			 0x41e2f3c7, /// 0x664
			 0x4ddf8384, /// 0x668
			 0xb7ad2593, /// 0x66c
			 0xb71362b6, /// 0x670
			 0x086a0712, /// 0x674
			 0xd2e00f42, /// 0x678
			 0xe5256bf1, /// 0x67c
			 0x8947f571, /// 0x680
			 0x25d4a5f6, /// 0x684
			 0x8bc3a107, /// 0x688
			 0xcfc5db1a, /// 0x68c
			 0xd76c4a74, /// 0x690
			 0x66789c02, /// 0x694
			 0x0fa24c05, /// 0x698
			 0xe7dd2399, /// 0x69c
			 0x60cee9c1, /// 0x6a0
			 0xb969daec, /// 0x6a4
			 0x506630ea, /// 0x6a8
			 0x88edf105, /// 0x6ac
			 0x30335c9b, /// 0x6b0
			 0x71c580bc, /// 0x6b4
			 0x065031a1, /// 0x6b8
			 0xeca6948c, /// 0x6bc
			 0x6cddfe83, /// 0x6c0
			 0x2400a198, /// 0x6c4
			 0xcfda5f06, /// 0x6c8
			 0x730bddb2, /// 0x6cc
			 0x468ea638, /// 0x6d0
			 0xd91402df, /// 0x6d4
			 0x9896ce55, /// 0x6d8
			 0x301fb4ba, /// 0x6dc
			 0x7f63d59e, /// 0x6e0
			 0xd3372beb, /// 0x6e4
			 0x4a238ec7, /// 0x6e8
			 0xe7bd2c6f, /// 0x6ec
			 0xa46f7474, /// 0x6f0
			 0x2c50b95e, /// 0x6f4
			 0x9a421c93, /// 0x6f8
			 0x20065d16, /// 0x6fc
			 0xdc62c50c, /// 0x700
			 0x5e09ebd8, /// 0x704
			 0x2e26370e, /// 0x708
			 0xb3647006, /// 0x70c
			 0x703f7f66, /// 0x710
			 0x5b9b0ad3, /// 0x714
			 0x30996302, /// 0x718
			 0xea6a7e57, /// 0x71c
			 0xdd0313ca, /// 0x720
			 0xf06e55ee, /// 0x724
			 0x3c74ef15, /// 0x728
			 0x83700a33, /// 0x72c
			 0x0f7dd7bc, /// 0x730
			 0x4d6eab6e, /// 0x734
			 0x4b55c0fb, /// 0x738
			 0x4214f001, /// 0x73c
			 0x5804c184, /// 0x740
			 0x7a39196a, /// 0x744
			 0xad4ddcaa, /// 0x748
			 0x5c3af7dd, /// 0x74c
			 0x7f830101, /// 0x750
			 0x241f38cc, /// 0x754
			 0x1836ce02, /// 0x758
			 0xbb0375e3, /// 0x75c
			 0x756d1edc, /// 0x760
			 0x8876cfcc, /// 0x764
			 0x5b31ac78, /// 0x768
			 0x0c00d455, /// 0x76c
			 0x2360b74a, /// 0x770
			 0x07256554, /// 0x774
			 0xaf2ed760, /// 0x778
			 0x4c601e08, /// 0x77c
			 0x0dd28d0d, /// 0x780
			 0x2a6ad4a1, /// 0x784
			 0x5f7745e5, /// 0x788
			 0x07184bbb, /// 0x78c
			 0xcc8863f7, /// 0x790
			 0xf307413a, /// 0x794
			 0x99a71f5f, /// 0x798
			 0xd09385a4, /// 0x79c
			 0xaa29ac6a, /// 0x7a0
			 0x3f4148fb, /// 0x7a4
			 0x25421fb4, /// 0x7a8
			 0x7683b937, /// 0x7ac
			 0x8fb64ece, /// 0x7b0
			 0xa5b5cebe, /// 0x7b4
			 0x786213e0, /// 0x7b8
			 0x009d02ef, /// 0x7bc
			 0x485fcad3, /// 0x7c0
			 0x1ae36f49, /// 0x7c4
			 0xa1dd4930, /// 0x7c8
			 0x7a66659e, /// 0x7cc
			 0x6e74e681, /// 0x7d0
			 0xa0537b18, /// 0x7d4
			 0x11a57f9e, /// 0x7d8
			 0xcda8080f, /// 0x7dc
			 0x26711037, /// 0x7e0
			 0x7257a03f, /// 0x7e4
			 0xc65460bf, /// 0x7e8
			 0xc86abb89, /// 0x7ec
			 0xe492e143, /// 0x7f0
			 0xfb523c9b, /// 0x7f4
			 0x1594c926, /// 0x7f8
			 0x0bb838e6, /// 0x7fc
			 0xe7d827b7, /// 0x800
			 0x2c2826dd, /// 0x804
			 0xe76eebce, /// 0x808
			 0xe28e393c, /// 0x80c
			 0x115db37d, /// 0x810
			 0x9a3624d1, /// 0x814
			 0x532cd64a, /// 0x818
			 0x4535e363, /// 0x81c
			 0x69f3bd7b, /// 0x820
			 0xd8d2a00a, /// 0x824
			 0x1ecf8b51, /// 0x828
			 0x6826c874, /// 0x82c
			 0x9ae52fce, /// 0x830
			 0x1d666bc5, /// 0x834
			 0xe633a188, /// 0x838
			 0xb31df5dc, /// 0x83c
			 0x341e878a, /// 0x840
			 0xc6065fbc, /// 0x844
			 0x619a88da, /// 0x848
			 0x8a0d9c19, /// 0x84c
			 0x3c8adfb5, /// 0x850
			 0x62163dde, /// 0x854
			 0xd21b16d5, /// 0x858
			 0x9e1c11de, /// 0x85c
			 0xc9cfb166, /// 0x860
			 0xd8326d6b, /// 0x864
			 0x7260c23e, /// 0x868
			 0x8074d393, /// 0x86c
			 0x055520a8, /// 0x870
			 0xfcfbfc61, /// 0x874
			 0x21e0206a, /// 0x878
			 0xd5239b9f, /// 0x87c
			 0x7731a76c, /// 0x880
			 0xf7ab46c2, /// 0x884
			 0x0c475816, /// 0x888
			 0x309b8cd2, /// 0x88c
			 0xbc1a53d0, /// 0x890
			 0x7aea5719, /// 0x894
			 0x5f137d76, /// 0x898
			 0xc48641e1, /// 0x89c
			 0x1f2d07cd, /// 0x8a0
			 0xe17a3fc9, /// 0x8a4
			 0x573c5648, /// 0x8a8
			 0xef569202, /// 0x8ac
			 0x7a161d25, /// 0x8b0
			 0x154813b7, /// 0x8b4
			 0x6a7ca365, /// 0x8b8
			 0x0610f9ee, /// 0x8bc
			 0x5e1dfebb, /// 0x8c0
			 0xf3f617d5, /// 0x8c4
			 0xa80fad70, /// 0x8c8
			 0x3b15afaa, /// 0x8cc
			 0xd643f91e, /// 0x8d0
			 0x9a38f228, /// 0x8d4
			 0xa91f2056, /// 0x8d8
			 0x20108891, /// 0x8dc
			 0xde859f09, /// 0x8e0
			 0x72f0743d, /// 0x8e4
			 0xb0867e3f, /// 0x8e8
			 0x95983575, /// 0x8ec
			 0x385da1bc, /// 0x8f0
			 0x0eed68be, /// 0x8f4
			 0x63bccda1, /// 0x8f8
			 0xf0382730, /// 0x8fc
			 0x7a9c8abb, /// 0x900
			 0x9fd67704, /// 0x904
			 0xd0507a29, /// 0x908
			 0xf3d82958, /// 0x90c
			 0x9361eef0, /// 0x910
			 0x36992a82, /// 0x914
			 0x0591ce77, /// 0x918
			 0x10618bfd, /// 0x91c
			 0x55d291bd, /// 0x920
			 0x01aed424, /// 0x924
			 0xc7fc2d05, /// 0x928
			 0x0daf461b, /// 0x92c
			 0xd2c9d12d, /// 0x930
			 0x175479ab, /// 0x934
			 0xe02a7d75, /// 0x938
			 0x99369fa2, /// 0x93c
			 0x9249c4a8, /// 0x940
			 0x8c865a23, /// 0x944
			 0x4850d28e, /// 0x948
			 0x62293d81, /// 0x94c
			 0x2c7cf017, /// 0x950
			 0xdd5d9ca3, /// 0x954
			 0x012be127, /// 0x958
			 0xf8e49912, /// 0x95c
			 0xd19ad8a7, /// 0x960
			 0x2604c549, /// 0x964
			 0x5ade02c6, /// 0x968
			 0x972e9163, /// 0x96c
			 0x1284e0d9, /// 0x970
			 0xa47d3051, /// 0x974
			 0xe698ab45, /// 0x978
			 0x8d210720, /// 0x97c
			 0x3f06de23, /// 0x980
			 0x4aa6f2f5, /// 0x984
			 0xf38ef7c1, /// 0x988
			 0x3d3be81e, /// 0x98c
			 0x4573d49d, /// 0x990
			 0x4dbca266, /// 0x994
			 0xa3cc4e8b, /// 0x998
			 0xe74ffc72, /// 0x99c
			 0x3d506e71, /// 0x9a0
			 0x70a48071, /// 0x9a4
			 0x418c95a3, /// 0x9a8
			 0x1cead9c4, /// 0x9ac
			 0x50779071, /// 0x9b0
			 0x61be9823, /// 0x9b4
			 0x4d81aa73, /// 0x9b8
			 0xd9d09b92, /// 0x9bc
			 0x4dda62fa, /// 0x9c0
			 0x380ff67e, /// 0x9c4
			 0x91c874af, /// 0x9c8
			 0xd444473a, /// 0x9cc
			 0x10174b52, /// 0x9d0
			 0x71d1c8bf, /// 0x9d4
			 0x0dc7235c, /// 0x9d8
			 0x751c733c, /// 0x9dc
			 0xfe902a36, /// 0x9e0
			 0x997a2168, /// 0x9e4
			 0xb0141bca, /// 0x9e8
			 0xc0c5dab3, /// 0x9ec
			 0xc873a67d, /// 0x9f0
			 0x09a02fde, /// 0x9f4
			 0x9bc24d0d, /// 0x9f8
			 0x0b45829e, /// 0x9fc
			 0xd1307c0e, /// 0xa00
			 0x86942e60, /// 0xa04
			 0xd2dd1d2c, /// 0xa08
			 0xe693cc25, /// 0xa0c
			 0x2667af89, /// 0xa10
			 0xd32421ba, /// 0xa14
			 0x1219ec19, /// 0xa18
			 0xe20587ab, /// 0xa1c
			 0x779c7877, /// 0xa20
			 0xba04fa08, /// 0xa24
			 0x48930e49, /// 0xa28
			 0x84cadd8b, /// 0xa2c
			 0xf7ad2ae3, /// 0xa30
			 0xcce6807a, /// 0xa34
			 0xc0b05123, /// 0xa38
			 0xdf50d329, /// 0xa3c
			 0x0f387306, /// 0xa40
			 0xff4072b6, /// 0xa44
			 0xef8d3ff5, /// 0xa48
			 0x91f8920b, /// 0xa4c
			 0x697f0ce8, /// 0xa50
			 0x204ede70, /// 0xa54
			 0xe98715e4, /// 0xa58
			 0x86917bc7, /// 0xa5c
			 0x72088d19, /// 0xa60
			 0x04b637a2, /// 0xa64
			 0xfe1ce4a7, /// 0xa68
			 0x0ce3284e, /// 0xa6c
			 0xf4fb399c, /// 0xa70
			 0x943ea258, /// 0xa74
			 0x1cc163d4, /// 0xa78
			 0x533a6470, /// 0xa7c
			 0x9c6e0115, /// 0xa80
			 0xb39ddaa7, /// 0xa84
			 0x48607c36, /// 0xa88
			 0x7de5750f, /// 0xa8c
			 0x0f5cdafe, /// 0xa90
			 0xc70efeab, /// 0xa94
			 0x12ebfc47, /// 0xa98
			 0x65c43e58, /// 0xa9c
			 0xaceaf129, /// 0xaa0
			 0xe8094a79, /// 0xaa4
			 0x2bfe33a9, /// 0xaa8
			 0x7acbdd57, /// 0xaac
			 0xa2e7feee, /// 0xab0
			 0x4e44a7a1, /// 0xab4
			 0x24dce497, /// 0xab8
			 0x4997494a, /// 0xabc
			 0x407ea611, /// 0xac0
			 0x2ed36a1d, /// 0xac4
			 0x7e8bbcd9, /// 0xac8
			 0xf97082d2, /// 0xacc
			 0x07e544ff, /// 0xad0
			 0xd5b4ec8a, /// 0xad4
			 0xf76e973b, /// 0xad8
			 0x0aaa1aa3, /// 0xadc
			 0x0fe4a33a, /// 0xae0
			 0xe5010292, /// 0xae4
			 0x43e7d109, /// 0xae8
			 0x24bd7fd8, /// 0xaec
			 0xd25dc967, /// 0xaf0
			 0x91da14dc, /// 0xaf4
			 0x7c920e8d, /// 0xaf8
			 0x0c6a6a78, /// 0xafc
			 0x3686c805, /// 0xb00
			 0x5fc613fa, /// 0xb04
			 0x3bc429a1, /// 0xb08
			 0xc8350eca, /// 0xb0c
			 0x64535d12, /// 0xb10
			 0x6b20e27f, /// 0xb14
			 0xa58dfe71, /// 0xb18
			 0x9ef94da0, /// 0xb1c
			 0xd32e7d55, /// 0xb20
			 0xb5eb9a7a, /// 0xb24
			 0xb2931d33, /// 0xb28
			 0x515aba5b, /// 0xb2c
			 0xcb631d4e, /// 0xb30
			 0x369e445a, /// 0xb34
			 0x2fabcce4, /// 0xb38
			 0x14fc0443, /// 0xb3c
			 0xeab3fe03, /// 0xb40
			 0x38303261, /// 0xb44
			 0xb9f994f0, /// 0xb48
			 0xbc9f9810, /// 0xb4c
			 0x490412a9, /// 0xb50
			 0x237938cf, /// 0xb54
			 0x6ec39769, /// 0xb58
			 0x232b2a92, /// 0xb5c
			 0xb4ed11bf, /// 0xb60
			 0xaea60298, /// 0xb64
			 0xdb3087b5, /// 0xb68
			 0x1f87f982, /// 0xb6c
			 0x48ef01d8, /// 0xb70
			 0xe0217fba, /// 0xb74
			 0x293054bb, /// 0xb78
			 0x50225326, /// 0xb7c
			 0x7b495cc7, /// 0xb80
			 0x46a83987, /// 0xb84
			 0xc59e8fd7, /// 0xb88
			 0x7590d460, /// 0xb8c
			 0xd98456cd, /// 0xb90
			 0x5ab171da, /// 0xb94
			 0x3c7b6973, /// 0xb98
			 0xa8252465, /// 0xb9c
			 0xedd2bcd7, /// 0xba0
			 0xc1987e18, /// 0xba4
			 0xb09a8e13, /// 0xba8
			 0xd59056d8, /// 0xbac
			 0x408f3488, /// 0xbb0
			 0x55ab96e2, /// 0xbb4
			 0xe3e222ae, /// 0xbb8
			 0x62e0623d, /// 0xbbc
			 0x76f2e8d7, /// 0xbc0
			 0x9dc3af01, /// 0xbc4
			 0xc75b4b29, /// 0xbc8
			 0x9c44cdbe, /// 0xbcc
			 0xf6d46b61, /// 0xbd0
			 0xf663c980, /// 0xbd4
			 0x4c9c7905, /// 0xbd8
			 0xe91cb5a9, /// 0xbdc
			 0xfb46fcd7, /// 0xbe0
			 0x3fac4506, /// 0xbe4
			 0x7c9c730d, /// 0xbe8
			 0xf0fb3443, /// 0xbec
			 0xd53eac49, /// 0xbf0
			 0x256904bd, /// 0xbf4
			 0xbfd346b3, /// 0xbf8
			 0xf45cd4eb, /// 0xbfc
			 0x46f18b00, /// 0xc00
			 0xae1cb10c, /// 0xc04
			 0x9c58d039, /// 0xc08
			 0xb8f4863d, /// 0xc0c
			 0x67a632b6, /// 0xc10
			 0x20c7c83a, /// 0xc14
			 0xfda02ed6, /// 0xc18
			 0x91898612, /// 0xc1c
			 0x143443cc, /// 0xc20
			 0xc1610fcf, /// 0xc24
			 0xbd6c1a49, /// 0xc28
			 0xb5b0cdef, /// 0xc2c
			 0x88e6d456, /// 0xc30
			 0x869e6826, /// 0xc34
			 0xbd221469, /// 0xc38
			 0xdbb89410, /// 0xc3c
			 0x983d76c7, /// 0xc40
			 0xf0480e85, /// 0xc44
			 0xac727010, /// 0xc48
			 0x33d167ad, /// 0xc4c
			 0x46ebc4f4, /// 0xc50
			 0xbcee05f3, /// 0xc54
			 0x62399b88, /// 0xc58
			 0x1b05d180, /// 0xc5c
			 0x3d68d4ab, /// 0xc60
			 0x8ecc42d5, /// 0xc64
			 0xd744ffbd, /// 0xc68
			 0x1f03f11f, /// 0xc6c
			 0x3da9acc9, /// 0xc70
			 0xde31881d, /// 0xc74
			 0xecd0cace, /// 0xc78
			 0xb87add99, /// 0xc7c
			 0xe005a248, /// 0xc80
			 0xd6202ef0, /// 0xc84
			 0x579fe371, /// 0xc88
			 0x41ffb662, /// 0xc8c
			 0xf1655d8f, /// 0xc90
			 0xf53e6d7f, /// 0xc94
			 0x83a480cf, /// 0xc98
			 0x30da9653, /// 0xc9c
			 0x0e657b96, /// 0xca0
			 0x7fa1e0df, /// 0xca4
			 0x05aa6db6, /// 0xca8
			 0x395d6ee0, /// 0xcac
			 0xa26aeb3a, /// 0xcb0
			 0xdf808541, /// 0xcb4
			 0xfc8e6c7b, /// 0xcb8
			 0x04d8c4b6, /// 0xcbc
			 0x0fd724e6, /// 0xcc0
			 0x31fe73a9, /// 0xcc4
			 0x50985899, /// 0xcc8
			 0x0d9ced01, /// 0xccc
			 0x8e1bc078, /// 0xcd0
			 0x82fbee43, /// 0xcd4
			 0x5b5af696, /// 0xcd8
			 0x311aa9b9, /// 0xcdc
			 0xf4189afe, /// 0xce0
			 0x156c3544, /// 0xce4
			 0x15e003c5, /// 0xce8
			 0xac729d79, /// 0xcec
			 0x33944775, /// 0xcf0
			 0x40a3f72d, /// 0xcf4
			 0x22042e94, /// 0xcf8
			 0xb2b37d63, /// 0xcfc
			 0x5ee9e56a, /// 0xd00
			 0x62500a78, /// 0xd04
			 0x9fe4cac9, /// 0xd08
			 0x9c2212e4, /// 0xd0c
			 0x70dcb159, /// 0xd10
			 0x81bef1a5, /// 0xd14
			 0x1745fa71, /// 0xd18
			 0x40158ccb, /// 0xd1c
			 0xaa597b64, /// 0xd20
			 0x368939a4, /// 0xd24
			 0x14deca93, /// 0xd28
			 0xe2fee6ac, /// 0xd2c
			 0x6b95abc9, /// 0xd30
			 0xc48eabf8, /// 0xd34
			 0xed89100b, /// 0xd38
			 0xffec1a0f, /// 0xd3c
			 0xa5fd89be, /// 0xd40
			 0xb56e8d71, /// 0xd44
			 0x11993c71, /// 0xd48
			 0x538deb2b, /// 0xd4c
			 0xcd8091c7, /// 0xd50
			 0x50452568, /// 0xd54
			 0xc92b818f, /// 0xd58
			 0x215290ed, /// 0xd5c
			 0xbb8f45d0, /// 0xd60
			 0x8f8b17e9, /// 0xd64
			 0x34e8c2d7, /// 0xd68
			 0x868acd62, /// 0xd6c
			 0x6b5d4978, /// 0xd70
			 0xdc7e7b73, /// 0xd74
			 0xa93e2015, /// 0xd78
			 0xcc3ddf2c, /// 0xd7c
			 0x951f7375, /// 0xd80
			 0x3e0a28fb, /// 0xd84
			 0x29d93e57, /// 0xd88
			 0x1bd07172, /// 0xd8c
			 0x759a8ab8, /// 0xd90
			 0x4948c7bd, /// 0xd94
			 0x2abfe26b, /// 0xd98
			 0x734a98e4, /// 0xd9c
			 0x74e3e295, /// 0xda0
			 0xa5cb9f0c, /// 0xda4
			 0x9a46b5b9, /// 0xda8
			 0xf44425cc, /// 0xdac
			 0x0816d640, /// 0xdb0
			 0xccd9c460, /// 0xdb4
			 0xed0be957, /// 0xdb8
			 0x04518980, /// 0xdbc
			 0xc283a970, /// 0xdc0
			 0xe308cd42, /// 0xdc4
			 0x131d76eb, /// 0xdc8
			 0xe9dfc1f1, /// 0xdcc
			 0x5d320f14, /// 0xdd0
			 0x709f0c55, /// 0xdd4
			 0x982bdab4, /// 0xdd8
			 0xa3373397, /// 0xddc
			 0x1cd22923, /// 0xde0
			 0x142fbdf2, /// 0xde4
			 0xc2af6531, /// 0xde8
			 0xf9d5eaf0, /// 0xdec
			 0x22a2b34d, /// 0xdf0
			 0x016ff661, /// 0xdf4
			 0x28516a6b, /// 0xdf8
			 0xd5235872, /// 0xdfc
			 0xf877fe19, /// 0xe00
			 0xb07a52a1, /// 0xe04
			 0x13158949, /// 0xe08
			 0x6ff3c556, /// 0xe0c
			 0xc96c77ec, /// 0xe10
			 0x8f0856a3, /// 0xe14
			 0x2add5b0b, /// 0xe18
			 0xad6051b3, /// 0xe1c
			 0x637f42c2, /// 0xe20
			 0xab78e10c, /// 0xe24
			 0xbff7aeb9, /// 0xe28
			 0x19e783af, /// 0xe2c
			 0xf8f683a7, /// 0xe30
			 0x30ab5667, /// 0xe34
			 0xc1496c77, /// 0xe38
			 0x25620a2b, /// 0xe3c
			 0x1ac34e26, /// 0xe40
			 0xd82343a7, /// 0xe44
			 0x5cce869e, /// 0xe48
			 0xf6caa2b3, /// 0xe4c
			 0xc440156d, /// 0xe50
			 0x273935ba, /// 0xe54
			 0x38b27e64, /// 0xe58
			 0x3baf701f, /// 0xe5c
			 0xc39d80a3, /// 0xe60
			 0xff9ee1f8, /// 0xe64
			 0xecc6f02e, /// 0xe68
			 0xc1373941, /// 0xe6c
			 0xbc7cf0d8, /// 0xe70
			 0xa30060bb, /// 0xe74
			 0x9613fed8, /// 0xe78
			 0x69a02415, /// 0xe7c
			 0x40a2bbc7, /// 0xe80
			 0x582041f3, /// 0xe84
			 0x47de1e52, /// 0xe88
			 0x5b4ec2b8, /// 0xe8c
			 0xe571f107, /// 0xe90
			 0x38e2717d, /// 0xe94
			 0x55a6e0c7, /// 0xe98
			 0xe455b828, /// 0xe9c
			 0x69b7b294, /// 0xea0
			 0x95645ae3, /// 0xea4
			 0x0978a813, /// 0xea8
			 0x038bb523, /// 0xeac
			 0x78d84cf3, /// 0xeb0
			 0x8af19be0, /// 0xeb4
			 0x4dde2091, /// 0xeb8
			 0xd6179210, /// 0xebc
			 0x59e2071b, /// 0xec0
			 0x3974cb62, /// 0xec4
			 0x0e0e144b, /// 0xec8
			 0x33797901, /// 0xecc
			 0xb446197d, /// 0xed0
			 0xd5fe0f53, /// 0xed4
			 0x461aa76d, /// 0xed8
			 0xe5517216, /// 0xedc
			 0x3a993259, /// 0xee0
			 0xf2837a76, /// 0xee4
			 0xf744bd7b, /// 0xee8
			 0xe4278fb4, /// 0xeec
			 0x6cef06ac, /// 0xef0
			 0xe33cd72e, /// 0xef4
			 0x34674000, /// 0xef8
			 0x97d1b65c, /// 0xefc
			 0x754d9921, /// 0xf00
			 0x6a05312d, /// 0xf04
			 0xc5d6bb47, /// 0xf08
			 0xe757853b, /// 0xf0c
			 0x029d1560, /// 0xf10
			 0xa7e7748e, /// 0xf14
			 0xe0a55b41, /// 0xf18
			 0x9bf0550a, /// 0xf1c
			 0xf4834c46, /// 0xf20
			 0x6a8cdff4, /// 0xf24
			 0x2a453c43, /// 0xf28
			 0xcdf68b83, /// 0xf2c
			 0x13d7b7c0, /// 0xf30
			 0xc544f71a, /// 0xf34
			 0xa3a3335c, /// 0xf38
			 0x85384593, /// 0xf3c
			 0x966fccdf, /// 0xf40
			 0x2f142a7d, /// 0xf44
			 0x563a6276, /// 0xf48
			 0x237ea221, /// 0xf4c
			 0x0985c049, /// 0xf50
			 0x9e3554f4, /// 0xf54
			 0xb141312f, /// 0xf58
			 0x0089a48a, /// 0xf5c
			 0x854f45c2, /// 0xf60
			 0x9fc8c320, /// 0xf64
			 0xcdfdf68e, /// 0xf68
			 0x00f39ece, /// 0xf6c
			 0x6dcff2d7, /// 0xf70
			 0xf92bba00, /// 0xf74
			 0xb77492d4, /// 0xf78
			 0x21400cf9, /// 0xf7c
			 0x036373c6, /// 0xf80
			 0x613647fe, /// 0xf84
			 0x0efe3253, /// 0xf88
			 0x613e6174, /// 0xf8c
			 0x712d5bdd, /// 0xf90
			 0x8a321350, /// 0xf94
			 0xb93d95b9, /// 0xf98
			 0x2f06b776, /// 0xf9c
			 0xaddb8828, /// 0xfa0
			 0x279fb7ac, /// 0xfa4
			 0xdb6a8bb4, /// 0xfa8
			 0xa6c7bd19, /// 0xfac
			 0xaca08df5, /// 0xfb0
			 0xef992ba0, /// 0xfb4
			 0x240fc7f6, /// 0xfb8
			 0xfc4a61a6, /// 0xfbc
			 0xd6ee8e96, /// 0xfc0
			 0x71664fe5, /// 0xfc4
			 0xc94c4118, /// 0xfc8
			 0x72d00f04, /// 0xfcc
			 0x2490ca23, /// 0xfd0
			 0xf7004e23, /// 0xfd4
			 0x432c1f64, /// 0xfd8
			 0x4b75f39d, /// 0xfdc
			 0xe993cb95, /// 0xfe0
			 0xc8b70f2d, /// 0xfe4
			 0xf8239576, /// 0xfe8
			 0x932dfeb5, /// 0xfec
			 0xfc58b958, /// 0xff0
			 0x1bd2807a, /// 0xff4
			 0x73819a8e, /// 0xff8
			 0x7f6f3e4d /// last
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
			 0x0000007c,
			 0x00000730,
			 0x000002f4,
			 0x000000b0,
			 0x00000128,
			 0x00000338,
			 0x000005e4,
			 0x00000514,

			 /// vpu register f2
			 0x41800000,
			 0x41d00000,
			 0x41f00000,
			 0x41d80000,
			 0x40000000,
			 0x41d80000,
			 0x41f00000,
			 0x40e00000,

			 /// vpu register f3
			 0x41b80000,
			 0x41d80000,
			 0x41800000,
			 0x41a00000,
			 0x40000000,
			 0x41b00000,
			 0x41e00000,
			 0x40e00000,

			 /// vpu register f4
			 0x40000000,
			 0x40c00000,
			 0x41f80000,
			 0x40400000,
			 0x41000000,
			 0x41e80000,
			 0x41200000,
			 0x41900000,

			 /// vpu register f5
			 0x41000000,
			 0x41f00000,
			 0x40000000,
			 0x41d00000,
			 0x41300000,
			 0x3f800000,
			 0x40a00000,
			 0x41a00000,

			 /// vpu register f6
			 0x42000000,
			 0x40e00000,
			 0x41c00000,
			 0x41100000,
			 0x41200000,
			 0x41e00000,
			 0x40c00000,
			 0x41400000,

			 /// vpu register f7
			 0x41500000,
			 0x41b00000,
			 0x41b00000,
			 0x3f800000,
			 0x41f00000,
			 0x41800000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f8
			 0x41400000,
			 0x41000000,
			 0x41c80000,
			 0x41600000,
			 0x40e00000,
			 0x40c00000,
			 0x41d00000,
			 0x41d00000,

			 /// vpu register f9
			 0x41100000,
			 0x40a00000,
			 0x40400000,
			 0x41b00000,
			 0x40400000,
			 0x41800000,
			 0x40000000,
			 0x41f80000,

			 /// vpu register f10
			 0x41000000,
			 0x41c80000,
			 0x41200000,
			 0x3f800000,
			 0x41c80000,
			 0x41700000,
			 0x41100000,
			 0x41980000,

			 /// vpu register f11
			 0x41700000,
			 0x41300000,
			 0x41b00000,
			 0x41500000,
			 0x41e00000,
			 0x41980000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f12
			 0x41c00000,
			 0x41e00000,
			 0x41a80000,
			 0x41800000,
			 0x41c00000,
			 0x41400000,
			 0x41800000,
			 0x41000000,

			 /// vpu register f13
			 0x40800000,
			 0x41880000,
			 0x40800000,
			 0x41e80000,
			 0x41b00000,
			 0x41c80000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f14
			 0x41200000,
			 0x41800000,
			 0x41f80000,
			 0x40e00000,
			 0x3f800000,
			 0x40e00000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f15
			 0x41f00000,
			 0x41300000,
			 0x41d00000,
			 0x41880000,
			 0x41d80000,
			 0x41980000,
			 0x41b00000,
			 0x41700000,

			 /// vpu register f16
			 0x41d80000,
			 0x41a80000,
			 0x41b00000,
			 0x41600000,
			 0x40000000,
			 0x41500000,
			 0x41800000,
			 0x41980000,

			 /// vpu register f17
			 0x41e80000,
			 0x41e80000,
			 0x41400000,
			 0x40a00000,
			 0x40c00000,
			 0x41800000,
			 0x41980000,
			 0x40400000,

			 /// vpu register f18
			 0x41a00000,
			 0x41d80000,
			 0x41800000,
			 0x41400000,
			 0x40400000,
			 0x41900000,
			 0x41a80000,
			 0x41f80000,

			 /// vpu register f19
			 0x41f80000,
			 0x41f80000,
			 0x41f00000,
			 0x41f80000,
			 0x42000000,
			 0x41600000,
			 0x41a80000,
			 0x40e00000,

			 /// vpu register f20
			 0x41300000,
			 0x41e80000,
			 0x41a80000,
			 0x41980000,
			 0x41c80000,
			 0x41b00000,
			 0x3f800000,
			 0x41880000,

			 /// vpu register f21
			 0x40000000,
			 0x41c80000,
			 0x41300000,
			 0x41c80000,
			 0x41800000,
			 0x41200000,
			 0x41c00000,
			 0x41c80000,

			 /// vpu register f22
			 0x40e00000,
			 0x41d00000,
			 0x41d80000,
			 0x41400000,
			 0x41f00000,
			 0x42000000,
			 0x40800000,
			 0x41e00000,

			 /// vpu register f23
			 0x41c80000,
			 0x41b00000,
			 0x41700000,
			 0x41a00000,
			 0x41880000,
			 0x3f800000,
			 0x40e00000,
			 0x41400000,

			 /// vpu register f24
			 0x41800000,
			 0x41c00000,
			 0x40800000,
			 0x41b80000,
			 0x41900000,
			 0x41300000,
			 0x41e00000,
			 0x41a00000,

			 /// vpu register f25
			 0x41700000,
			 0x3f800000,
			 0x41e80000,
			 0x41500000,
			 0x41800000,
			 0x40a00000,
			 0x42000000,
			 0x41c80000,

			 /// vpu register f26
			 0x40c00000,
			 0x41b80000,
			 0x41b00000,
			 0x40400000,
			 0x40e00000,
			 0x41980000,
			 0x41f80000,
			 0x41980000,

			 /// vpu register f27
			 0x41b80000,
			 0x41f80000,
			 0x41a80000,
			 0x41200000,
			 0x41c80000,
			 0x40000000,
			 0x41d80000,
			 0x40400000,

			 /// vpu register f28
			 0x41500000,
			 0x41700000,
			 0x41e00000,
			 0x41300000,
			 0x41d00000,
			 0x40a00000,
			 0x41a00000,
			 0x41600000,

			 /// vpu register f29
			 0x41880000,
			 0x41200000,
			 0x41880000,
			 0x41300000,
			 0x41a80000,
			 0x41880000,
			 0x41700000,
			 0x41880000,

			 /// vpu register f30
			 0x41900000,
			 0x41e00000,
			 0x41c80000,
			 0x41d80000,
			 0x40e00000,
			 0x41d00000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f31
			 0x41f00000,
			 0x40e00000,
			 0x41880000,
			 0x40a00000,
			 0x42000000,
			 0x41600000,
			 0x40e00000,
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
		"fmvs.x.ps x6, f3, 4\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f8, 0\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f3, 6\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f1, 4\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f4, 2\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f1, 0\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f7, 2\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f22, 3\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f6, 6\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f9, 3\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f16, 3\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f26, 2\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f11, 5\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f20, 2\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f9, 3\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f6, 4\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f3, 0\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f6, 2\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f2, 1\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f20, 3\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f0, 0\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f15, 6\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f3, 6\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f0, 0\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f19, 2\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f1, 2\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f21, 3\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f21, 1\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f27, 0\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f2, 0\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f0, 1\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f30, 7\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f0, 5\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f11, 0\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f15, 7\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f18, 4\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f11, 7\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f1, 6\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f19, 0\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f24, 2\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f18, 5\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f8, 6\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f30, 2\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f4, 0\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f6, 0\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f29, 6\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f25, 2\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f26, 0\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f11, 5\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f1, 6\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f8, 1\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f17, 0\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f11, 4\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f21, 1\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f17, 6\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f18, 4\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f9, 1\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f20, 6\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f29, 7\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f15, 5\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f13, 1\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f20, 7\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f3, 7\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f20, 6\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f30, 6\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f26, 0\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f3, 6\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f28, 4\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f7, 3\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f21, 3\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f17, 6\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f29, 5\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f22, 2\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f21, 0\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f12, 7\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f24, 2\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f27, 6\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x6, f16, 7\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f20, 7\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f24, 0\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f12, 7\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f2, 7\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f17, 0\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f18, 1\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f25, 6\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f28, 3\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f7, 4\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f2, 4\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x7, f27, 1\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f0, 0\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f4, 3\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x5, f26, 0\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f2, 1\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f13, 1\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f6, 0\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x3, f8, 0\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x9, f0, 1\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f1, 4\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmvs.x.ps x4, f2, 5\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
