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
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00000
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00004
			 0xff800000,                                                  // -inf                                        /// 00008
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00010
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00014
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00018
			 0x0e000007,                                                  // Trailing 1s:                                /// 0001c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00024
			 0x80000000,                                                  // -zero                                       /// 00028
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00030
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00034
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00038
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00040
			 0x0c400000,                                                  // Leading 1s:                                 /// 00044
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00054
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00058
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0005c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00060
			 0x0c700000,                                                  // Leading 1s:                                 /// 00064
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00068
			 0xffc00001,                                                  // -signaling NaN                              /// 0006c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00074
			 0x0e000003,                                                  // Trailing 1s:                                /// 00078
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00080
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00084
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00088
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00090
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00094
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00098
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0009c
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0xff800000,                                                  // -inf                                        /// 000b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000dc
			 0xffc00001,                                                  // -signaling NaN                              /// 000e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00100
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00104
			 0x80000000,                                                  // -zero                                       /// 00108
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00110
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00114
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00118
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00124
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00128
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00130
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00134
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00138
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff800000,                                                  // -inf                                        /// 00148
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0014c
			 0x4b000000,                                                  // 8388608.0                                   /// 00150
			 0x3f028f5c,                                                  // 0.51                                        /// 00154
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00158
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00160
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0016c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00170
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00178
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0017c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00180
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00184
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00190
			 0x55555555,                                                  // 4 random values                             /// 00194
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00198
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0xbf028f5c,                                                  // -0.51                                       /// 001bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001d8
			 0x33333333,                                                  // 4 random values                             /// 001dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00200
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0xbf028f5c,                                                  // -0.51                                       /// 00208
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0020c
			 0x55555555,                                                  // 4 random values                             /// 00210
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00214
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0021c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00220
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00228
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0022c
			 0x7fc00001,                                                  // signaling NaN                               /// 00230
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00234
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00238
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00240
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00248
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00250
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00254
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00258
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00260
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00264
			 0x00011111,                                                  // 9.7958E-41                                  /// 00268
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0026c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00274
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00278
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00280
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00284
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00288
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0028c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00298
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0029c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a0
			 0x7fc00001,                                                  // signaling NaN                               /// 002a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002bc
			 0xcb000000,                                                  // -8388608.0                                  /// 002c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002d8
			 0x55555555,                                                  // 4 random values                             /// 002dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002f4
			 0x00000000,                                                  // zero                                        /// 002f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00300
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0c600000,                                                  // Leading 1s:                                 /// 00308
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0030c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00310
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00314
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00318
			 0x4b000000,                                                  // 8388608.0                                   /// 0031c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x33333333,                                                  // 4 random values                             /// 00324
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00328
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0032c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00330
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00334
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00338
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0033c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00340
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0034c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00350
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00358
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0035c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00360
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00364
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00368
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00370
			 0x80000000,                                                  // -zero                                       /// 00374
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00378
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00388
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0038c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00390
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00394
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00398
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x33333333,                                                  // 4 random values                             /// 003a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003cc
			 0x3f028f5c,                                                  // 0.51                                        /// 003d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00400
			 0x0e000003,                                                  // Trailing 1s:                                /// 00404
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00408
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0040c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00414
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00418
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00420
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00424
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x3f028f5c,                                                  // 0.51                                        /// 00430
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00434
			 0x55555555,                                                  // 4 random values                             /// 00438
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00440
			 0xff800000,                                                  // -inf                                        /// 00444
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00448
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00454
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0045c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00464
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0046c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00474
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00478
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x0c780000,                                                  // Leading 1s:                                 /// 00484
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0048c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0c600000,                                                  // Leading 1s:                                 /// 00494
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00498
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0xffc00001,                                                  // -signaling NaN                              /// 004a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x4b000000,                                                  // 8388608.0                                   /// 004b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004bc
			 0x3f028f5c,                                                  // 0.51                                        /// 004c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00508
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0050c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00510
			 0x00000000,                                                  // zero                                        /// 00514
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00518
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0051c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0xff800000,                                                  // -inf                                        /// 00524
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00528
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0052c
			 0x33333333,                                                  // 4 random values                             /// 00530
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00538
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0053c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00544
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00548
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0054c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00550
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00554
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0055c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0056c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00574
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0057c
			 0x33333333,                                                  // 4 random values                             /// 00580
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00584
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00588
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0058c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00594
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0059c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b4
			 0x3f028f5c,                                                  // 0.51                                        /// 005b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005d4
			 0xbf028f5c,                                                  // -0.51                                       /// 005d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00600
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00604
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00608
			 0x7fc00001,                                                  // signaling NaN                               /// 0060c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00610
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00614
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00618
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00620
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00628
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0062c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00630
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00638
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0063c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00640
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00644
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0064c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00650
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00654
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00660
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00668
			 0x0c700000,                                                  // Leading 1s:                                 /// 0066c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00670
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00674
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00680
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0068c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00694
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0069c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 006e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80011111,                                                  // -9.7958E-41                                 /// 00704
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0070c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00714
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x3f028f5c,                                                  // 0.51                                        /// 00720
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00724
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0072c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0073c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00748
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x7f800000,                                                  // inf                                         /// 00754
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00758
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00768
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00774
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00778
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0077c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00780
			 0x00011111,                                                  // 9.7958E-41                                  /// 00784
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00788
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0078c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00800
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00808
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00810
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00814
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00818
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0081c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00820
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00824
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00828
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0082c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00830
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00834
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00838
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00840
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00850
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0085c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00860
			 0x80000000,                                                  // -zero                                       /// 00864
			 0xffc00001,                                                  // -signaling NaN                              /// 00868
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00870
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00874
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0087c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00880
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00884
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00888
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00890
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00898
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00900
			 0x00011111,                                                  // 9.7958E-41                                  /// 00904
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0090c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00910
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00914
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0091c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00920
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0092c
			 0xcb000000,                                                  // -8388608.0                                  /// 00930
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00934
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0093c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00948
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0094c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00950
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00958
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0095c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00960
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00964
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00968
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0096c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00970
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0097c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x3f028f5c,                                                  // 0.51                                        /// 00988
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00994
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xcb000000,                                                  // -8388608.0                                  /// 0099c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x7fc00001,                                                  // signaling NaN                               /// 009ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f4
			 0x4b000000,                                                  // 8388608.0                                   /// 009f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00a00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a58
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a74
			 0x80000000,                                                  // -zero                                       /// 00a78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aa0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aa4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00abc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ac8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ad0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00adc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ae0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00af4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af8
			 0x55555555,                                                  // 4 random values                             /// 00afc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b18
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x33333333,                                                  // 4 random values                             /// 00b34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7f800000,                                                  // inf                                         /// 00b58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ba0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ba4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bcc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bdc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bf4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bf8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c04
			 0x3f028f5c,                                                  // 0.51                                        /// 00c08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c40
			 0x00000000,                                                  // zero                                        /// 00c44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c90
			 0x7f800000,                                                  // inf                                         /// 00c94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cdc
			 0x80000000,                                                  // -zero                                       /// 00ce0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ce4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ce8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cf0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x33333333,                                                  // 4 random values                             /// 00d08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d18
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x33333333,                                                  // 4 random values                             /// 00d4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d8c
			 0xff800000,                                                  // -inf                                        /// 00d90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00da0
			 0x4b000000,                                                  // 8388608.0                                   /// 00da4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00da8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00db4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dcc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00dd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ddc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00df4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e04
			 0x3f028f5c,                                                  // 0.51                                        /// 00e08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ea8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00eac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ec0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ec4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ec8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ed0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ed8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ee0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00eec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ef4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ef8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00efc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f28
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f74
			 0x4b000000,                                                  // 8388608.0                                   /// 00f78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fa0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fcc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fd0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fdc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fe0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ff0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ff4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff8
			 0x0c7ffc00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff800000,                                                  // -inf                                        /// 00000
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00004
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00008
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00010
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00014
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00018
			 0x0e000001,                                                  // Trailing 1s:                                /// 0001c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00030
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00034
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0003c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00044
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00048
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0004c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00058
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0005c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00060
			 0x0c400000,                                                  // Leading 1s:                                 /// 00064
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00068
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0006c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x7f800000,                                                  // inf                                         /// 00078
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00080
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00084
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00088
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0008c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00090
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x7f800000,                                                  // inf                                         /// 000d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00100
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00108
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00110
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00114
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00118
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00120
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00124
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0c400000,                                                  // Leading 1s:                                 /// 00134
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00138
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0013c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00140
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00148
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00150
			 0x7f800000,                                                  // inf                                         /// 00154
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00158
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00160
			 0x55555555,                                                  // 4 random values                             /// 00164
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00168
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00170
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00174
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00180
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00188
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0018c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00000000,                                                  // zero                                        /// 00194
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00198
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001a4
			 0x55555555,                                                  // 4 random values                             /// 001a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x55555555,                                                  // 4 random values                             /// 001d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00200
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00204
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00208
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00210
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00214
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00228
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00230
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00234
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00238
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0023c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00240
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00248
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0024c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00250
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00254
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0025c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00264
			 0x0c780000,                                                  // Leading 1s:                                 /// 00268
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0026c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c700000,                                                  // Leading 1s:                                 /// 00274
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00278
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0027c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00284
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0028c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00290
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00294
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00298
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0029c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 002f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00300
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00308
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0030c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00310
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00318
			 0x7fc00001,                                                  // signaling NaN                               /// 0031c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00320
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00324
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00328
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0032c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00330
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00334
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00338
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00344
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80011111,                                                  // -9.7958E-41                                 /// 00358
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0035c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xbf028f5c,                                                  // -0.51                                       /// 00364
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0036c
			 0x80000000,                                                  // -zero                                       /// 00370
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00374
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00378
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00380
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00388
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00390
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00394
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00398
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0039c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00000000,                                                  // zero                                        /// 003dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x00000000,                                                  // zero                                        /// 00404
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00408
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0040c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00410
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00418
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00420
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0c400000,                                                  // Leading 1s:                                 /// 00428
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0042c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00430
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00434
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00438
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00440
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xcb000000,                                                  // -8388608.0                                  /// 00448
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0044c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0xffc00001,                                                  // -signaling NaN                              /// 0045c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00464
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00468
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00478
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00488
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00490
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00494
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00498
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 004f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00504
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00508
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0050c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00510
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00514
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00524
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00528
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00530
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00534
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00538
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0053c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00540
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00544
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00548
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0054c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00550
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00554
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0055c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00560
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x7f800000,                                                  // inf                                         /// 0056c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00570
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00574
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00580
			 0x80011111,                                                  // -9.7958E-41                                 /// 00584
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00588
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0058c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00590
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00594
			 0x80011111,                                                  // -9.7958E-41                                 /// 00598
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00600
			 0xffc00001,                                                  // -signaling NaN                              /// 00604
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x0c400000,                                                  // Leading 1s:                                 /// 0060c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00610
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00614
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0061c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00620
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00628
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0062c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00634
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00638
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00640
			 0x7fc00001,                                                  // signaling NaN                               /// 00644
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00648
			 0x0c400000,                                                  // Leading 1s:                                 /// 0064c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00650
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00654
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00658
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0065c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00660
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00674
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00678
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0xcb000000,                                                  // -8388608.0                                  /// 00680
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00688
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00698
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0069c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c0
			 0x3f028f5c,                                                  // 0.51                                        /// 006c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006e8
			 0x55555555,                                                  // 4 random values                             /// 006ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00700
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00704
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00710
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00714
			 0x0e000001,                                                  // Trailing 1s:                                /// 00718
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0071c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00720
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0072c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00730
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00734
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x7fc00001,                                                  // signaling NaN                               /// 0073c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00740
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00744
			 0xffc00001,                                                  // -signaling NaN                              /// 00748
			 0xbf028f5c,                                                  // -0.51                                       /// 0074c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00750
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00754
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00758
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0075c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00764
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00770
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00774
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0077c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00788
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00790
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00798
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0079c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00804
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00808
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00810
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00814
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0081c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00820
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00824
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00828
			 0x0c600000,                                                  // Leading 1s:                                 /// 0082c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00830
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00838
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00844
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00854
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00858
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0085c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00860
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00864
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00868
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0086c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00874
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00878
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00880
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00888
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0088c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00890
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00898
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 008c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c4
			 0x33333333,                                                  // 4 random values                             /// 008c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d0
			 0xbf028f5c,                                                  // -0.51                                       /// 008d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00900
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00904
			 0x3f028f5c,                                                  // 0.51                                        /// 00908
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0xffc00001,                                                  // -signaling NaN                              /// 00910
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00918
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0091c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00924
			 0x0c780000,                                                  // Leading 1s:                                 /// 00928
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0092c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00930
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00934
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00938
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0093c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00940
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00948
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00958
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00964
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0096c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00970
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00974
			 0x7fc00001,                                                  // signaling NaN                               /// 00978
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0097c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00980
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00984
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00988
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00994
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0099c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0xcb000000,                                                  // -8388608.0                                  /// 009f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a00
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aa0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00aac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ab0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ac0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00acc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ad8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00adc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00af0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b04
			 0x80000000,                                                  // -zero                                       /// 00b08
			 0x80000000,                                                  // -zero                                       /// 00b0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b24
			 0x7fc00001,                                                  // signaling NaN                               /// 00b28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b80
			 0x80000000,                                                  // -zero                                       /// 00b84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bdc
			 0x80000000,                                                  // -zero                                       /// 00be0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00be8
			 0x00000000,                                                  // zero                                        /// 00bec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c08
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c58
			 0xff800000,                                                  // -inf                                        /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0xbf028f5c,                                                  // -0.51                                       /// 00c88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c94
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ca4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ca8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x7f800000,                                                  // inf                                         /// 00cd0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cdc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ce0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ce4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xffc00001,                                                  // -signaling NaN                              /// 00cf4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7fc00001,                                                  // signaling NaN                               /// 00d2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00db0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00db8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xffc00001,                                                  // -signaling NaN                              /// 00dc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dcc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00de8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00df4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dfc
			 0x00000000,                                                  // zero                                        /// 00e00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x33333333,                                                  // 4 random values                             /// 00e38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e50
			 0xcb000000,                                                  // -8388608.0                                  /// 00e54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e90
			 0x33333333,                                                  // 4 random values                             /// 00e94
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ea0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eac
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ebc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ec4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ec8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ed8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00edc
			 0x33333333,                                                  // 4 random values                             /// 00ee0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ef4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ef8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00efc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f28
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x7f800000,                                                  // inf                                         /// 00f50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0xcb000000,                                                  // -8388608.0                                  /// 00f68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f6c
			 0x55555555,                                                  // 4 random values                             /// 00f70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fa0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fa4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fa8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00fc4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fe4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ff4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ff8
			 0x00004000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00000
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00008
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0xbf028f5c,                                                  // -0.51                                       /// 00014
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x0c700000,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00020
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00030
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00034
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00038
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00040
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0004c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0xbf028f5c,                                                  // -0.51                                       /// 00054
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0005c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00060
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00064
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0006c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00074
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0008c
			 0xff800000,                                                  // -inf                                        /// 00090
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00094
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00098
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0009c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000ac
			 0xcb000000,                                                  // -8388608.0                                  /// 000b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00100
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00104
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00108
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0011c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00124
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00134
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0013c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00144
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00148
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00150
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00154
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0015c
			 0xff800000,                                                  // -inf                                        /// 00160
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00168
			 0x00000000,                                                  // zero                                        /// 0016c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00178
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0017c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00180
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00184
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00188
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0018c
			 0x7f800000,                                                  // inf                                         /// 00190
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00194
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0019c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00200
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00204
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00208
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0020c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00214
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00218
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0021c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00224
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00228
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00230
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00234
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0023c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00240
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00244
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00248
			 0xff800000,                                                  // -inf                                        /// 0024c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00254
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00258
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0025c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00260
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00264
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00270
			 0x3f028f5c,                                                  // 0.51                                        /// 00274
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0c600000,                                                  // Leading 1s:                                 /// 0027c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00280
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00284
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00288
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0028c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00290
			 0x3f028f5c,                                                  // 0.51                                        /// 00294
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80000000,                                                  // -zero                                       /// 002b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002bc
			 0x55555555,                                                  // 4 random values                             /// 002c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002cc
			 0x33333333,                                                  // 4 random values                             /// 002d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00300
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00304
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xff800000,                                                  // -inf                                        /// 00310
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00318
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0031c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00320
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00328
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0032c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00330
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00334
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0033c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00340
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00350
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0035c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00360
			 0x0c700000,                                                  // Leading 1s:                                 /// 00364
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00368
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0036c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0e000001,                                                  // Trailing 1s:                                /// 00378
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0037c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x4b000000,                                                  // 8388608.0                                   /// 00384
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00388
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0038c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00390
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00398
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0039c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003ac
			 0x7f800000,                                                  // inf                                         /// 003b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xffc00001,                                                  // -signaling NaN                              /// 003e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0xbf028f5c,                                                  // -0.51                                       /// 003fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00404
			 0x7fc00001,                                                  // signaling NaN                               /// 00408
			 0xcb000000,                                                  // -8388608.0                                  /// 0040c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00410
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00414
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00418
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0041c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00420
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0042c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00434
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00438
			 0x0e000007,                                                  // Trailing 1s:                                /// 0043c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00444
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00448
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00450
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00464
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00468
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0047c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00484
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00488
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0048c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00490
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00494
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00498
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004a0
			 0x7fc00001,                                                  // signaling NaN                               /// 004a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ac
			 0xffc00001,                                                  // -signaling NaN                              /// 004b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004dc
			 0x3f028f5c,                                                  // 0.51                                        /// 004e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xffc00001,                                                  // -signaling NaN                              /// 004f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00500
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00504
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00508
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00510
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00514
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00520
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00528
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0052c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00534
			 0xcb000000,                                                  // -8388608.0                                  /// 00538
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00540
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00544
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00550
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00564
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00568
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0056c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00574
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0057c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00580
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00588
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00594
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0059c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x00011111,                                                  // 9.7958E-41                                  /// 0060c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00610
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00618
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0061c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00624
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x0e000003,                                                  // Trailing 1s:                                /// 0062c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0063c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00640
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00644
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00648
			 0x33333333,                                                  // 4 random values                             /// 0064c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00650
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0065c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00660
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0066c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00670
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0c780000,                                                  // Leading 1s:                                 /// 00678
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00684
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00690
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00698
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0069c
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 006c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006c8
			 0x7fc00001,                                                  // signaling NaN                               /// 006cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x4b000000,                                                  // 8388608.0                                   /// 006dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e0
			 0x3f028f5c,                                                  // 0.51                                        /// 006e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00700
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0070c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00710
			 0x0c400000,                                                  // Leading 1s:                                 /// 00714
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0071c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00720
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00728
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0072c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c400000,                                                  // Leading 1s:                                 /// 00734
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00738
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00744
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00750
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00754
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00758
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00760
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00764
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0076c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00770
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00778
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0077c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00780
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00784
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00788
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0078c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00790
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00794
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x4b000000,                                                  // 8388608.0                                   /// 007d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00800
			 0x55555555,                                                  // 4 random values                             /// 00804
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00808
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00810
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00814
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0081c
			 0x00000000,                                                  // zero                                        /// 00820
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0e000003,                                                  // Trailing 1s:                                /// 0082c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00830
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00834
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0083c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00840
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00844
			 0x0e000001,                                                  // Trailing 1s:                                /// 00848
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0084c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00850
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00860
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00864
			 0x00011111,                                                  // 9.7958E-41                                  /// 00868
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00870
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00878
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0087c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00884
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00890
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00894
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00898
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0089c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008b0
			 0xcb000000,                                                  // -8388608.0                                  /// 008b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008f0
			 0x4b000000,                                                  // 8388608.0                                   /// 008f4
			 0x33333333,                                                  // 4 random values                             /// 008f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00908
			 0x00011111,                                                  // 9.7958E-41                                  /// 0090c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00914
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00918
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0091c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00920
			 0x4b000000,                                                  // 8388608.0                                   /// 00924
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00928
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0092c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00934
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0093c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00940
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00944
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0094c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00950
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00954
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0095c
			 0x3f028f5c,                                                  // 0.51                                        /// 00960
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00970
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00974
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00978
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0097c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00980
			 0x3f028f5c,                                                  // 0.51                                        /// 00984
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00988
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0098c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00990
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x33333333,                                                  // 4 random values                             /// 00998
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x7f800000,                                                  // inf                                         /// 009b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x7fc00001,                                                  // signaling NaN                               /// 009dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009ec
			 0xbf028f5c,                                                  // -0.51                                       /// 009f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a0c
			 0x55555555,                                                  // 4 random values                             /// 00a10
			 0x3f028f5c,                                                  // 0.51                                        /// 00a14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a20
			 0xcb000000,                                                  // -8388608.0                                  /// 00a24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a74
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aa0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aa4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00abc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ac0
			 0x80000000,                                                  // -zero                                       /// 00ac4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00acc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ad4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00adc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ae0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ae4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00af0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00af8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00afc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b24
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b30
			 0x3f028f5c,                                                  // 0.51                                        /// 00b34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x33333333,                                                  // 4 random values                             /// 00b60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b78
			 0x4b000000,                                                  // 8388608.0                                   /// 00b7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ba4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bb4
			 0x33333333,                                                  // 4 random values                             /// 00bb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bbc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bcc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bdc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00be4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bf0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c38
			 0x7fc00001,                                                  // signaling NaN                               /// 00c3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c54
			 0x80000000,                                                  // -zero                                       /// 00c58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c60
			 0x80000000,                                                  // -zero                                       /// 00c64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ce0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ce4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cfc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d18
			 0xcb000000,                                                  // -8388608.0                                  /// 00d1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xcb000000,                                                  // -8388608.0                                  /// 00d48
			 0x00000000,                                                  // zero                                        /// 00d4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d74
			 0x3f028f5c,                                                  // 0.51                                        /// 00d78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d7c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d80
			 0xbf028f5c,                                                  // -0.51                                       /// 00d84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d8c
			 0x33333333,                                                  // 4 random values                             /// 00d90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00da4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00db0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00db4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dc0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ddc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e54
			 0x80000000,                                                  // -zero                                       /// 00e58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ea0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ebc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ec0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ecc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ed8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ee0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ef4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ef8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f00
			 0xff800000,                                                  // -inf                                        /// 00f04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f24
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f68
			 0x55555555,                                                  // 4 random values                             /// 00f6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f80
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fa0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fcc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fe0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ff8
			 0x00800002 // min norm + 2ulp                               // subnormals +ve                              /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x860f44e3, /// 0x0
			 0x0008f088, /// 0x4
			 0x5e64e10b, /// 0x8
			 0x7a0305e6, /// 0xc
			 0xe7cd7c0b, /// 0x10
			 0x845a208b, /// 0x14
			 0x83bbf8d2, /// 0x18
			 0x40e73b09, /// 0x1c
			 0x4e73c20b, /// 0x20
			 0x4b42b7d0, /// 0x24
			 0x52cf4514, /// 0x28
			 0x3edd9b2d, /// 0x2c
			 0x4eff4d40, /// 0x30
			 0x439f943b, /// 0x34
			 0xaf3963c9, /// 0x38
			 0x33a678a1, /// 0x3c
			 0x13c702dd, /// 0x40
			 0x6d5d37de, /// 0x44
			 0xbf5095b5, /// 0x48
			 0xefe15b1b, /// 0x4c
			 0xbb82dba4, /// 0x50
			 0xbacaa15b, /// 0x54
			 0x526fc7f9, /// 0x58
			 0xb2107180, /// 0x5c
			 0xb2428217, /// 0x60
			 0xcb105bdb, /// 0x64
			 0xd5508755, /// 0x68
			 0x5d589c83, /// 0x6c
			 0x33e8a411, /// 0x70
			 0x47da1cdb, /// 0x74
			 0xfc6cc9cf, /// 0x78
			 0x80c02f3c, /// 0x7c
			 0xd4ed44f4, /// 0x80
			 0xe1bc8915, /// 0x84
			 0x58b986e0, /// 0x88
			 0x41b230c6, /// 0x8c
			 0xd6533c56, /// 0x90
			 0x20bd3fe4, /// 0x94
			 0x43d0cf2b, /// 0x98
			 0x401717ff, /// 0x9c
			 0x950ac353, /// 0xa0
			 0x53cd80fa, /// 0xa4
			 0x26466ff2, /// 0xa8
			 0x35d25601, /// 0xac
			 0x3c9f1278, /// 0xb0
			 0x250153d4, /// 0xb4
			 0x77844e13, /// 0xb8
			 0xac34dcce, /// 0xbc
			 0x5011dacd, /// 0xc0
			 0x183f00f5, /// 0xc4
			 0xb68bae7b, /// 0xc8
			 0xc36ef368, /// 0xcc
			 0xb7ff989a, /// 0xd0
			 0xebdde3cb, /// 0xd4
			 0x2bd0c402, /// 0xd8
			 0xae6cef89, /// 0xdc
			 0x82a86fd6, /// 0xe0
			 0x785448cb, /// 0xe4
			 0xf8f63ba8, /// 0xe8
			 0x41a2e948, /// 0xec
			 0xd241ac98, /// 0xf0
			 0x2107d1fd, /// 0xf4
			 0x33bad912, /// 0xf8
			 0xd0247722, /// 0xfc
			 0xbb4d2b4b, /// 0x100
			 0xfc04baec, /// 0x104
			 0x12ff807a, /// 0x108
			 0xa16706fa, /// 0x10c
			 0x03775e96, /// 0x110
			 0x9305100d, /// 0x114
			 0x15f463c0, /// 0x118
			 0x6e5d123b, /// 0x11c
			 0xc40829b8, /// 0x120
			 0x54e612b7, /// 0x124
			 0x1106498f, /// 0x128
			 0x8a74014a, /// 0x12c
			 0x9461d783, /// 0x130
			 0x49875e1c, /// 0x134
			 0x4759e1ad, /// 0x138
			 0x18c20eb9, /// 0x13c
			 0x34967ee5, /// 0x140
			 0x150bfed2, /// 0x144
			 0x056d3c97, /// 0x148
			 0xfb81c3f6, /// 0x14c
			 0x6cd0f19c, /// 0x150
			 0xd9909f8d, /// 0x154
			 0x83b9a37a, /// 0x158
			 0x53a5c5d7, /// 0x15c
			 0x33481778, /// 0x160
			 0x6ce1f5d2, /// 0x164
			 0x23d8c465, /// 0x168
			 0xd95ccf0e, /// 0x16c
			 0x82000df6, /// 0x170
			 0x63cbd39a, /// 0x174
			 0xb9d9e08e, /// 0x178
			 0xd8746d3c, /// 0x17c
			 0x26ab7935, /// 0x180
			 0xae046965, /// 0x184
			 0x9126750b, /// 0x188
			 0x6cbdd37c, /// 0x18c
			 0xa4e99fa0, /// 0x190
			 0xaf24b453, /// 0x194
			 0x7b3c6862, /// 0x198
			 0x1b03c7c3, /// 0x19c
			 0xfee6b940, /// 0x1a0
			 0x3c11748b, /// 0x1a4
			 0x2539aeb3, /// 0x1a8
			 0x23829c28, /// 0x1ac
			 0x627bd1b5, /// 0x1b0
			 0xa00efdca, /// 0x1b4
			 0xe7cdcc5b, /// 0x1b8
			 0x45288948, /// 0x1bc
			 0x7cac32e5, /// 0x1c0
			 0x165aa4e9, /// 0x1c4
			 0xa9b19fc0, /// 0x1c8
			 0x9a3afbb2, /// 0x1cc
			 0xc5d7fc1d, /// 0x1d0
			 0x97d8a038, /// 0x1d4
			 0x67f5a7c3, /// 0x1d8
			 0xdf8ee153, /// 0x1dc
			 0xcfcee593, /// 0x1e0
			 0xe9935545, /// 0x1e4
			 0xda9636ad, /// 0x1e8
			 0x0eff399c, /// 0x1ec
			 0x27094ce6, /// 0x1f0
			 0x4482d8d5, /// 0x1f4
			 0x2f666a34, /// 0x1f8
			 0x91a4494c, /// 0x1fc
			 0x6b10eee0, /// 0x200
			 0x93599097, /// 0x204
			 0x3ee69d4d, /// 0x208
			 0x69d75f57, /// 0x20c
			 0xf5200ea1, /// 0x210
			 0xd16cc40b, /// 0x214
			 0x0aba58ac, /// 0x218
			 0x646a4aac, /// 0x21c
			 0xb07bd574, /// 0x220
			 0x418916d1, /// 0x224
			 0x34dfd5b7, /// 0x228
			 0xa1548972, /// 0x22c
			 0xb066f75e, /// 0x230
			 0x31945242, /// 0x234
			 0x27efb3c3, /// 0x238
			 0x662ec769, /// 0x23c
			 0x05abcc78, /// 0x240
			 0xf8f54f67, /// 0x244
			 0x1ebe103a, /// 0x248
			 0x41923e8d, /// 0x24c
			 0xfd7d22d5, /// 0x250
			 0xd1b28988, /// 0x254
			 0x18ce9372, /// 0x258
			 0x1c26639c, /// 0x25c
			 0xdfcffdb4, /// 0x260
			 0x2b1e21e0, /// 0x264
			 0xd22d8f72, /// 0x268
			 0xe61cb340, /// 0x26c
			 0xcecb75f3, /// 0x270
			 0x14eb86b2, /// 0x274
			 0xf6ff23ae, /// 0x278
			 0xa91a2cca, /// 0x27c
			 0x74e23036, /// 0x280
			 0x565689f7, /// 0x284
			 0x80ab2394, /// 0x288
			 0x3fc3ef5d, /// 0x28c
			 0x772550df, /// 0x290
			 0xd88352cc, /// 0x294
			 0x2858dd19, /// 0x298
			 0x213eb3ea, /// 0x29c
			 0x04f00c80, /// 0x2a0
			 0x857fbd25, /// 0x2a4
			 0x6893c02b, /// 0x2a8
			 0x3274917b, /// 0x2ac
			 0x26b1c532, /// 0x2b0
			 0x71c5b3cc, /// 0x2b4
			 0x38ce137e, /// 0x2b8
			 0xcf8a67f2, /// 0x2bc
			 0x0698502c, /// 0x2c0
			 0x131972ce, /// 0x2c4
			 0x93bc9585, /// 0x2c8
			 0x41741f07, /// 0x2cc
			 0xf897357b, /// 0x2d0
			 0xb6960f5f, /// 0x2d4
			 0xdc6f1996, /// 0x2d8
			 0xa4f14a58, /// 0x2dc
			 0xf1b83993, /// 0x2e0
			 0xa956a5a4, /// 0x2e4
			 0xca9e0618, /// 0x2e8
			 0x41cb8d72, /// 0x2ec
			 0x8d4d950d, /// 0x2f0
			 0x29d919b3, /// 0x2f4
			 0xe8554aff, /// 0x2f8
			 0x72391f7c, /// 0x2fc
			 0x56330ced, /// 0x300
			 0x7003d808, /// 0x304
			 0xddf36aca, /// 0x308
			 0x2f9ce9b2, /// 0x30c
			 0xb4c7dc1b, /// 0x310
			 0x6ff50b2c, /// 0x314
			 0x1991b65c, /// 0x318
			 0x70f59876, /// 0x31c
			 0xb41174d9, /// 0x320
			 0xc525a039, /// 0x324
			 0x212e8fc5, /// 0x328
			 0xcd4a7b7e, /// 0x32c
			 0xf5c4f94b, /// 0x330
			 0x6d82d162, /// 0x334
			 0x8847b0ab, /// 0x338
			 0xf8494b7e, /// 0x33c
			 0x110865ca, /// 0x340
			 0x56c6ef8e, /// 0x344
			 0xb0549de3, /// 0x348
			 0xd42fdf4d, /// 0x34c
			 0x9f1d8222, /// 0x350
			 0x3776e5a1, /// 0x354
			 0x2f7b204d, /// 0x358
			 0x4b3d904a, /// 0x35c
			 0x4841070b, /// 0x360
			 0x99f08c02, /// 0x364
			 0x2d8099ff, /// 0x368
			 0xcbbf9f6b, /// 0x36c
			 0x509fdf59, /// 0x370
			 0xf0ffb3c2, /// 0x374
			 0xc0261613, /// 0x378
			 0xb0968028, /// 0x37c
			 0x127fa78b, /// 0x380
			 0x5e43fcf0, /// 0x384
			 0xdd7ecae0, /// 0x388
			 0x3b8d7fc3, /// 0x38c
			 0x4c58bd38, /// 0x390
			 0x7329a3c7, /// 0x394
			 0xabcb91cd, /// 0x398
			 0x8e41fee4, /// 0x39c
			 0x1b3adbb5, /// 0x3a0
			 0x5f265944, /// 0x3a4
			 0xe9883df8, /// 0x3a8
			 0xabe54ddc, /// 0x3ac
			 0x997ddcf4, /// 0x3b0
			 0x376a96b2, /// 0x3b4
			 0x97af35de, /// 0x3b8
			 0x976096a5, /// 0x3bc
			 0x4d7e661f, /// 0x3c0
			 0xe64c15ee, /// 0x3c4
			 0x36896be8, /// 0x3c8
			 0xf94f3980, /// 0x3cc
			 0x383fba7f, /// 0x3d0
			 0x5d643793, /// 0x3d4
			 0x3c65341c, /// 0x3d8
			 0x09167894, /// 0x3dc
			 0xaa3053ea, /// 0x3e0
			 0xc30e1bd5, /// 0x3e4
			 0xb413e829, /// 0x3e8
			 0x6db7e479, /// 0x3ec
			 0xa94f2705, /// 0x3f0
			 0x2373ef34, /// 0x3f4
			 0xb7cae923, /// 0x3f8
			 0xad5f1e5e, /// 0x3fc
			 0x20c07a9f, /// 0x400
			 0xaa92c2e3, /// 0x404
			 0xd1289ca2, /// 0x408
			 0xf0622ea8, /// 0x40c
			 0xcab93bfb, /// 0x410
			 0x2fbaf388, /// 0x414
			 0x640303ff, /// 0x418
			 0x5b159899, /// 0x41c
			 0xfff66e39, /// 0x420
			 0xfa2be765, /// 0x424
			 0xcd69328d, /// 0x428
			 0x67ae39cc, /// 0x42c
			 0xa93a464b, /// 0x430
			 0x692a93e4, /// 0x434
			 0xfe7d1fd5, /// 0x438
			 0x9b4ffaf9, /// 0x43c
			 0x3e47b6e1, /// 0x440
			 0x8297463f, /// 0x444
			 0x3c96ce7d, /// 0x448
			 0xc1f595c0, /// 0x44c
			 0xa8a26e26, /// 0x450
			 0x1323a27b, /// 0x454
			 0x4413d35c, /// 0x458
			 0xf4fcbfb2, /// 0x45c
			 0xc7be34b6, /// 0x460
			 0x8338255d, /// 0x464
			 0xd40d4713, /// 0x468
			 0x6e75c102, /// 0x46c
			 0x5f8629eb, /// 0x470
			 0x25e7ad0c, /// 0x474
			 0xe252a5e8, /// 0x478
			 0xbaf963e9, /// 0x47c
			 0x8409bd30, /// 0x480
			 0xba457241, /// 0x484
			 0xc1da3a8c, /// 0x488
			 0x15d7ab97, /// 0x48c
			 0x2b6df68a, /// 0x490
			 0xfa9f54ab, /// 0x494
			 0xd0d4659c, /// 0x498
			 0xf6cce871, /// 0x49c
			 0xa2c8bcf2, /// 0x4a0
			 0x76cab49a, /// 0x4a4
			 0x46747fbe, /// 0x4a8
			 0xbb788df4, /// 0x4ac
			 0xd2fb5298, /// 0x4b0
			 0x7daa5e3c, /// 0x4b4
			 0x086b9927, /// 0x4b8
			 0x29b011ad, /// 0x4bc
			 0x4a6606e7, /// 0x4c0
			 0x3bfa2726, /// 0x4c4
			 0x73d4e537, /// 0x4c8
			 0xc57a7988, /// 0x4cc
			 0x536752f4, /// 0x4d0
			 0x8090d994, /// 0x4d4
			 0x69ab16a3, /// 0x4d8
			 0xd683d0e5, /// 0x4dc
			 0xfafaa3e0, /// 0x4e0
			 0xd584d5a7, /// 0x4e4
			 0x14699517, /// 0x4e8
			 0xf22f62ce, /// 0x4ec
			 0x53d6b93f, /// 0x4f0
			 0x9728340a, /// 0x4f4
			 0x242b0b0b, /// 0x4f8
			 0xe76319c4, /// 0x4fc
			 0x8d7f6387, /// 0x500
			 0xbac6241c, /// 0x504
			 0xd9572956, /// 0x508
			 0x5f5ca950, /// 0x50c
			 0x38d99089, /// 0x510
			 0xa5dad37f, /// 0x514
			 0xdaad61d5, /// 0x518
			 0x6d23aece, /// 0x51c
			 0xdb1b77bc, /// 0x520
			 0xd5ddb88b, /// 0x524
			 0x67245fd7, /// 0x528
			 0xef454ac1, /// 0x52c
			 0xf95d6507, /// 0x530
			 0xa4d378de, /// 0x534
			 0x098d0943, /// 0x538
			 0x9f0d4a8c, /// 0x53c
			 0xe6717048, /// 0x540
			 0xb5080195, /// 0x544
			 0x849c5d9d, /// 0x548
			 0x7d3a3008, /// 0x54c
			 0xd0f828a4, /// 0x550
			 0x9c2f3519, /// 0x554
			 0x78720e0b, /// 0x558
			 0xd88cfeb4, /// 0x55c
			 0x21ecf118, /// 0x560
			 0xd3fe8f19, /// 0x564
			 0x2c6773e7, /// 0x568
			 0x2a7d4f2b, /// 0x56c
			 0x7d0f1396, /// 0x570
			 0x937c5b7d, /// 0x574
			 0x1522ce40, /// 0x578
			 0xc2ebaf3f, /// 0x57c
			 0x1c56d0bd, /// 0x580
			 0x852919ae, /// 0x584
			 0xb8dbc9c6, /// 0x588
			 0xdd507230, /// 0x58c
			 0x6951fea4, /// 0x590
			 0x90e4f25e, /// 0x594
			 0x39c3a3e9, /// 0x598
			 0xb1f51c79, /// 0x59c
			 0x7b85bf17, /// 0x5a0
			 0x7aa488e7, /// 0x5a4
			 0xb3decc6b, /// 0x5a8
			 0xae98fc72, /// 0x5ac
			 0xcd466464, /// 0x5b0
			 0x791305e7, /// 0x5b4
			 0xa8b91e06, /// 0x5b8
			 0xb2702644, /// 0x5bc
			 0x85c65285, /// 0x5c0
			 0x63d1441b, /// 0x5c4
			 0xf2bbc688, /// 0x5c8
			 0x43dcc389, /// 0x5cc
			 0x097c8c7d, /// 0x5d0
			 0xae553cb5, /// 0x5d4
			 0xa3ce9e1b, /// 0x5d8
			 0x384b7a71, /// 0x5dc
			 0xcba83e6b, /// 0x5e0
			 0xc73c7d12, /// 0x5e4
			 0x5566eda9, /// 0x5e8
			 0x67362768, /// 0x5ec
			 0x20aca3d7, /// 0x5f0
			 0x9d0aef6e, /// 0x5f4
			 0x9b11b6f3, /// 0x5f8
			 0x7029eed5, /// 0x5fc
			 0x87be6ed5, /// 0x600
			 0x9dbcd4f4, /// 0x604
			 0xf50faee1, /// 0x608
			 0x6979f385, /// 0x60c
			 0xc4a60c38, /// 0x610
			 0xa7eec7bf, /// 0x614
			 0xb6178658, /// 0x618
			 0x9b4cd490, /// 0x61c
			 0x8c5a664b, /// 0x620
			 0x32837293, /// 0x624
			 0x43d961de, /// 0x628
			 0x775eb2a4, /// 0x62c
			 0x38eda989, /// 0x630
			 0x3cc92801, /// 0x634
			 0x2797be62, /// 0x638
			 0x6c55fc6e, /// 0x63c
			 0xf152e170, /// 0x640
			 0x93c921e6, /// 0x644
			 0xec63548f, /// 0x648
			 0xb7d218a7, /// 0x64c
			 0x498254a9, /// 0x650
			 0x8edf7541, /// 0x654
			 0xd2167371, /// 0x658
			 0x1bd31e83, /// 0x65c
			 0x656f0e31, /// 0x660
			 0x4b1f509e, /// 0x664
			 0x4601e898, /// 0x668
			 0xcee932c3, /// 0x66c
			 0x21425fee, /// 0x670
			 0xa962b705, /// 0x674
			 0xc5476950, /// 0x678
			 0xf40f4af3, /// 0x67c
			 0x15dac44c, /// 0x680
			 0xed8f0ac2, /// 0x684
			 0xf39a88ba, /// 0x688
			 0x8e1492be, /// 0x68c
			 0x98aae8fd, /// 0x690
			 0x7a05a6bf, /// 0x694
			 0xbbb168fb, /// 0x698
			 0xeae06429, /// 0x69c
			 0x11dc5a1d, /// 0x6a0
			 0xb3faf468, /// 0x6a4
			 0x5bb1388b, /// 0x6a8
			 0xaa92d4b4, /// 0x6ac
			 0x3b0cd63b, /// 0x6b0
			 0x3e97b388, /// 0x6b4
			 0x9192ffdf, /// 0x6b8
			 0x24f6b1c0, /// 0x6bc
			 0xfbeee6cd, /// 0x6c0
			 0x41ac1dfe, /// 0x6c4
			 0xea9e536e, /// 0x6c8
			 0xaba08777, /// 0x6cc
			 0x4f222bca, /// 0x6d0
			 0xec9e1cf0, /// 0x6d4
			 0x67dd3f5b, /// 0x6d8
			 0x0e7cefb8, /// 0x6dc
			 0xf5ab12fe, /// 0x6e0
			 0xb697c4b3, /// 0x6e4
			 0x29dbe6af, /// 0x6e8
			 0x1e514a93, /// 0x6ec
			 0x954ef03d, /// 0x6f0
			 0x6a44b555, /// 0x6f4
			 0x2c85e450, /// 0x6f8
			 0x21f226d8, /// 0x6fc
			 0xf9336d13, /// 0x700
			 0x70acb9df, /// 0x704
			 0xa52838d3, /// 0x708
			 0xb2af53af, /// 0x70c
			 0xf3f390f2, /// 0x710
			 0xc39a2b24, /// 0x714
			 0x33d92d69, /// 0x718
			 0x902fb03f, /// 0x71c
			 0x72b56ac6, /// 0x720
			 0x0f588c26, /// 0x724
			 0xd29e2402, /// 0x728
			 0x6abeb985, /// 0x72c
			 0x13843df7, /// 0x730
			 0x13fd2448, /// 0x734
			 0xec64eeb0, /// 0x738
			 0xd0cb8059, /// 0x73c
			 0x336a48f9, /// 0x740
			 0x03baa3dd, /// 0x744
			 0xe6cc1822, /// 0x748
			 0x7787a116, /// 0x74c
			 0x89bc5c7a, /// 0x750
			 0xb3f43691, /// 0x754
			 0x8317dd01, /// 0x758
			 0x18b5f72f, /// 0x75c
			 0x17d59279, /// 0x760
			 0x307b0ee0, /// 0x764
			 0x118f749a, /// 0x768
			 0x08f56994, /// 0x76c
			 0xab5acf79, /// 0x770
			 0x5f963ad7, /// 0x774
			 0x2722cd64, /// 0x778
			 0x28ddd3c1, /// 0x77c
			 0xe1823489, /// 0x780
			 0x43d3333e, /// 0x784
			 0xa73a220a, /// 0x788
			 0x5b14719c, /// 0x78c
			 0x72aa1617, /// 0x790
			 0xb1f6d622, /// 0x794
			 0xf415b49e, /// 0x798
			 0xe3179b10, /// 0x79c
			 0x305a633c, /// 0x7a0
			 0x50448cd6, /// 0x7a4
			 0xfafb2a3d, /// 0x7a8
			 0xa67f3fff, /// 0x7ac
			 0x91a1edac, /// 0x7b0
			 0x5695a1c2, /// 0x7b4
			 0x1bed7a22, /// 0x7b8
			 0xac8c76ad, /// 0x7bc
			 0x48f531b8, /// 0x7c0
			 0x27a1235f, /// 0x7c4
			 0x50eb97d7, /// 0x7c8
			 0x5b35f3a8, /// 0x7cc
			 0x92530cfd, /// 0x7d0
			 0xfa9e2d4f, /// 0x7d4
			 0xc79c6772, /// 0x7d8
			 0xa62fccf2, /// 0x7dc
			 0x3344310e, /// 0x7e0
			 0x2edb94e5, /// 0x7e4
			 0x2b330666, /// 0x7e8
			 0x88dcb014, /// 0x7ec
			 0x0004abb1, /// 0x7f0
			 0x47372458, /// 0x7f4
			 0xe2a6c12c, /// 0x7f8
			 0x6af01148, /// 0x7fc
			 0xea1ccd81, /// 0x800
			 0xa19f5b61, /// 0x804
			 0xc07ddb89, /// 0x808
			 0xbe583295, /// 0x80c
			 0x78b1ab62, /// 0x810
			 0x6c019989, /// 0x814
			 0x95e98da2, /// 0x818
			 0xce9ce910, /// 0x81c
			 0xd7d6e044, /// 0x820
			 0xc3a0f12c, /// 0x824
			 0x45086cc0, /// 0x828
			 0x78d713ac, /// 0x82c
			 0xf517b74c, /// 0x830
			 0x9f109013, /// 0x834
			 0xb5857eb3, /// 0x838
			 0x2ab40cf2, /// 0x83c
			 0x1bf0c368, /// 0x840
			 0x7780d5b6, /// 0x844
			 0xdd432453, /// 0x848
			 0xe334a92f, /// 0x84c
			 0x9d7f1470, /// 0x850
			 0xaacdae19, /// 0x854
			 0x95d64538, /// 0x858
			 0xae824e1c, /// 0x85c
			 0x7237fa72, /// 0x860
			 0xd365e51b, /// 0x864
			 0x4c0a43af, /// 0x868
			 0x13ddf430, /// 0x86c
			 0x9d350997, /// 0x870
			 0xa65ef610, /// 0x874
			 0x7d19edee, /// 0x878
			 0xbdc16d65, /// 0x87c
			 0x418b5d9a, /// 0x880
			 0xe9ea25dc, /// 0x884
			 0xbd715c3c, /// 0x888
			 0xa225dca3, /// 0x88c
			 0x8cf5a460, /// 0x890
			 0x4227ddb5, /// 0x894
			 0xf82ac85f, /// 0x898
			 0x7ae5070b, /// 0x89c
			 0x172d89db, /// 0x8a0
			 0x1604bf00, /// 0x8a4
			 0x073e9dc3, /// 0x8a8
			 0xddc78b0f, /// 0x8ac
			 0x0404b3ff, /// 0x8b0
			 0x53a459f7, /// 0x8b4
			 0xa2ee6336, /// 0x8b8
			 0x28fb8236, /// 0x8bc
			 0x1941fd92, /// 0x8c0
			 0x49aec115, /// 0x8c4
			 0x41e99270, /// 0x8c8
			 0x01eb0767, /// 0x8cc
			 0x1cc9e454, /// 0x8d0
			 0xf4e28c59, /// 0x8d4
			 0x029224fa, /// 0x8d8
			 0x2aa91208, /// 0x8dc
			 0xe20605a1, /// 0x8e0
			 0xe51e98c7, /// 0x8e4
			 0x275a4a14, /// 0x8e8
			 0x0fbf5694, /// 0x8ec
			 0xef64b27f, /// 0x8f0
			 0x4d4fda45, /// 0x8f4
			 0x550bfff9, /// 0x8f8
			 0x4ed41c6c, /// 0x8fc
			 0xb556b0d5, /// 0x900
			 0xe3318b72, /// 0x904
			 0xe9eed34b, /// 0x908
			 0x2706e2f8, /// 0x90c
			 0x2e999a3f, /// 0x910
			 0xceed3e0a, /// 0x914
			 0x048f01e9, /// 0x918
			 0xdd612c57, /// 0x91c
			 0x2bb7aec4, /// 0x920
			 0xb29569f4, /// 0x924
			 0x31e30b8d, /// 0x928
			 0xac818031, /// 0x92c
			 0x9069bcba, /// 0x930
			 0xd9051f46, /// 0x934
			 0x794ae2db, /// 0x938
			 0xc6cb5d8c, /// 0x93c
			 0xe71839c5, /// 0x940
			 0xddc2ea7e, /// 0x944
			 0xfcd9e5cf, /// 0x948
			 0xd903306f, /// 0x94c
			 0x010971a3, /// 0x950
			 0x77b36688, /// 0x954
			 0xe5db93da, /// 0x958
			 0xfe4b28a9, /// 0x95c
			 0xb689bc9d, /// 0x960
			 0xb70289c6, /// 0x964
			 0xba82f47f, /// 0x968
			 0xceecec90, /// 0x96c
			 0xac868ef6, /// 0x970
			 0x0bdba656, /// 0x974
			 0x591bafbe, /// 0x978
			 0xf5c95197, /// 0x97c
			 0x5888a501, /// 0x980
			 0x23e9a4e8, /// 0x984
			 0x7f076cb9, /// 0x988
			 0x8f1ad2cd, /// 0x98c
			 0x9cdcfa5e, /// 0x990
			 0x0af3eec2, /// 0x994
			 0xec28e0b9, /// 0x998
			 0xc338d087, /// 0x99c
			 0x7f178e3c, /// 0x9a0
			 0x63cde9ef, /// 0x9a4
			 0x04c1c982, /// 0x9a8
			 0x365f1b9a, /// 0x9ac
			 0xa25dccb6, /// 0x9b0
			 0x797fce64, /// 0x9b4
			 0x831b8ff7, /// 0x9b8
			 0x1dea7155, /// 0x9bc
			 0x619fe8f0, /// 0x9c0
			 0xee438192, /// 0x9c4
			 0x57722fda, /// 0x9c8
			 0x2c2d7d18, /// 0x9cc
			 0x0d4d7aa6, /// 0x9d0
			 0xd3e7e491, /// 0x9d4
			 0x1a3f703e, /// 0x9d8
			 0x1340d1f6, /// 0x9dc
			 0xf1419fe0, /// 0x9e0
			 0x436e4f7f, /// 0x9e4
			 0xbf13b08f, /// 0x9e8
			 0xb1aa90e3, /// 0x9ec
			 0xda69da48, /// 0x9f0
			 0xa9f0169a, /// 0x9f4
			 0x91b0e430, /// 0x9f8
			 0x6925f2cf, /// 0x9fc
			 0xab37fe4f, /// 0xa00
			 0x0bf82709, /// 0xa04
			 0x463ba1ce, /// 0xa08
			 0xa56a04aa, /// 0xa0c
			 0x66d4e7b3, /// 0xa10
			 0x6ed130d5, /// 0xa14
			 0xaf3c4ad9, /// 0xa18
			 0xec461936, /// 0xa1c
			 0xdc0fb270, /// 0xa20
			 0x410ca646, /// 0xa24
			 0xdd9d9763, /// 0xa28
			 0xe95b7735, /// 0xa2c
			 0x975efa3e, /// 0xa30
			 0xb8cd219e, /// 0xa34
			 0x18fc423f, /// 0xa38
			 0xc4be7a04, /// 0xa3c
			 0xb92876a8, /// 0xa40
			 0x69409d30, /// 0xa44
			 0xabfd60f8, /// 0xa48
			 0xad923c30, /// 0xa4c
			 0x7355f6b9, /// 0xa50
			 0x25057067, /// 0xa54
			 0xa9a3a6af, /// 0xa58
			 0x7864662b, /// 0xa5c
			 0xdb63ebeb, /// 0xa60
			 0x223b7f6b, /// 0xa64
			 0xaee22278, /// 0xa68
			 0xe50a3631, /// 0xa6c
			 0xbc22e52d, /// 0xa70
			 0xb1ef7086, /// 0xa74
			 0x3b8dd8f3, /// 0xa78
			 0x1166d9da, /// 0xa7c
			 0x4a4a9c87, /// 0xa80
			 0xcce23828, /// 0xa84
			 0xef8c1cc3, /// 0xa88
			 0x467a04a6, /// 0xa8c
			 0xb8b88cd4, /// 0xa90
			 0x34de7719, /// 0xa94
			 0x59799c97, /// 0xa98
			 0x85a4c911, /// 0xa9c
			 0x225cc0c7, /// 0xaa0
			 0x547ff290, /// 0xaa4
			 0xf28214ac, /// 0xaa8
			 0xd51f392e, /// 0xaac
			 0xba6cd3b1, /// 0xab0
			 0x41d36f7e, /// 0xab4
			 0x7baea89d, /// 0xab8
			 0x71069151, /// 0xabc
			 0x58f94dfe, /// 0xac0
			 0x47551dc5, /// 0xac4
			 0x7b31dad0, /// 0xac8
			 0x69396dd0, /// 0xacc
			 0xccd55b66, /// 0xad0
			 0x70165b90, /// 0xad4
			 0x8a70d0d1, /// 0xad8
			 0x932f17b4, /// 0xadc
			 0x2257a181, /// 0xae0
			 0x47833d28, /// 0xae4
			 0x183465c8, /// 0xae8
			 0xdc6f2367, /// 0xaec
			 0xe100b201, /// 0xaf0
			 0x293b16d5, /// 0xaf4
			 0x30e8b6ca, /// 0xaf8
			 0xa3dac02f, /// 0xafc
			 0xc1ab2c2e, /// 0xb00
			 0xe3ec0de3, /// 0xb04
			 0x2204baf2, /// 0xb08
			 0x38e1edea, /// 0xb0c
			 0x0fbb1e34, /// 0xb10
			 0x6c8d4f50, /// 0xb14
			 0xfa6c3146, /// 0xb18
			 0xa75bde77, /// 0xb1c
			 0xdff082ad, /// 0xb20
			 0xe9bdf0a8, /// 0xb24
			 0x4f029679, /// 0xb28
			 0x6dddeb5a, /// 0xb2c
			 0x601a43e7, /// 0xb30
			 0x6e52a50a, /// 0xb34
			 0xe01ac27a, /// 0xb38
			 0xadb22e1d, /// 0xb3c
			 0x156b0052, /// 0xb40
			 0x8ee96668, /// 0xb44
			 0xb846ada6, /// 0xb48
			 0x98772a8a, /// 0xb4c
			 0xd49ed0e5, /// 0xb50
			 0x643237e2, /// 0xb54
			 0x18a17859, /// 0xb58
			 0x9c14c7d3, /// 0xb5c
			 0x0ae0d0d0, /// 0xb60
			 0x48444e50, /// 0xb64
			 0x987b6f21, /// 0xb68
			 0x858485c6, /// 0xb6c
			 0xeb34b768, /// 0xb70
			 0x09d29c02, /// 0xb74
			 0x25aef611, /// 0xb78
			 0xb38158dd, /// 0xb7c
			 0xdbcc0f09, /// 0xb80
			 0xb0c82db8, /// 0xb84
			 0xde5ec3f5, /// 0xb88
			 0x837cee09, /// 0xb8c
			 0x89ddcd97, /// 0xb90
			 0xce782105, /// 0xb94
			 0x07396473, /// 0xb98
			 0x2d507799, /// 0xb9c
			 0x5b3db170, /// 0xba0
			 0x135b37c7, /// 0xba4
			 0xd5561292, /// 0xba8
			 0x57c4ad85, /// 0xbac
			 0x55cea5ef, /// 0xbb0
			 0x74bc556a, /// 0xbb4
			 0x7f28c885, /// 0xbb8
			 0xf5367d83, /// 0xbbc
			 0xcbed565b, /// 0xbc0
			 0x32a5a260, /// 0xbc4
			 0x9c14c1b8, /// 0xbc8
			 0xec964b32, /// 0xbcc
			 0x077406e2, /// 0xbd0
			 0x8f929693, /// 0xbd4
			 0x4198d4d8, /// 0xbd8
			 0x1bc160f3, /// 0xbdc
			 0x1ceec1fb, /// 0xbe0
			 0x2ee00acc, /// 0xbe4
			 0x42fc3d40, /// 0xbe8
			 0xff51519f, /// 0xbec
			 0xb4fa1326, /// 0xbf0
			 0xcf8db21e, /// 0xbf4
			 0x49846db4, /// 0xbf8
			 0x071bd305, /// 0xbfc
			 0x9ea6cf7b, /// 0xc00
			 0x26235877, /// 0xc04
			 0xe434f315, /// 0xc08
			 0x3f911721, /// 0xc0c
			 0xa89d5c38, /// 0xc10
			 0x83a2306c, /// 0xc14
			 0xc7405f00, /// 0xc18
			 0x1da1d984, /// 0xc1c
			 0xfce65db1, /// 0xc20
			 0x5a68dad6, /// 0xc24
			 0xc3a5dd37, /// 0xc28
			 0xc0c96691, /// 0xc2c
			 0x3eb327ac, /// 0xc30
			 0xcf2e3ecc, /// 0xc34
			 0x283fb0b7, /// 0xc38
			 0x222966b2, /// 0xc3c
			 0x3d40cbc6, /// 0xc40
			 0x2eafe286, /// 0xc44
			 0x457aba56, /// 0xc48
			 0xbd8365c8, /// 0xc4c
			 0x3d0e2aa9, /// 0xc50
			 0x3e539625, /// 0xc54
			 0x00f1d43b, /// 0xc58
			 0x7fc4ec00, /// 0xc5c
			 0x0642d7ba, /// 0xc60
			 0xac13a0f5, /// 0xc64
			 0xcae08d0f, /// 0xc68
			 0xf1cedb99, /// 0xc6c
			 0xe2813934, /// 0xc70
			 0xe3dc8d25, /// 0xc74
			 0x589d3151, /// 0xc78
			 0xcb8ae219, /// 0xc7c
			 0xd11999fe, /// 0xc80
			 0x848e5b29, /// 0xc84
			 0x57b0dfa3, /// 0xc88
			 0xea81a8d4, /// 0xc8c
			 0xa6985f04, /// 0xc90
			 0x05f03728, /// 0xc94
			 0x41b5f197, /// 0xc98
			 0x39f9a10a, /// 0xc9c
			 0x404ae273, /// 0xca0
			 0xf0c20168, /// 0xca4
			 0x4c023175, /// 0xca8
			 0xf2d63364, /// 0xcac
			 0xcbaf7254, /// 0xcb0
			 0x6fcbcab0, /// 0xcb4
			 0x2cae7bce, /// 0xcb8
			 0x2a3da886, /// 0xcbc
			 0xf23778f8, /// 0xcc0
			 0x11e72dfb, /// 0xcc4
			 0x47801e0e, /// 0xcc8
			 0xa068bb2b, /// 0xccc
			 0x268f406c, /// 0xcd0
			 0x741f1a02, /// 0xcd4
			 0x040aeb74, /// 0xcd8
			 0xe788f839, /// 0xcdc
			 0xa86fde3c, /// 0xce0
			 0xb4c83f80, /// 0xce4
			 0xe1251bad, /// 0xce8
			 0x7d36c2a0, /// 0xcec
			 0x7c91017e, /// 0xcf0
			 0x2cdea698, /// 0xcf4
			 0xa8dcc5e7, /// 0xcf8
			 0x4ca1e825, /// 0xcfc
			 0xa7dfef5d, /// 0xd00
			 0xaa36638a, /// 0xd04
			 0x3082de83, /// 0xd08
			 0xcf72f3cd, /// 0xd0c
			 0xf5873d26, /// 0xd10
			 0xb9438d89, /// 0xd14
			 0x4b5c716d, /// 0xd18
			 0x501e331c, /// 0xd1c
			 0x9d858d7f, /// 0xd20
			 0xd5b1b754, /// 0xd24
			 0xf1df7c43, /// 0xd28
			 0x971cc31c, /// 0xd2c
			 0x75a6d0d1, /// 0xd30
			 0xe8cbdf10, /// 0xd34
			 0x9adebf50, /// 0xd38
			 0x815c5480, /// 0xd3c
			 0x146384d0, /// 0xd40
			 0xf36e17bd, /// 0xd44
			 0x5fc1da8e, /// 0xd48
			 0xd40842f1, /// 0xd4c
			 0xca716adc, /// 0xd50
			 0x1664fbcc, /// 0xd54
			 0x3ae1b31c, /// 0xd58
			 0x0b077d44, /// 0xd5c
			 0xcedca3b8, /// 0xd60
			 0x7a141213, /// 0xd64
			 0x2f3dacee, /// 0xd68
			 0x77ad01ef, /// 0xd6c
			 0x6ab08c2e, /// 0xd70
			 0xf80876ac, /// 0xd74
			 0xf7ec1c6f, /// 0xd78
			 0x8128e20a, /// 0xd7c
			 0x16e9c2ac, /// 0xd80
			 0xf910c917, /// 0xd84
			 0x0c1c96aa, /// 0xd88
			 0xdb2c44ad, /// 0xd8c
			 0x463374ed, /// 0xd90
			 0x567c3d79, /// 0xd94
			 0xd67040a3, /// 0xd98
			 0xa85fb454, /// 0xd9c
			 0x3e054177, /// 0xda0
			 0x077d60a8, /// 0xda4
			 0x7f4b86e8, /// 0xda8
			 0xb438f5f1, /// 0xdac
			 0xe1f768bf, /// 0xdb0
			 0x267af74c, /// 0xdb4
			 0xdaab1bc8, /// 0xdb8
			 0x4d0cf7d7, /// 0xdbc
			 0x18d3fb6a, /// 0xdc0
			 0x2e95e0cf, /// 0xdc4
			 0x771304a3, /// 0xdc8
			 0xd9063e2d, /// 0xdcc
			 0x04c730b2, /// 0xdd0
			 0x4d30df10, /// 0xdd4
			 0x6c02668a, /// 0xdd8
			 0xd08d1c51, /// 0xddc
			 0x38bc408b, /// 0xde0
			 0x9dc9100b, /// 0xde4
			 0x9e9fb569, /// 0xde8
			 0x3616e34f, /// 0xdec
			 0x24b16faa, /// 0xdf0
			 0x1fc2927f, /// 0xdf4
			 0x1555b070, /// 0xdf8
			 0x61cdddc2, /// 0xdfc
			 0x52d983a0, /// 0xe00
			 0x6885040e, /// 0xe04
			 0x621673a6, /// 0xe08
			 0x6c0ad986, /// 0xe0c
			 0xd6bd2fbf, /// 0xe10
			 0x901a0b37, /// 0xe14
			 0x25aaa2c0, /// 0xe18
			 0x69c17f97, /// 0xe1c
			 0xe9292d8f, /// 0xe20
			 0xdd990f46, /// 0xe24
			 0x0131bd19, /// 0xe28
			 0x60cdbe06, /// 0xe2c
			 0x24584306, /// 0xe30
			 0x1e4e6398, /// 0xe34
			 0xda6f0bbb, /// 0xe38
			 0x479cad7a, /// 0xe3c
			 0x10238ab8, /// 0xe40
			 0x23b2c4c3, /// 0xe44
			 0xc89559e2, /// 0xe48
			 0x4d4eaf6b, /// 0xe4c
			 0x40c46a7a, /// 0xe50
			 0x4e77b7a6, /// 0xe54
			 0xf163e320, /// 0xe58
			 0x3da996bd, /// 0xe5c
			 0x8fb038ca, /// 0xe60
			 0x876b6be7, /// 0xe64
			 0x53de01bb, /// 0xe68
			 0x3992ca40, /// 0xe6c
			 0x5c6a8ad0, /// 0xe70
			 0x0af56747, /// 0xe74
			 0x6a7cbce2, /// 0xe78
			 0x17773744, /// 0xe7c
			 0x7fe20778, /// 0xe80
			 0xfbf8c299, /// 0xe84
			 0xa505bb30, /// 0xe88
			 0x31097fd1, /// 0xe8c
			 0x86df20fe, /// 0xe90
			 0xde6e706d, /// 0xe94
			 0x3735983c, /// 0xe98
			 0x96ecea74, /// 0xe9c
			 0x656d1778, /// 0xea0
			 0xbe14cf24, /// 0xea4
			 0x35ef0fe6, /// 0xea8
			 0x7d3f34fe, /// 0xeac
			 0xfacc3477, /// 0xeb0
			 0x8bce2b5e, /// 0xeb4
			 0xe36f5733, /// 0xeb8
			 0x3304a538, /// 0xebc
			 0x638ba64f, /// 0xec0
			 0x478df07d, /// 0xec4
			 0x7e5378f3, /// 0xec8
			 0x81351b69, /// 0xecc
			 0xb35c647c, /// 0xed0
			 0x55320bdc, /// 0xed4
			 0x394da6f5, /// 0xed8
			 0x1273e27c, /// 0xedc
			 0x3adda89f, /// 0xee0
			 0xda35b6ac, /// 0xee4
			 0xa8190137, /// 0xee8
			 0x90d101e6, /// 0xeec
			 0x50759978, /// 0xef0
			 0x47b4a318, /// 0xef4
			 0xd569328a, /// 0xef8
			 0x60fbf241, /// 0xefc
			 0x048ce9cc, /// 0xf00
			 0xab80061a, /// 0xf04
			 0xecfb0eb8, /// 0xf08
			 0x256785b2, /// 0xf0c
			 0xea9de57c, /// 0xf10
			 0x04e0a617, /// 0xf14
			 0x97121839, /// 0xf18
			 0x79cb1f6e, /// 0xf1c
			 0x6e8795d7, /// 0xf20
			 0x17faef6d, /// 0xf24
			 0x46ec14e5, /// 0xf28
			 0xc89efc7b, /// 0xf2c
			 0xa452b529, /// 0xf30
			 0xac48be7d, /// 0xf34
			 0xa96f89ad, /// 0xf38
			 0xe93f99ee, /// 0xf3c
			 0x1d0de989, /// 0xf40
			 0x4a2e17f8, /// 0xf44
			 0xdb7ba8cc, /// 0xf48
			 0x24015a4b, /// 0xf4c
			 0x22333279, /// 0xf50
			 0x3097de74, /// 0xf54
			 0x8b815efb, /// 0xf58
			 0x39f4f353, /// 0xf5c
			 0x0c08abb9, /// 0xf60
			 0x063a9335, /// 0xf64
			 0x5f96c172, /// 0xf68
			 0xdb144353, /// 0xf6c
			 0x91fb1772, /// 0xf70
			 0x9c1670e2, /// 0xf74
			 0xe4777c64, /// 0xf78
			 0x8796afb0, /// 0xf7c
			 0x283af97e, /// 0xf80
			 0x2a0be4c7, /// 0xf84
			 0xe4a2126b, /// 0xf88
			 0x4d9150e3, /// 0xf8c
			 0x45c34bc2, /// 0xf90
			 0xbb0ae4b3, /// 0xf94
			 0x4d375d8f, /// 0xf98
			 0x5e3d6582, /// 0xf9c
			 0x1c9ebcd9, /// 0xfa0
			 0xeff26a49, /// 0xfa4
			 0xb2a93d1e, /// 0xfa8
			 0x900bf2f3, /// 0xfac
			 0xcca2dbaa, /// 0xfb0
			 0xd129a170, /// 0xfb4
			 0x2f381a4a, /// 0xfb8
			 0x9c9fcf16, /// 0xfbc
			 0xe5b76b38, /// 0xfc0
			 0x0469d211, /// 0xfc4
			 0x31a6a74c, /// 0xfc8
			 0xb139b284, /// 0xfcc
			 0xc5f31d7a, /// 0xfd0
			 0xf6aa836a, /// 0xfd4
			 0xa8d8c476, /// 0xfd8
			 0xcda17ec3, /// 0xfdc
			 0xdecb2ee6, /// 0xfe0
			 0x56329dff, /// 0xfe4
			 0x23dced25, /// 0xfe8
			 0xabe3a9b0, /// 0xfec
			 0x65bb2acf, /// 0xff0
			 0xaa412a6d, /// 0xff4
			 0xd0865651, /// 0xff8
			 0x2fbf1fff /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00008
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0000c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00010
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00014
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00024
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0002c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00030
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00034
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00044
			 0x0e000001,                                                  // Trailing 1s:                                /// 00048
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00060
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00064
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00068
			 0x0e000003,                                                  // Trailing 1s:                                /// 0006c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00070
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00074
			 0x7f800000,                                                  // inf                                         /// 00078
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00080
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00084
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00088
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0008c
			 0x7fc00001,                                                  // signaling NaN                               /// 00090
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00094
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x3f028f5c,                                                  // 0.51                                        /// 0009c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00100
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00108
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0010c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00110
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00114
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00118
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00124
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00128
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0012c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00130
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00134
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0013c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00140
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00148
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00150
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0015c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00160
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00164
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00168
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00170
			 0x00000000,                                                  // zero                                        /// 00174
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00178
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0017c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00184
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00188
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0018c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00190
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00194
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0019c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00200
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00210
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00218
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00220
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00224
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00228
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00230
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00238
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0023c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00240
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00244
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00250
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00254
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0025c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00260
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00264
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00268
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0026c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00274
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0027c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00280
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00284
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00288
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0028c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00290
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00294
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0029c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002dc
			 0x33333333,                                                  // 4 random values                             /// 002e0
			 0xbf028f5c,                                                  // -0.51                                       /// 002e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00304
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00310
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00314
			 0x0e000001,                                                  // Trailing 1s:                                /// 00318
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0031c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00328
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0032c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00330
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00334
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0033c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00344
			 0x00000000,                                                  // zero                                        /// 00348
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0034c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00350
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0e000003,                                                  // Trailing 1s:                                /// 0035c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00360
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00364
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00368
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0036c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00370
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00384
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0038c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00390
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00394
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00398
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0039c
			 0xbf028f5c,                                                  // -0.51                                       /// 003a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 003f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00400
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00404
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0c400000,                                                  // Leading 1s:                                 /// 0040c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00410
			 0xcb000000,                                                  // -8388608.0                                  /// 00414
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00418
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00420
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00424
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00428
			 0xff800000,                                                  // -inf                                        /// 0042c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00434
			 0xffc00001,                                                  // -signaling NaN                              /// 00438
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00444
			 0x55555555,                                                  // 4 random values                             /// 00448
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0044c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00450
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00454
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0045c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00464
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00000000,                                                  // zero                                        /// 00470
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00474
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00478
			 0x00000000,                                                  // zero                                        /// 0047c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00480
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00484
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00488
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0048c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00494
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x33333333,                                                  // 4 random values                             /// 004c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004e8
			 0x80000000,                                                  // -zero                                       /// 004ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0050c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00514
			 0x80000000,                                                  // -zero                                       /// 00518
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00524
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00528
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00530
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00534
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00538
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0053c
			 0x7f800000,                                                  // inf                                         /// 00540
			 0x0c780000,                                                  // Leading 1s:                                 /// 00544
			 0x0e000003,                                                  // Trailing 1s:                                /// 00548
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0054c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00550
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00554
			 0x0e000003,                                                  // Trailing 1s:                                /// 00558
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0055c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00560
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0056c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00570
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00578
			 0x7fc00001,                                                  // signaling NaN                               /// 0057c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00580
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00584
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00588
			 0x4b000000,                                                  // 8388608.0                                   /// 0058c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00590
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0xcb000000,                                                  // -8388608.0                                  /// 00598
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0059c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005b0
			 0xffc00001,                                                  // -signaling NaN                              /// 005b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00600
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00610
			 0xbf028f5c,                                                  // -0.51                                       /// 00614
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0061c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00620
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00624
			 0x0e000003,                                                  // Trailing 1s:                                /// 00628
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00630
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00640
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00644
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0xff800000,                                                  // -inf                                        /// 00654
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00658
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0065c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00660
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00664
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0066c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00674
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00678
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0067c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0068c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00690
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00694
			 0x00000000,                                                  // zero                                        /// 00698
			 0x80000000,                                                  // -zero                                       /// 0069c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x7f800000,                                                  // inf                                         /// 006ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00704
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00708
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0e000003,                                                  // Trailing 1s:                                /// 00718
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0071c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00720
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00724
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00728
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0072c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00730
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00738
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0073c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00740
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x55555555,                                                  // 4 random values                             /// 00748
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0074c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00750
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00754
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00760
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0076c
			 0x33333333,                                                  // 4 random values                             /// 00770
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00774
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00778
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00780
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00790
			 0x00000000,                                                  // zero                                        /// 00794
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00798
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0079c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0xff800000,                                                  // -inf                                        /// 007f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00800
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0c780000,                                                  // Leading 1s:                                 /// 00808
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00814
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00818
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0081c
			 0x80000000,                                                  // -zero                                       /// 00820
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00828
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00830
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00834
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00838
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0083c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00840
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00848
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0084c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00858
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00860
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00864
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x0e000003,                                                  // Trailing 1s:                                /// 00874
			 0x0e000007,                                                  // Trailing 1s:                                /// 00878
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0087c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00880
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00884
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00888
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0088c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x0e000007,                                                  // Trailing 1s:                                /// 00894
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0089c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0xff800000,                                                  // -inf                                        /// 008e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00908
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0090c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00914
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0091c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00920
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00924
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00928
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0092c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00930
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0e000003,                                                  // Trailing 1s:                                /// 00938
			 0x0e000007,                                                  // Trailing 1s:                                /// 0093c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00940
			 0x0c700000,                                                  // Leading 1s:                                 /// 00944
			 0xcb000000,                                                  // -8388608.0                                  /// 00948
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00950
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00954
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0095c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00960
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0096c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00970
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00974
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0097c
			 0xff800000,                                                  // -inf                                        /// 00980
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00984
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00988
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00990
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00998
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0099c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00000000,                                                  // zero                                        /// 009c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00000000,                                                  // zero                                        /// 00a30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a4c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a5c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aa8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ab0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00adc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ae4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00af0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00afc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x7fc00001,                                                  // signaling NaN                               /// 00b28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b34
			 0x4b000000,                                                  // 8388608.0                                   /// 00b38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b48
			 0x3f028f5c,                                                  // 0.51                                        /// 00b4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b6c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bdc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00be4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bf0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bf8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c0c
			 0x33333333,                                                  // 4 random values                             /// 00c10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c34
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c50
			 0x7f800000,                                                  // inf                                         /// 00c54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c80
			 0x4b000000,                                                  // 8388608.0                                   /// 00c84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ca0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x7fc00001,                                                  // signaling NaN                               /// 00cc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ce4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d00
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d60
			 0xcb000000,                                                  // -8388608.0                                  /// 00d64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00da0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00da8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd4
			 0x00000000,                                                  // zero                                        /// 00dd8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ddc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00de4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00de8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00df0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df4
			 0xcb000000,                                                  // -8388608.0                                  /// 00df8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00000000,                                                  // zero                                        /// 00e20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ea0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ea8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00eb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ec0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec8
			 0x55555555,                                                  // 4 random values                             /// 00ecc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ed0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ee4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ee8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ef0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00efc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f60
			 0x4b000000,                                                  // 8388608.0                                   /// 00f64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f70
			 0x00000000,                                                  // zero                                        /// 00f74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fa0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fb8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fd0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fe4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe8
			 0xff800000,                                                  // -inf                                        /// 00fec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ff0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ff4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0e003fff                                                  // Trailing 1s:                                /// last
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
			 0x0000004c,
			 0x00000064,
			 0x000000c8,
			 0x000004b4,
			 0x00000418,
			 0x00000304,
			 0x0000009c,
			 0x0000077c,

			 /// vpu register f2
			 0x40000000,
			 0x41900000,
			 0x41600000,
			 0x41800000,
			 0x41980000,
			 0x41500000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f3
			 0x41980000,
			 0x3f800000,
			 0x41000000,
			 0x40e00000,
			 0x41d80000,
			 0x41800000,
			 0x41100000,
			 0x42000000,

			 /// vpu register f4
			 0x41500000,
			 0x41800000,
			 0x40e00000,
			 0x41500000,
			 0x42000000,
			 0x41700000,
			 0x41e80000,
			 0x40a00000,

			 /// vpu register f5
			 0x42000000,
			 0x41880000,
			 0x41400000,
			 0x41a00000,
			 0x41880000,
			 0x41c00000,
			 0x41f80000,
			 0x42000000,

			 /// vpu register f6
			 0x41600000,
			 0x40a00000,
			 0x41100000,
			 0x40c00000,
			 0x41c00000,
			 0x41600000,
			 0x41300000,
			 0x41800000,

			 /// vpu register f7
			 0x41100000,
			 0x3f800000,
			 0x40000000,
			 0x40c00000,
			 0x40c00000,
			 0x41e80000,
			 0x41d80000,
			 0x40800000,

			 /// vpu register f8
			 0x41f80000,
			 0x40400000,
			 0x41c80000,
			 0x41c80000,
			 0x41b80000,
			 0x41a00000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f9
			 0x40000000,
			 0x41400000,
			 0x41200000,
			 0x41100000,
			 0x40000000,
			 0x41200000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f10
			 0x41000000,
			 0x41c80000,
			 0x41b80000,
			 0x40a00000,
			 0x41d00000,
			 0x41c80000,
			 0x41d80000,
			 0x40800000,

			 /// vpu register f11
			 0x41600000,
			 0x41000000,
			 0x41500000,
			 0x41880000,
			 0x41b80000,
			 0x41a80000,
			 0x41900000,
			 0x40400000,

			 /// vpu register f12
			 0x42000000,
			 0x41b80000,
			 0x41700000,
			 0x41200000,
			 0x41c00000,
			 0x41600000,
			 0x42000000,
			 0x41e00000,

			 /// vpu register f13
			 0x41a80000,
			 0x41700000,
			 0x42000000,
			 0x41900000,
			 0x42000000,
			 0x41a00000,
			 0x41500000,
			 0x41200000,

			 /// vpu register f14
			 0x41a00000,
			 0x41100000,
			 0x40000000,
			 0x41e80000,
			 0x41e80000,
			 0x41000000,
			 0x41880000,
			 0x41000000,

			 /// vpu register f15
			 0x41200000,
			 0x41a00000,
			 0x41c80000,
			 0x41d00000,
			 0x3f800000,
			 0x41f80000,
			 0x41a80000,
			 0x41000000,

			 /// vpu register f16
			 0x41f80000,
			 0x41000000,
			 0x41e00000,
			 0x40000000,
			 0x40c00000,
			 0x41c80000,
			 0x40c00000,
			 0x41400000,

			 /// vpu register f17
			 0x41d80000,
			 0x41e80000,
			 0x3f800000,
			 0x41b00000,
			 0x41500000,
			 0x41b80000,
			 0x41400000,
			 0x41000000,

			 /// vpu register f18
			 0x40c00000,
			 0x41f00000,
			 0x40e00000,
			 0x41d80000,
			 0x41100000,
			 0x41b00000,
			 0x41500000,
			 0x3f800000,

			 /// vpu register f19
			 0x41f80000,
			 0x41880000,
			 0x41c80000,
			 0x41900000,
			 0x41400000,
			 0x41d00000,
			 0x41c00000,
			 0x40a00000,

			 /// vpu register f20
			 0x41400000,
			 0x41100000,
			 0x41e80000,
			 0x41b80000,
			 0x41100000,
			 0x41d00000,
			 0x41400000,
			 0x41400000,

			 /// vpu register f21
			 0x41900000,
			 0x40c00000,
			 0x41400000,
			 0x41400000,
			 0x41100000,
			 0x41b80000,
			 0x41f00000,
			 0x41200000,

			 /// vpu register f22
			 0x41b80000,
			 0x41100000,
			 0x41400000,
			 0x41200000,
			 0x41b80000,
			 0x41880000,
			 0x41400000,
			 0x41d80000,

			 /// vpu register f23
			 0x41b80000,
			 0x41900000,
			 0x40800000,
			 0x41e80000,
			 0x41400000,
			 0x42000000,
			 0x41e80000,
			 0x41a80000,

			 /// vpu register f24
			 0x41e80000,
			 0x41200000,
			 0x41900000,
			 0x41980000,
			 0x41700000,
			 0x41d80000,
			 0x41c00000,
			 0x41880000,

			 /// vpu register f25
			 0x41400000,
			 0x41000000,
			 0x41900000,
			 0x41600000,
			 0x41900000,
			 0x41600000,
			 0x41f00000,
			 0x41e80000,

			 /// vpu register f26
			 0x3f800000,
			 0x41a00000,
			 0x41f80000,
			 0x41b00000,
			 0x40e00000,
			 0x41e80000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f27
			 0x40c00000,
			 0x41c00000,
			 0x41500000,
			 0x42000000,
			 0x41880000,
			 0x41880000,
			 0x41e80000,
			 0x41500000,

			 /// vpu register f28
			 0x42000000,
			 0x41980000,
			 0x40000000,
			 0x41100000,
			 0x41c80000,
			 0x41700000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f29
			 0x40000000,
			 0x40c00000,
			 0x41880000,
			 0x41a80000,
			 0x40400000,
			 0x42000000,
			 0x40800000,
			 0x41700000,

			 /// vpu register f30
			 0x41d80000,
			 0x42000000,
			 0x3f800000,
			 0x41d80000,
			 0x41b80000,
			 0x41e00000,
			 0x41f00000,
			 0x41900000,

			 /// vpu register f31
			 0x41f00000,
			 0x41000000,
			 0x40400000,
			 0x41e80000,
			 0x41e80000,
			 0x3f800000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f25, f1 (x14)\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f18, f1 (x13)\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f5, f1 (x14)\n"                           ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f4, f1 (x14)\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f16, f1 (x11)\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f8, f1 (x13)\n"                           ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f20, f1 (x14)\n"                          ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f13, f1 (x14)\n"                          ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f21, f1 (x13)\n"                          ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f7, f1 (x13)\n"                           ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f9, f1 (x11)\n"                           ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f22, f1 (x13)\n"                          ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f10, f1 (x12)\n"                          ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f21, f1 (x15)\n"                          ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f14, f1 (x12)\n"                          ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f13, f1 (x15)\n"                          ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f2, f1 (x13)\n"                           ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f20, f1 (x11)\n"                          ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f24, f1 (x15)\n"                          ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f5, f1 (x14)\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f7, f1 (x13)\n"                           ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f2, f1 (x12)\n"                           ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f16, f1 (x13)\n"                          ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f13, f1 (x12)\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f19, f1 (x13)\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f19, f1 (x12)\n"                          ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f16, f1 (x12)\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f7, f1 (x11)\n"                           ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f8, f1 (x11)\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f2, f1 (x13)\n"                           ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f10, f1 (x14)\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f4, f1 (x11)\n"                           ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f25, f1 (x14)\n"                          ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f19, f1 (x12)\n"                          ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f9, f1 (x15)\n"                           ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f12, f1 (x15)\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f14, f1 (x15)\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f28, f1 (x13)\n"                          ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f15, f1 (x12)\n"                          ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f24, f1 (x14)\n"                          ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f27, f1 (x12)\n"                          ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f18, f1 (x11)\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f15, f1 (x13)\n"                          ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f23, f1 (x13)\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f12, f1 (x15)\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f10, f1 (x14)\n"                          ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f15, f1 (x11)\n"                          ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f27, f1 (x15)\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f30, f1 (x13)\n"                          ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f10, f1 (x12)\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f23, f1 (x12)\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f21, f1 (x13)\n"                          ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f5, f1 (x14)\n"                           ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f15, f1 (x12)\n"                          ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f24, f1 (x12)\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f7, f1 (x14)\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f17, f1 (x13)\n"                          ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f27, f1 (x13)\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f23, f1 (x13)\n"                          ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f12, f1 (x13)\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f13, f1 (x15)\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f21, f1 (x14)\n"                          ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f17, f1 (x15)\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f17, f1 (x13)\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f25, f1 (x13)\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f22, f1 (x13)\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f27, f1 (x14)\n"                          ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f6, f1 (x14)\n"                           ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f14, f1 (x13)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f30, f1 (x12)\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f14, f1 (x15)\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f29, f1 (x13)\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f13, f1 (x14)\n"                          ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f7, f1 (x12)\n"                           ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f23, f1 (x14)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f7, f1 (x13)\n"                           ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f23, f1 (x14)\n"                          ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f24, f1 (x13)\n"                          ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f16, f1 (x13)\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f15, f1 (x15)\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f16, f1 (x12)\n"                          ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f26, f1 (x11)\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f14, f1 (x14)\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f24, f1 (x13)\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f11, f1 (x14)\n"                          ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f5, f1 (x11)\n"                           ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f12, f1 (x15)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f13, f1 (x14)\n"                          ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f16, f1 (x14)\n"                          ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f26, f1 (x15)\n"                          ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f28, f1 (x15)\n"                          ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f28, f1 (x14)\n"                          ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f21, f1 (x11)\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f10, f1 (x14)\n"                          ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f25, f1 (x14)\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f8, f1 (x14)\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f7, f1 (x15)\n"                           ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f9, f1 (x11)\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoorl.pi f3, f1 (x13)\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
