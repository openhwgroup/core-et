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
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00000
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00004
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00008
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0e000001,                                                  // Trailing 1s:                                /// 00018
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00024
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0002c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0003c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00040
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00050
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00000000,                                                  // zero                                        /// 0005c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x33333333,                                                  // 4 random values                             /// 00064
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00068
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00070
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00074
			 0x0e000003,                                                  // Trailing 1s:                                /// 00078
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0007c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00088
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0008c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00098
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0009c
			 0xbf028f5c,                                                  // -0.51                                       /// 000a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x55555555,                                                  // 4 random values                             /// 000c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x4b000000,                                                  // 8388608.0                                   /// 000d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xff800000,                                                  // -inf                                        /// 00104
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00108
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00114
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00118
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0011c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xbf028f5c,                                                  // -0.51                                       /// 00128
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0012c
			 0x80000000,                                                  // -zero                                       /// 00130
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00134
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00138
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00140
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00144
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00148
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0xff800000,                                                  // -inf                                        /// 00150
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00154
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0015c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00160
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00164
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00168
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0016c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00174
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00180
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00184
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00188
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00190
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00194
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00198
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0019c
			 0xcb000000,                                                  // -8388608.0                                  /// 001a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00000000,                                                  // zero                                        /// 001b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x7f800000,                                                  // inf                                         /// 001e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00200
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00208
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0020c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00210
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00214
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00218
			 0x33333333,                                                  // 4 random values                             /// 0021c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00220
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00224
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00228
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00230
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00238
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0023c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00240
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00244
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00248
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00250
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00264
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00268
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00270
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00274
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00278
			 0x33333333,                                                  // 4 random values                             /// 0027c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00280
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00284
			 0x33333333,                                                  // 4 random values                             /// 00288
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0028c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00290
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f4
			 0xffc00001,                                                  // -signaling NaN                              /// 002f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00300
			 0x7f800000,                                                  // inf                                         /// 00304
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00308
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0030c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0031c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00320
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0032c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00340
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00344
			 0x0e000003,                                                  // Trailing 1s:                                /// 00348
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0034c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00354
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00358
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00360
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00364
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0036c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00374
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0037c
			 0xcb000000,                                                  // -8388608.0                                  /// 00380
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00388
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00394
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00398
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00000000,                                                  // zero                                        /// 003dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00400
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00404
			 0x55555555,                                                  // 4 random values                             /// 00408
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0040c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00410
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0041c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00420
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00424
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00430
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00434
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00438
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00440
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00444
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00448
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00450
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00454
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0045c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00464
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0046c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00470
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00474
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x4b000000,                                                  // 8388608.0                                   /// 0047c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00480
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00484
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00488
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0048c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00498
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004b4
			 0x55555555,                                                  // 4 random values                             /// 004b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x4b000000,                                                  // 8388608.0                                   /// 004fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00500
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00508
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0050c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00514
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0051c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00524
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x4b000000,                                                  // 8388608.0                                   /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00534
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00544
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00548
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00550
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00558
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00560
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00568
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0056c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00574
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00578
			 0x0e000003,                                                  // Trailing 1s:                                /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00584
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00588
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0058c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00590
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00598
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005a0
			 0xcb000000,                                                  // -8388608.0                                  /// 005a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005ec
			 0x33333333,                                                  // 4 random values                             /// 005f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0e000007,                                                  // Trailing 1s:                                /// 00608
			 0x3f028f5c,                                                  // 0.51                                        /// 0060c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00610
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00614
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00618
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00620
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x3f028f5c,                                                  // 0.51                                        /// 00628
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0062c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00630
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00634
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0c400000,                                                  // Leading 1s:                                 /// 0063c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00640
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00648
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0064c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00654
			 0x55555555,                                                  // 4 random values                             /// 00658
			 0xbf028f5c,                                                  // -0.51                                       /// 0065c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00660
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00664
			 0x4b000000,                                                  // 8388608.0                                   /// 00668
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0066c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00674
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00678
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00694
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00698
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0069c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006f0
			 0x3f028f5c,                                                  // 0.51                                        /// 006f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00708
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0070c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00710
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00718
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0071c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00720
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00728
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0072c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00730
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0073c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00744
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x4b000000,                                                  // 8388608.0                                   /// 0074c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00754
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00764
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00770
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00778
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0077c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00780
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00788
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0xff800000,                                                  // -inf                                        /// 00790
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00794
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00798
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0079c
			 0x3f028f5c,                                                  // 0.51                                        /// 007a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007bc
			 0x55555555,                                                  // 4 random values                             /// 007c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d0
			 0x3f028f5c,                                                  // 0.51                                        /// 007d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00804
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00810
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00814
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00830
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00834
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00838
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0083c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00858
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0085c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00860
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00864
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00868
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0086c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00870
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00874
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0087c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00880
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00888
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0088c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00890
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00894
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0089c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c8
			 0x3f028f5c,                                                  // 0.51                                        /// 008cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00904
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00908
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00914
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00918
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00920
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00924
			 0x80000000,                                                  // -zero                                       /// 00928
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0092c
			 0xbf028f5c,                                                  // -0.51                                       /// 00930
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00934
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00938
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0093c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00940
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00944
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00950
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xbf028f5c,                                                  // -0.51                                       /// 00964
			 0xff800000,                                                  // -inf                                        /// 00968
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0096c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x3f028f5c,                                                  // 0.51                                        /// 00974
			 0x3f028f5c,                                                  // 0.51                                        /// 00978
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0097c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00980
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00990
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00998
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0099c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0xff800000,                                                  // -inf                                        /// 009d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a94
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aa0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aa8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ab0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00acc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ad0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00adc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ae0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ae4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00af8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00afc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b04
			 0xffc00001,                                                  // -signaling NaN                              /// 00b08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0xbf028f5c,                                                  // -0.51                                       /// 00b24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x4b000000,                                                  // 8388608.0                                   /// 00b54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bdc
			 0x33333333,                                                  // 4 random values                             /// 00be0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00be4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bf4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bf8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00c00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c04
			 0x80000000,                                                  // -zero                                       /// 00c08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c14
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c70
			 0x55555555,                                                  // 4 random values                             /// 00c74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x7fc00001,                                                  // signaling NaN                               /// 00c84
			 0x33333333,                                                  // 4 random values                             /// 00c88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c94
			 0x4b000000,                                                  // 8388608.0                                   /// 00c98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ca0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ca4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cb0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cb8
			 0x80000000,                                                  // -zero                                       /// 00cbc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cdc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ce4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cfc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d4c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d64
			 0xcb000000,                                                  // -8388608.0                                  /// 00d68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0xcb000000,                                                  // -8388608.0                                  /// 00d74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00da0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ddc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dec
			 0x55555555,                                                  // 4 random values                             /// 00df0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00df8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e08
			 0x55555555,                                                  // 4 random values                             /// 00e0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e50
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ea4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eb0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ebc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ec0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00edc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ef0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ef4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ef8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00efc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f14
			 0xbf028f5c,                                                  // -0.51                                       /// 00f18
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f28
			 0x4b000000,                                                  // 8388608.0                                   /// 00f2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f50
			 0x7fc00001,                                                  // signaling NaN                               /// 00f54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x4b000000,                                                  // 8388608.0                                   /// 00f68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fa8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fc0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fe8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff8
			 0x00800000 // min norm                                      // subnormals +ve                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xb2abdeb8, /// 0x0
			 0xdc8aad75, /// 0x4
			 0x28d6aa8c, /// 0x8
			 0xc4fc07bb, /// 0xc
			 0x27040304, /// 0x10
			 0x414746c9, /// 0x14
			 0x143bab17, /// 0x18
			 0x8fd32982, /// 0x1c
			 0x516f2c12, /// 0x20
			 0xdc5cf1f7, /// 0x24
			 0xd6323817, /// 0x28
			 0x2733e014, /// 0x2c
			 0x60cb9dee, /// 0x30
			 0x2772ee80, /// 0x34
			 0x4381336d, /// 0x38
			 0x21c35714, /// 0x3c
			 0x9ba0fd33, /// 0x40
			 0x388af161, /// 0x44
			 0xece81583, /// 0x48
			 0x60ac081e, /// 0x4c
			 0x8db84a9e, /// 0x50
			 0x4e820bc9, /// 0x54
			 0x80d77894, /// 0x58
			 0x6cba0040, /// 0x5c
			 0x100a7d02, /// 0x60
			 0xebdbfade, /// 0x64
			 0x71db548f, /// 0x68
			 0x0c0a6feb, /// 0x6c
			 0x56ebd80f, /// 0x70
			 0xe82849f6, /// 0x74
			 0x443fd877, /// 0x78
			 0x93538c87, /// 0x7c
			 0xb36a69b2, /// 0x80
			 0xe5c419b6, /// 0x84
			 0xa45d76b6, /// 0x88
			 0x048a7325, /// 0x8c
			 0x2a3c0ae9, /// 0x90
			 0xaea19921, /// 0x94
			 0xa03c1f0a, /// 0x98
			 0x2b640a46, /// 0x9c
			 0x12102e4b, /// 0xa0
			 0x61344639, /// 0xa4
			 0x1493717b, /// 0xa8
			 0x74d92034, /// 0xac
			 0x45b0eb04, /// 0xb0
			 0x9d9c78f7, /// 0xb4
			 0x6d404948, /// 0xb8
			 0xaddcf6bd, /// 0xbc
			 0xedcf7b12, /// 0xc0
			 0x9354d891, /// 0xc4
			 0x3c63e5eb, /// 0xc8
			 0xe9c27680, /// 0xcc
			 0xce4a599e, /// 0xd0
			 0xd57a28aa, /// 0xd4
			 0x594c5d93, /// 0xd8
			 0xc06fe863, /// 0xdc
			 0xee4f64d7, /// 0xe0
			 0x3ce13a7d, /// 0xe4
			 0xd5f68f6b, /// 0xe8
			 0x2adee0e1, /// 0xec
			 0x5e1d9064, /// 0xf0
			 0xfa8636c7, /// 0xf4
			 0x02e7d160, /// 0xf8
			 0x6f788abd, /// 0xfc
			 0x544a9989, /// 0x100
			 0xb2d328a3, /// 0x104
			 0xf9d0fbcd, /// 0x108
			 0x9b41d8b2, /// 0x10c
			 0x4fd163c5, /// 0x110
			 0x6b4c313c, /// 0x114
			 0xf6c2f229, /// 0x118
			 0x7c2c8a62, /// 0x11c
			 0xaf789d33, /// 0x120
			 0xef270870, /// 0x124
			 0x1394b21c, /// 0x128
			 0x29dff0bf, /// 0x12c
			 0x2b63e5ec, /// 0x130
			 0x4c3e2a0f, /// 0x134
			 0xea0c5d24, /// 0x138
			 0xcbbca991, /// 0x13c
			 0xe39df234, /// 0x140
			 0xa56c9040, /// 0x144
			 0x7a3aee06, /// 0x148
			 0x3a59229a, /// 0x14c
			 0xeed47874, /// 0x150
			 0xa8f2a3d1, /// 0x154
			 0x948877b5, /// 0x158
			 0x0e88d394, /// 0x15c
			 0x899309f5, /// 0x160
			 0x9641d273, /// 0x164
			 0x568bca4b, /// 0x168
			 0xa65433a3, /// 0x16c
			 0x21f0df97, /// 0x170
			 0x2733ce9c, /// 0x174
			 0x07fa5d50, /// 0x178
			 0xa7cbfa67, /// 0x17c
			 0x41be9566, /// 0x180
			 0xd98d39bc, /// 0x184
			 0x3e508239, /// 0x188
			 0x482dd7e9, /// 0x18c
			 0x08130dd0, /// 0x190
			 0x76abbe23, /// 0x194
			 0xd3e0289f, /// 0x198
			 0xd19ad90d, /// 0x19c
			 0xee9105d9, /// 0x1a0
			 0x5fa3c228, /// 0x1a4
			 0xe308b722, /// 0x1a8
			 0xf4273866, /// 0x1ac
			 0x437cc978, /// 0x1b0
			 0x4902eb3b, /// 0x1b4
			 0xd47a8779, /// 0x1b8
			 0xa53e1d86, /// 0x1bc
			 0x1a78eca9, /// 0x1c0
			 0xac6e6a93, /// 0x1c4
			 0x71add734, /// 0x1c8
			 0x32114e23, /// 0x1cc
			 0x42bcd6c0, /// 0x1d0
			 0x0c14a208, /// 0x1d4
			 0xf2655e95, /// 0x1d8
			 0x856325bf, /// 0x1dc
			 0xecde5e89, /// 0x1e0
			 0x786aa4ee, /// 0x1e4
			 0x54a97b8d, /// 0x1e8
			 0xb11893dc, /// 0x1ec
			 0xb77383cb, /// 0x1f0
			 0x7637ade1, /// 0x1f4
			 0x204ea90f, /// 0x1f8
			 0x2c73eb30, /// 0x1fc
			 0x51314987, /// 0x200
			 0x188cc34f, /// 0x204
			 0x9b35c6f2, /// 0x208
			 0x17bb12b4, /// 0x20c
			 0xe5f5f5b3, /// 0x210
			 0xda4dd771, /// 0x214
			 0x272637d8, /// 0x218
			 0x775ff41c, /// 0x21c
			 0x99d09082, /// 0x220
			 0x11fcac5b, /// 0x224
			 0xc6ee6618, /// 0x228
			 0xe08b7793, /// 0x22c
			 0x4fa4665c, /// 0x230
			 0x3b5a133b, /// 0x234
			 0xff70e71f, /// 0x238
			 0x1b7a135b, /// 0x23c
			 0xcf292bbc, /// 0x240
			 0x69541fe5, /// 0x244
			 0x03f97b68, /// 0x248
			 0x025de893, /// 0x24c
			 0x44460650, /// 0x250
			 0x2586b572, /// 0x254
			 0xe24cc751, /// 0x258
			 0x1ef2bb1d, /// 0x25c
			 0xfb5149fc, /// 0x260
			 0xfe1ed0a1, /// 0x264
			 0xf7ce54f0, /// 0x268
			 0x1a6b07b9, /// 0x26c
			 0xaaa799be, /// 0x270
			 0x10b950e0, /// 0x274
			 0x5ef81d8e, /// 0x278
			 0x81734beb, /// 0x27c
			 0xb96c7bb4, /// 0x280
			 0xb4718abf, /// 0x284
			 0xce2239b4, /// 0x288
			 0x8163c34d, /// 0x28c
			 0x37709b1e, /// 0x290
			 0x53d074fb, /// 0x294
			 0x3f560cde, /// 0x298
			 0x4a97d9da, /// 0x29c
			 0x22b5fe29, /// 0x2a0
			 0x7add45dc, /// 0x2a4
			 0xffc9c6fd, /// 0x2a8
			 0x81e34117, /// 0x2ac
			 0x78472464, /// 0x2b0
			 0x527f154c, /// 0x2b4
			 0x2e7699f9, /// 0x2b8
			 0xe367039c, /// 0x2bc
			 0x02f5e0be, /// 0x2c0
			 0xb0cd15e9, /// 0x2c4
			 0xc2b6ef96, /// 0x2c8
			 0x8b1a4697, /// 0x2cc
			 0x5fcb8b64, /// 0x2d0
			 0xe73aed1e, /// 0x2d4
			 0x8aa07cf6, /// 0x2d8
			 0x8e973096, /// 0x2dc
			 0xdeac3302, /// 0x2e0
			 0x33d82045, /// 0x2e4
			 0x8fb668fd, /// 0x2e8
			 0x2298ff58, /// 0x2ec
			 0xf7b47054, /// 0x2f0
			 0x1f532d83, /// 0x2f4
			 0x9580eaba, /// 0x2f8
			 0x02e7cede, /// 0x2fc
			 0xfcec2f8d, /// 0x300
			 0x54fb4f52, /// 0x304
			 0xaf01079d, /// 0x308
			 0x8078b088, /// 0x30c
			 0xaa228064, /// 0x310
			 0xf7ca5d2e, /// 0x314
			 0x133c5989, /// 0x318
			 0xc8859f9e, /// 0x31c
			 0x2e6b0a16, /// 0x320
			 0x981a2289, /// 0x324
			 0xb0915e44, /// 0x328
			 0xace51759, /// 0x32c
			 0x16440c82, /// 0x330
			 0xa978edbb, /// 0x334
			 0x4b901129, /// 0x338
			 0xbdc879e6, /// 0x33c
			 0x275e7f63, /// 0x340
			 0x2519da87, /// 0x344
			 0x7e6c4b36, /// 0x348
			 0x7acd2385, /// 0x34c
			 0x24d60a2d, /// 0x350
			 0xb4a30202, /// 0x354
			 0xf9a2dd36, /// 0x358
			 0x18ca17bb, /// 0x35c
			 0xf4211212, /// 0x360
			 0x2b4a4330, /// 0x364
			 0x233ce877, /// 0x368
			 0xa42383b9, /// 0x36c
			 0x7bba099b, /// 0x370
			 0x09c107f6, /// 0x374
			 0x4d15c52b, /// 0x378
			 0x246b7e87, /// 0x37c
			 0x182d9c1a, /// 0x380
			 0x4fc35761, /// 0x384
			 0x0331d6bb, /// 0x388
			 0xe2fb3d23, /// 0x38c
			 0xa74aa5d9, /// 0x390
			 0x5130ecb7, /// 0x394
			 0x7ad01379, /// 0x398
			 0x508c89bb, /// 0x39c
			 0x36da5292, /// 0x3a0
			 0xc8b16e85, /// 0x3a4
			 0x4f90ee87, /// 0x3a8
			 0x0718f59c, /// 0x3ac
			 0xbd4e466a, /// 0x3b0
			 0xeb2ba4c7, /// 0x3b4
			 0x2e9bc744, /// 0x3b8
			 0x46942a4e, /// 0x3bc
			 0xa5fade45, /// 0x3c0
			 0x6cd80a6a, /// 0x3c4
			 0xd0d72283, /// 0x3c8
			 0x66e3c1f3, /// 0x3cc
			 0x17182ed3, /// 0x3d0
			 0x2f15c81d, /// 0x3d4
			 0xbe1da5b8, /// 0x3d8
			 0x0357d00e, /// 0x3dc
			 0x168039ef, /// 0x3e0
			 0xe5b6f900, /// 0x3e4
			 0x5c3b163f, /// 0x3e8
			 0x2f80b760, /// 0x3ec
			 0xeecf4bd5, /// 0x3f0
			 0xd033a82b, /// 0x3f4
			 0x0b4dc8eb, /// 0x3f8
			 0xd07138cc, /// 0x3fc
			 0xfb989ee9, /// 0x400
			 0x890d1dc7, /// 0x404
			 0xcbf9e41f, /// 0x408
			 0x3bf73212, /// 0x40c
			 0xf24d550c, /// 0x410
			 0xa8489166, /// 0x414
			 0xaaa1bc2b, /// 0x418
			 0xbec573ae, /// 0x41c
			 0x31bf21bb, /// 0x420
			 0xf3fde641, /// 0x424
			 0x82446863, /// 0x428
			 0xf3e645c2, /// 0x42c
			 0x638ba28e, /// 0x430
			 0x699c93e3, /// 0x434
			 0x565d39de, /// 0x438
			 0x9ea58ba2, /// 0x43c
			 0x3acaadb3, /// 0x440
			 0x19b8267d, /// 0x444
			 0x976f3a6a, /// 0x448
			 0x9800174a, /// 0x44c
			 0x4edfaa71, /// 0x450
			 0x47c4ade5, /// 0x454
			 0xd6c7a072, /// 0x458
			 0x74ad18f1, /// 0x45c
			 0xbda962d4, /// 0x460
			 0x120a2696, /// 0x464
			 0x1af87c6e, /// 0x468
			 0x8f3e2a81, /// 0x46c
			 0x7cd10c48, /// 0x470
			 0xd9e6ac51, /// 0x474
			 0x2a20e053, /// 0x478
			 0xccb0d57f, /// 0x47c
			 0xd5b8745f, /// 0x480
			 0xcb4ead25, /// 0x484
			 0x66ca76e2, /// 0x488
			 0x1a4fce0f, /// 0x48c
			 0x897ead6b, /// 0x490
			 0xac1b1f72, /// 0x494
			 0xe711e732, /// 0x498
			 0x83acc471, /// 0x49c
			 0x94de75a6, /// 0x4a0
			 0x03827d57, /// 0x4a4
			 0x3c4cd127, /// 0x4a8
			 0xd4631716, /// 0x4ac
			 0xa004187c, /// 0x4b0
			 0x4e088408, /// 0x4b4
			 0xa8edf8d9, /// 0x4b8
			 0x6211ce98, /// 0x4bc
			 0x007b7736, /// 0x4c0
			 0xe399102e, /// 0x4c4
			 0x6d22853f, /// 0x4c8
			 0xe1e7206f, /// 0x4cc
			 0x8d1ad03e, /// 0x4d0
			 0x588688d8, /// 0x4d4
			 0x3c347912, /// 0x4d8
			 0xf7668a90, /// 0x4dc
			 0x332f6fd7, /// 0x4e0
			 0xd1656f52, /// 0x4e4
			 0xcd4579aa, /// 0x4e8
			 0x9c20d09c, /// 0x4ec
			 0x12da4aa2, /// 0x4f0
			 0x62295a8b, /// 0x4f4
			 0x9a479594, /// 0x4f8
			 0x9c5141f5, /// 0x4fc
			 0xe12655f0, /// 0x500
			 0x5c313871, /// 0x504
			 0x0ae7561b, /// 0x508
			 0xd5cd4325, /// 0x50c
			 0xcb5dffe1, /// 0x510
			 0x4e27e2ec, /// 0x514
			 0x89bbac14, /// 0x518
			 0x1a7549bd, /// 0x51c
			 0x7686e1f3, /// 0x520
			 0xf87695bc, /// 0x524
			 0xbba1ee0c, /// 0x528
			 0x10e484c4, /// 0x52c
			 0x357d808e, /// 0x530
			 0x67aab8b1, /// 0x534
			 0x5dee96e8, /// 0x538
			 0x5b1e612e, /// 0x53c
			 0x915a6ef1, /// 0x540
			 0x3d6d6a12, /// 0x544
			 0x0211d53a, /// 0x548
			 0x4cb0a629, /// 0x54c
			 0xd6353d3d, /// 0x550
			 0x1cde7596, /// 0x554
			 0x35a0cccd, /// 0x558
			 0x8ddd4405, /// 0x55c
			 0xf5b64187, /// 0x560
			 0x26bdda6e, /// 0x564
			 0x5eb70b93, /// 0x568
			 0x64194706, /// 0x56c
			 0xb7804bb1, /// 0x570
			 0x7b9aa272, /// 0x574
			 0x77eb55a1, /// 0x578
			 0xd7f8f428, /// 0x57c
			 0x74ccc107, /// 0x580
			 0x6e0f6a46, /// 0x584
			 0xa6e3bc56, /// 0x588
			 0xba3bfbef, /// 0x58c
			 0x0d706c48, /// 0x590
			 0x1639ba80, /// 0x594
			 0x60bae483, /// 0x598
			 0xe430647d, /// 0x59c
			 0x2822dfd2, /// 0x5a0
			 0x4697e69c, /// 0x5a4
			 0x47942dc8, /// 0x5a8
			 0x7027ec29, /// 0x5ac
			 0xb79b1937, /// 0x5b0
			 0x011ee8ed, /// 0x5b4
			 0xe41c1a11, /// 0x5b8
			 0x5c9eb9ea, /// 0x5bc
			 0x2538d11f, /// 0x5c0
			 0x16a7e368, /// 0x5c4
			 0xff510aec, /// 0x5c8
			 0x0d42d378, /// 0x5cc
			 0xc43ece2c, /// 0x5d0
			 0xe1589e5c, /// 0x5d4
			 0x32b2b25d, /// 0x5d8
			 0x459165ff, /// 0x5dc
			 0x83091baa, /// 0x5e0
			 0xb686ce00, /// 0x5e4
			 0x72078aff, /// 0x5e8
			 0x69aae82e, /// 0x5ec
			 0xf9a87fae, /// 0x5f0
			 0xeb5110ba, /// 0x5f4
			 0x98549d27, /// 0x5f8
			 0x19009600, /// 0x5fc
			 0x6eeb91e1, /// 0x600
			 0xac06695e, /// 0x604
			 0x085d6ce4, /// 0x608
			 0xced1e65b, /// 0x60c
			 0x35044a69, /// 0x610
			 0x58a43ed2, /// 0x614
			 0x836572eb, /// 0x618
			 0xa0ec0e27, /// 0x61c
			 0xc72cc011, /// 0x620
			 0x4747248e, /// 0x624
			 0xf6c58067, /// 0x628
			 0xc1df03be, /// 0x62c
			 0xd7fdb783, /// 0x630
			 0x43268b42, /// 0x634
			 0x2f475811, /// 0x638
			 0x39bcf379, /// 0x63c
			 0x8bf3a013, /// 0x640
			 0x67c9230e, /// 0x644
			 0x315086e7, /// 0x648
			 0x13a6edb5, /// 0x64c
			 0xe9b089a3, /// 0x650
			 0x5ffd0641, /// 0x654
			 0x4a16fd9b, /// 0x658
			 0x100a6e9e, /// 0x65c
			 0x223a0b17, /// 0x660
			 0xf1d0d9ab, /// 0x664
			 0x75ced4e7, /// 0x668
			 0x963f6d1a, /// 0x66c
			 0x1c71bf20, /// 0x670
			 0xd5f46f91, /// 0x674
			 0x9ac6fb89, /// 0x678
			 0x73df1618, /// 0x67c
			 0xdaaf0049, /// 0x680
			 0xc28410ef, /// 0x684
			 0xa617e13f, /// 0x688
			 0x0af2a891, /// 0x68c
			 0x3e337369, /// 0x690
			 0x850e3f6b, /// 0x694
			 0x67ff349c, /// 0x698
			 0x76baab2d, /// 0x69c
			 0x1ee15ae3, /// 0x6a0
			 0x05a60d89, /// 0x6a4
			 0xdae4805c, /// 0x6a8
			 0x9ef86b62, /// 0x6ac
			 0x18242946, /// 0x6b0
			 0x52d2ba50, /// 0x6b4
			 0x9d2305c5, /// 0x6b8
			 0x8fdeb42e, /// 0x6bc
			 0xe3b0eae8, /// 0x6c0
			 0x8563ab8b, /// 0x6c4
			 0xb616e3f2, /// 0x6c8
			 0xa39bdfb8, /// 0x6cc
			 0x6da90efe, /// 0x6d0
			 0x6b77b418, /// 0x6d4
			 0x2c4ea384, /// 0x6d8
			 0x91d4b8aa, /// 0x6dc
			 0xf365aa2a, /// 0x6e0
			 0x31235236, /// 0x6e4
			 0x6da2ff5a, /// 0x6e8
			 0x1590c8e4, /// 0x6ec
			 0x9ab758af, /// 0x6f0
			 0x4c3763a3, /// 0x6f4
			 0x77041cf3, /// 0x6f8
			 0x930e3616, /// 0x6fc
			 0x733a829e, /// 0x700
			 0x0e1fb8e5, /// 0x704
			 0x59f0c351, /// 0x708
			 0x15ca4404, /// 0x70c
			 0x560af26d, /// 0x710
			 0x8d749046, /// 0x714
			 0x7aa132fa, /// 0x718
			 0x724a7298, /// 0x71c
			 0xeb380660, /// 0x720
			 0x601d0a96, /// 0x724
			 0x51f25870, /// 0x728
			 0xdd1171c4, /// 0x72c
			 0x1398efe2, /// 0x730
			 0x19792a67, /// 0x734
			 0xf5b1245b, /// 0x738
			 0x8ba16c0a, /// 0x73c
			 0xe050d8d0, /// 0x740
			 0x545c4ecc, /// 0x744
			 0x9e1f7059, /// 0x748
			 0x852ac239, /// 0x74c
			 0x3eed5d71, /// 0x750
			 0xb2dbcf3e, /// 0x754
			 0xbbd1ca8e, /// 0x758
			 0xe6e2b3c0, /// 0x75c
			 0x7c69d8c7, /// 0x760
			 0x181517b0, /// 0x764
			 0x86ebfaa1, /// 0x768
			 0xda14d597, /// 0x76c
			 0x7c5c4db4, /// 0x770
			 0x7826b48f, /// 0x774
			 0x244c3f10, /// 0x778
			 0x02d5ccdf, /// 0x77c
			 0xfb8be45f, /// 0x780
			 0xdf2db096, /// 0x784
			 0xe687e66d, /// 0x788
			 0x107baf44, /// 0x78c
			 0x2300b2ed, /// 0x790
			 0x983986e2, /// 0x794
			 0x815779b3, /// 0x798
			 0x723bba9c, /// 0x79c
			 0xa1d875f0, /// 0x7a0
			 0x0d584632, /// 0x7a4
			 0x02d42806, /// 0x7a8
			 0x3f0154fd, /// 0x7ac
			 0x146af23d, /// 0x7b0
			 0x61936afb, /// 0x7b4
			 0xea2f53ab, /// 0x7b8
			 0x3834d299, /// 0x7bc
			 0x8e429567, /// 0x7c0
			 0x1d53b7ff, /// 0x7c4
			 0xa31e2d4e, /// 0x7c8
			 0x98485ea6, /// 0x7cc
			 0xb2ddb9f0, /// 0x7d0
			 0x42c98ff8, /// 0x7d4
			 0xd0e5000b, /// 0x7d8
			 0xbefc3ace, /// 0x7dc
			 0xab371400, /// 0x7e0
			 0xfc0e713d, /// 0x7e4
			 0x7053c1de, /// 0x7e8
			 0x49b65c36, /// 0x7ec
			 0x9d2400b2, /// 0x7f0
			 0x3b4e583a, /// 0x7f4
			 0x312f5c4d, /// 0x7f8
			 0x552a736f, /// 0x7fc
			 0x8922f8f2, /// 0x800
			 0x4b8afbe6, /// 0x804
			 0x4bc4a73a, /// 0x808
			 0x5ff1c591, /// 0x80c
			 0x837b73bc, /// 0x810
			 0xefc56ffe, /// 0x814
			 0x22a3a006, /// 0x818
			 0x6d96e303, /// 0x81c
			 0xf3aae3c2, /// 0x820
			 0x2f7df406, /// 0x824
			 0xc29530c2, /// 0x828
			 0xf059129b, /// 0x82c
			 0xb1d10640, /// 0x830
			 0xffc605ea, /// 0x834
			 0x07f63c6f, /// 0x838
			 0x1130150a, /// 0x83c
			 0xc1cfd926, /// 0x840
			 0xd728a2e6, /// 0x844
			 0xbdf60c1c, /// 0x848
			 0xfecd8155, /// 0x84c
			 0x19b0a40d, /// 0x850
			 0x3f026e8e, /// 0x854
			 0xf7ebe4f0, /// 0x858
			 0xc7ec0c5d, /// 0x85c
			 0xd50a7433, /// 0x860
			 0x0cb9ba31, /// 0x864
			 0x99b20eff, /// 0x868
			 0xce1b4e2e, /// 0x86c
			 0x687d2589, /// 0x870
			 0xf362ba60, /// 0x874
			 0x0db73ad7, /// 0x878
			 0xc85d49cd, /// 0x87c
			 0xd0eea31a, /// 0x880
			 0xb1a06a43, /// 0x884
			 0x1e362526, /// 0x888
			 0x8dcf8847, /// 0x88c
			 0xaf84add3, /// 0x890
			 0x67b08138, /// 0x894
			 0xaef60e2f, /// 0x898
			 0x74fe4c5d, /// 0x89c
			 0xfe78b030, /// 0x8a0
			 0x43a707c3, /// 0x8a4
			 0x8469ed4b, /// 0x8a8
			 0x6c50af9c, /// 0x8ac
			 0x7a7345da, /// 0x8b0
			 0xf8b8a52f, /// 0x8b4
			 0x3989f940, /// 0x8b8
			 0x0d892c09, /// 0x8bc
			 0x9b12dbd7, /// 0x8c0
			 0x31f621f8, /// 0x8c4
			 0x464d0f7e, /// 0x8c8
			 0x3612839f, /// 0x8cc
			 0xcc767840, /// 0x8d0
			 0xd8ab97fe, /// 0x8d4
			 0xed0407d4, /// 0x8d8
			 0x5bac674b, /// 0x8dc
			 0xb5d9f500, /// 0x8e0
			 0x000ddf23, /// 0x8e4
			 0x6b3fa600, /// 0x8e8
			 0x26e1aca8, /// 0x8ec
			 0x0e1a2502, /// 0x8f0
			 0x63675b2f, /// 0x8f4
			 0xf98d8e6e, /// 0x8f8
			 0x6c5ea4f2, /// 0x8fc
			 0x4f1e88ef, /// 0x900
			 0xf1bc647b, /// 0x904
			 0x61610db3, /// 0x908
			 0x4d8d006d, /// 0x90c
			 0x800d92f3, /// 0x910
			 0xa3f90ee7, /// 0x914
			 0x4fd139e1, /// 0x918
			 0x4a398a86, /// 0x91c
			 0xe2603e3e, /// 0x920
			 0x5cd9a989, /// 0x924
			 0x06c66785, /// 0x928
			 0xb09f5feb, /// 0x92c
			 0xf0ae5a4a, /// 0x930
			 0x1bdd2b97, /// 0x934
			 0xdf44afec, /// 0x938
			 0x874a00d8, /// 0x93c
			 0x6d640ce9, /// 0x940
			 0x54d1a569, /// 0x944
			 0x237f9c96, /// 0x948
			 0x3439e7be, /// 0x94c
			 0xdd194b9f, /// 0x950
			 0xed117e52, /// 0x954
			 0x57a11877, /// 0x958
			 0xdf9a0dc7, /// 0x95c
			 0x23a9484b, /// 0x960
			 0x46871ab8, /// 0x964
			 0x9c689fd1, /// 0x968
			 0x9416ab75, /// 0x96c
			 0x74a6ae20, /// 0x970
			 0x6833d579, /// 0x974
			 0xe65508a3, /// 0x978
			 0x2773f75a, /// 0x97c
			 0xa13a8efd, /// 0x980
			 0x5c0849e9, /// 0x984
			 0x2416b942, /// 0x988
			 0x21f0ccb5, /// 0x98c
			 0x89751f1a, /// 0x990
			 0x29818287, /// 0x994
			 0xe8f45fa7, /// 0x998
			 0xfb56b7d8, /// 0x99c
			 0xeb4ccc9b, /// 0x9a0
			 0xcdae146f, /// 0x9a4
			 0xf570d5d9, /// 0x9a8
			 0x74306b0a, /// 0x9ac
			 0x1b544cef, /// 0x9b0
			 0x84701eb0, /// 0x9b4
			 0xf0e60c6d, /// 0x9b8
			 0xf374b237, /// 0x9bc
			 0x2e73e05c, /// 0x9c0
			 0x042c2b16, /// 0x9c4
			 0xdceaf8d4, /// 0x9c8
			 0xe2293fc4, /// 0x9cc
			 0xccfebda6, /// 0x9d0
			 0x9304abe8, /// 0x9d4
			 0xb6216cff, /// 0x9d8
			 0x39a8194a, /// 0x9dc
			 0x520da2ce, /// 0x9e0
			 0xe1042a1d, /// 0x9e4
			 0x0019202d, /// 0x9e8
			 0xd8390c24, /// 0x9ec
			 0x251a2c2c, /// 0x9f0
			 0xd7b8626d, /// 0x9f4
			 0x5039c3e9, /// 0x9f8
			 0x42b2d4a8, /// 0x9fc
			 0xae64ddc3, /// 0xa00
			 0x5c0a4eb7, /// 0xa04
			 0xfccb3928, /// 0xa08
			 0xfef5cc81, /// 0xa0c
			 0xe2aa44c4, /// 0xa10
			 0x7f4d75e2, /// 0xa14
			 0xab54f8e4, /// 0xa18
			 0x14ed2420, /// 0xa1c
			 0x9b4a9c1b, /// 0xa20
			 0xb67ebe02, /// 0xa24
			 0x32666cfc, /// 0xa28
			 0x68cecf66, /// 0xa2c
			 0x32f6acf3, /// 0xa30
			 0x31b2ae08, /// 0xa34
			 0x041705d7, /// 0xa38
			 0x0fca12b1, /// 0xa3c
			 0xe3a337f6, /// 0xa40
			 0xd8333e9d, /// 0xa44
			 0xad5f635c, /// 0xa48
			 0x246679f5, /// 0xa4c
			 0x2b17de54, /// 0xa50
			 0xdbdbc5fb, /// 0xa54
			 0x0cc334ba, /// 0xa58
			 0x1d4bc850, /// 0xa5c
			 0x48d7413e, /// 0xa60
			 0x7ec446c2, /// 0xa64
			 0x345d9212, /// 0xa68
			 0x0fb73dc1, /// 0xa6c
			 0x0e0c5102, /// 0xa70
			 0xc2a192e8, /// 0xa74
			 0xf93482f7, /// 0xa78
			 0x5446afc5, /// 0xa7c
			 0x1ecf9ce7, /// 0xa80
			 0x4801fa1a, /// 0xa84
			 0x77dad7e8, /// 0xa88
			 0x294c6d55, /// 0xa8c
			 0x85991b31, /// 0xa90
			 0x238f7f19, /// 0xa94
			 0xf183cde6, /// 0xa98
			 0x54c6bcc8, /// 0xa9c
			 0x12483293, /// 0xaa0
			 0x49eb370d, /// 0xaa4
			 0x17429ab6, /// 0xaa8
			 0x69c39550, /// 0xaac
			 0x8a1098f9, /// 0xab0
			 0x0d7fe3bd, /// 0xab4
			 0x4c0fd41d, /// 0xab8
			 0x4538cf03, /// 0xabc
			 0x8fa59cf6, /// 0xac0
			 0x2ece3943, /// 0xac4
			 0x54342317, /// 0xac8
			 0x56e36441, /// 0xacc
			 0x46339a1f, /// 0xad0
			 0x89392d6e, /// 0xad4
			 0xf2ad4f7a, /// 0xad8
			 0xf7565a50, /// 0xadc
			 0xcf77132a, /// 0xae0
			 0xfea449f1, /// 0xae4
			 0xb967d826, /// 0xae8
			 0xac25686a, /// 0xaec
			 0x589d17e6, /// 0xaf0
			 0xbd463a4d, /// 0xaf4
			 0xf1e533c9, /// 0xaf8
			 0xba7998e8, /// 0xafc
			 0x64e72af8, /// 0xb00
			 0x6343771f, /// 0xb04
			 0xe09a16db, /// 0xb08
			 0xfd51ceb9, /// 0xb0c
			 0x325e978a, /// 0xb10
			 0x9f474b12, /// 0xb14
			 0x9255d72d, /// 0xb18
			 0xf7dbfb9a, /// 0xb1c
			 0xdf4ace72, /// 0xb20
			 0x53e67dbe, /// 0xb24
			 0x14655f41, /// 0xb28
			 0x4e292355, /// 0xb2c
			 0x7fb3031c, /// 0xb30
			 0x5df16b21, /// 0xb34
			 0x037b67c6, /// 0xb38
			 0x0386eeea, /// 0xb3c
			 0x2235edfa, /// 0xb40
			 0x6d562d6a, /// 0xb44
			 0x0855fe8f, /// 0xb48
			 0x43b7b004, /// 0xb4c
			 0xaae94e55, /// 0xb50
			 0x8b9c7851, /// 0xb54
			 0x9cc8e1a7, /// 0xb58
			 0xe69bac48, /// 0xb5c
			 0x79584ec0, /// 0xb60
			 0xe071cbf2, /// 0xb64
			 0xd8106f13, /// 0xb68
			 0x70562768, /// 0xb6c
			 0x03bc154d, /// 0xb70
			 0x3e58b4ef, /// 0xb74
			 0x73e0fa51, /// 0xb78
			 0x3cb52f29, /// 0xb7c
			 0x6887aedb, /// 0xb80
			 0xd7aa9b95, /// 0xb84
			 0x05dcf7f9, /// 0xb88
			 0x1feb9c91, /// 0xb8c
			 0xd99f7f41, /// 0xb90
			 0xd8843f99, /// 0xb94
			 0xda6c053d, /// 0xb98
			 0x6c16b078, /// 0xb9c
			 0x05e7edb9, /// 0xba0
			 0x3fb8caa7, /// 0xba4
			 0xcc735365, /// 0xba8
			 0x294a7464, /// 0xbac
			 0x186f9a25, /// 0xbb0
			 0x9dd5ecbe, /// 0xbb4
			 0x266955bb, /// 0xbb8
			 0x4adb1437, /// 0xbbc
			 0x5f17b4d3, /// 0xbc0
			 0x9c4cfa28, /// 0xbc4
			 0x930f6162, /// 0xbc8
			 0x05a8a290, /// 0xbcc
			 0xd799c34b, /// 0xbd0
			 0x79df27a4, /// 0xbd4
			 0xcc01c490, /// 0xbd8
			 0x7e3cfe84, /// 0xbdc
			 0x5c9e09c7, /// 0xbe0
			 0xff634400, /// 0xbe4
			 0x354a5fbc, /// 0xbe8
			 0x2825bb41, /// 0xbec
			 0x2ffeb1db, /// 0xbf0
			 0xaac6554f, /// 0xbf4
			 0x393a9037, /// 0xbf8
			 0x4ebfd78d, /// 0xbfc
			 0xd7bb98bb, /// 0xc00
			 0x209fa6cd, /// 0xc04
			 0x00566964, /// 0xc08
			 0xb871a126, /// 0xc0c
			 0x323fc047, /// 0xc10
			 0x6def2790, /// 0xc14
			 0x3dc6bedf, /// 0xc18
			 0xea699b62, /// 0xc1c
			 0xb39db822, /// 0xc20
			 0x99e4b7a7, /// 0xc24
			 0x4ad338bc, /// 0xc28
			 0xb1d49e15, /// 0xc2c
			 0x69dd96b9, /// 0xc30
			 0xcbec6b23, /// 0xc34
			 0x25cc1565, /// 0xc38
			 0x1c349bd9, /// 0xc3c
			 0x4ef2a0a0, /// 0xc40
			 0xe630af47, /// 0xc44
			 0x069561ec, /// 0xc48
			 0x5417ce74, /// 0xc4c
			 0x7777938d, /// 0xc50
			 0xc19116fd, /// 0xc54
			 0x55d2759e, /// 0xc58
			 0x0066c0df, /// 0xc5c
			 0x39effa6e, /// 0xc60
			 0x050348f6, /// 0xc64
			 0x3425aec3, /// 0xc68
			 0x463fd7c6, /// 0xc6c
			 0x49e0dca7, /// 0xc70
			 0x3e343d23, /// 0xc74
			 0xdb359537, /// 0xc78
			 0xc2206636, /// 0xc7c
			 0x10b292b0, /// 0xc80
			 0x59dbe905, /// 0xc84
			 0x7120544e, /// 0xc88
			 0x4238b0f4, /// 0xc8c
			 0xc9111933, /// 0xc90
			 0x8228ef1b, /// 0xc94
			 0x9ed48b5d, /// 0xc98
			 0x1162f147, /// 0xc9c
			 0xc2e86d30, /// 0xca0
			 0x2d1ca958, /// 0xca4
			 0x423d733c, /// 0xca8
			 0xb54f40cd, /// 0xcac
			 0x95a034a5, /// 0xcb0
			 0x0218c5d3, /// 0xcb4
			 0xd8f6a8c0, /// 0xcb8
			 0x5dd89cc6, /// 0xcbc
			 0x4c27c6d6, /// 0xcc0
			 0x47722a0a, /// 0xcc4
			 0x400ac5f2, /// 0xcc8
			 0xd488d323, /// 0xccc
			 0x86d2e550, /// 0xcd0
			 0xdb67f55f, /// 0xcd4
			 0xb43d4270, /// 0xcd8
			 0x23018ad1, /// 0xcdc
			 0xc7bf30fa, /// 0xce0
			 0x3960426b, /// 0xce4
			 0x9c57b762, /// 0xce8
			 0xfc8980a8, /// 0xcec
			 0xda835c3b, /// 0xcf0
			 0x4126fd77, /// 0xcf4
			 0xfe511789, /// 0xcf8
			 0x41718a45, /// 0xcfc
			 0x123235e6, /// 0xd00
			 0xabaeb252, /// 0xd04
			 0x01513479, /// 0xd08
			 0x16bb39f9, /// 0xd0c
			 0x3d724a0d, /// 0xd10
			 0xa8f4d5dd, /// 0xd14
			 0x77026314, /// 0xd18
			 0x0ca66d43, /// 0xd1c
			 0x57238b7c, /// 0xd20
			 0x6bbd673e, /// 0xd24
			 0x29bfdbb1, /// 0xd28
			 0x6bdef5fc, /// 0xd2c
			 0x3cda96ee, /// 0xd30
			 0xb47b682e, /// 0xd34
			 0xb287f1d9, /// 0xd38
			 0x96e1d457, /// 0xd3c
			 0xfeb59885, /// 0xd40
			 0x5fda73e7, /// 0xd44
			 0x6119a7a4, /// 0xd48
			 0x01581aed, /// 0xd4c
			 0xd3370e06, /// 0xd50
			 0x3f1ce508, /// 0xd54
			 0x1435fc84, /// 0xd58
			 0xd017421e, /// 0xd5c
			 0x54db5510, /// 0xd60
			 0x999d077d, /// 0xd64
			 0x1e0a5969, /// 0xd68
			 0x9363270c, /// 0xd6c
			 0x01a32ead, /// 0xd70
			 0xd02f3e8c, /// 0xd74
			 0x63195512, /// 0xd78
			 0x423b1d52, /// 0xd7c
			 0xc15c363a, /// 0xd80
			 0xa20a3597, /// 0xd84
			 0xc763016f, /// 0xd88
			 0x3fe491d2, /// 0xd8c
			 0x793b1ad9, /// 0xd90
			 0x27d22205, /// 0xd94
			 0xffb2789a, /// 0xd98
			 0xe6eb113a, /// 0xd9c
			 0x15b4175b, /// 0xda0
			 0x4403b307, /// 0xda4
			 0x94863c0f, /// 0xda8
			 0x811fe4fa, /// 0xdac
			 0x9c4b8f97, /// 0xdb0
			 0x54569683, /// 0xdb4
			 0xf51f0198, /// 0xdb8
			 0x59772cb1, /// 0xdbc
			 0x5cb2786b, /// 0xdc0
			 0x85882a76, /// 0xdc4
			 0x44906b49, /// 0xdc8
			 0x187a45ba, /// 0xdcc
			 0x5f4d4a85, /// 0xdd0
			 0xd3d264a4, /// 0xdd4
			 0x635bbdf9, /// 0xdd8
			 0x7ac853fa, /// 0xddc
			 0xcbc1bcab, /// 0xde0
			 0x06d8c688, /// 0xde4
			 0xe927be23, /// 0xde8
			 0xb85679c4, /// 0xdec
			 0x9bedf2a7, /// 0xdf0
			 0xea459e58, /// 0xdf4
			 0x1a98336a, /// 0xdf8
			 0x708fb25b, /// 0xdfc
			 0x3d130198, /// 0xe00
			 0xb6d61375, /// 0xe04
			 0xdfb320d1, /// 0xe08
			 0x782577d1, /// 0xe0c
			 0x7fead3e4, /// 0xe10
			 0x182a2050, /// 0xe14
			 0xdd39dc97, /// 0xe18
			 0xa580c3ef, /// 0xe1c
			 0x73cddac1, /// 0xe20
			 0xd25f47ab, /// 0xe24
			 0x37f865c3, /// 0xe28
			 0x51b8f31a, /// 0xe2c
			 0x3dfb9382, /// 0xe30
			 0x399c5316, /// 0xe34
			 0xe304d368, /// 0xe38
			 0x82a71c7e, /// 0xe3c
			 0x3b11c980, /// 0xe40
			 0xac9c46f5, /// 0xe44
			 0x2708b16c, /// 0xe48
			 0x5717b14f, /// 0xe4c
			 0xf042e46d, /// 0xe50
			 0x50f6751f, /// 0xe54
			 0x9c1c5c2a, /// 0xe58
			 0x05d55623, /// 0xe5c
			 0x682ebce8, /// 0xe60
			 0x5ef462cf, /// 0xe64
			 0x0da88d9b, /// 0xe68
			 0x0d0df256, /// 0xe6c
			 0x1dbff98a, /// 0xe70
			 0x7ebaf28f, /// 0xe74
			 0x608c8ed3, /// 0xe78
			 0x106ef836, /// 0xe7c
			 0x462e3d07, /// 0xe80
			 0xf01aa2a0, /// 0xe84
			 0x52e4e791, /// 0xe88
			 0xb9d40b6e, /// 0xe8c
			 0x58b7c5dc, /// 0xe90
			 0xfc698cd5, /// 0xe94
			 0xadba500f, /// 0xe98
			 0xee40084f, /// 0xe9c
			 0xf5b24b03, /// 0xea0
			 0x096ec727, /// 0xea4
			 0xd3f0e606, /// 0xea8
			 0x0d909472, /// 0xeac
			 0x8b63ca21, /// 0xeb0
			 0x0444a413, /// 0xeb4
			 0x2642b1d3, /// 0xeb8
			 0x9b00a4e0, /// 0xebc
			 0x36a732a7, /// 0xec0
			 0x567a02a9, /// 0xec4
			 0xef12d3ac, /// 0xec8
			 0x45accf0b, /// 0xecc
			 0x732fa34b, /// 0xed0
			 0x5d80c0ff, /// 0xed4
			 0x5094a8d1, /// 0xed8
			 0xf46c5f82, /// 0xedc
			 0xb4473b15, /// 0xee0
			 0x88bdfc30, /// 0xee4
			 0xa89581f8, /// 0xee8
			 0x88cd0279, /// 0xeec
			 0xb29dddb2, /// 0xef0
			 0xb174e646, /// 0xef4
			 0x9935e622, /// 0xef8
			 0x6c1fc551, /// 0xefc
			 0x56721e27, /// 0xf00
			 0x8481cecf, /// 0xf04
			 0x8db33aee, /// 0xf08
			 0x72010ecb, /// 0xf0c
			 0x8ae3b63b, /// 0xf10
			 0xc57e5524, /// 0xf14
			 0x10edee1c, /// 0xf18
			 0xf3a4de92, /// 0xf1c
			 0xd3a796a1, /// 0xf20
			 0xf946b462, /// 0xf24
			 0x8cb6aea9, /// 0xf28
			 0x567d85c7, /// 0xf2c
			 0x37e90618, /// 0xf30
			 0xa1db0f5a, /// 0xf34
			 0x45fed4ec, /// 0xf38
			 0x558d06c0, /// 0xf3c
			 0xcc249a6b, /// 0xf40
			 0xb4f32185, /// 0xf44
			 0x36b4a7ba, /// 0xf48
			 0x81e7c947, /// 0xf4c
			 0x81211716, /// 0xf50
			 0x1f16498b, /// 0xf54
			 0x5604b1b0, /// 0xf58
			 0xd82c5843, /// 0xf5c
			 0xbf8777a6, /// 0xf60
			 0x8a0ef217, /// 0xf64
			 0x7697749f, /// 0xf68
			 0xe28b8717, /// 0xf6c
			 0x24b269f5, /// 0xf70
			 0x07625a73, /// 0xf74
			 0xbcffdea8, /// 0xf78
			 0x9206f19e, /// 0xf7c
			 0x994508a7, /// 0xf80
			 0x8c865ba9, /// 0xf84
			 0x817e015d, /// 0xf88
			 0x2da7ae65, /// 0xf8c
			 0x3858e97d, /// 0xf90
			 0xbeb783cf, /// 0xf94
			 0xe3882d47, /// 0xf98
			 0xe64a6159, /// 0xf9c
			 0x30b5a176, /// 0xfa0
			 0x3a838ed6, /// 0xfa4
			 0x363f4e8c, /// 0xfa8
			 0x93aa64fc, /// 0xfac
			 0x5f38206b, /// 0xfb0
			 0x3d1ad687, /// 0xfb4
			 0x76e79fd9, /// 0xfb8
			 0x9723fba3, /// 0xfbc
			 0x19b2048e, /// 0xfc0
			 0xfc75ac88, /// 0xfc4
			 0x75eac4a9, /// 0xfc8
			 0xb6c7bd5e, /// 0xfcc
			 0x242d1076, /// 0xfd0
			 0x026aeaa1, /// 0xfd4
			 0xc93556b3, /// 0xfd8
			 0x118ae22d, /// 0xfdc
			 0xc35fff81, /// 0xfe0
			 0x607b6339, /// 0xfe4
			 0x222f4a54, /// 0xfe8
			 0xc9a34ed0, /// 0xfec
			 0x9ef90245, /// 0xff0
			 0x9fe10dd3, /// 0xff4
			 0xc0ba2c8b, /// 0xff8
			 0x67600f08 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00004
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00014
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00018
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0001c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00020
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0002c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00034
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0003c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00040
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00044
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00050
			 0x4b000000,                                                  // 8388608.0                                   /// 00054
			 0xffc00001,                                                  // -signaling NaN                              /// 00058
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0005c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00060
			 0x0c700000,                                                  // Leading 1s:                                 /// 00064
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00068
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00074
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0007c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00080
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00084
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00088
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00090
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e000001,                                                  // Trailing 1s:                                /// 00098
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000b4
			 0x7fc00001,                                                  // signaling NaN                               /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000e0
			 0xbf028f5c,                                                  // -0.51                                       /// 000e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00100
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0010c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00110
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00114
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00118
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00124
			 0xbf028f5c,                                                  // -0.51                                       /// 00128
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00130
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00134
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00138
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0013c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00144
			 0x0c780000,                                                  // Leading 1s:                                 /// 00148
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00150
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00160
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00164
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00168
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0016c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00170
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00174
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00178
			 0x0c700000,                                                  // Leading 1s:                                 /// 0017c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00180
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00184
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0018c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00190
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001dc
			 0xff800000,                                                  // -inf                                        /// 001e0
			 0x7fc00001,                                                  // signaling NaN                               /// 001e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00200
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00204
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0020c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00214
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00218
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0021c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x33333333,                                                  // 4 random values                             /// 00228
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0022c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00230
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00234
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00238
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0023c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x7fc00001,                                                  // signaling NaN                               /// 00244
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00248
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0024c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00254
			 0x80000000,                                                  // -zero                                       /// 00258
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0025c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00260
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00268
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x55555555,                                                  // 4 random values                             /// 00270
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00274
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00278
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00280
			 0x0c780000,                                                  // Leading 1s:                                 /// 00284
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0028c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00290
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0029c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00300
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00304
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00308
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0030c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00314
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00320
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00328
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00330
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00334
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0033c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00340
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00344
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0034c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00350
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0035c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00364
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0xffc00001,                                                  // -signaling NaN                              /// 00370
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00374
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0037c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00380
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00388
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00398
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00404
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00408
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00410
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00418
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00420
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00428
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00434
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00438
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0043c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00444
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c780000,                                                  // Leading 1s:                                 /// 0044c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00454
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00458
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0045c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00460
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00468
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0046c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x7fc00001,                                                  // signaling NaN                               /// 00474
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0047c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00480
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00484
			 0x80011111,                                                  // -9.7958E-41                                 /// 00488
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00490
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00494
			 0x0e000003,                                                  // Trailing 1s:                                /// 00498
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x3f028f5c,                                                  // 0.51                                        /// 004ac
			 0xbf028f5c,                                                  // -0.51                                       /// 004b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004f4
			 0x7fc00001,                                                  // signaling NaN                               /// 004f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00500
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00504
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00510
			 0x0e000007,                                                  // Trailing 1s:                                /// 00514
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00518
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00520
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00528
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0xff800000,                                                  // -inf                                        /// 00530
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0053c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00540
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00550
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00554
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00558
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0055c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00564
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00574
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00578
			 0x3f028f5c,                                                  // 0.51                                        /// 0057c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00580
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00584
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00588
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00590
			 0xff800000,                                                  // -inf                                        /// 00594
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x4b000000,                                                  // 8388608.0                                   /// 0059c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00604
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00608
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0060c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00614
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00618
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0061c
			 0x7f800000,                                                  // inf                                         /// 00620
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00624
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00628
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00630
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00634
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00638
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00640
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00644
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00648
			 0x00011111,                                                  // 9.7958E-41                                  /// 0064c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00660
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00664
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0066c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00670
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00674
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00678
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0067c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00680
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0068c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00000000,                                                  // zero                                        /// 00694
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00698
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0069c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c4
			 0x33333333,                                                  // 4 random values                             /// 006c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006fc
			 0x33333333,                                                  // 4 random values                             /// 00700
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00704
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00708
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0070c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0e000001,                                                  // Trailing 1s:                                /// 00718
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0071c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00720
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00728
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0072c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00730
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00734
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0073c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00740
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00744
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00748
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0074c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00750
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00754
			 0xff800000,                                                  // -inf                                        /// 00758
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00764
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00768
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0076c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00774
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80011111,                                                  // -9.7958E-41                                 /// 00784
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00788
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0078c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00790
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00794
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00798
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0079c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x4b000000,                                                  // 8388608.0                                   /// 007d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007e0
			 0x7f800000,                                                  // inf                                         /// 007e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x33333333,                                                  // 4 random values                             /// 00810
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00814
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00818
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00820
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00828
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0082c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00830
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00834
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00838
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00844
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00848
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00850
			 0xcb000000,                                                  // -8388608.0                                  /// 00854
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00858
			 0xbf028f5c,                                                  // -0.51                                       /// 0085c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00864
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00868
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0086c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00870
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00874
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00878
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0087c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00880
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00884
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00888
			 0x00011111,                                                  // 9.7958E-41                                  /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00894
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x33333333,                                                  // 4 random values                             /// 0089c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008bc
			 0xffc00001,                                                  // -signaling NaN                              /// 008c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008c8
			 0x33333333,                                                  // 4 random values                             /// 008cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008ec
			 0xffc00001,                                                  // -signaling NaN                              /// 008f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00900
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x7f800000,                                                  // inf                                         /// 00908
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0090c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00910
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00914
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00918
			 0xcb000000,                                                  // -8388608.0                                  /// 0091c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00920
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00928
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0xbf028f5c,                                                  // -0.51                                       /// 00934
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00938
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0093c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00948
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x00000000,                                                  // zero                                        /// 00954
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00958
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00960
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00964
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0096c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00970
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00974
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00978
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0097c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00980
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00984
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00988
			 0x80011111,                                                  // -9.7958E-41                                 /// 0098c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00990
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00998
			 0x80000000,                                                  // -zero                                       /// 0099c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e0
			 0xff800000,                                                  // -inf                                        /// 009e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a3c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x00000000,                                                  // zero                                        /// 00a4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a64
			 0xbf028f5c,                                                  // -0.51                                       /// 00a68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00aa0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ab4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00abc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00acc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ae0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00af4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00afc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b30
			 0x3f028f5c,                                                  // 0.51                                        /// 00b34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b40
			 0xbf028f5c,                                                  // -0.51                                       /// 00b44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b68
			 0xbf028f5c,                                                  // -0.51                                       /// 00b6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b74
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b98
			 0xcb000000,                                                  // -8388608.0                                  /// 00b9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ba8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bc8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00be4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c30
			 0x33333333,                                                  // 4 random values                             /// 00c34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ca4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb4
			 0x00000000,                                                  // zero                                        /// 00cb8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ce8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf0
			 0xff800000,                                                  // -inf                                        /// 00cf4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cf8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d04
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x33333333,                                                  // 4 random values                             /// 00d20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d64
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d6c
			 0xff800000,                                                  // -inf                                        /// 00d70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00da8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00db0
			 0x33333333,                                                  // 4 random values                             /// 00db4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dcc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00df8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00e00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e60
			 0x33333333,                                                  // 4 random values                             /// 00e64
			 0xbf028f5c,                                                  // -0.51                                       /// 00e68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x33333333,                                                  // 4 random values                             /// 00e98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ea0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ea4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00eb4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eb8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ebc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ec4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ed0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ed4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ed8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00edc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ee4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ee8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ef0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ef4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00efc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f04
			 0xff800000,                                                  // -inf                                        /// 00f08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f10
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fbc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fd4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fdc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e0007ff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00000
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00004
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00008
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0000c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00010
			 0x80011111,                                                  // -9.7958E-41                                 /// 00014
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0001c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00024
			 0xffc00001,                                                  // -signaling NaN                              /// 00028
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0002c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00034
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00038
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0003c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00040
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00044
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00048
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0004c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00054
			 0x7f800000,                                                  // inf                                         /// 00058
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0005c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00060
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00068
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00070
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00074
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x7f800000,                                                  // inf                                         /// 0007c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00080
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00084
			 0x55555555,                                                  // 4 random values                             /// 00088
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x3f028f5c,                                                  // 0.51                                        /// 00090
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00094
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00098
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 000e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x7fc00001,                                                  // signaling NaN                               /// 000f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00104
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80000000,                                                  // -zero                                       /// 0010c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00110
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00114
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00118
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00120
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00124
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00128
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0012c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x33333333,                                                  // 4 random values                             /// 0013c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00140
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00144
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00148
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0014c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00154
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00158
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0015c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00164
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00168
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0016c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00174
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00178
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0017c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0018c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00190
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00194
			 0x00000000,                                                  // zero                                        /// 00198
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00000000,                                                  // zero                                        /// 001d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00200
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00208
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0020c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00210
			 0x0c600000,                                                  // Leading 1s:                                 /// 00214
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00218
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0021c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00220
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00224
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00228
			 0xbf028f5c,                                                  // -0.51                                       /// 0022c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00230
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00234
			 0xcb000000,                                                  // -8388608.0                                  /// 00238
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00244
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00250
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00254
			 0xbf028f5c,                                                  // -0.51                                       /// 00258
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0025c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00260
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00264
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0026c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00270
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00274
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00278
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0027c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00280
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0028c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00290
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0xbf028f5c,                                                  // -0.51                                       /// 002a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00304
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00308
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0030c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00310
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00314
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0031c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00320
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00324
			 0x0c780000,                                                  // Leading 1s:                                 /// 00328
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00334
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00338
			 0x0c780000,                                                  // Leading 1s:                                 /// 0033c
			 0x4b000000,                                                  // 8388608.0                                   /// 00340
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00344
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0034c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00354
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00358
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0035c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00360
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00364
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00368
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0036c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00370
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00374
			 0x0e000007,                                                  // Trailing 1s:                                /// 00378
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0xcb000000,                                                  // -8388608.0                                  /// 00380
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00388
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0038c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00390
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0039c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003cc
			 0xffc00001,                                                  // -signaling NaN                              /// 003d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e000003,                                                  // Trailing 1s:                                /// 00404
			 0x80000000,                                                  // -zero                                       /// 00408
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0040c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00414
			 0x0c700000,                                                  // Leading 1s:                                 /// 00418
			 0xcb000000,                                                  // -8388608.0                                  /// 0041c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00420
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00428
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0042c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00434
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00438
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0043c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00440
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00444
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00450
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00458
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00464
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00468
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0046c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00478
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0047c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00480
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00484
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00488
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0048c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00490
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00498
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0049c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0xff800000,                                                  // -inf                                        /// 004b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00508
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00518
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0051c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00520
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00524
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00528
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0052c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00530
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0053c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00540
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00548
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0054c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00554
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00558
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0055c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00560
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00564
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00568
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0056c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00570
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00578
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00580
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00584
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00588
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00598
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0059c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x80000000,                                                  // -zero                                       /// 005d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00604
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0060c
			 0x7fc00001,                                                  // signaling NaN                               /// 00610
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00614
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00618
			 0x00011111,                                                  // 9.7958E-41                                  /// 0061c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00628
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00630
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0063c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00648
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00650
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00654
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00658
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0065c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00660
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00664
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00668
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0066c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00670
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00674
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0067c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00680
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00684
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00688
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0068c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00690
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00694
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0069c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e8
			 0x7fc00001,                                                  // signaling NaN                               /// 006ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00700
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00704
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00710
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80011111,                                                  // -9.7958E-41                                 /// 0071c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00720
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00724
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00728
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0072c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00730
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00734
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00738
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0073c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00744
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00748
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0074c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00750
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00754
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0075c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00764
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00768
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00774
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00780
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x7fc00001,                                                  // signaling NaN                               /// 00788
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0078c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00790
			 0x0e000007,                                                  // Trailing 1s:                                /// 00794
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0079c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007cc
			 0x33333333,                                                  // 4 random values                             /// 007d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00000000,                                                  // zero                                        /// 007fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00800
			 0x0c700000,                                                  // Leading 1s:                                 /// 00804
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00808
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00810
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0081c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00820
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00824
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00828
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0082c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00834
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0084c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00850
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0085c
			 0x55555555,                                                  // 4 random values                             /// 00860
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00868
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00874
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0087c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00880
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0088c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00890
			 0x0c400000,                                                  // Leading 1s:                                 /// 00894
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0089c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 008ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008bc
			 0x80000000,                                                  // -zero                                       /// 008c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00900
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00908
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0090c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00914
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0091c
			 0xbf028f5c,                                                  // -0.51                                       /// 00920
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00924
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00930
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00934
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00938
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0093c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00940
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00948
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0xff800000,                                                  // -inf                                        /// 00958
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00960
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00964
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00970
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00974
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00980
			 0x0e000003,                                                  // Trailing 1s:                                /// 00984
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0099c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009b8
			 0x3f028f5c,                                                  // 0.51                                        /// 009bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a00
			 0x55555555,                                                  // 4 random values                             /// 00a04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00000000,                                                  // zero                                        /// 00a1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00aac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ab8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00abc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ac4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00acc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ad4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00adc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ae0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ae8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00af8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00afc
			 0x80000000,                                                  // -zero                                       /// 00b00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b0c
			 0xff800000,                                                  // -inf                                        /// 00b10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b20
			 0x7f800000,                                                  // inf                                         /// 00b24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b28
			 0xcb000000,                                                  // -8388608.0                                  /// 00b2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b70
			 0xcb000000,                                                  // -8388608.0                                  /// 00b74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b9c
			 0x33333333,                                                  // 4 random values                             /// 00ba0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bbc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bcc
			 0x00000000,                                                  // zero                                        /// 00bd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bdc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00be8
			 0x00000000,                                                  // zero                                        /// 00bec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c1c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c30
			 0xbf028f5c,                                                  // -0.51                                       /// 00c34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c48
			 0xff800000,                                                  // -inf                                        /// 00c4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c8c
			 0x33333333,                                                  // 4 random values                             /// 00c90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c98
			 0xff800000,                                                  // -inf                                        /// 00c9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80000000,                                                  // -zero                                       /// 00cc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cdc
			 0x00000000,                                                  // zero                                        /// 00ce0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ce4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cf4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cfc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0xffc00001,                                                  // -signaling NaN                              /// 00d2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00000000,                                                  // zero                                        /// 00d38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xbf028f5c,                                                  // -0.51                                       /// 00d48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d64
			 0x80000000,                                                  // -zero                                       /// 00d68
			 0x3f028f5c,                                                  // 0.51                                        /// 00d6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d78
			 0x7fc00001,                                                  // signaling NaN                               /// 00d7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d9c
			 0xff800000,                                                  // -inf                                        /// 00da0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dcc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ddc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00de4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dfc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e14
			 0x33333333,                                                  // 4 random values                             /// 00e18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e24
			 0xffc00001,                                                  // -signaling NaN                              /// 00e28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e44
			 0xffc00001,                                                  // -signaling NaN                              /// 00e48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e50
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ecc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ed4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00edc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ee4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ef0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00efc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f20
			 0x4b000000,                                                  // 8388608.0                                   /// 00f24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f40
			 0xff800000,                                                  // -inf                                        /// 00f44
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f88
			 0x55555555,                                                  // 4 random values                             /// 00f8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fa8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00fc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fe4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x55555555                                                  // 4 random values                             /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x62fa518f, /// 0x0
			 0xa5747859, /// 0x4
			 0x079d59fc, /// 0x8
			 0xa1901f86, /// 0xc
			 0x3700eedf, /// 0x10
			 0x89b037a3, /// 0x14
			 0x63d572f8, /// 0x18
			 0x3bf377c4, /// 0x1c
			 0xf6877c52, /// 0x20
			 0x9ae7ed37, /// 0x24
			 0x0084c6bf, /// 0x28
			 0xc7486909, /// 0x2c
			 0x85ef08ca, /// 0x30
			 0x6f25b2b3, /// 0x34
			 0x8c0b91d8, /// 0x38
			 0xde595105, /// 0x3c
			 0x63e57eb0, /// 0x40
			 0x29d1ac45, /// 0x44
			 0xf321f8b0, /// 0x48
			 0x5e143a29, /// 0x4c
			 0x74c61bd0, /// 0x50
			 0x7d745a2a, /// 0x54
			 0xa3d13666, /// 0x58
			 0x84895a7e, /// 0x5c
			 0xdc43c187, /// 0x60
			 0x69fdace8, /// 0x64
			 0x72f5111b, /// 0x68
			 0xf481106a, /// 0x6c
			 0x7731a01b, /// 0x70
			 0x62219df8, /// 0x74
			 0x04b9584a, /// 0x78
			 0xcfee3d5e, /// 0x7c
			 0x67c56ebf, /// 0x80
			 0xd909d46a, /// 0x84
			 0x430c0e90, /// 0x88
			 0x7852e935, /// 0x8c
			 0xe6a58d8a, /// 0x90
			 0x92262447, /// 0x94
			 0xe84193bd, /// 0x98
			 0xd4c400b5, /// 0x9c
			 0x4d53309d, /// 0xa0
			 0x593152f9, /// 0xa4
			 0x89f58f3e, /// 0xa8
			 0x52d897d2, /// 0xac
			 0xc6f3e069, /// 0xb0
			 0x0a194205, /// 0xb4
			 0x66489675, /// 0xb8
			 0x46693243, /// 0xbc
			 0x178fc788, /// 0xc0
			 0xfd93f807, /// 0xc4
			 0x94168d7d, /// 0xc8
			 0x25a1bcc9, /// 0xcc
			 0x655cb464, /// 0xd0
			 0xe3b68fd4, /// 0xd4
			 0x03af5d51, /// 0xd8
			 0xc052ed73, /// 0xdc
			 0x618b6b86, /// 0xe0
			 0x4bcd0e6e, /// 0xe4
			 0x88c79910, /// 0xe8
			 0xe652b137, /// 0xec
			 0x0b5eb2fc, /// 0xf0
			 0xb74eb386, /// 0xf4
			 0xeb00d181, /// 0xf8
			 0xec30dd31, /// 0xfc
			 0x146d8039, /// 0x100
			 0xcad3b12c, /// 0x104
			 0x9c81a5f7, /// 0x108
			 0xb98ce72d, /// 0x10c
			 0x04af0a4c, /// 0x110
			 0x2813def7, /// 0x114
			 0x150d5acf, /// 0x118
			 0xc62bbcbd, /// 0x11c
			 0xf6fcdc1a, /// 0x120
			 0x70575df6, /// 0x124
			 0x0117532c, /// 0x128
			 0xd6a95571, /// 0x12c
			 0xb5f5994c, /// 0x130
			 0x9c2f1158, /// 0x134
			 0xf808f2f4, /// 0x138
			 0x8b1f0816, /// 0x13c
			 0x6b275fd6, /// 0x140
			 0x636be9ca, /// 0x144
			 0x35688fb4, /// 0x148
			 0x34892fd1, /// 0x14c
			 0xab8a73dc, /// 0x150
			 0xf7f1e30f, /// 0x154
			 0x1ccd0182, /// 0x158
			 0xd3035e66, /// 0x15c
			 0x388d2014, /// 0x160
			 0x5b0e4849, /// 0x164
			 0xcdec507f, /// 0x168
			 0xf80c4abf, /// 0x16c
			 0x658c1c3a, /// 0x170
			 0xb058fe79, /// 0x174
			 0xfc57ce2e, /// 0x178
			 0xa103a600, /// 0x17c
			 0x25cda9a3, /// 0x180
			 0xe632f0ec, /// 0x184
			 0x1456511e, /// 0x188
			 0x07d37283, /// 0x18c
			 0x370519ab, /// 0x190
			 0xb833fa20, /// 0x194
			 0xdce0f055, /// 0x198
			 0x6ffd2db1, /// 0x19c
			 0xbea2c825, /// 0x1a0
			 0x23814ec0, /// 0x1a4
			 0xeb832cdc, /// 0x1a8
			 0x12de037c, /// 0x1ac
			 0x7767c9c7, /// 0x1b0
			 0x23b8f9af, /// 0x1b4
			 0xe5b52f00, /// 0x1b8
			 0xf8a94d9a, /// 0x1bc
			 0xabd0e411, /// 0x1c0
			 0xef0a003c, /// 0x1c4
			 0xe45c437c, /// 0x1c8
			 0xcff856e7, /// 0x1cc
			 0x68e1375a, /// 0x1d0
			 0x109b647c, /// 0x1d4
			 0xc4579f97, /// 0x1d8
			 0xc7119977, /// 0x1dc
			 0x499c31dc, /// 0x1e0
			 0x30d32647, /// 0x1e4
			 0x15a9fe0b, /// 0x1e8
			 0xaa734b61, /// 0x1ec
			 0x02838f13, /// 0x1f0
			 0xa9fadf41, /// 0x1f4
			 0x19aee041, /// 0x1f8
			 0x1b458252, /// 0x1fc
			 0x5b4deb1f, /// 0x200
			 0x4d7337ea, /// 0x204
			 0x139063a9, /// 0x208
			 0x890ec014, /// 0x20c
			 0x482305ec, /// 0x210
			 0x0fbfc6e4, /// 0x214
			 0x6bfa3b51, /// 0x218
			 0x3d21d2a9, /// 0x21c
			 0x909263c7, /// 0x220
			 0x12a755a0, /// 0x224
			 0x6248ab00, /// 0x228
			 0x06b0a4cb, /// 0x22c
			 0xc0de21d0, /// 0x230
			 0xd5099ccb, /// 0x234
			 0xf124118f, /// 0x238
			 0x6dc4a168, /// 0x23c
			 0xb4fe228d, /// 0x240
			 0xafb76ea1, /// 0x244
			 0x6e656cfc, /// 0x248
			 0xb76fbcd3, /// 0x24c
			 0xc8835e0a, /// 0x250
			 0xcc347497, /// 0x254
			 0x8ea5b314, /// 0x258
			 0x6547d5b5, /// 0x25c
			 0xf57bdaf3, /// 0x260
			 0x5619e34d, /// 0x264
			 0xf4e2bc9b, /// 0x268
			 0xce5f6176, /// 0x26c
			 0x3038bfe2, /// 0x270
			 0x43a579ce, /// 0x274
			 0xde58cc05, /// 0x278
			 0xaf047106, /// 0x27c
			 0x18366913, /// 0x280
			 0x12e9bcad, /// 0x284
			 0xa999d0ae, /// 0x288
			 0x345f4ff2, /// 0x28c
			 0x6aa0b577, /// 0x290
			 0xb8974221, /// 0x294
			 0x7e3b0492, /// 0x298
			 0x16353e08, /// 0x29c
			 0x072c6ea3, /// 0x2a0
			 0x747b256e, /// 0x2a4
			 0xd0bb8d37, /// 0x2a8
			 0xbfb0e786, /// 0x2ac
			 0xaa96af5f, /// 0x2b0
			 0xadb05642, /// 0x2b4
			 0x223554b5, /// 0x2b8
			 0xe8fc549f, /// 0x2bc
			 0xe3ea6311, /// 0x2c0
			 0x5817746e, /// 0x2c4
			 0x0627ede4, /// 0x2c8
			 0x95e12dbe, /// 0x2cc
			 0x162e86fe, /// 0x2d0
			 0xd56e67fd, /// 0x2d4
			 0x51598482, /// 0x2d8
			 0xaab1ad56, /// 0x2dc
			 0x8a57ba84, /// 0x2e0
			 0x5169534b, /// 0x2e4
			 0x5ba8f415, /// 0x2e8
			 0x03211427, /// 0x2ec
			 0xf36f5ff2, /// 0x2f0
			 0xb9e4072f, /// 0x2f4
			 0x533d4f61, /// 0x2f8
			 0x42089d3d, /// 0x2fc
			 0x8356fe51, /// 0x300
			 0x61bd67cd, /// 0x304
			 0x6ee53c38, /// 0x308
			 0xbcfb5b7c, /// 0x30c
			 0x4464431d, /// 0x310
			 0x613cf338, /// 0x314
			 0xa16480b5, /// 0x318
			 0xf9f69802, /// 0x31c
			 0x6a90f54a, /// 0x320
			 0x57ed44dd, /// 0x324
			 0xa07857eb, /// 0x328
			 0xe8b757ed, /// 0x32c
			 0xa36cdb8a, /// 0x330
			 0xf42c75ba, /// 0x334
			 0x423a7fe5, /// 0x338
			 0x1f67b928, /// 0x33c
			 0x2a762d4b, /// 0x340
			 0xcba8bcdd, /// 0x344
			 0x79775029, /// 0x348
			 0x59326dca, /// 0x34c
			 0xb2db7bf8, /// 0x350
			 0xe270d427, /// 0x354
			 0xb4289e4f, /// 0x358
			 0x2d6c3cdf, /// 0x35c
			 0xf0960d64, /// 0x360
			 0x4b13bed3, /// 0x364
			 0xcea90857, /// 0x368
			 0xa3171b58, /// 0x36c
			 0x15d07d58, /// 0x370
			 0xbf87027c, /// 0x374
			 0x1249fec4, /// 0x378
			 0xfa53075b, /// 0x37c
			 0x9dc7af8b, /// 0x380
			 0xb5677ccb, /// 0x384
			 0x961b7fae, /// 0x388
			 0x137516bc, /// 0x38c
			 0x7e04e685, /// 0x390
			 0xf8380677, /// 0x394
			 0x0d495d8b, /// 0x398
			 0xbf2526f6, /// 0x39c
			 0xfed45ab0, /// 0x3a0
			 0x4ffc10a8, /// 0x3a4
			 0x74cdf379, /// 0x3a8
			 0xee2a79e5, /// 0x3ac
			 0xe313d24c, /// 0x3b0
			 0xefeadcb1, /// 0x3b4
			 0x5023431b, /// 0x3b8
			 0x1cc2da42, /// 0x3bc
			 0xe2f05dd1, /// 0x3c0
			 0x9177a859, /// 0x3c4
			 0x3391723a, /// 0x3c8
			 0x7ae9bdbf, /// 0x3cc
			 0xa11cc001, /// 0x3d0
			 0x37219c7f, /// 0x3d4
			 0x1127705e, /// 0x3d8
			 0xbb45ab66, /// 0x3dc
			 0x2c9e3b16, /// 0x3e0
			 0xc3d6bea8, /// 0x3e4
			 0x5a6de7ac, /// 0x3e8
			 0xc4bfda85, /// 0x3ec
			 0xe953e623, /// 0x3f0
			 0xb46020aa, /// 0x3f4
			 0xf557d637, /// 0x3f8
			 0x36df1640, /// 0x3fc
			 0x7b8f6aff, /// 0x400
			 0xdcc4e265, /// 0x404
			 0xe88bb8b7, /// 0x408
			 0x98e21d0c, /// 0x40c
			 0x4583d935, /// 0x410
			 0xdfc3ba8c, /// 0x414
			 0xf4ef7e44, /// 0x418
			 0xee9bf5e4, /// 0x41c
			 0x066eba38, /// 0x420
			 0xd2afc254, /// 0x424
			 0x6d4c43e6, /// 0x428
			 0x077d9a72, /// 0x42c
			 0xcc411b7d, /// 0x430
			 0x83df1400, /// 0x434
			 0xaf34479c, /// 0x438
			 0xc430cd77, /// 0x43c
			 0x33a1e6a3, /// 0x440
			 0x851d1984, /// 0x444
			 0x2f084f63, /// 0x448
			 0xd33f0c72, /// 0x44c
			 0xca74fa43, /// 0x450
			 0x75b9f990, /// 0x454
			 0x3bf6d158, /// 0x458
			 0x9fc1489e, /// 0x45c
			 0x2904d926, /// 0x460
			 0x7c325611, /// 0x464
			 0x228d8435, /// 0x468
			 0xf0122061, /// 0x46c
			 0x949135e7, /// 0x470
			 0x13fecba0, /// 0x474
			 0xeb6f73da, /// 0x478
			 0x44586aca, /// 0x47c
			 0xea24ed74, /// 0x480
			 0x40be1352, /// 0x484
			 0x538690b9, /// 0x488
			 0x586bdaf1, /// 0x48c
			 0x2ddee0b9, /// 0x490
			 0x15782f18, /// 0x494
			 0x926976f4, /// 0x498
			 0x239a11dd, /// 0x49c
			 0xaaaf319e, /// 0x4a0
			 0x334cfd54, /// 0x4a4
			 0xfc3fc199, /// 0x4a8
			 0xd584901d, /// 0x4ac
			 0xef69ead0, /// 0x4b0
			 0xacd8f0a1, /// 0x4b4
			 0x93b1e496, /// 0x4b8
			 0xc73d1a76, /// 0x4bc
			 0x1c963def, /// 0x4c0
			 0xe2f42a65, /// 0x4c4
			 0x257a8ec8, /// 0x4c8
			 0x84758714, /// 0x4cc
			 0x3802f31b, /// 0x4d0
			 0x235520a7, /// 0x4d4
			 0x301aee1e, /// 0x4d8
			 0xf23d4e87, /// 0x4dc
			 0xc3f35460, /// 0x4e0
			 0xb4612491, /// 0x4e4
			 0x4761461c, /// 0x4e8
			 0x23b226ad, /// 0x4ec
			 0x9d021e27, /// 0x4f0
			 0x8e076483, /// 0x4f4
			 0x648a9fd2, /// 0x4f8
			 0xb6fcc1ab, /// 0x4fc
			 0x62f9fb8d, /// 0x500
			 0x2ac5186b, /// 0x504
			 0x8aec8a59, /// 0x508
			 0xec4b9c99, /// 0x50c
			 0x8bfcfd88, /// 0x510
			 0xc6f2e514, /// 0x514
			 0xe5a3854d, /// 0x518
			 0xc69ad614, /// 0x51c
			 0xd60f9a46, /// 0x520
			 0x2be4285d, /// 0x524
			 0x52c94d4e, /// 0x528
			 0x891ed591, /// 0x52c
			 0x24c8d5f5, /// 0x530
			 0x6a54e71b, /// 0x534
			 0xdcc9e212, /// 0x538
			 0x26eb3cf5, /// 0x53c
			 0xd8651f28, /// 0x540
			 0x411c7fb2, /// 0x544
			 0x606f6c03, /// 0x548
			 0xd457c263, /// 0x54c
			 0xce765716, /// 0x550
			 0xa4da5040, /// 0x554
			 0x4b8bc617, /// 0x558
			 0xa8b14356, /// 0x55c
			 0xedc4dc74, /// 0x560
			 0xc377ab6d, /// 0x564
			 0xfb25d224, /// 0x568
			 0x0b3b8e96, /// 0x56c
			 0xa2167637, /// 0x570
			 0x917721d1, /// 0x574
			 0xd8a68bf7, /// 0x578
			 0x27d22379, /// 0x57c
			 0xf71c3f93, /// 0x580
			 0x5605b2d3, /// 0x584
			 0x6b26c6b4, /// 0x588
			 0x5b815b19, /// 0x58c
			 0x10c5207c, /// 0x590
			 0x84d0fe0e, /// 0x594
			 0xc0f88132, /// 0x598
			 0xd0c3454e, /// 0x59c
			 0xe7bdb4a1, /// 0x5a0
			 0xb7579e10, /// 0x5a4
			 0xfe1e8322, /// 0x5a8
			 0xce2f310c, /// 0x5ac
			 0xb13cff53, /// 0x5b0
			 0xf7077266, /// 0x5b4
			 0xa7edbd49, /// 0x5b8
			 0xf85e88c8, /// 0x5bc
			 0xa917d4a8, /// 0x5c0
			 0x3fe91e02, /// 0x5c4
			 0x4c38b82f, /// 0x5c8
			 0xc0a88b79, /// 0x5cc
			 0xbe55ae15, /// 0x5d0
			 0xf3283eb5, /// 0x5d4
			 0x51902171, /// 0x5d8
			 0xa35aa56a, /// 0x5dc
			 0xce080e6b, /// 0x5e0
			 0x11918cb1, /// 0x5e4
			 0x0d448ccb, /// 0x5e8
			 0x5d41516f, /// 0x5ec
			 0x3877ce2b, /// 0x5f0
			 0x168101a0, /// 0x5f4
			 0x53a8631a, /// 0x5f8
			 0x2443b07c, /// 0x5fc
			 0x57a39c27, /// 0x600
			 0x859591d9, /// 0x604
			 0xecd65928, /// 0x608
			 0xa458042b, /// 0x60c
			 0xa3300e0e, /// 0x610
			 0xb218f779, /// 0x614
			 0x4c276c56, /// 0x618
			 0x0c67f18d, /// 0x61c
			 0x8d9c9e6c, /// 0x620
			 0xdb17ebfc, /// 0x624
			 0x5afba59b, /// 0x628
			 0x332cc6b9, /// 0x62c
			 0x240e5a90, /// 0x630
			 0x45eed03c, /// 0x634
			 0x21906b6b, /// 0x638
			 0x8098ee51, /// 0x63c
			 0xafb4a788, /// 0x640
			 0x768ad545, /// 0x644
			 0xc94f4e24, /// 0x648
			 0x44476712, /// 0x64c
			 0xae91a3f9, /// 0x650
			 0x69e8e9b3, /// 0x654
			 0x5f1363d8, /// 0x658
			 0x7f15f754, /// 0x65c
			 0xa5d65350, /// 0x660
			 0x87b0ea79, /// 0x664
			 0xe534499a, /// 0x668
			 0x724164ed, /// 0x66c
			 0x5fd0a51b, /// 0x670
			 0xacef33a9, /// 0x674
			 0x8704fca6, /// 0x678
			 0x6e317af5, /// 0x67c
			 0xb129c07b, /// 0x680
			 0x49ec16e0, /// 0x684
			 0x212ab463, /// 0x688
			 0xe7e65a99, /// 0x68c
			 0xb26cfb3b, /// 0x690
			 0x53e0b36c, /// 0x694
			 0x3d24e4d2, /// 0x698
			 0x7fcb4599, /// 0x69c
			 0x4f3963c1, /// 0x6a0
			 0x4cf1ba3d, /// 0x6a4
			 0x33abfa0a, /// 0x6a8
			 0x7e4b8ed4, /// 0x6ac
			 0x358928fa, /// 0x6b0
			 0x82e3618a, /// 0x6b4
			 0x79dc4b58, /// 0x6b8
			 0xa1814edd, /// 0x6bc
			 0xae356564, /// 0x6c0
			 0xe8d3a431, /// 0x6c4
			 0x742a9585, /// 0x6c8
			 0xd86017be, /// 0x6cc
			 0xf9f0d598, /// 0x6d0
			 0xc4011c3e, /// 0x6d4
			 0x388afa7a, /// 0x6d8
			 0x125d466c, /// 0x6dc
			 0x38807ab5, /// 0x6e0
			 0xfa937398, /// 0x6e4
			 0x1b190631, /// 0x6e8
			 0x33c344d0, /// 0x6ec
			 0x33e69e18, /// 0x6f0
			 0x8b2a5dd4, /// 0x6f4
			 0x80231dbb, /// 0x6f8
			 0xc5b04c52, /// 0x6fc
			 0x913f989e, /// 0x700
			 0x8cfe5418, /// 0x704
			 0xc91a11c5, /// 0x708
			 0xbd37adcc, /// 0x70c
			 0x07311cd8, /// 0x710
			 0xd8851cdd, /// 0x714
			 0x4a95d224, /// 0x718
			 0x5b471362, /// 0x71c
			 0x473850d4, /// 0x720
			 0x3b161231, /// 0x724
			 0xb001f5e4, /// 0x728
			 0x2a2dd6e4, /// 0x72c
			 0x55c7c1d6, /// 0x730
			 0x83d4a5e0, /// 0x734
			 0x066b5f52, /// 0x738
			 0x8cc5f4ed, /// 0x73c
			 0xffc9825b, /// 0x740
			 0x1f25ad83, /// 0x744
			 0xb02dc9a0, /// 0x748
			 0x666e94bf, /// 0x74c
			 0x9fcf7f5c, /// 0x750
			 0x3ef5708d, /// 0x754
			 0x108c982e, /// 0x758
			 0xa73d8da2, /// 0x75c
			 0x50511007, /// 0x760
			 0xc5384be8, /// 0x764
			 0xd0f9da59, /// 0x768
			 0xe8f6826b, /// 0x76c
			 0x5fc436f0, /// 0x770
			 0x087c7c21, /// 0x774
			 0x13658dd4, /// 0x778
			 0x94c2a605, /// 0x77c
			 0x1e521ba9, /// 0x780
			 0xe5342fba, /// 0x784
			 0x817758fc, /// 0x788
			 0x9cb06aba, /// 0x78c
			 0xb0e66557, /// 0x790
			 0x9b49cbd8, /// 0x794
			 0x62938582, /// 0x798
			 0xd5e52d1e, /// 0x79c
			 0x795dc70f, /// 0x7a0
			 0x38209287, /// 0x7a4
			 0x2e38c654, /// 0x7a8
			 0x5cc1eb21, /// 0x7ac
			 0x3b317f41, /// 0x7b0
			 0xc7751526, /// 0x7b4
			 0x97b070e1, /// 0x7b8
			 0x2c3b3232, /// 0x7bc
			 0x6d031b6f, /// 0x7c0
			 0x8c86c7d9, /// 0x7c4
			 0x87181156, /// 0x7c8
			 0xcaf6ba62, /// 0x7cc
			 0x810ea789, /// 0x7d0
			 0x0a823d98, /// 0x7d4
			 0xdfdb076e, /// 0x7d8
			 0xc158a3d3, /// 0x7dc
			 0x0c598825, /// 0x7e0
			 0x592b0f35, /// 0x7e4
			 0x33ac4163, /// 0x7e8
			 0x5ab6846d, /// 0x7ec
			 0x4d758821, /// 0x7f0
			 0xdf2221ed, /// 0x7f4
			 0x23c2078d, /// 0x7f8
			 0xf5b93341, /// 0x7fc
			 0xe5068b7b, /// 0x800
			 0x6ba1919a, /// 0x804
			 0xbf142d78, /// 0x808
			 0x49b9440f, /// 0x80c
			 0x3239b6f3, /// 0x810
			 0x30b8aeec, /// 0x814
			 0xe6a2890b, /// 0x818
			 0x9bc4ec8d, /// 0x81c
			 0x4a4130eb, /// 0x820
			 0x895777de, /// 0x824
			 0x813a2f22, /// 0x828
			 0x24e852b6, /// 0x82c
			 0x6d1740c9, /// 0x830
			 0xbad2450d, /// 0x834
			 0x6be62747, /// 0x838
			 0xd2c76857, /// 0x83c
			 0x31808c23, /// 0x840
			 0x15b0314e, /// 0x844
			 0x6c39f458, /// 0x848
			 0xf34e4f35, /// 0x84c
			 0xcf4527f2, /// 0x850
			 0xdaac4474, /// 0x854
			 0x7548e363, /// 0x858
			 0xe38e1ff2, /// 0x85c
			 0xbde51e93, /// 0x860
			 0x47da1f61, /// 0x864
			 0x77d32135, /// 0x868
			 0x02767756, /// 0x86c
			 0x06815ae4, /// 0x870
			 0x353dfd4a, /// 0x874
			 0x3e5ceb26, /// 0x878
			 0x4159a3bc, /// 0x87c
			 0x075a3b55, /// 0x880
			 0x3515d626, /// 0x884
			 0x2e3731f8, /// 0x888
			 0x31773d55, /// 0x88c
			 0x99da9b72, /// 0x890
			 0xcec52291, /// 0x894
			 0x3dafbc83, /// 0x898
			 0x273039b7, /// 0x89c
			 0x4ac9fa46, /// 0x8a0
			 0x1f390561, /// 0x8a4
			 0x37d6e091, /// 0x8a8
			 0x4ee4d457, /// 0x8ac
			 0x4b2ab1d2, /// 0x8b0
			 0x601e78ee, /// 0x8b4
			 0x59be66f2, /// 0x8b8
			 0xf8d7752c, /// 0x8bc
			 0xb8521193, /// 0x8c0
			 0x1c4b8e0b, /// 0x8c4
			 0xd3fe1feb, /// 0x8c8
			 0xc75f513e, /// 0x8cc
			 0xee37a7f4, /// 0x8d0
			 0xeef3a59f, /// 0x8d4
			 0xb2a74f43, /// 0x8d8
			 0xf247dcc6, /// 0x8dc
			 0xf002d52c, /// 0x8e0
			 0x81958768, /// 0x8e4
			 0xc6389d09, /// 0x8e8
			 0x777ead9a, /// 0x8ec
			 0xeb58330d, /// 0x8f0
			 0x5aa1604e, /// 0x8f4
			 0xb9638568, /// 0x8f8
			 0x8cdb80fd, /// 0x8fc
			 0x49b786f6, /// 0x900
			 0x454c7728, /// 0x904
			 0x19ee01e9, /// 0x908
			 0x8784e795, /// 0x90c
			 0x348f86bb, /// 0x910
			 0x1cbc0475, /// 0x914
			 0xfa0bcd8f, /// 0x918
			 0x522df47d, /// 0x91c
			 0x06bdf698, /// 0x920
			 0xb07e2ec6, /// 0x924
			 0x1d9ff07d, /// 0x928
			 0x3071e26c, /// 0x92c
			 0x1d6dec6e, /// 0x930
			 0x3df4f661, /// 0x934
			 0x82ad633d, /// 0x938
			 0x4dede7b4, /// 0x93c
			 0x44b573b2, /// 0x940
			 0x702234b4, /// 0x944
			 0x2a71abf9, /// 0x948
			 0x6549ef28, /// 0x94c
			 0x7f346041, /// 0x950
			 0x64336901, /// 0x954
			 0xf18b6f6b, /// 0x958
			 0x17ddbcf2, /// 0x95c
			 0x693b58fb, /// 0x960
			 0x93e1dd32, /// 0x964
			 0x2ced876a, /// 0x968
			 0x379fb87f, /// 0x96c
			 0xf003e7be, /// 0x970
			 0x3ae4da0f, /// 0x974
			 0x1dc93198, /// 0x978
			 0xc7d030fe, /// 0x97c
			 0x6f7f3644, /// 0x980
			 0xdd35cedd, /// 0x984
			 0xbeaf97e0, /// 0x988
			 0xe56097ef, /// 0x98c
			 0x6bb48018, /// 0x990
			 0xcfb71ca9, /// 0x994
			 0xccef0c84, /// 0x998
			 0xa10bc2e6, /// 0x99c
			 0x72c443de, /// 0x9a0
			 0xa5073949, /// 0x9a4
			 0x83559494, /// 0x9a8
			 0xc34aff68, /// 0x9ac
			 0xab010820, /// 0x9b0
			 0x9ad07547, /// 0x9b4
			 0xf768f793, /// 0x9b8
			 0x34c80242, /// 0x9bc
			 0xb59a47e6, /// 0x9c0
			 0x5fcaeef5, /// 0x9c4
			 0x3ec63119, /// 0x9c8
			 0xcead35d8, /// 0x9cc
			 0x0b5769ce, /// 0x9d0
			 0x6fac3051, /// 0x9d4
			 0x0b2b7be0, /// 0x9d8
			 0xa1a13deb, /// 0x9dc
			 0xe87dad10, /// 0x9e0
			 0x273e5adf, /// 0x9e4
			 0x70df3d88, /// 0x9e8
			 0x9a9e01b3, /// 0x9ec
			 0x99481f1d, /// 0x9f0
			 0x198f010c, /// 0x9f4
			 0x41afa72d, /// 0x9f8
			 0x5c813e2a, /// 0x9fc
			 0x2683a551, /// 0xa00
			 0xb73bf70e, /// 0xa04
			 0xfd83ee82, /// 0xa08
			 0xe3350176, /// 0xa0c
			 0x0da7691a, /// 0xa10
			 0x6b255587, /// 0xa14
			 0x48a8d8b6, /// 0xa18
			 0x58d3b202, /// 0xa1c
			 0x81f9eeff, /// 0xa20
			 0xe2af41e5, /// 0xa24
			 0x418fecc2, /// 0xa28
			 0x99d0fa5c, /// 0xa2c
			 0xd85c6288, /// 0xa30
			 0xce92e8c6, /// 0xa34
			 0xb0da031c, /// 0xa38
			 0xa5ec1693, /// 0xa3c
			 0x3176c808, /// 0xa40
			 0x05f15ae1, /// 0xa44
			 0x13b3eb7a, /// 0xa48
			 0x16fa6ba5, /// 0xa4c
			 0xc16eae20, /// 0xa50
			 0xacea543c, /// 0xa54
			 0x069de7b2, /// 0xa58
			 0xada37b1a, /// 0xa5c
			 0x8a3bef8b, /// 0xa60
			 0x72632603, /// 0xa64
			 0x6a92dfc6, /// 0xa68
			 0x6bc4bca3, /// 0xa6c
			 0x10979cc3, /// 0xa70
			 0xe45c4f3b, /// 0xa74
			 0x5f4e7a97, /// 0xa78
			 0x2fa7e549, /// 0xa7c
			 0x87275fb3, /// 0xa80
			 0x449b69ab, /// 0xa84
			 0x2cff83d7, /// 0xa88
			 0x06f8e229, /// 0xa8c
			 0x12161800, /// 0xa90
			 0x7fa1a91a, /// 0xa94
			 0x8cc56cb9, /// 0xa98
			 0xa2b26683, /// 0xa9c
			 0x7868bfa2, /// 0xaa0
			 0x27e733b3, /// 0xaa4
			 0x40e54da5, /// 0xaa8
			 0xaa984c93, /// 0xaac
			 0x8ecd6895, /// 0xab0
			 0x1d74e9ba, /// 0xab4
			 0xadfdcbab, /// 0xab8
			 0xe9de1b37, /// 0xabc
			 0x9aa1ab45, /// 0xac0
			 0x98f92ff2, /// 0xac4
			 0xea14c7a8, /// 0xac8
			 0xe0e5a834, /// 0xacc
			 0x3f0196e2, /// 0xad0
			 0x7b26ab8d, /// 0xad4
			 0x4ff91b01, /// 0xad8
			 0x11eef3db, /// 0xadc
			 0xb3ed98d7, /// 0xae0
			 0x0eac3d44, /// 0xae4
			 0x1a86ec9d, /// 0xae8
			 0xbf64b87b, /// 0xaec
			 0x32941de1, /// 0xaf0
			 0xcee0d30f, /// 0xaf4
			 0x33ae47e8, /// 0xaf8
			 0xa45c9d45, /// 0xafc
			 0x5490576f, /// 0xb00
			 0x792a2ace, /// 0xb04
			 0x26065f1c, /// 0xb08
			 0x7303ae57, /// 0xb0c
			 0x92338e1f, /// 0xb10
			 0x14ede38b, /// 0xb14
			 0x1db96960, /// 0xb18
			 0xbb117e3b, /// 0xb1c
			 0xc914baae, /// 0xb20
			 0xa6688b10, /// 0xb24
			 0x50b5f8ca, /// 0xb28
			 0xd062cc53, /// 0xb2c
			 0x8f0afbf0, /// 0xb30
			 0x4ee684b8, /// 0xb34
			 0xf1a9c478, /// 0xb38
			 0xecf0d995, /// 0xb3c
			 0x7334eca0, /// 0xb40
			 0x6db490f8, /// 0xb44
			 0xb8ff52ff, /// 0xb48
			 0x509f307a, /// 0xb4c
			 0xb75e11ce, /// 0xb50
			 0x91f05feb, /// 0xb54
			 0x7946eea9, /// 0xb58
			 0xaec715ac, /// 0xb5c
			 0xa6243157, /// 0xb60
			 0x491e2fdd, /// 0xb64
			 0x84aefb27, /// 0xb68
			 0xfd555b05, /// 0xb6c
			 0xdf49d9e7, /// 0xb70
			 0x5c945f45, /// 0xb74
			 0x63a47333, /// 0xb78
			 0xbf334f9a, /// 0xb7c
			 0x464e658f, /// 0xb80
			 0x33eace6e, /// 0xb84
			 0xd55f87ff, /// 0xb88
			 0xb3145659, /// 0xb8c
			 0xc2b5a8b1, /// 0xb90
			 0x94ef4632, /// 0xb94
			 0xf1c2c4bc, /// 0xb98
			 0xfe189102, /// 0xb9c
			 0x463e255c, /// 0xba0
			 0xe259f4f3, /// 0xba4
			 0x3d5f40a9, /// 0xba8
			 0x477c2aea, /// 0xbac
			 0x4ab7e74c, /// 0xbb0
			 0x2edbcff7, /// 0xbb4
			 0xb3bb3b86, /// 0xbb8
			 0x579ea54b, /// 0xbbc
			 0xcc15fd45, /// 0xbc0
			 0x13dfc3b6, /// 0xbc4
			 0x2c7d7505, /// 0xbc8
			 0x0cec44af, /// 0xbcc
			 0x3b5bb88a, /// 0xbd0
			 0x60baf782, /// 0xbd4
			 0xc698d6a5, /// 0xbd8
			 0x1463c221, /// 0xbdc
			 0xb0a0a311, /// 0xbe0
			 0xde78aee0, /// 0xbe4
			 0x7c86bc4c, /// 0xbe8
			 0xc685cdd9, /// 0xbec
			 0x9545e9be, /// 0xbf0
			 0x0ff933b1, /// 0xbf4
			 0x222568c8, /// 0xbf8
			 0xa26ad627, /// 0xbfc
			 0x8d1786cf, /// 0xc00
			 0x95aea2cb, /// 0xc04
			 0x3ac5ebb4, /// 0xc08
			 0xb9178e5f, /// 0xc0c
			 0xb8df4d1c, /// 0xc10
			 0x5a455624, /// 0xc14
			 0x83fd18d3, /// 0xc18
			 0xe4024ae1, /// 0xc1c
			 0x7b50b28b, /// 0xc20
			 0xbff61c91, /// 0xc24
			 0x7882419d, /// 0xc28
			 0xe50e26c5, /// 0xc2c
			 0xf61922b8, /// 0xc30
			 0xd2ed923d, /// 0xc34
			 0xca8d5ab5, /// 0xc38
			 0x24f73724, /// 0xc3c
			 0xe23cd766, /// 0xc40
			 0xa955b3d3, /// 0xc44
			 0xfa2438ca, /// 0xc48
			 0x1325abdf, /// 0xc4c
			 0xcbe6a5c4, /// 0xc50
			 0xf5c2a1df, /// 0xc54
			 0x177af84f, /// 0xc58
			 0x63466fcb, /// 0xc5c
			 0xdb8f560d, /// 0xc60
			 0x9943c2bd, /// 0xc64
			 0x73d0c15b, /// 0xc68
			 0xe2602e32, /// 0xc6c
			 0x36c7cbb0, /// 0xc70
			 0x4b2770ac, /// 0xc74
			 0xc4319580, /// 0xc78
			 0x548b088f, /// 0xc7c
			 0x787d9ac9, /// 0xc80
			 0xcb891ace, /// 0xc84
			 0xd132f136, /// 0xc88
			 0x0171ce90, /// 0xc8c
			 0x296ff753, /// 0xc90
			 0x3ed04a94, /// 0xc94
			 0xc651f0ba, /// 0xc98
			 0x3378b409, /// 0xc9c
			 0x15c9680a, /// 0xca0
			 0xe4bc7732, /// 0xca4
			 0x413dad01, /// 0xca8
			 0xfd7796a2, /// 0xcac
			 0xca761b23, /// 0xcb0
			 0x1b26d89f, /// 0xcb4
			 0x6b4c352c, /// 0xcb8
			 0x245b31c9, /// 0xcbc
			 0xd2b5ab2c, /// 0xcc0
			 0x99f5da08, /// 0xcc4
			 0x2b97c4e3, /// 0xcc8
			 0x477fe5ee, /// 0xccc
			 0x7304c79d, /// 0xcd0
			 0x023ed1e8, /// 0xcd4
			 0xc93e2a1a, /// 0xcd8
			 0xb8072749, /// 0xcdc
			 0xaa8495f0, /// 0xce0
			 0x0e091a9c, /// 0xce4
			 0x953565ef, /// 0xce8
			 0x186ca282, /// 0xcec
			 0x2c14646a, /// 0xcf0
			 0x435cce19, /// 0xcf4
			 0xb9ba6e64, /// 0xcf8
			 0xd1a4f36a, /// 0xcfc
			 0x07593efc, /// 0xd00
			 0x10337895, /// 0xd04
			 0xf03e4a68, /// 0xd08
			 0xa3022f94, /// 0xd0c
			 0x7f954e75, /// 0xd10
			 0x952a29f9, /// 0xd14
			 0x7e4803b0, /// 0xd18
			 0x96bc1d2a, /// 0xd1c
			 0xbef13043, /// 0xd20
			 0x917791ea, /// 0xd24
			 0x3ae61063, /// 0xd28
			 0x68b57d36, /// 0xd2c
			 0x1f19f9ec, /// 0xd30
			 0xe5900050, /// 0xd34
			 0x830ed648, /// 0xd38
			 0x57be7d9d, /// 0xd3c
			 0xf7b5f60e, /// 0xd40
			 0xf1a2126d, /// 0xd44
			 0xb11d487d, /// 0xd48
			 0x44f4cc9b, /// 0xd4c
			 0x896f94cc, /// 0xd50
			 0x1387db67, /// 0xd54
			 0x2f27d23d, /// 0xd58
			 0xa6f76622, /// 0xd5c
			 0x28664ac6, /// 0xd60
			 0xfbbfb5a7, /// 0xd64
			 0x39f41ba3, /// 0xd68
			 0xebc2dfe3, /// 0xd6c
			 0x1110f524, /// 0xd70
			 0xa9930216, /// 0xd74
			 0xb69089cd, /// 0xd78
			 0xfcdb060d, /// 0xd7c
			 0x50b9ff51, /// 0xd80
			 0xcf556c3d, /// 0xd84
			 0x0c7c8111, /// 0xd88
			 0x508d9d3c, /// 0xd8c
			 0x5c9490f9, /// 0xd90
			 0x1276b31d, /// 0xd94
			 0xfd5a84cc, /// 0xd98
			 0x4fc46ebf, /// 0xd9c
			 0xf1a2cad5, /// 0xda0
			 0x8041018b, /// 0xda4
			 0xf280b148, /// 0xda8
			 0xa92a4792, /// 0xdac
			 0xfa2a2948, /// 0xdb0
			 0x32478501, /// 0xdb4
			 0x3930f3f1, /// 0xdb8
			 0xc3436d0a, /// 0xdbc
			 0x7b744e00, /// 0xdc0
			 0x15668413, /// 0xdc4
			 0x411dec13, /// 0xdc8
			 0xa2b898ba, /// 0xdcc
			 0xa9888efd, /// 0xdd0
			 0x54e063b8, /// 0xdd4
			 0xf1686b63, /// 0xdd8
			 0xaef9fbeb, /// 0xddc
			 0xaefa8ee1, /// 0xde0
			 0x38cda6be, /// 0xde4
			 0xe76cc841, /// 0xde8
			 0x5cc8f617, /// 0xdec
			 0x6408ac2d, /// 0xdf0
			 0x5d7e3ebb, /// 0xdf4
			 0x822a5ce6, /// 0xdf8
			 0x6dc79ef0, /// 0xdfc
			 0x72d8f45a, /// 0xe00
			 0xf751e27b, /// 0xe04
			 0x90c53c01, /// 0xe08
			 0x9576c92b, /// 0xe0c
			 0x2dd9ef0e, /// 0xe10
			 0x315ef90e, /// 0xe14
			 0x1cccd9df, /// 0xe18
			 0x06be40af, /// 0xe1c
			 0x68b82015, /// 0xe20
			 0x08e6937d, /// 0xe24
			 0xfa847283, /// 0xe28
			 0xdbb2da5a, /// 0xe2c
			 0xa2a4689a, /// 0xe30
			 0xb5f44f57, /// 0xe34
			 0xc4826b40, /// 0xe38
			 0xa18f36df, /// 0xe3c
			 0x90ec15a2, /// 0xe40
			 0x06589a4b, /// 0xe44
			 0xf148e81d, /// 0xe48
			 0xa457aae2, /// 0xe4c
			 0x5e7a5209, /// 0xe50
			 0xf138d720, /// 0xe54
			 0x9631d164, /// 0xe58
			 0xfb6c4d1b, /// 0xe5c
			 0xab7f916a, /// 0xe60
			 0x2c59ed6f, /// 0xe64
			 0xdbbd2284, /// 0xe68
			 0x85e03bdf, /// 0xe6c
			 0x36802907, /// 0xe70
			 0xef7c5dc2, /// 0xe74
			 0xed763522, /// 0xe78
			 0xb892c631, /// 0xe7c
			 0x773479e9, /// 0xe80
			 0x31a8fd06, /// 0xe84
			 0xe12645f2, /// 0xe88
			 0xa1ef0924, /// 0xe8c
			 0xe6ea7495, /// 0xe90
			 0x3da40407, /// 0xe94
			 0x8f743418, /// 0xe98
			 0xe5bd40a2, /// 0xe9c
			 0xf6a8ee70, /// 0xea0
			 0xb05e3150, /// 0xea4
			 0xf5d501ed, /// 0xea8
			 0xee2327bb, /// 0xeac
			 0x892ecd7b, /// 0xeb0
			 0x612fe7f0, /// 0xeb4
			 0x24aab54d, /// 0xeb8
			 0xbb68eda6, /// 0xebc
			 0xd27b83fa, /// 0xec0
			 0x01992826, /// 0xec4
			 0x95212b1a, /// 0xec8
			 0x8b1e7bb7, /// 0xecc
			 0x6f6161b9, /// 0xed0
			 0xb70b5bc9, /// 0xed4
			 0x0ab29c16, /// 0xed8
			 0x75c30537, /// 0xedc
			 0xdf9f07c4, /// 0xee0
			 0xf796cd39, /// 0xee4
			 0x236b2e15, /// 0xee8
			 0x7a56a8d2, /// 0xeec
			 0xed5cb928, /// 0xef0
			 0x018b5c7f, /// 0xef4
			 0xa8869d75, /// 0xef8
			 0x4b717b5a, /// 0xefc
			 0x80c3626b, /// 0xf00
			 0xf57d63ee, /// 0xf04
			 0x4cce5c74, /// 0xf08
			 0x5019669a, /// 0xf0c
			 0x2508d42f, /// 0xf10
			 0x781fc3a5, /// 0xf14
			 0x6353f3be, /// 0xf18
			 0x2d43188e, /// 0xf1c
			 0x7506c553, /// 0xf20
			 0xe146e575, /// 0xf24
			 0xd297c4dd, /// 0xf28
			 0x28bf9386, /// 0xf2c
			 0x4e7db8cf, /// 0xf30
			 0xf7ed28c8, /// 0xf34
			 0x536125b9, /// 0xf38
			 0x7c6208fb, /// 0xf3c
			 0x20bb5b83, /// 0xf40
			 0x8963742d, /// 0xf44
			 0xdec27860, /// 0xf48
			 0xbe09a282, /// 0xf4c
			 0x38d99369, /// 0xf50
			 0x92edde6e, /// 0xf54
			 0x6a030d43, /// 0xf58
			 0xd187c623, /// 0xf5c
			 0x9701b250, /// 0xf60
			 0x89b7fcea, /// 0xf64
			 0xcbb9b0c4, /// 0xf68
			 0xf3a14c89, /// 0xf6c
			 0xad6011ff, /// 0xf70
			 0x646a3f1e, /// 0xf74
			 0x0e1e4a97, /// 0xf78
			 0x51f588ef, /// 0xf7c
			 0xa91cebc9, /// 0xf80
			 0x71e8287a, /// 0xf84
			 0xd214bb77, /// 0xf88
			 0x97e05364, /// 0xf8c
			 0x33a61896, /// 0xf90
			 0x381bf1d5, /// 0xf94
			 0x1973d310, /// 0xf98
			 0x0b4d084e, /// 0xf9c
			 0xf98972d4, /// 0xfa0
			 0xddc2fd45, /// 0xfa4
			 0x882ea32c, /// 0xfa8
			 0x770d4751, /// 0xfac
			 0x2128dac4, /// 0xfb0
			 0x46a5784a, /// 0xfb4
			 0x0d57bb6f, /// 0xfb8
			 0xce006db9, /// 0xfbc
			 0x7fc0aac4, /// 0xfc0
			 0xf2595c3b, /// 0xfc4
			 0xee29a69b, /// 0xfc8
			 0xc364a6b6, /// 0xfcc
			 0x60d8d695, /// 0xfd0
			 0xec7d2b07, /// 0xfd4
			 0x85952cfb, /// 0xfd8
			 0x3bffb3f2, /// 0xfdc
			 0xc5aa8628, /// 0xfe0
			 0xa8547a7b, /// 0xfe4
			 0xc990872c, /// 0xfe8
			 0xb9209a48, /// 0xfec
			 0x9ed1c97c, /// 0xff0
			 0xfb59c005, /// 0xff4
			 0x657223c0, /// 0xff8
			 0x987fa9a5 /// last
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
			 0x000006a8,
			 0x00000754,
			 0x000004b0,
			 0x00000640,
			 0x000001d8,
			 0x0000070c,
			 0x000006ac,
			 0x0000063c,

			 /// vpu register f2
			 0x41300000,
			 0x41e80000,
			 0x41e80000,
			 0x40400000,
			 0x41400000,
			 0x41880000,
			 0x40800000,
			 0x41d80000,

			 /// vpu register f3
			 0x41c00000,
			 0x41500000,
			 0x42000000,
			 0x3f800000,
			 0x40800000,
			 0x41e00000,
			 0x40e00000,
			 0x41000000,

			 /// vpu register f4
			 0x41000000,
			 0x41e80000,
			 0x41c80000,
			 0x40400000,
			 0x40c00000,
			 0x41000000,
			 0x41900000,
			 0x41900000,

			 /// vpu register f5
			 0x41600000,
			 0x41500000,
			 0x41700000,
			 0x3f800000,
			 0x41980000,
			 0x41d00000,
			 0x41e00000,
			 0x41f00000,

			 /// vpu register f6
			 0x41200000,
			 0x41b80000,
			 0x41e80000,
			 0x41800000,
			 0x41c80000,
			 0x40e00000,
			 0x40400000,
			 0x41f00000,

			 /// vpu register f7
			 0x41f00000,
			 0x41800000,
			 0x41f00000,
			 0x40000000,
			 0x41200000,
			 0x42000000,
			 0x41100000,
			 0x41a00000,

			 /// vpu register f8
			 0x40800000,
			 0x41c80000,
			 0x41980000,
			 0x41b00000,
			 0x41500000,
			 0x41b80000,
			 0x41f80000,
			 0x40c00000,

			 /// vpu register f9
			 0x41d00000,
			 0x41a00000,
			 0x41880000,
			 0x41900000,
			 0x41f00000,
			 0x41b00000,
			 0x41880000,
			 0x41f80000,

			 /// vpu register f10
			 0x41e00000,
			 0x41100000,
			 0x41d00000,
			 0x41f80000,
			 0x41000000,
			 0x41e00000,
			 0x3f800000,
			 0x41a80000,

			 /// vpu register f11
			 0x41b00000,
			 0x40000000,
			 0x41600000,
			 0x3f800000,
			 0x41f80000,
			 0x41c00000,
			 0x41b80000,
			 0x41600000,

			 /// vpu register f12
			 0x40800000,
			 0x41b00000,
			 0x41880000,
			 0x41a00000,
			 0x41f00000,
			 0x41f80000,
			 0x41500000,
			 0x41b00000,

			 /// vpu register f13
			 0x41700000,
			 0x41980000,
			 0x41880000,
			 0x41200000,
			 0x41a80000,
			 0x41900000,
			 0x41900000,
			 0x41f00000,

			 /// vpu register f14
			 0x41e00000,
			 0x41600000,
			 0x40400000,
			 0x41880000,
			 0x41880000,
			 0x41f80000,
			 0x41f80000,
			 0x41e80000,

			 /// vpu register f15
			 0x40400000,
			 0x41f00000,
			 0x41f00000,
			 0x41f80000,
			 0x41800000,
			 0x41400000,
			 0x41e00000,
			 0x41900000,

			 /// vpu register f16
			 0x40000000,
			 0x41a80000,
			 0x41b00000,
			 0x40800000,
			 0x41700000,
			 0x40e00000,
			 0x41500000,
			 0x40e00000,

			 /// vpu register f17
			 0x41000000,
			 0x41c00000,
			 0x41100000,
			 0x3f800000,
			 0x41e00000,
			 0x41200000,
			 0x41100000,
			 0x41880000,

			 /// vpu register f18
			 0x41b80000,
			 0x40e00000,
			 0x41a00000,
			 0x41c00000,
			 0x41e00000,
			 0x40800000,
			 0x42000000,
			 0x40000000,

			 /// vpu register f19
			 0x40800000,
			 0x41880000,
			 0x3f800000,
			 0x3f800000,
			 0x3f800000,
			 0x41500000,
			 0x41c80000,
			 0x41c00000,

			 /// vpu register f20
			 0x41f00000,
			 0x41d80000,
			 0x41d80000,
			 0x41f00000,
			 0x3f800000,
			 0x40800000,
			 0x41880000,
			 0x41400000,

			 /// vpu register f21
			 0x40c00000,
			 0x41d00000,
			 0x41e00000,
			 0x41400000,
			 0x41900000,
			 0x41880000,
			 0x40000000,
			 0x42000000,

			 /// vpu register f22
			 0x41600000,
			 0x41e80000,
			 0x40e00000,
			 0x41d00000,
			 0x41b00000,
			 0x41400000,
			 0x41b00000,
			 0x40400000,

			 /// vpu register f23
			 0x41980000,
			 0x41980000,
			 0x41b80000,
			 0x41400000,
			 0x41200000,
			 0x40800000,
			 0x41c00000,
			 0x41880000,

			 /// vpu register f24
			 0x40000000,
			 0x41c00000,
			 0x3f800000,
			 0x3f800000,
			 0x41c00000,
			 0x40e00000,
			 0x41200000,
			 0x40e00000,

			 /// vpu register f25
			 0x41600000,
			 0x42000000,
			 0x40400000,
			 0x41b00000,
			 0x3f800000,
			 0x41100000,
			 0x41f80000,
			 0x41880000,

			 /// vpu register f26
			 0x41b80000,
			 0x41400000,
			 0x41e00000,
			 0x41880000,
			 0x41e80000,
			 0x40a00000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f27
			 0x41100000,
			 0x40400000,
			 0x41200000,
			 0x41800000,
			 0x41a00000,
			 0x41300000,
			 0x40c00000,
			 0x41500000,

			 /// vpu register f28
			 0x41d80000,
			 0x41900000,
			 0x41000000,
			 0x41b80000,
			 0x41100000,
			 0x41d80000,
			 0x41400000,
			 0x40e00000,

			 /// vpu register f29
			 0x41b00000,
			 0x41a80000,
			 0x41f80000,
			 0x41a80000,
			 0x40a00000,
			 0x41d80000,
			 0x41900000,
			 0x41d00000,

			 /// vpu register f30
			 0x41000000,
			 0x41f00000,
			 0x40e00000,
			 0x41f00000,
			 0x41000000,
			 0x41c80000,
			 0x41200000,
			 0x40e00000,

			 /// vpu register f31
			 0x40c00000,
			 0x41400000,
			 0x41f00000,
			 0x41880000,
			 0x41c80000,
			 0x41400000,
			 0x41d80000,
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
		"fandi.pi f19, f10, 419\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f28, f29, 48\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f24, f26, 442\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f18, f19, 483\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f19, f1, 272\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f24, f7, 142\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f29, 100\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f26, f6, 445\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f9, f12, 307\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f9, f10, 288\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f2, f8, 456\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f13, f6, 208\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f3, f27, 195\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f18, f28, 496\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f4, f16, 473\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f16, f20, 458\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f19, f4, 448\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f5, f27, 371\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f27, f16, 120\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f8, f10, 347\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f29, f10, 274\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f8, f13, 29\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f25, 338\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f19, f13, 407\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f6, f12, 74\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f26, f25, 227\n"                            ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f30, f24, 18\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f12, 280\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f13, f5, 287\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f7, f29, 31\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f23, f26, 83\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f10, 102\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f23, f0, 172\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f27, f2, 20\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f21, f23, 92\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f22, f21, 1\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f30, f22, 226\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f5, f2, 459\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f16, f4, 163\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f14, f9, 284\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f23, f3, 433\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f7, f22, 332\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f3, f17, 241\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f23, f13, 133\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f28, f26, 292\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f15, f6, 411\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f23, f15, 7\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f12, f14, 336\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f4, f11, 37\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f16, f25, 425\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f21, f21, 17\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f29, f30, 290\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f9, 99\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f14, f28, 388\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f21, f14, 371\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f7, f18, 451\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f16, f9, 496\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f6, f12, 412\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f21, f8, 418\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f9, f6, 381\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f13, f24, 499\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f19, f1, 501\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f22, f18, 230\n"                            ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f9, f0, 171\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f22, f21, 145\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f16, 413\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f19, f13, 99\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f7, f30, 451\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f24, f28, 244\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f24, f11, 232\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f16, f22, 249\n"                            ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f9, f0, 382\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f14, f10, 397\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f30, 423\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f16, f25, 13\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f3, f24, 475\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f4, f0, 183\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f22, f19, 429\n"                            ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f27, f15, 366\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f5, 344\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f25, f25, 484\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f14, f16, 489\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f17, f5, 12\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f21, f8, 238\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f17, f10, 451\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f26, 201\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f3, f10, 257\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f11, f8, 182\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f17, f10, 220\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f22, f29, 170\n"                            ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f15, f6, 181\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f30, f30, 240\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f16, f16, 386\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f4, f28, 46\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f24, f21, 379\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f13, f30, 165\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f24, f14, 79\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f19, f1, 403\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fandi.pi f19, f21, 101\n"                            ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
