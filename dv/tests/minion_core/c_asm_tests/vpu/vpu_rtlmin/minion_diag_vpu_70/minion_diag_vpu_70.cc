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
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00000
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00004
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x00000000,                                                  // zero                                        /// 00010
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00018
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0001c
			 0xbf028f5c,                                                  // -0.51                                       /// 00020
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00024
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0002c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00034
			 0xffc00001,                                                  // -signaling NaN                              /// 00038
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00040
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00044
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00048
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0004c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00050
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00054
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00058
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0005c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00060
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00064
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00070
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00074
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00084
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00088
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0008c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00090
			 0x0e000003,                                                  // Trailing 1s:                                /// 00094
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00098
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00100
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0010c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00114
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00118
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0011c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00120
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00128
			 0xffc00001,                                                  // -signaling NaN                              /// 0012c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00130
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00134
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0013c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00140
			 0x55555555,                                                  // 4 random values                             /// 00144
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00148
			 0x80011111,                                                  // -9.7958E-41                                 /// 0014c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0015c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0016c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00174
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00180
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00188
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00190
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00194
			 0x0e000003,                                                  // Trailing 1s:                                /// 00198
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0019c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0xcb000000,                                                  // -8388608.0                                  /// 001a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001b0
			 0x33333333,                                                  // 4 random values                             /// 001b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001c0
			 0x7f800000,                                                  // inf                                         /// 001c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00200
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00204
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00208
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00218
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0021c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00228
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00230
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x7f800000,                                                  // inf                                         /// 00238
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0xcb000000,                                                  // -8388608.0                                  /// 00240
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00244
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00248
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00250
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00254
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00260
			 0x0c700000,                                                  // Leading 1s:                                 /// 00264
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00268
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0026c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00274
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00278
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00280
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00288
			 0x80000000,                                                  // -zero                                       /// 0028c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00290
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00298
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00314
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00324
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00328
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0032c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00330
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0033c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00340
			 0x3f028f5c,                                                  // 0.51                                        /// 00344
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00348
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00354
			 0x80011111,                                                  // -9.7958E-41                                 /// 00358
			 0xff800000,                                                  // -inf                                        /// 0035c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00360
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00368
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0036c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x3f028f5c,                                                  // 0.51                                        /// 00374
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00380
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00388
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00394
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0039c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0xff800000,                                                  // -inf                                        /// 003a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003c8
			 0x7f800000,                                                  // inf                                         /// 003cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003e0
			 0x80000000,                                                  // -zero                                       /// 003e4
			 0x33333333,                                                  // 4 random values                             /// 003e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00400
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00404
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00408
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0040c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0041c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00428
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0042c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00438
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0043c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00440
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xff800000,                                                  // -inf                                        /// 00448
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00450
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00454
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00458
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00460
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00464
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00468
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00478
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00494
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0xcb000000,                                                  // -8388608.0                                  /// 0049c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c4
			 0x7f800000,                                                  // inf                                         /// 004c8
			 0x3f028f5c,                                                  // 0.51                                        /// 004cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80000000,                                                  // -zero                                       /// 004e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00508
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0050c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0c600000,                                                  // Leading 1s:                                 /// 00514
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00518
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0051c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00524
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00528
			 0xff800000,                                                  // -inf                                        /// 0052c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000000,                                                  // zero                                        /// 00538
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0053c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00540
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0054c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00550
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00554
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00558
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0055c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00568
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00574
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00580
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00584
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0058c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00590
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00594
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0059c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00600
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00608
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0060c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00610
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00614
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0061c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00620
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00624
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00628
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0062c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00630
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00638
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0063c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00640
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00644
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00648
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0064c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00650
			 0xff800000,                                                  // -inf                                        /// 00654
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00658
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0065c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00664
			 0xff800000,                                                  // -inf                                        /// 00668
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00674
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0067c
			 0x00000000,                                                  // zero                                        /// 00680
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00688
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00690
			 0x7fc00001,                                                  // signaling NaN                               /// 00694
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00698
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0069c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006cc
			 0x7f800000,                                                  // inf                                         /// 006d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00700
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00704
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00710
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00718
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0071c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00720
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00724
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0072c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00730
			 0x0c780000,                                                  // Leading 1s:                                 /// 00734
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00738
			 0xffc00001,                                                  // -signaling NaN                              /// 0073c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00748
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00750
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00754
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0075c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00760
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00764
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00768
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0076c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00770
			 0x00000000,                                                  // zero                                        /// 00774
			 0x4b000000,                                                  // 8388608.0                                   /// 00778
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0077c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00784
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00788
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0078c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00794
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00798
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0079c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00800
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00804
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00814
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00820
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00828
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0082c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00830
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00834
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00838
			 0x0c600000,                                                  // Leading 1s:                                 /// 0083c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00840
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0084c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00858
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00860
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00868
			 0x80000000,                                                  // -zero                                       /// 0086c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00870
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00874
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00880
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00884
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x0e000003,                                                  // Trailing 1s:                                /// 0088c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00890
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00894
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008cc
			 0xffc00001,                                                  // -signaling NaN                              /// 008d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008dc
			 0xffc00001,                                                  // -signaling NaN                              /// 008e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x7f800000,                                                  // inf                                         /// 008f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00900
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00011111,                                                  // 9.7958E-41                                  /// 00908
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0090c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00910
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00918
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00924
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00928
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00930
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00934
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00938
			 0xbf028f5c,                                                  // -0.51                                       /// 0093c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00940
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00944
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00950
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00954
			 0x7fc00001,                                                  // signaling NaN                               /// 00958
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0095c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0096c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00970
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00974
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00978
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0097c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00984
			 0x00011111,                                                  // 9.7958E-41                                  /// 00988
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00990
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00998
			 0x3f028f5c,                                                  // 0.51                                        /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 009b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x55555555,                                                  // 4 random values                             /// 009b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009cc
			 0x80000000,                                                  // -zero                                       /// 009d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a30
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a44
			 0x80000000,                                                  // -zero                                       /// 00a48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a64
			 0xcb000000,                                                  // -8388608.0                                  /// 00a68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a88
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00aa8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab8
			 0x33333333,                                                  // 4 random values                             /// 00abc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ac0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ac4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00acc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ad0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ad8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ae4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00af4
			 0xffc00001,                                                  // -signaling NaN                              /// 00af8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b14
			 0x00000000,                                                  // zero                                        /// 00b18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x55555555,                                                  // 4 random values                             /// 00b9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ba0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bdc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bfc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c04
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c58
			 0xff800000,                                                  // -inf                                        /// 00c5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cbc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ccc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cdc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ce4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf0
			 0x00000000,                                                  // zero                                        /// 00cf4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d04
			 0xff800000,                                                  // -inf                                        /// 00d08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d10
			 0x7f800000,                                                  // inf                                         /// 00d14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00da8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00db0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00db8
			 0xffc00001,                                                  // -signaling NaN                              /// 00dbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dcc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ddc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x33333333,                                                  // 4 random values                             /// 00e0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ecc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ee4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00eec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ef0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ef4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ef8
			 0xff800000,                                                  // -inf                                        /// 00efc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x55555555,                                                  // 4 random values                             /// 00f80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f94
			 0x7f800000,                                                  // inf                                         /// 00f98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x7f800000,                                                  // inf                                         /// 00fbc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fc8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fd0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fdc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fe0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fe4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ff4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff8
			 0x80200000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00000
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00008
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00014
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0001c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00024
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0002c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00034
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00038
			 0x0c600000,                                                  // Leading 1s:                                 /// 0003c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00040
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00048
			 0x0e000003,                                                  // Trailing 1s:                                /// 0004c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00054
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00058
			 0x80011111,                                                  // -9.7958E-41                                 /// 0005c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00060
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00064
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00068
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0006c
			 0x4b000000,                                                  // 8388608.0                                   /// 00070
			 0x7f800000,                                                  // inf                                         /// 00074
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0007c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00080
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00084
			 0x00011111,                                                  // 9.7958E-41                                  /// 00088
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0008c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00090
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00094
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00098
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0009c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e4
			 0xbf028f5c,                                                  // -0.51                                       /// 000e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f4
			 0xff800000,                                                  // -inf                                        /// 000f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00100
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00108
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0010c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00110
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00114
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00118
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0011c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00120
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00124
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0012c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00130
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00134
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00138
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0013c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00140
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00144
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00148
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0014c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00150
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00011111,                                                  // 9.7958E-41                                  /// 00158
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00160
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00164
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0016c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00170
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00178
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0017c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00180
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0018c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00190
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00194
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00198
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0019c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001b4
			 0x7fc00001,                                                  // signaling NaN                               /// 001b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f4
			 0x3f028f5c,                                                  // 0.51                                        /// 001f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00200
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00208
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0020c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00210
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00214
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00218
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0021c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00220
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00224
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0022c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00230
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00234
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0023c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00248
			 0x0e000001,                                                  // Trailing 1s:                                /// 0024c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00254
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00258
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0025c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00260
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00264
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00268
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00274
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00278
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00280
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00288
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0028c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00290
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0029c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d8
			 0x7f800000,                                                  // inf                                         /// 002dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00011111,                                                  // 9.7958E-41                                  /// 00308
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0030c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00310
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00314
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00318
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0031c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00320
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00324
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00330
			 0x0e000003,                                                  // Trailing 1s:                                /// 00334
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00338
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0034c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00350
			 0x3f028f5c,                                                  // 0.51                                        /// 00354
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00358
			 0x0e000007,                                                  // Trailing 1s:                                /// 0035c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00360
			 0x55555555,                                                  // 4 random values                             /// 00364
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00368
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00370
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00374
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00378
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0037c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00380
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0038c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00390
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00394
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00400
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00408
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0040c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00410
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0c780000,                                                  // Leading 1s:                                 /// 00418
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0041c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00420
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00424
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00428
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0042c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00430
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00434
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0043c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00440
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00444
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0044c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00454
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00458
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00460
			 0x0c600000,                                                  // Leading 1s:                                 /// 00464
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0046c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00470
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00474
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00478
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0047c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00484
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00488
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0049c
			 0x0e000001,                                                  // Trailing 1s:                                /// 004a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x7f800000,                                                  // inf                                         /// 004e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00500
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00508
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0050c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00514
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00518
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00520
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e000007,                                                  // Trailing 1s:                                /// 00528
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00534
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00540
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00548
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0054c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00550
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00554
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00558
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0055c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00560
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00568
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00570
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00574
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00580
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00588
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00000000,                                                  // zero                                        /// 00594
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00598
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0059c
			 0x4b000000,                                                  // 8388608.0                                   /// 005a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c0
			 0x4b000000,                                                  // 8388608.0                                   /// 005c4
			 0xffc00001,                                                  // -signaling NaN                              /// 005c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00600
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00604
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00608
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0060c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00610
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00618
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0061c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00620
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00624
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0062c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00630
			 0x0e000007,                                                  // Trailing 1s:                                /// 00634
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00638
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0063c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00640
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00644
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00648
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0064c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00650
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00658
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0065c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00660
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00664
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00668
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0066c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0e000007,                                                  // Trailing 1s:                                /// 00678
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0068c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00698
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0069c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0xff800000,                                                  // -inf                                        /// 006e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00700
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00704
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00708
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0070c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00710
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00714
			 0x0c600000,                                                  // Leading 1s:                                 /// 00718
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0071c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00720
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00724
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00738
			 0xff800000,                                                  // -inf                                        /// 0073c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00740
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00744
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00748
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0074c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0075c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00760
			 0x55555555,                                                  // 4 random values                             /// 00764
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00768
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0076c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00770
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0077c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00784
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0xbf028f5c,                                                  // -0.51                                       /// 00790
			 0x0e000001,                                                  // Trailing 1s:                                /// 00794
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00798
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c4
			 0xffc00001,                                                  // -signaling NaN                              /// 007c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007d8
			 0x7fc00001,                                                  // signaling NaN                               /// 007dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007f4
			 0x55555555,                                                  // 4 random values                             /// 007f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00800
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00804
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0080c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00814
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00818
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00820
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00824
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00828
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0082c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00830
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00834
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00838
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00840
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00844
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00848
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0084c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00858
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0085c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00864
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00870
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0087c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00880
			 0x80000000,                                                  // -zero                                       /// 00884
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0088c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00894
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00904
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00908
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00910
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00914
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00918
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0091c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00924
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00928
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0092c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00934
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00938
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0093c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x80011111,                                                  // -9.7958E-41                                 /// 00944
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00948
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0094c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0095c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00960
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00964
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0096c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00970
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00974
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00978
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0097c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00980
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00984
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00988
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0098c
			 0xcb000000,                                                  // -8388608.0                                  /// 00990
			 0x00000000,                                                  // zero                                        /// 00994
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x7fc00001,                                                  // signaling NaN                               /// 009b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b4
			 0x3f028f5c,                                                  // 0.51                                        /// 009b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009c4
			 0x00000000,                                                  // zero                                        /// 009c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009e4
			 0x3f028f5c,                                                  // 0.51                                        /// 009e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a00
			 0x3f028f5c,                                                  // 0.51                                        /// 00a04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a28
			 0xcb000000,                                                  // -8388608.0                                  /// 00a2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a34
			 0x4b000000,                                                  // 8388608.0                                   /// 00a38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a84
			 0xcb000000,                                                  // -8388608.0                                  /// 00a88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00aa0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aa4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00abc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x00000000,                                                  // zero                                        /// 00ac4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00acc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x3f028f5c,                                                  // 0.51                                        /// 00adc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00af8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00afc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b34
			 0xff800000,                                                  // -inf                                        /// 00b38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b60
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b70
			 0x33333333,                                                  // 4 random values                             /// 00b74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bbc
			 0x33333333,                                                  // 4 random values                             /// 00bc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00be0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00be8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bec
			 0x55555555,                                                  // 4 random values                             /// 00bf0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c14
			 0x3f028f5c,                                                  // 0.51                                        /// 00c18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x00000000,                                                  // zero                                        /// 00c30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ca4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cc4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ccc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cdc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ce0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ce4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cf0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d08
			 0xff800000,                                                  // -inf                                        /// 00d0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x7f800000,                                                  // inf                                         /// 00d18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x7fc00001,                                                  // signaling NaN                               /// 00d64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00da0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00da4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00db4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dbc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dc8
			 0xff800000,                                                  // -inf                                        /// 00dcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dd4
			 0x80000000,                                                  // -zero                                       /// 00dd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ddc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e00
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x7fc00001,                                                  // signaling NaN                               /// 00e1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ea8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ebc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00edc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ef4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f64
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0xff800000,                                                  // -inf                                        /// 00fa4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fa8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fe0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ff8
			 0x0e000007                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x533bf122, /// 0x0
			 0x610d7879, /// 0x4
			 0xff3bb6cd, /// 0x8
			 0xb84ac16b, /// 0xc
			 0x7230295f, /// 0x10
			 0x3cd41df6, /// 0x14
			 0x003776bc, /// 0x18
			 0xcabeac41, /// 0x1c
			 0xab4dd779, /// 0x20
			 0x014bb408, /// 0x24
			 0x5d851edf, /// 0x28
			 0x92bd6a5e, /// 0x2c
			 0xc1a6bd97, /// 0x30
			 0xd91aa47f, /// 0x34
			 0x5358d936, /// 0x38
			 0xa6438581, /// 0x3c
			 0x5dfa37c5, /// 0x40
			 0xa2e74825, /// 0x44
			 0x52cc1b7c, /// 0x48
			 0x66f0c312, /// 0x4c
			 0xa0ec40c3, /// 0x50
			 0x469854bf, /// 0x54
			 0x00c021d1, /// 0x58
			 0x4455604d, /// 0x5c
			 0x9f3875fa, /// 0x60
			 0x2e5cecd7, /// 0x64
			 0xb0410708, /// 0x68
			 0x049f1b44, /// 0x6c
			 0x7c50fb97, /// 0x70
			 0x64409bbc, /// 0x74
			 0x28a6fb11, /// 0x78
			 0x2f5e6896, /// 0x7c
			 0x68116e4f, /// 0x80
			 0x9d3e665d, /// 0x84
			 0x4e569a4c, /// 0x88
			 0x315168a8, /// 0x8c
			 0x9c31fc23, /// 0x90
			 0x7d0a6591, /// 0x94
			 0x819d472f, /// 0x98
			 0x2528ee7b, /// 0x9c
			 0x1ab7df87, /// 0xa0
			 0xec6d72ab, /// 0xa4
			 0x1617001b, /// 0xa8
			 0x3ad49d70, /// 0xac
			 0x91ba7232, /// 0xb0
			 0x1427ee89, /// 0xb4
			 0xd2c12a84, /// 0xb8
			 0x8861f453, /// 0xbc
			 0x9cde4a60, /// 0xc0
			 0xb0901b6f, /// 0xc4
			 0x0cc01b1b, /// 0xc8
			 0x657dcdb9, /// 0xcc
			 0x41aad9f3, /// 0xd0
			 0xbf25be8b, /// 0xd4
			 0xff7f0ea8, /// 0xd8
			 0xb12e5c71, /// 0xdc
			 0x48ca2b47, /// 0xe0
			 0xfa47e308, /// 0xe4
			 0x05e71b7f, /// 0xe8
			 0x514a45ab, /// 0xec
			 0x93ef9695, /// 0xf0
			 0x97195c61, /// 0xf4
			 0xd50864e3, /// 0xf8
			 0x3d7ceede, /// 0xfc
			 0xce87dcda, /// 0x100
			 0x99f2aa22, /// 0x104
			 0x2b4d68df, /// 0x108
			 0x1f50e2fa, /// 0x10c
			 0x50841ece, /// 0x110
			 0x18b1f40b, /// 0x114
			 0x490f53e4, /// 0x118
			 0x11c0fec9, /// 0x11c
			 0xc6e53ad1, /// 0x120
			 0xc87a2f75, /// 0x124
			 0x84b2bcdf, /// 0x128
			 0x97cf0579, /// 0x12c
			 0x7e34ee81, /// 0x130
			 0x409d05d4, /// 0x134
			 0x7b15ff87, /// 0x138
			 0x36f5c6b7, /// 0x13c
			 0xa4dd294e, /// 0x140
			 0x370bf5e0, /// 0x144
			 0xaf1703af, /// 0x148
			 0x391839dc, /// 0x14c
			 0x4e44002d, /// 0x150
			 0x2c3ca8b6, /// 0x154
			 0x39b0134b, /// 0x158
			 0x734c066c, /// 0x15c
			 0xf09a7b24, /// 0x160
			 0x860ec4e9, /// 0x164
			 0xad4696ca, /// 0x168
			 0x9491b5b2, /// 0x16c
			 0x637526a2, /// 0x170
			 0x84b38421, /// 0x174
			 0x2bce118f, /// 0x178
			 0xe400e391, /// 0x17c
			 0xe69bde35, /// 0x180
			 0x217a83f4, /// 0x184
			 0x33ebb974, /// 0x188
			 0x02f2dd55, /// 0x18c
			 0x0cb0655a, /// 0x190
			 0xeb1eb110, /// 0x194
			 0x1cb81831, /// 0x198
			 0x537f6f05, /// 0x19c
			 0x42323f20, /// 0x1a0
			 0x5d2ddf46, /// 0x1a4
			 0xc654b98c, /// 0x1a8
			 0x3e411387, /// 0x1ac
			 0xa06685eb, /// 0x1b0
			 0x0550ad90, /// 0x1b4
			 0xa733acbb, /// 0x1b8
			 0xdd33c551, /// 0x1bc
			 0x73f2c74a, /// 0x1c0
			 0xc1a05600, /// 0x1c4
			 0x8ad2911f, /// 0x1c8
			 0xc2433aa9, /// 0x1cc
			 0xeeed146d, /// 0x1d0
			 0x50760580, /// 0x1d4
			 0x30c850a7, /// 0x1d8
			 0xa0237b5e, /// 0x1dc
			 0xd5f7d791, /// 0x1e0
			 0x6a4c05d2, /// 0x1e4
			 0xd2ab5e6b, /// 0x1e8
			 0xebcdf006, /// 0x1ec
			 0x83280a42, /// 0x1f0
			 0xdfdf9def, /// 0x1f4
			 0x390daf7f, /// 0x1f8
			 0xf82d2219, /// 0x1fc
			 0x49dba021, /// 0x200
			 0x1106fca0, /// 0x204
			 0x4495234f, /// 0x208
			 0xd6433834, /// 0x20c
			 0x38d0129c, /// 0x210
			 0x78ad4c57, /// 0x214
			 0x5c6331a6, /// 0x218
			 0x80da05a4, /// 0x21c
			 0x8b4169d0, /// 0x220
			 0x7c4f55ce, /// 0x224
			 0x03336344, /// 0x228
			 0x5613b5ad, /// 0x22c
			 0x1126605b, /// 0x230
			 0x0b0d7f59, /// 0x234
			 0x2fa9147f, /// 0x238
			 0x2294b6cd, /// 0x23c
			 0x298b2dfd, /// 0x240
			 0x7b9a087c, /// 0x244
			 0x9748f60d, /// 0x248
			 0x74085f53, /// 0x24c
			 0x25f486d1, /// 0x250
			 0x7cb6d24e, /// 0x254
			 0xd7a48cef, /// 0x258
			 0x6271d4cd, /// 0x25c
			 0x7364d26d, /// 0x260
			 0x05dd677c, /// 0x264
			 0x705911bc, /// 0x268
			 0x2c15557c, /// 0x26c
			 0x802f3e33, /// 0x270
			 0x367f3620, /// 0x274
			 0xcad4c804, /// 0x278
			 0x2bed1882, /// 0x27c
			 0xfc43fea8, /// 0x280
			 0x492fe41d, /// 0x284
			 0xddfb2b06, /// 0x288
			 0xa46c96d6, /// 0x28c
			 0xd3849eff, /// 0x290
			 0x0822d0d8, /// 0x294
			 0xf4ec73d4, /// 0x298
			 0x2284578f, /// 0x29c
			 0x7e8e52c8, /// 0x2a0
			 0x94a3c5d5, /// 0x2a4
			 0xe0492138, /// 0x2a8
			 0x50241d21, /// 0x2ac
			 0x9338cdde, /// 0x2b0
			 0x5f8bd696, /// 0x2b4
			 0x9539a91f, /// 0x2b8
			 0x75610785, /// 0x2bc
			 0xb4c0de43, /// 0x2c0
			 0x5ac02ad2, /// 0x2c4
			 0x90cdae5b, /// 0x2c8
			 0x35e57832, /// 0x2cc
			 0xc9e12c29, /// 0x2d0
			 0x147b5f78, /// 0x2d4
			 0x3ae3a683, /// 0x2d8
			 0x3ce2916a, /// 0x2dc
			 0xe1bb085b, /// 0x2e0
			 0xd632fc26, /// 0x2e4
			 0xe9815715, /// 0x2e8
			 0xf6eb8180, /// 0x2ec
			 0xe02bce12, /// 0x2f0
			 0x0cced813, /// 0x2f4
			 0x4a640084, /// 0x2f8
			 0x6ebaa285, /// 0x2fc
			 0x8c98a822, /// 0x300
			 0x55e4bd13, /// 0x304
			 0x70c7d464, /// 0x308
			 0xedbae147, /// 0x30c
			 0xa96d7fcf, /// 0x310
			 0xd5ab7a9a, /// 0x314
			 0x88adff4b, /// 0x318
			 0x2d3ba723, /// 0x31c
			 0x4df40594, /// 0x320
			 0xbb282fa1, /// 0x324
			 0x57adde67, /// 0x328
			 0xb95bc964, /// 0x32c
			 0xdba7feea, /// 0x330
			 0xdaa22057, /// 0x334
			 0xbea8afd8, /// 0x338
			 0x06bd233d, /// 0x33c
			 0xfe119b21, /// 0x340
			 0x7e30c3c7, /// 0x344
			 0x40900020, /// 0x348
			 0x8b4cc287, /// 0x34c
			 0xb75e02a1, /// 0x350
			 0x530d143f, /// 0x354
			 0xc5612b9b, /// 0x358
			 0x8f8779ca, /// 0x35c
			 0xb930fb0c, /// 0x360
			 0x843e159d, /// 0x364
			 0x7cdf850f, /// 0x368
			 0x0ac38bbc, /// 0x36c
			 0x22f121bb, /// 0x370
			 0x7fd17ef8, /// 0x374
			 0x639539a9, /// 0x378
			 0xcbfef3b4, /// 0x37c
			 0xcf31a70d, /// 0x380
			 0xadad0f59, /// 0x384
			 0xea454af0, /// 0x388
			 0xd7adfddf, /// 0x38c
			 0xc364185d, /// 0x390
			 0x99fd3c03, /// 0x394
			 0x8fbcdc8e, /// 0x398
			 0xfb1e2eda, /// 0x39c
			 0x1e11a72b, /// 0x3a0
			 0x6028f81d, /// 0x3a4
			 0xca13862a, /// 0x3a8
			 0x6efa9505, /// 0x3ac
			 0x69d58a82, /// 0x3b0
			 0xa8c4e3a0, /// 0x3b4
			 0x79cf3023, /// 0x3b8
			 0xb4045366, /// 0x3bc
			 0x2a778e6c, /// 0x3c0
			 0x347f86cd, /// 0x3c4
			 0xbdaf4969, /// 0x3c8
			 0xd67fafc6, /// 0x3cc
			 0x937772f8, /// 0x3d0
			 0x0d39176a, /// 0x3d4
			 0xc34d7e6e, /// 0x3d8
			 0xf6eb4b29, /// 0x3dc
			 0x5d95e3da, /// 0x3e0
			 0x379bd2d2, /// 0x3e4
			 0xe39f0a78, /// 0x3e8
			 0xb6724c0a, /// 0x3ec
			 0xb2bb6168, /// 0x3f0
			 0x3561e007, /// 0x3f4
			 0x84a63072, /// 0x3f8
			 0xbecb5809, /// 0x3fc
			 0x1c754e16, /// 0x400
			 0x8c11b37f, /// 0x404
			 0xa6262fe4, /// 0x408
			 0xb82d61c6, /// 0x40c
			 0x85dbf8ef, /// 0x410
			 0xbaca71b1, /// 0x414
			 0xbc709325, /// 0x418
			 0xb49c07f6, /// 0x41c
			 0xe6c4593f, /// 0x420
			 0x300a8e20, /// 0x424
			 0x7d336c3a, /// 0x428
			 0xe9764381, /// 0x42c
			 0xaef5c732, /// 0x430
			 0x70b3fd22, /// 0x434
			 0x71bd86ea, /// 0x438
			 0xe2d40192, /// 0x43c
			 0x15f6f529, /// 0x440
			 0x419f97f2, /// 0x444
			 0x122e69dc, /// 0x448
			 0xba3dd8e6, /// 0x44c
			 0x17d3df5d, /// 0x450
			 0x80971d9e, /// 0x454
			 0x9905ed4a, /// 0x458
			 0x47dacd59, /// 0x45c
			 0xbbff355a, /// 0x460
			 0x521cc2b6, /// 0x464
			 0x838edd45, /// 0x468
			 0x29ad43e8, /// 0x46c
			 0xd02298ee, /// 0x470
			 0xd9cee19f, /// 0x474
			 0x54a9bed6, /// 0x478
			 0x8da986ee, /// 0x47c
			 0x5c0132c0, /// 0x480
			 0x4c8e2b12, /// 0x484
			 0x433a3b7c, /// 0x488
			 0x5d5d7f18, /// 0x48c
			 0x391a2f69, /// 0x490
			 0x018f5ae7, /// 0x494
			 0x0f43aa9f, /// 0x498
			 0x75ac0d2e, /// 0x49c
			 0x93295350, /// 0x4a0
			 0x527b74e9, /// 0x4a4
			 0x346b0681, /// 0x4a8
			 0x16f9db4e, /// 0x4ac
			 0xae6a05bd, /// 0x4b0
			 0xd25f43f3, /// 0x4b4
			 0xe383796b, /// 0x4b8
			 0xda46dbb6, /// 0x4bc
			 0x8102d11a, /// 0x4c0
			 0xb7afd98e, /// 0x4c4
			 0x295fda6c, /// 0x4c8
			 0x0614c59a, /// 0x4cc
			 0xb59e8e8e, /// 0x4d0
			 0x8fc7ccfc, /// 0x4d4
			 0xf2ba1408, /// 0x4d8
			 0xc7b7e267, /// 0x4dc
			 0x78c28ca8, /// 0x4e0
			 0xf11bb907, /// 0x4e4
			 0xb0f5b13e, /// 0x4e8
			 0xf492757d, /// 0x4ec
			 0xb48d56d7, /// 0x4f0
			 0xa835e643, /// 0x4f4
			 0xb36c6d02, /// 0x4f8
			 0xc6bd205c, /// 0x4fc
			 0x29cb97e9, /// 0x500
			 0xfdfe55ca, /// 0x504
			 0xb31c992f, /// 0x508
			 0xa12030a4, /// 0x50c
			 0x6dbababf, /// 0x510
			 0xfdb7d008, /// 0x514
			 0x7ef1b68e, /// 0x518
			 0x2a0f00e7, /// 0x51c
			 0x68a8c013, /// 0x520
			 0xdc2d532b, /// 0x524
			 0xd0f62a67, /// 0x528
			 0x44311eac, /// 0x52c
			 0xbab69500, /// 0x530
			 0xf4a4e0ca, /// 0x534
			 0xeec2239b, /// 0x538
			 0x6c26ed10, /// 0x53c
			 0x7abf3aaa, /// 0x540
			 0x376538fe, /// 0x544
			 0xadba6361, /// 0x548
			 0x9fcd05fe, /// 0x54c
			 0xaf1252e8, /// 0x550
			 0xb2afed1d, /// 0x554
			 0xb35539f0, /// 0x558
			 0x5e8215c6, /// 0x55c
			 0x703bf0df, /// 0x560
			 0xf43670bc, /// 0x564
			 0xe0896b86, /// 0x568
			 0xa7a21054, /// 0x56c
			 0x6f3bf5f2, /// 0x570
			 0x4c814ff9, /// 0x574
			 0x50c3686d, /// 0x578
			 0x88fe36b0, /// 0x57c
			 0x18964d27, /// 0x580
			 0x591d499e, /// 0x584
			 0x9d7f68a9, /// 0x588
			 0x5c6732c8, /// 0x58c
			 0xf133f026, /// 0x590
			 0xbdcd091b, /// 0x594
			 0x63c58b88, /// 0x598
			 0x6b051033, /// 0x59c
			 0x7217c734, /// 0x5a0
			 0x3e8d7f00, /// 0x5a4
			 0xf8d311c6, /// 0x5a8
			 0x21d54f3f, /// 0x5ac
			 0xf31953b8, /// 0x5b0
			 0x3383d3fc, /// 0x5b4
			 0x50221e84, /// 0x5b8
			 0xe0960f94, /// 0x5bc
			 0x4674d3bd, /// 0x5c0
			 0xe4e2745c, /// 0x5c4
			 0x78aa3e02, /// 0x5c8
			 0x66208610, /// 0x5cc
			 0x2878f1da, /// 0x5d0
			 0x818d7fe3, /// 0x5d4
			 0x27b56c31, /// 0x5d8
			 0x2ab7427a, /// 0x5dc
			 0xefe5a7ad, /// 0x5e0
			 0x4ea2c8dc, /// 0x5e4
			 0x8c5be456, /// 0x5e8
			 0x10b9c323, /// 0x5ec
			 0xce5b434d, /// 0x5f0
			 0x739dbd30, /// 0x5f4
			 0xa4876401, /// 0x5f8
			 0x35dd9776, /// 0x5fc
			 0x9c22a59f, /// 0x600
			 0x8770598c, /// 0x604
			 0xf329c404, /// 0x608
			 0x47dac949, /// 0x60c
			 0x7d52d5a5, /// 0x610
			 0xf14c95c7, /// 0x614
			 0x1dcc9a85, /// 0x618
			 0x4e175cb2, /// 0x61c
			 0x0716a70d, /// 0x620
			 0x72ce0a06, /// 0x624
			 0xcb98c630, /// 0x628
			 0x1ecce151, /// 0x62c
			 0xff00c4dc, /// 0x630
			 0x7b253ed8, /// 0x634
			 0x19ce771c, /// 0x638
			 0xd00c2ac9, /// 0x63c
			 0x1c355417, /// 0x640
			 0x3cfa7711, /// 0x644
			 0x3f64d580, /// 0x648
			 0x5952824f, /// 0x64c
			 0x6bb77237, /// 0x650
			 0xa9a0ab8c, /// 0x654
			 0xd59e0e25, /// 0x658
			 0xac39dda2, /// 0x65c
			 0x981cc254, /// 0x660
			 0x4498e5d0, /// 0x664
			 0x03354026, /// 0x668
			 0x1a2afb8a, /// 0x66c
			 0xde8319b8, /// 0x670
			 0x7c2873c4, /// 0x674
			 0x56676c98, /// 0x678
			 0x2dc6b046, /// 0x67c
			 0xb3ad1a43, /// 0x680
			 0x5c951298, /// 0x684
			 0x7cac5970, /// 0x688
			 0xa423084e, /// 0x68c
			 0x8b23420a, /// 0x690
			 0xc53557ef, /// 0x694
			 0xa762ea71, /// 0x698
			 0x22cd2b50, /// 0x69c
			 0x998c659e, /// 0x6a0
			 0x3be99ab5, /// 0x6a4
			 0x09ac9931, /// 0x6a8
			 0x0958ebf3, /// 0x6ac
			 0xab8af352, /// 0x6b0
			 0x8c2a1308, /// 0x6b4
			 0x3e55bbba, /// 0x6b8
			 0xf710af2f, /// 0x6bc
			 0xec167134, /// 0x6c0
			 0x8a6e8e66, /// 0x6c4
			 0x5af9724e, /// 0x6c8
			 0x9731e27f, /// 0x6cc
			 0x9786f874, /// 0x6d0
			 0xf6ba24f4, /// 0x6d4
			 0x42f488f6, /// 0x6d8
			 0xb9553f3b, /// 0x6dc
			 0xf37e5871, /// 0x6e0
			 0x578aafdf, /// 0x6e4
			 0x700568a5, /// 0x6e8
			 0xf9a0f5d4, /// 0x6ec
			 0xd9806d65, /// 0x6f0
			 0x777a3adf, /// 0x6f4
			 0xbb435ed0, /// 0x6f8
			 0xdb3afc51, /// 0x6fc
			 0xcb43047b, /// 0x700
			 0x7899284a, /// 0x704
			 0x847c2136, /// 0x708
			 0x16641dd0, /// 0x70c
			 0x05594307, /// 0x710
			 0x50155a9c, /// 0x714
			 0x0fa6dc2f, /// 0x718
			 0x7fc68732, /// 0x71c
			 0xdd8a0b9a, /// 0x720
			 0x99c46f76, /// 0x724
			 0x5176c202, /// 0x728
			 0x8db3a192, /// 0x72c
			 0x463b7d87, /// 0x730
			 0xcff73375, /// 0x734
			 0xde5c1e22, /// 0x738
			 0x4bd6bc49, /// 0x73c
			 0xcdeac960, /// 0x740
			 0x6ae67cf6, /// 0x744
			 0xad31e963, /// 0x748
			 0xca7d22d2, /// 0x74c
			 0x9605f8ac, /// 0x750
			 0x6c763584, /// 0x754
			 0x08740ecb, /// 0x758
			 0x0ce3b385, /// 0x75c
			 0x8bab8f17, /// 0x760
			 0x64b7a44d, /// 0x764
			 0x3eecdb7c, /// 0x768
			 0xccf462e5, /// 0x76c
			 0x2eecd35f, /// 0x770
			 0xc8688341, /// 0x774
			 0x48464a1c, /// 0x778
			 0x3176502e, /// 0x77c
			 0xbbb17f1c, /// 0x780
			 0xb3ffbe83, /// 0x784
			 0xcb364ca3, /// 0x788
			 0xde621426, /// 0x78c
			 0xeaa655a8, /// 0x790
			 0x313bf83b, /// 0x794
			 0xa43df649, /// 0x798
			 0x52d0f617, /// 0x79c
			 0x746e432f, /// 0x7a0
			 0xf7aee9f5, /// 0x7a4
			 0x5a4540d3, /// 0x7a8
			 0x669fce6b, /// 0x7ac
			 0xb744e6ea, /// 0x7b0
			 0xcedad8a0, /// 0x7b4
			 0xa47f6eb8, /// 0x7b8
			 0xe951e20e, /// 0x7bc
			 0x3a37cb55, /// 0x7c0
			 0xc7558e47, /// 0x7c4
			 0x36af2135, /// 0x7c8
			 0x079748cc, /// 0x7cc
			 0x2deded60, /// 0x7d0
			 0x23335dd3, /// 0x7d4
			 0x0ff110cb, /// 0x7d8
			 0x88207e95, /// 0x7dc
			 0x2845e38f, /// 0x7e0
			 0xe987201b, /// 0x7e4
			 0x61955b0d, /// 0x7e8
			 0xf8df2d6a, /// 0x7ec
			 0x86312f00, /// 0x7f0
			 0x3f02cd9b, /// 0x7f4
			 0x8ab78024, /// 0x7f8
			 0xd650d28b, /// 0x7fc
			 0xe4cba8fe, /// 0x800
			 0xcd96d71d, /// 0x804
			 0x388d0c7f, /// 0x808
			 0x17b96836, /// 0x80c
			 0x48a646f6, /// 0x810
			 0x322df022, /// 0x814
			 0x49000851, /// 0x818
			 0xec26a368, /// 0x81c
			 0x28d0f367, /// 0x820
			 0x80cc92e5, /// 0x824
			 0xf02f374c, /// 0x828
			 0xd5bc4ca2, /// 0x82c
			 0x70b3d5f2, /// 0x830
			 0x206edb51, /// 0x834
			 0xc33df7f3, /// 0x838
			 0x1e71e197, /// 0x83c
			 0x44706e9f, /// 0x840
			 0x2006dc7a, /// 0x844
			 0x39907ef0, /// 0x848
			 0xea463b2e, /// 0x84c
			 0x82553b57, /// 0x850
			 0x9419386b, /// 0x854
			 0x005fb5fa, /// 0x858
			 0x6a69a74f, /// 0x85c
			 0x4690c17f, /// 0x860
			 0x74e76dfe, /// 0x864
			 0xc3a43fca, /// 0x868
			 0x0fdcb7fe, /// 0x86c
			 0x72179d14, /// 0x870
			 0x918aa4e3, /// 0x874
			 0x39de8dce, /// 0x878
			 0x9142e902, /// 0x87c
			 0xb15bab92, /// 0x880
			 0xb6a0186a, /// 0x884
			 0x022b7f1a, /// 0x888
			 0xd6a55253, /// 0x88c
			 0x8e4bf1c6, /// 0x890
			 0x580a0d32, /// 0x894
			 0x83fa73e2, /// 0x898
			 0xf65388b0, /// 0x89c
			 0x0244d06b, /// 0x8a0
			 0x7f3bab84, /// 0x8a4
			 0x6baa100c, /// 0x8a8
			 0xd8b3a102, /// 0x8ac
			 0x07cea58c, /// 0x8b0
			 0xd29863dc, /// 0x8b4
			 0x3c2f3764, /// 0x8b8
			 0x341e5212, /// 0x8bc
			 0xa2a54112, /// 0x8c0
			 0x8e8811e6, /// 0x8c4
			 0x31941473, /// 0x8c8
			 0xb83e09a0, /// 0x8cc
			 0x8c2db812, /// 0x8d0
			 0xad5e8e4d, /// 0x8d4
			 0x14b9cee3, /// 0x8d8
			 0xd40aab04, /// 0x8dc
			 0x25c14bec, /// 0x8e0
			 0x4f0389b9, /// 0x8e4
			 0xd93a7bb9, /// 0x8e8
			 0xaa4dbd7f, /// 0x8ec
			 0x2fca7189, /// 0x8f0
			 0x3e4454c2, /// 0x8f4
			 0x7454ba33, /// 0x8f8
			 0x6b96918c, /// 0x8fc
			 0xf65c33b7, /// 0x900
			 0x2256c15c, /// 0x904
			 0xe42c505b, /// 0x908
			 0x60d97362, /// 0x90c
			 0x264f91cc, /// 0x910
			 0x9aa1a635, /// 0x914
			 0x42fd4a63, /// 0x918
			 0x06501c0f, /// 0x91c
			 0x344434ee, /// 0x920
			 0x1ec0b6af, /// 0x924
			 0x6bec4073, /// 0x928
			 0x362c1750, /// 0x92c
			 0x232b3291, /// 0x930
			 0x6d2cbd83, /// 0x934
			 0x39e55ff3, /// 0x938
			 0xa207c7cf, /// 0x93c
			 0x3df8db8c, /// 0x940
			 0x23604848, /// 0x944
			 0x813081fe, /// 0x948
			 0xb1bbcb7e, /// 0x94c
			 0x39780fa8, /// 0x950
			 0x58ff310e, /// 0x954
			 0x94640795, /// 0x958
			 0x4e4f97ab, /// 0x95c
			 0x442fc9d9, /// 0x960
			 0x05342642, /// 0x964
			 0xeda27d21, /// 0x968
			 0xa0727940, /// 0x96c
			 0xbe968555, /// 0x970
			 0x055eb8e8, /// 0x974
			 0xb4261c88, /// 0x978
			 0x064b239d, /// 0x97c
			 0xabb1178f, /// 0x980
			 0x07c7501f, /// 0x984
			 0xa543d2ee, /// 0x988
			 0xcf950c9d, /// 0x98c
			 0x3b4702fe, /// 0x990
			 0x9ce431d1, /// 0x994
			 0x19165916, /// 0x998
			 0x8459194e, /// 0x99c
			 0x6caa9ca4, /// 0x9a0
			 0x8b936e2e, /// 0x9a4
			 0x52a4ad1a, /// 0x9a8
			 0x4c575abe, /// 0x9ac
			 0xb03e111f, /// 0x9b0
			 0x5f0904c8, /// 0x9b4
			 0x727ee588, /// 0x9b8
			 0xa5774596, /// 0x9bc
			 0x5fa0cf23, /// 0x9c0
			 0xb05995dc, /// 0x9c4
			 0x8b559f6b, /// 0x9c8
			 0x2a3bbc29, /// 0x9cc
			 0xbfccdb48, /// 0x9d0
			 0x235a9ef8, /// 0x9d4
			 0xfc5d8763, /// 0x9d8
			 0x42b9d456, /// 0x9dc
			 0x692bd309, /// 0x9e0
			 0x3aa5b165, /// 0x9e4
			 0xfec26fd0, /// 0x9e8
			 0xae370a95, /// 0x9ec
			 0x93df8744, /// 0x9f0
			 0x061a0f8d, /// 0x9f4
			 0x4eb73837, /// 0x9f8
			 0x3f88fbab, /// 0x9fc
			 0x06c3319a, /// 0xa00
			 0x5e72ff90, /// 0xa04
			 0x5312e278, /// 0xa08
			 0x84de08f0, /// 0xa0c
			 0x40a6ec25, /// 0xa10
			 0xa0af8800, /// 0xa14
			 0xb4952570, /// 0xa18
			 0x884d37ba, /// 0xa1c
			 0x2d056541, /// 0xa20
			 0x5fb52cbf, /// 0xa24
			 0x11284755, /// 0xa28
			 0x4c9eb29e, /// 0xa2c
			 0x8dcbe85a, /// 0xa30
			 0xfb83416a, /// 0xa34
			 0x4304053c, /// 0xa38
			 0x25515cf0, /// 0xa3c
			 0x39096f04, /// 0xa40
			 0x71dade4c, /// 0xa44
			 0x72bc1166, /// 0xa48
			 0x955bd3b3, /// 0xa4c
			 0xefc6ceee, /// 0xa50
			 0xf2525f44, /// 0xa54
			 0xccf964cb, /// 0xa58
			 0x0f79071f, /// 0xa5c
			 0xe45ec4e6, /// 0xa60
			 0x3e9d2d80, /// 0xa64
			 0xe998ec32, /// 0xa68
			 0x8416f533, /// 0xa6c
			 0x2fd159db, /// 0xa70
			 0xc1897874, /// 0xa74
			 0x14391db5, /// 0xa78
			 0x0b7df641, /// 0xa7c
			 0xf5a7e15b, /// 0xa80
			 0xe123a18d, /// 0xa84
			 0xb47d69f5, /// 0xa88
			 0x3f65f3c5, /// 0xa8c
			 0xe5eafae9, /// 0xa90
			 0xcee1fead, /// 0xa94
			 0x20c26d07, /// 0xa98
			 0xe59dec87, /// 0xa9c
			 0x8c63840f, /// 0xaa0
			 0x7f7d3171, /// 0xaa4
			 0xf0fd37ed, /// 0xaa8
			 0x62e60bfe, /// 0xaac
			 0x4dcf611d, /// 0xab0
			 0x0e265412, /// 0xab4
			 0x149ee9c1, /// 0xab8
			 0x4955bd7d, /// 0xabc
			 0xc1ef8793, /// 0xac0
			 0x10d1359d, /// 0xac4
			 0x694b5164, /// 0xac8
			 0xfc563767, /// 0xacc
			 0xac203417, /// 0xad0
			 0x0fea4e8b, /// 0xad4
			 0x2379d2b1, /// 0xad8
			 0x129275ae, /// 0xadc
			 0xde7b0d47, /// 0xae0
			 0x671c9914, /// 0xae4
			 0xff6e4fdf, /// 0xae8
			 0xc73d2016, /// 0xaec
			 0x7aa8c086, /// 0xaf0
			 0x8520e269, /// 0xaf4
			 0xf309b2d1, /// 0xaf8
			 0xfe053170, /// 0xafc
			 0x57484c78, /// 0xb00
			 0xd15f93c4, /// 0xb04
			 0x27bc55bd, /// 0xb08
			 0x6a0704f7, /// 0xb0c
			 0xf1df5f1f, /// 0xb10
			 0xb7563e7e, /// 0xb14
			 0x9962fcfd, /// 0xb18
			 0xf211d2ea, /// 0xb1c
			 0x1f91eb38, /// 0xb20
			 0xefc3239b, /// 0xb24
			 0x7be2ebfa, /// 0xb28
			 0x77f8a747, /// 0xb2c
			 0x3320b111, /// 0xb30
			 0x430e158b, /// 0xb34
			 0xa05345fe, /// 0xb38
			 0xc559c6a0, /// 0xb3c
			 0xc1a501a3, /// 0xb40
			 0x3de0f280, /// 0xb44
			 0xac815f98, /// 0xb48
			 0x37fd6ed6, /// 0xb4c
			 0x5537dd62, /// 0xb50
			 0xbde1fc7f, /// 0xb54
			 0x73a62d43, /// 0xb58
			 0xb25c3ca2, /// 0xb5c
			 0xe52d1925, /// 0xb60
			 0xcd02f4b1, /// 0xb64
			 0xd7d1d444, /// 0xb68
			 0x1d795bc9, /// 0xb6c
			 0xa41a61d4, /// 0xb70
			 0xba747ba1, /// 0xb74
			 0xe36972fb, /// 0xb78
			 0x80b842b6, /// 0xb7c
			 0xb86f5da9, /// 0xb80
			 0xc79cadf0, /// 0xb84
			 0xa30ac513, /// 0xb88
			 0x6afeeca8, /// 0xb8c
			 0x9d1f52ef, /// 0xb90
			 0xf0194306, /// 0xb94
			 0xd32f00e6, /// 0xb98
			 0x6fa5d196, /// 0xb9c
			 0xaf6872da, /// 0xba0
			 0x6f6226c1, /// 0xba4
			 0x0f169777, /// 0xba8
			 0x40942aca, /// 0xbac
			 0xfe3589a5, /// 0xbb0
			 0x3a1e00a1, /// 0xbb4
			 0x84bc488e, /// 0xbb8
			 0xb6e216d6, /// 0xbbc
			 0x870a92fe, /// 0xbc0
			 0x0a6a7ce3, /// 0xbc4
			 0x0a6ef1a3, /// 0xbc8
			 0xb65dff8d, /// 0xbcc
			 0x7e5c0501, /// 0xbd0
			 0xdabca553, /// 0xbd4
			 0x1922c4fe, /// 0xbd8
			 0x30f8f253, /// 0xbdc
			 0x54aeca00, /// 0xbe0
			 0xdbf930bc, /// 0xbe4
			 0x87303358, /// 0xbe8
			 0xef5fb6c3, /// 0xbec
			 0xcb4495d7, /// 0xbf0
			 0x6163a50f, /// 0xbf4
			 0xc19242e8, /// 0xbf8
			 0x3df74c32, /// 0xbfc
			 0x33a12725, /// 0xc00
			 0xd10b886e, /// 0xc04
			 0x71185468, /// 0xc08
			 0x4d831d46, /// 0xc0c
			 0x945a4619, /// 0xc10
			 0x046cda95, /// 0xc14
			 0xfdf5a41e, /// 0xc18
			 0xb277b035, /// 0xc1c
			 0xefd73511, /// 0xc20
			 0xc59f9d0a, /// 0xc24
			 0x99d7652a, /// 0xc28
			 0xcf8765a4, /// 0xc2c
			 0x47677ad2, /// 0xc30
			 0x369ce63b, /// 0xc34
			 0x1d2c497c, /// 0xc38
			 0x61be763d, /// 0xc3c
			 0xba9b2d9e, /// 0xc40
			 0xda8f692f, /// 0xc44
			 0x2ca77088, /// 0xc48
			 0x3f5798e6, /// 0xc4c
			 0x6bed3814, /// 0xc50
			 0x6c466122, /// 0xc54
			 0xf4be8b4d, /// 0xc58
			 0xf677ca40, /// 0xc5c
			 0x160e3835, /// 0xc60
			 0x71ea402b, /// 0xc64
			 0x5fdb8a63, /// 0xc68
			 0xddad449b, /// 0xc6c
			 0xb592cb2a, /// 0xc70
			 0x4d4116ac, /// 0xc74
			 0x6ead53e0, /// 0xc78
			 0xe1a866b5, /// 0xc7c
			 0xadabf9c3, /// 0xc80
			 0x4b34d282, /// 0xc84
			 0x793a8755, /// 0xc88
			 0x9d8fff06, /// 0xc8c
			 0x9851ae1f, /// 0xc90
			 0xf8d8f09b, /// 0xc94
			 0x7dedade8, /// 0xc98
			 0x5b99cc4f, /// 0xc9c
			 0x240fffd7, /// 0xca0
			 0xfcbb41e8, /// 0xca4
			 0x8bf2ba58, /// 0xca8
			 0xfef17caa, /// 0xcac
			 0x065a8326, /// 0xcb0
			 0x29b67ca5, /// 0xcb4
			 0xc124daed, /// 0xcb8
			 0xc316f38c, /// 0xcbc
			 0x1175e498, /// 0xcc0
			 0x2b4a8a3c, /// 0xcc4
			 0x12165bfe, /// 0xcc8
			 0x606ca72a, /// 0xccc
			 0x857389d3, /// 0xcd0
			 0x2cd0fca6, /// 0xcd4
			 0x86057a74, /// 0xcd8
			 0x4e6b46f3, /// 0xcdc
			 0xa2dfccd7, /// 0xce0
			 0xf7848fd7, /// 0xce4
			 0x8afb2321, /// 0xce8
			 0x6faa6c23, /// 0xcec
			 0x9770f136, /// 0xcf0
			 0x0c4da655, /// 0xcf4
			 0x231a55ed, /// 0xcf8
			 0x11bfbda7, /// 0xcfc
			 0x85ff2c32, /// 0xd00
			 0x1c35b03c, /// 0xd04
			 0x40f1a649, /// 0xd08
			 0xac2fb4fb, /// 0xd0c
			 0xcfda3643, /// 0xd10
			 0x44a4423a, /// 0xd14
			 0xbad8c551, /// 0xd18
			 0x029cc710, /// 0xd1c
			 0x59bed219, /// 0xd20
			 0xe2b55768, /// 0xd24
			 0x20891e97, /// 0xd28
			 0x5b673944, /// 0xd2c
			 0x88b126ae, /// 0xd30
			 0xc1cd5d84, /// 0xd34
			 0x362fe35c, /// 0xd38
			 0x0c952e37, /// 0xd3c
			 0xe9e864f9, /// 0xd40
			 0xfe494ba2, /// 0xd44
			 0x99d2c843, /// 0xd48
			 0x0c92cc96, /// 0xd4c
			 0xdc8a8c30, /// 0xd50
			 0x62f46ef4, /// 0xd54
			 0x2bf34c78, /// 0xd58
			 0x7c91aca4, /// 0xd5c
			 0x40eb6558, /// 0xd60
			 0x37411c65, /// 0xd64
			 0xde4ff9f7, /// 0xd68
			 0x9023245a, /// 0xd6c
			 0xd8796d2d, /// 0xd70
			 0x61d1f6da, /// 0xd74
			 0x471b2bab, /// 0xd78
			 0x9efc5bcc, /// 0xd7c
			 0xead72260, /// 0xd80
			 0x5226f35b, /// 0xd84
			 0x285f7961, /// 0xd88
			 0x385cd982, /// 0xd8c
			 0x44479be0, /// 0xd90
			 0x56847bb9, /// 0xd94
			 0xe4b4ebb5, /// 0xd98
			 0x682f28ef, /// 0xd9c
			 0x71fe2ba0, /// 0xda0
			 0xcdeeb12e, /// 0xda4
			 0x8d28e526, /// 0xda8
			 0x6d68dc50, /// 0xdac
			 0x044809ef, /// 0xdb0
			 0x0f87917c, /// 0xdb4
			 0x664c952b, /// 0xdb8
			 0x92db1571, /// 0xdbc
			 0x2fd74dca, /// 0xdc0
			 0x0a9df13f, /// 0xdc4
			 0xf4e61f9b, /// 0xdc8
			 0x92cfe0a4, /// 0xdcc
			 0x1e003aa6, /// 0xdd0
			 0xbac9d139, /// 0xdd4
			 0x6f37c438, /// 0xdd8
			 0xab42d3f5, /// 0xddc
			 0x780f3458, /// 0xde0
			 0x67be4e3d, /// 0xde4
			 0x1c3a6b70, /// 0xde8
			 0x2ca626b4, /// 0xdec
			 0x2b24538c, /// 0xdf0
			 0xbaa8dfe2, /// 0xdf4
			 0x77e68035, /// 0xdf8
			 0x267dd07f, /// 0xdfc
			 0xe844a404, /// 0xe00
			 0x50ee9510, /// 0xe04
			 0x7607c6ed, /// 0xe08
			 0xe8de9d26, /// 0xe0c
			 0x69d579c3, /// 0xe10
			 0x5068bcb7, /// 0xe14
			 0xe3b95ccd, /// 0xe18
			 0x4d221df5, /// 0xe1c
			 0x9657e41b, /// 0xe20
			 0x8daea0f1, /// 0xe24
			 0x35a53bcf, /// 0xe28
			 0x8d9156c7, /// 0xe2c
			 0x92c90692, /// 0xe30
			 0x785f018c, /// 0xe34
			 0xf073bc4a, /// 0xe38
			 0x3bb0d8b7, /// 0xe3c
			 0xb8a4d6ed, /// 0xe40
			 0x7def8ec4, /// 0xe44
			 0x9fa440cd, /// 0xe48
			 0x6c6492e6, /// 0xe4c
			 0x143ef33b, /// 0xe50
			 0xb68c008f, /// 0xe54
			 0xe2ec081d, /// 0xe58
			 0x26adc68a, /// 0xe5c
			 0x16677100, /// 0xe60
			 0xe8f29431, /// 0xe64
			 0x9a8fc888, /// 0xe68
			 0x2f03f7ff, /// 0xe6c
			 0x1196cde3, /// 0xe70
			 0x45aaec7a, /// 0xe74
			 0x95ef6754, /// 0xe78
			 0xc6d9f74c, /// 0xe7c
			 0xebbc7026, /// 0xe80
			 0x4be827e9, /// 0xe84
			 0x9838121d, /// 0xe88
			 0x3fa13a4e, /// 0xe8c
			 0x76efb870, /// 0xe90
			 0x70a62c33, /// 0xe94
			 0xbe436323, /// 0xe98
			 0x628342db, /// 0xe9c
			 0xfc9de20a, /// 0xea0
			 0xac177b5e, /// 0xea4
			 0x37a22af7, /// 0xea8
			 0xa1ee7412, /// 0xeac
			 0x4e9f5d46, /// 0xeb0
			 0x1a1fe65f, /// 0xeb4
			 0x6130485b, /// 0xeb8
			 0x26a1f98a, /// 0xebc
			 0xc015239f, /// 0xec0
			 0x7214d836, /// 0xec4
			 0x0e29b869, /// 0xec8
			 0xd1699148, /// 0xecc
			 0xbe5494d3, /// 0xed0
			 0xa978ee80, /// 0xed4
			 0xb2471b82, /// 0xed8
			 0xc8c250eb, /// 0xedc
			 0xedcc375a, /// 0xee0
			 0x47749ff4, /// 0xee4
			 0xf6aa3749, /// 0xee8
			 0x09a0e0c6, /// 0xeec
			 0x84bd8f8e, /// 0xef0
			 0x535c34b5, /// 0xef4
			 0xc1eb6ca5, /// 0xef8
			 0xb465d758, /// 0xefc
			 0x46c4ce76, /// 0xf00
			 0xcbd86973, /// 0xf04
			 0xac7ae8ba, /// 0xf08
			 0x27dff8e6, /// 0xf0c
			 0x32c17690, /// 0xf10
			 0x882438f6, /// 0xf14
			 0xff1055a0, /// 0xf18
			 0x26c8fc1a, /// 0xf1c
			 0x1d5336fa, /// 0xf20
			 0xa44d71d9, /// 0xf24
			 0xfc74ed33, /// 0xf28
			 0x9d4128ac, /// 0xf2c
			 0x7ada69b5, /// 0xf30
			 0x2903c3c1, /// 0xf34
			 0x1edb96c7, /// 0xf38
			 0x257de97e, /// 0xf3c
			 0x00e2709b, /// 0xf40
			 0xcc2b138b, /// 0xf44
			 0xbd211f08, /// 0xf48
			 0xb76d15d6, /// 0xf4c
			 0xad691e22, /// 0xf50
			 0xdf3508cf, /// 0xf54
			 0x65061742, /// 0xf58
			 0x14ab21e8, /// 0xf5c
			 0x0a6d11a4, /// 0xf60
			 0x2d2e5603, /// 0xf64
			 0x7799ad7e, /// 0xf68
			 0x3d8504f1, /// 0xf6c
			 0xcb512b91, /// 0xf70
			 0x47729d93, /// 0xf74
			 0x2587d6ab, /// 0xf78
			 0x2d78bafe, /// 0xf7c
			 0xca242990, /// 0xf80
			 0xbfbdb05a, /// 0xf84
			 0xc25d0c56, /// 0xf88
			 0xf716e75d, /// 0xf8c
			 0x874efab0, /// 0xf90
			 0x3211d74d, /// 0xf94
			 0x31eb0b5a, /// 0xf98
			 0x934ca7c4, /// 0xf9c
			 0xcba61ba8, /// 0xfa0
			 0xb19fa108, /// 0xfa4
			 0xbe2c5f99, /// 0xfa8
			 0x7dde68a0, /// 0xfac
			 0x66caa665, /// 0xfb0
			 0xb2b4fd04, /// 0xfb4
			 0x1910cea0, /// 0xfb8
			 0x440aaaea, /// 0xfbc
			 0x1d7e3a8e, /// 0xfc0
			 0x81ad368c, /// 0xfc4
			 0x243ce89c, /// 0xfc8
			 0x11413de4, /// 0xfcc
			 0x47aa2d41, /// 0xfd0
			 0x082e33ca, /// 0xfd4
			 0x97e0efdf, /// 0xfd8
			 0x5a8a5ab2, /// 0xfdc
			 0x6d2821d1, /// 0xfe0
			 0x983a17a0, /// 0xfe4
			 0x8bde95a6, /// 0xfe8
			 0xee361033, /// 0xfec
			 0xe548e77c, /// 0xff0
			 0x24a8d57c, /// 0xff4
			 0x29bd788b, /// 0xff8
			 0xc3734f7f /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x69d0f76b, /// 0x0
			 0x7e20833f, /// 0x4
			 0x8b9737e4, /// 0x8
			 0x311e2c1a, /// 0xc
			 0xec714312, /// 0x10
			 0xb3670a72, /// 0x14
			 0xf63a3e9a, /// 0x18
			 0x8007866d, /// 0x1c
			 0x8570e2cf, /// 0x20
			 0x95b92e7d, /// 0x24
			 0xd2a64f2d, /// 0x28
			 0xb2767165, /// 0x2c
			 0x8b803ace, /// 0x30
			 0x0e945137, /// 0x34
			 0x43f356e9, /// 0x38
			 0x08dafd6b, /// 0x3c
			 0xb04ca5a0, /// 0x40
			 0xd7cc5102, /// 0x44
			 0xe02539d5, /// 0x48
			 0xfc1c803e, /// 0x4c
			 0x6f887b24, /// 0x50
			 0x4f37c39f, /// 0x54
			 0x4113e63c, /// 0x58
			 0x42d9a866, /// 0x5c
			 0xc2747cd7, /// 0x60
			 0x8ce5527a, /// 0x64
			 0x5444c9b5, /// 0x68
			 0x22fbe1ff, /// 0x6c
			 0x55cb0b18, /// 0x70
			 0xa2e67c65, /// 0x74
			 0xea460011, /// 0x78
			 0x7c898555, /// 0x7c
			 0xa562c2db, /// 0x80
			 0x93f8fd5a, /// 0x84
			 0xa12f0bc3, /// 0x88
			 0xd4679f79, /// 0x8c
			 0xa85ee069, /// 0x90
			 0x63e1f3d3, /// 0x94
			 0x5d2d2a4e, /// 0x98
			 0x55b8ebf8, /// 0x9c
			 0xa0c9c69e, /// 0xa0
			 0x3483271e, /// 0xa4
			 0x24d77a3f, /// 0xa8
			 0xacd5c7e0, /// 0xac
			 0x7053f666, /// 0xb0
			 0x58640172, /// 0xb4
			 0x968fd83b, /// 0xb8
			 0x85f36194, /// 0xbc
			 0x59dc1983, /// 0xc0
			 0xe5299f93, /// 0xc4
			 0x48d6af99, /// 0xc8
			 0x751983cc, /// 0xcc
			 0xedfa7085, /// 0xd0
			 0x31b2b527, /// 0xd4
			 0xb7c1e576, /// 0xd8
			 0x648517e0, /// 0xdc
			 0xff5aa19b, /// 0xe0
			 0x4a44d80a, /// 0xe4
			 0xc73fb38b, /// 0xe8
			 0x0a94bd52, /// 0xec
			 0x7dd646c2, /// 0xf0
			 0x2641c63a, /// 0xf4
			 0xf0581b89, /// 0xf8
			 0x783f9498, /// 0xfc
			 0x10c19559, /// 0x100
			 0xc16c8a37, /// 0x104
			 0x84c58d29, /// 0x108
			 0x8a4cefb5, /// 0x10c
			 0xd2db8bcd, /// 0x110
			 0xfca34e82, /// 0x114
			 0x1cf21205, /// 0x118
			 0x7b46980d, /// 0x11c
			 0xbef6236d, /// 0x120
			 0xc30dbd75, /// 0x124
			 0x90991db1, /// 0x128
			 0x4b4095b0, /// 0x12c
			 0x48b22fde, /// 0x130
			 0xee282f27, /// 0x134
			 0xb02e8415, /// 0x138
			 0xf4d2b728, /// 0x13c
			 0xcb755fc8, /// 0x140
			 0xef3d50d2, /// 0x144
			 0x8e70313b, /// 0x148
			 0xcb4aaff4, /// 0x14c
			 0xb716f3b2, /// 0x150
			 0x68c18719, /// 0x154
			 0x784bd8f2, /// 0x158
			 0x5f102575, /// 0x15c
			 0x16204c8e, /// 0x160
			 0xa62b3870, /// 0x164
			 0xeba60734, /// 0x168
			 0xe6c8ddb2, /// 0x16c
			 0xb5ff1fa3, /// 0x170
			 0xf302a20e, /// 0x174
			 0xa8d2a6d4, /// 0x178
			 0x2b75ed8c, /// 0x17c
			 0xe8a2f274, /// 0x180
			 0x5697ce2b, /// 0x184
			 0xabf7c6ce, /// 0x188
			 0x4df445d3, /// 0x18c
			 0x030e1554, /// 0x190
			 0x36ab19c3, /// 0x194
			 0x2de6036d, /// 0x198
			 0xf4d93913, /// 0x19c
			 0xde3e1729, /// 0x1a0
			 0xfac0b458, /// 0x1a4
			 0x2849862e, /// 0x1a8
			 0xf8439823, /// 0x1ac
			 0xc91a97f5, /// 0x1b0
			 0xf0fd877c, /// 0x1b4
			 0xc86aa14a, /// 0x1b8
			 0x7560a1e6, /// 0x1bc
			 0x9eb39a6b, /// 0x1c0
			 0x6265dad7, /// 0x1c4
			 0xe3700e4d, /// 0x1c8
			 0x73e0ab1f, /// 0x1cc
			 0x60466516, /// 0x1d0
			 0x6d752b4a, /// 0x1d4
			 0x06c63e00, /// 0x1d8
			 0xe9689d5f, /// 0x1dc
			 0x461f54c4, /// 0x1e0
			 0xaa772a4d, /// 0x1e4
			 0x69430b69, /// 0x1e8
			 0xb683aaea, /// 0x1ec
			 0xfe00cc90, /// 0x1f0
			 0x65251e40, /// 0x1f4
			 0x1fe077eb, /// 0x1f8
			 0xca2913da, /// 0x1fc
			 0xefd2bd14, /// 0x200
			 0x734229cf, /// 0x204
			 0x52099552, /// 0x208
			 0x39bd42ac, /// 0x20c
			 0xe954d523, /// 0x210
			 0x43250018, /// 0x214
			 0x4245c237, /// 0x218
			 0xe38422a4, /// 0x21c
			 0xb9d15d15, /// 0x220
			 0x2a08bf89, /// 0x224
			 0xb583fd23, /// 0x228
			 0x487aa65f, /// 0x22c
			 0x1afd13a6, /// 0x230
			 0x4a4c3a9e, /// 0x234
			 0x5de05642, /// 0x238
			 0xa8d0619d, /// 0x23c
			 0x111a55cc, /// 0x240
			 0x97e6ea53, /// 0x244
			 0x2ac4ac94, /// 0x248
			 0x66271155, /// 0x24c
			 0xf5dd93b6, /// 0x250
			 0xc2cb00cf, /// 0x254
			 0xe818bfaf, /// 0x258
			 0xa0b9de53, /// 0x25c
			 0xb9733e2d, /// 0x260
			 0x2710405a, /// 0x264
			 0x3dca7c97, /// 0x268
			 0xfc8f011c, /// 0x26c
			 0xd603d1ea, /// 0x270
			 0x7ab035cc, /// 0x274
			 0x262f0916, /// 0x278
			 0x41640ee0, /// 0x27c
			 0x5163875d, /// 0x280
			 0x87c7f5c1, /// 0x284
			 0xea393bf7, /// 0x288
			 0xe5316c7d, /// 0x28c
			 0xadabdc77, /// 0x290
			 0x36bd52cf, /// 0x294
			 0xfcbd02de, /// 0x298
			 0xb963d4dd, /// 0x29c
			 0x173f5f7c, /// 0x2a0
			 0xb5cae6c0, /// 0x2a4
			 0x391ba5ce, /// 0x2a8
			 0x2123d169, /// 0x2ac
			 0xdf932652, /// 0x2b0
			 0x4be51f3a, /// 0x2b4
			 0x981247e9, /// 0x2b8
			 0x791b5631, /// 0x2bc
			 0xe07dfacd, /// 0x2c0
			 0x17257880, /// 0x2c4
			 0x53828068, /// 0x2c8
			 0x9bab3cc4, /// 0x2cc
			 0xc9f6cb27, /// 0x2d0
			 0x4a1f88c9, /// 0x2d4
			 0x4c898519, /// 0x2d8
			 0xbba4d5cc, /// 0x2dc
			 0xf491fd1d, /// 0x2e0
			 0xe7759853, /// 0x2e4
			 0x4c5b432d, /// 0x2e8
			 0x589059e3, /// 0x2ec
			 0x8aa25550, /// 0x2f0
			 0xe11342da, /// 0x2f4
			 0x4e94414c, /// 0x2f8
			 0x18c1fdf9, /// 0x2fc
			 0xc85178eb, /// 0x300
			 0xc2124bea, /// 0x304
			 0xdf972d8a, /// 0x308
			 0x6d52e7f1, /// 0x30c
			 0x1bbbb011, /// 0x310
			 0x3af682b3, /// 0x314
			 0x6b5c2b92, /// 0x318
			 0x75de4b35, /// 0x31c
			 0xc94007d7, /// 0x320
			 0xb3afebf4, /// 0x324
			 0xbcd26e25, /// 0x328
			 0xe15032c2, /// 0x32c
			 0x363c5f73, /// 0x330
			 0xb451058b, /// 0x334
			 0x2080bcf6, /// 0x338
			 0xb477e14d, /// 0x33c
			 0x0183efb0, /// 0x340
			 0xa3d7aebb, /// 0x344
			 0xe421bba1, /// 0x348
			 0xa445cad0, /// 0x34c
			 0x03992330, /// 0x350
			 0x43df5c1e, /// 0x354
			 0x3a0ee5c5, /// 0x358
			 0xa071a768, /// 0x35c
			 0xefc49502, /// 0x360
			 0x773fb0da, /// 0x364
			 0x7b455a5b, /// 0x368
			 0x1db22834, /// 0x36c
			 0x1c422e0e, /// 0x370
			 0xe336d86a, /// 0x374
			 0x9ccc7236, /// 0x378
			 0x6fde2559, /// 0x37c
			 0xa9d766a9, /// 0x380
			 0xf908dd0f, /// 0x384
			 0x5859a02b, /// 0x388
			 0xbefaee54, /// 0x38c
			 0xf3ad356a, /// 0x390
			 0xdef92277, /// 0x394
			 0x934f15c6, /// 0x398
			 0x39930f8c, /// 0x39c
			 0xe9976a9c, /// 0x3a0
			 0x0ac0ab18, /// 0x3a4
			 0x866cdefe, /// 0x3a8
			 0x32e27f9c, /// 0x3ac
			 0x54763459, /// 0x3b0
			 0x27bfec34, /// 0x3b4
			 0x53bf9073, /// 0x3b8
			 0xa9345088, /// 0x3bc
			 0x59269221, /// 0x3c0
			 0xc3d302a8, /// 0x3c4
			 0x10cfab6f, /// 0x3c8
			 0x42a11315, /// 0x3cc
			 0x43a7b62b, /// 0x3d0
			 0xbe8ce33b, /// 0x3d4
			 0x805b532f, /// 0x3d8
			 0x70187680, /// 0x3dc
			 0x80f93452, /// 0x3e0
			 0x20f31348, /// 0x3e4
			 0xfe1c9d51, /// 0x3e8
			 0xfb97e471, /// 0x3ec
			 0x2129777c, /// 0x3f0
			 0xff96211f, /// 0x3f4
			 0x2c96320b, /// 0x3f8
			 0x0feb33ca, /// 0x3fc
			 0x57ff5082, /// 0x400
			 0x3d9a606d, /// 0x404
			 0x41760809, /// 0x408
			 0xac5b02b8, /// 0x40c
			 0x8584ece4, /// 0x410
			 0x89dfc011, /// 0x414
			 0x4e6847df, /// 0x418
			 0x0bd01a1e, /// 0x41c
			 0x1bf85538, /// 0x420
			 0xfee6a161, /// 0x424
			 0xc7bb5439, /// 0x428
			 0x5aaa1bac, /// 0x42c
			 0x171f345f, /// 0x430
			 0x79af5788, /// 0x434
			 0x31ff5b70, /// 0x438
			 0x49e232af, /// 0x43c
			 0x06510237, /// 0x440
			 0x27598338, /// 0x444
			 0xd9e77a88, /// 0x448
			 0x5b7613f7, /// 0x44c
			 0x7f48ebc2, /// 0x450
			 0xf99873e6, /// 0x454
			 0x390c7391, /// 0x458
			 0x85374375, /// 0x45c
			 0x8851a2de, /// 0x460
			 0x1dc73b08, /// 0x464
			 0x57fef821, /// 0x468
			 0x921c7f5b, /// 0x46c
			 0x44f17923, /// 0x470
			 0x8c33de94, /// 0x474
			 0x6a52d960, /// 0x478
			 0xc024eb16, /// 0x47c
			 0xe844f256, /// 0x480
			 0xbde2d547, /// 0x484
			 0x493ca4d8, /// 0x488
			 0x3749855a, /// 0x48c
			 0x2ed5691c, /// 0x490
			 0x2193b6ed, /// 0x494
			 0xfabc3778, /// 0x498
			 0x47eaa0b4, /// 0x49c
			 0x7074559c, /// 0x4a0
			 0x748b7ee3, /// 0x4a4
			 0x45ff47d5, /// 0x4a8
			 0x961ae9f2, /// 0x4ac
			 0x3262cf90, /// 0x4b0
			 0xc8d04817, /// 0x4b4
			 0xacfabfc7, /// 0x4b8
			 0x31a0d5e4, /// 0x4bc
			 0xaf285cf2, /// 0x4c0
			 0xeaa0210b, /// 0x4c4
			 0xdc288538, /// 0x4c8
			 0x66e9b1cd, /// 0x4cc
			 0x8dd513af, /// 0x4d0
			 0x49b8c3a9, /// 0x4d4
			 0x60cd4522, /// 0x4d8
			 0xafbfbe5e, /// 0x4dc
			 0xc700cf5f, /// 0x4e0
			 0x9cc5c2b2, /// 0x4e4
			 0xa0ecba52, /// 0x4e8
			 0x2bf4887e, /// 0x4ec
			 0xe5aa1186, /// 0x4f0
			 0xec59a4f4, /// 0x4f4
			 0xab329316, /// 0x4f8
			 0x54022e1a, /// 0x4fc
			 0xbf9f464e, /// 0x500
			 0xb08e168c, /// 0x504
			 0x40fe788e, /// 0x508
			 0x612493ac, /// 0x50c
			 0xfef0f65f, /// 0x510
			 0x68b0639d, /// 0x514
			 0x50ec2899, /// 0x518
			 0xd1190802, /// 0x51c
			 0xed290d55, /// 0x520
			 0xccf2931f, /// 0x524
			 0x0a819d07, /// 0x528
			 0x0eda2361, /// 0x52c
			 0xfec7959e, /// 0x530
			 0xbb67d84e, /// 0x534
			 0x986ad55d, /// 0x538
			 0x4d931aa2, /// 0x53c
			 0x4a151042, /// 0x540
			 0xf1434dc9, /// 0x544
			 0x9603c62d, /// 0x548
			 0xe15dfb14, /// 0x54c
			 0x0ab0047a, /// 0x550
			 0x3f0325b4, /// 0x554
			 0xa09e223d, /// 0x558
			 0x402262a4, /// 0x55c
			 0xcd1c63cb, /// 0x560
			 0x5d9db643, /// 0x564
			 0x777b0ce5, /// 0x568
			 0x5163224f, /// 0x56c
			 0x100ba579, /// 0x570
			 0xa4235962, /// 0x574
			 0xb57df1f2, /// 0x578
			 0xad82be89, /// 0x57c
			 0xffa8fe56, /// 0x580
			 0x0ca31559, /// 0x584
			 0xf574fd33, /// 0x588
			 0xee910396, /// 0x58c
			 0x1cbd9725, /// 0x590
			 0xd6988441, /// 0x594
			 0x6b1d7ddd, /// 0x598
			 0x8688e801, /// 0x59c
			 0xaa18cb3a, /// 0x5a0
			 0xc0f43b2f, /// 0x5a4
			 0xbc0a600c, /// 0x5a8
			 0x8544da62, /// 0x5ac
			 0x17c1fec0, /// 0x5b0
			 0xaa3341af, /// 0x5b4
			 0x19471b08, /// 0x5b8
			 0x831f78f3, /// 0x5bc
			 0xef065288, /// 0x5c0
			 0x4b4d32b2, /// 0x5c4
			 0xa11e91d5, /// 0x5c8
			 0xf6a9a1fc, /// 0x5cc
			 0xdff3f858, /// 0x5d0
			 0x43c10f3a, /// 0x5d4
			 0xeee6ef76, /// 0x5d8
			 0x91a24646, /// 0x5dc
			 0x3d90a137, /// 0x5e0
			 0x7dd438a9, /// 0x5e4
			 0x19f4f6c9, /// 0x5e8
			 0xc0a02ffd, /// 0x5ec
			 0xe365c35b, /// 0x5f0
			 0xe87a2094, /// 0x5f4
			 0xb4f1c89c, /// 0x5f8
			 0x74aee564, /// 0x5fc
			 0x7af50cdc, /// 0x600
			 0x3ea91ddc, /// 0x604
			 0x5117ca6a, /// 0x608
			 0x02cca431, /// 0x60c
			 0x05dc8f3c, /// 0x610
			 0x9d809473, /// 0x614
			 0x518ca52c, /// 0x618
			 0x6cde8a41, /// 0x61c
			 0x718b40e5, /// 0x620
			 0x87d8a899, /// 0x624
			 0x7acb2f28, /// 0x628
			 0x26c3bd83, /// 0x62c
			 0x5ee06c18, /// 0x630
			 0x4e43b684, /// 0x634
			 0x0dc09816, /// 0x638
			 0x9d2d8617, /// 0x63c
			 0x5c220d33, /// 0x640
			 0x0d166dbd, /// 0x644
			 0xf02f470d, /// 0x648
			 0x860808f5, /// 0x64c
			 0x632e5447, /// 0x650
			 0x70fb0f56, /// 0x654
			 0xc3246b68, /// 0x658
			 0x3dbdeb5f, /// 0x65c
			 0x841ba672, /// 0x660
			 0x25e72896, /// 0x664
			 0x364db378, /// 0x668
			 0xe8efe0b8, /// 0x66c
			 0xc7138d17, /// 0x670
			 0xa6a18c49, /// 0x674
			 0x477194b8, /// 0x678
			 0x2f955ac9, /// 0x67c
			 0xa2d2dabd, /// 0x680
			 0xca4758d7, /// 0x684
			 0x56454d34, /// 0x688
			 0xb1efdf84, /// 0x68c
			 0xab06a0e6, /// 0x690
			 0x960a3983, /// 0x694
			 0xf9d29b25, /// 0x698
			 0x83863fdc, /// 0x69c
			 0x79b91968, /// 0x6a0
			 0x7dff7ebe, /// 0x6a4
			 0xc659722b, /// 0x6a8
			 0xbfe7d4c3, /// 0x6ac
			 0x904e46c1, /// 0x6b0
			 0x4b9ba973, /// 0x6b4
			 0x8985698c, /// 0x6b8
			 0x6809fc32, /// 0x6bc
			 0x23ef05a4, /// 0x6c0
			 0xa0163193, /// 0x6c4
			 0x1af634e9, /// 0x6c8
			 0x4ff257a8, /// 0x6cc
			 0x4fb256a0, /// 0x6d0
			 0x71311ada, /// 0x6d4
			 0xd1503945, /// 0x6d8
			 0x81f2a71d, /// 0x6dc
			 0xd7ad97f5, /// 0x6e0
			 0x71294ee5, /// 0x6e4
			 0x04702124, /// 0x6e8
			 0x4c416d6b, /// 0x6ec
			 0x11147a58, /// 0x6f0
			 0xd39980b8, /// 0x6f4
			 0x8f7173fb, /// 0x6f8
			 0x8eb3acd7, /// 0x6fc
			 0xf545a0bf, /// 0x700
			 0xa61f6eef, /// 0x704
			 0x519a75b0, /// 0x708
			 0x70afcb54, /// 0x70c
			 0x6b367ebc, /// 0x710
			 0x2198b7d8, /// 0x714
			 0x091e75af, /// 0x718
			 0x4832d162, /// 0x71c
			 0xa3371b40, /// 0x720
			 0xc109bb9c, /// 0x724
			 0xff9f44ca, /// 0x728
			 0x34c40c25, /// 0x72c
			 0x61f28ed2, /// 0x730
			 0xc7e9e634, /// 0x734
			 0xb8685bf0, /// 0x738
			 0x883b3ca0, /// 0x73c
			 0xd2ac5f4f, /// 0x740
			 0x2386244e, /// 0x744
			 0xe105536f, /// 0x748
			 0x322635c8, /// 0x74c
			 0x8e151a37, /// 0x750
			 0x1090f692, /// 0x754
			 0xe1fb2128, /// 0x758
			 0x58869009, /// 0x75c
			 0x43f9d0fd, /// 0x760
			 0x9c8b63ac, /// 0x764
			 0xf69bd82d, /// 0x768
			 0x6b4da8b2, /// 0x76c
			 0xc8c3ea1f, /// 0x770
			 0xea607826, /// 0x774
			 0x9c2901ed, /// 0x778
			 0x88ad05eb, /// 0x77c
			 0x16ea271e, /// 0x780
			 0x8acd28ed, /// 0x784
			 0x54a953ba, /// 0x788
			 0xf7280369, /// 0x78c
			 0x15da5d32, /// 0x790
			 0xb6cf64ca, /// 0x794
			 0x861f4f05, /// 0x798
			 0xa6966d70, /// 0x79c
			 0xcf0d3836, /// 0x7a0
			 0x8cffab7e, /// 0x7a4
			 0x2ad5fbb4, /// 0x7a8
			 0xf68f6fcb, /// 0x7ac
			 0x84c017ea, /// 0x7b0
			 0xf03f547f, /// 0x7b4
			 0x2643f30b, /// 0x7b8
			 0x54be83c0, /// 0x7bc
			 0x5adbf490, /// 0x7c0
			 0xadc4e1f5, /// 0x7c4
			 0x1a60f9be, /// 0x7c8
			 0x99816d72, /// 0x7cc
			 0x41e05684, /// 0x7d0
			 0x987fe4bd, /// 0x7d4
			 0x3319ec27, /// 0x7d8
			 0x91a8222b, /// 0x7dc
			 0x491925a5, /// 0x7e0
			 0x8e12cac4, /// 0x7e4
			 0x70b8500b, /// 0x7e8
			 0x17eafc81, /// 0x7ec
			 0xdbdef7a7, /// 0x7f0
			 0x5afb8492, /// 0x7f4
			 0x8778e43c, /// 0x7f8
			 0x8560f74c, /// 0x7fc
			 0xc21e628b, /// 0x800
			 0x5645ef97, /// 0x804
			 0x43a6ca8e, /// 0x808
			 0x4ff03fa8, /// 0x80c
			 0xcdd0cefa, /// 0x810
			 0xfc7bbf72, /// 0x814
			 0x2b55d910, /// 0x818
			 0x6da01b96, /// 0x81c
			 0xe4b1f986, /// 0x820
			 0x001880d6, /// 0x824
			 0x2942bd02, /// 0x828
			 0xbac3106e, /// 0x82c
			 0x22e7d7f2, /// 0x830
			 0x56d644bb, /// 0x834
			 0x2f95ccfc, /// 0x838
			 0xede760f4, /// 0x83c
			 0x735b61f4, /// 0x840
			 0xb20657e4, /// 0x844
			 0x1431bc56, /// 0x848
			 0x65f963da, /// 0x84c
			 0x7f6e21ee, /// 0x850
			 0x6e2da43b, /// 0x854
			 0xc8fddb99, /// 0x858
			 0x595f813c, /// 0x85c
			 0x384d919b, /// 0x860
			 0xbb9c5fc7, /// 0x864
			 0x73f2b09a, /// 0x868
			 0xb61e5ffa, /// 0x86c
			 0x27224379, /// 0x870
			 0xc869a8da, /// 0x874
			 0xcb7cbaf7, /// 0x878
			 0x8db5d31c, /// 0x87c
			 0xc3266b52, /// 0x880
			 0xb15feecc, /// 0x884
			 0x4f4cbc97, /// 0x888
			 0xe4ba0996, /// 0x88c
			 0xe347c4d6, /// 0x890
			 0x83cbbf02, /// 0x894
			 0x55ac2cbc, /// 0x898
			 0x5f7a4f56, /// 0x89c
			 0x031eecc7, /// 0x8a0
			 0x36b9b456, /// 0x8a4
			 0xb91b23db, /// 0x8a8
			 0x72efa65a, /// 0x8ac
			 0xb00fcb51, /// 0x8b0
			 0x53d23e86, /// 0x8b4
			 0x60ef0eef, /// 0x8b8
			 0x231dece6, /// 0x8bc
			 0xbf9de99b, /// 0x8c0
			 0x72088099, /// 0x8c4
			 0xaad5b4e3, /// 0x8c8
			 0xda46db5a, /// 0x8cc
			 0xf9218720, /// 0x8d0
			 0xc379726a, /// 0x8d4
			 0x3c170821, /// 0x8d8
			 0xebe308c2, /// 0x8dc
			 0xed7ff3f0, /// 0x8e0
			 0xfb8c9c9c, /// 0x8e4
			 0x4f5abc7d, /// 0x8e8
			 0x32c6d211, /// 0x8ec
			 0x8f988f81, /// 0x8f0
			 0x89fa9b9f, /// 0x8f4
			 0x89494b89, /// 0x8f8
			 0x02880f7e, /// 0x8fc
			 0x2653042f, /// 0x900
			 0x312cf67d, /// 0x904
			 0xf1a466e8, /// 0x908
			 0x34ed1c4e, /// 0x90c
			 0xfecf0e7b, /// 0x910
			 0xb58bbbd7, /// 0x914
			 0x0786bf20, /// 0x918
			 0x53b5f963, /// 0x91c
			 0x96f083e1, /// 0x920
			 0xfadefca8, /// 0x924
			 0xc733069a, /// 0x928
			 0x8a297369, /// 0x92c
			 0x03395a98, /// 0x930
			 0x6ccd14ab, /// 0x934
			 0xd6698e66, /// 0x938
			 0x53779458, /// 0x93c
			 0x0cb8a67a, /// 0x940
			 0xc40530e3, /// 0x944
			 0xbea7ce0f, /// 0x948
			 0xeece1e68, /// 0x94c
			 0x860d9d72, /// 0x950
			 0x722320b2, /// 0x954
			 0x937f051b, /// 0x958
			 0x72cc697f, /// 0x95c
			 0xd13a28d7, /// 0x960
			 0x2e619d0d, /// 0x964
			 0xc68eab22, /// 0x968
			 0xb01a6c46, /// 0x96c
			 0x81658ceb, /// 0x970
			 0xc47a0a79, /// 0x974
			 0xa0ddfdec, /// 0x978
			 0xe85f13cb, /// 0x97c
			 0x5c219624, /// 0x980
			 0xdbc57408, /// 0x984
			 0xc7c79c45, /// 0x988
			 0x6f6fdf59, /// 0x98c
			 0x251d21aa, /// 0x990
			 0x4d93a7e0, /// 0x994
			 0x3a18c734, /// 0x998
			 0xd819b884, /// 0x99c
			 0x45fa2ccd, /// 0x9a0
			 0xd45b589e, /// 0x9a4
			 0xd46922a2, /// 0x9a8
			 0xde826ef0, /// 0x9ac
			 0x2c29d3b6, /// 0x9b0
			 0x37eb1024, /// 0x9b4
			 0x89832f66, /// 0x9b8
			 0xfa6a39d9, /// 0x9bc
			 0x75169e07, /// 0x9c0
			 0x2907f88c, /// 0x9c4
			 0x27ca320e, /// 0x9c8
			 0x449d3a5c, /// 0x9cc
			 0x08420903, /// 0x9d0
			 0x46edb2c4, /// 0x9d4
			 0x11de4066, /// 0x9d8
			 0xa3e2f15d, /// 0x9dc
			 0x750c1f7b, /// 0x9e0
			 0x07186c47, /// 0x9e4
			 0x97f59721, /// 0x9e8
			 0x4bdb45ef, /// 0x9ec
			 0x325a2e49, /// 0x9f0
			 0xd8e96884, /// 0x9f4
			 0x0f408459, /// 0x9f8
			 0x6fc9db96, /// 0x9fc
			 0xf0e08963, /// 0xa00
			 0x7319880d, /// 0xa04
			 0x307b17a2, /// 0xa08
			 0x5eaf136c, /// 0xa0c
			 0xfd24195e, /// 0xa10
			 0x76696079, /// 0xa14
			 0xaffc1143, /// 0xa18
			 0x7dda728f, /// 0xa1c
			 0x6e0e1fc3, /// 0xa20
			 0xe8044474, /// 0xa24
			 0x0e81807b, /// 0xa28
			 0x0d2177ef, /// 0xa2c
			 0x9f568033, /// 0xa30
			 0xf0f4290a, /// 0xa34
			 0x5fa782e8, /// 0xa38
			 0x99f715b5, /// 0xa3c
			 0xf3f8a527, /// 0xa40
			 0x28c9b977, /// 0xa44
			 0x596a34f4, /// 0xa48
			 0x29e57788, /// 0xa4c
			 0x98c12308, /// 0xa50
			 0x88300fc2, /// 0xa54
			 0x506e147e, /// 0xa58
			 0xf7a8718e, /// 0xa5c
			 0xae0367de, /// 0xa60
			 0xbc47df78, /// 0xa64
			 0xe96d51c7, /// 0xa68
			 0xf320b0e7, /// 0xa6c
			 0x618428ee, /// 0xa70
			 0x04e92fa2, /// 0xa74
			 0x75be3adb, /// 0xa78
			 0xab9e206f, /// 0xa7c
			 0x7f31551c, /// 0xa80
			 0xdf684e63, /// 0xa84
			 0xd7c0f1b0, /// 0xa88
			 0x100c7b14, /// 0xa8c
			 0xad405729, /// 0xa90
			 0x8d2c9ba7, /// 0xa94
			 0x0412806e, /// 0xa98
			 0x2b66425a, /// 0xa9c
			 0xceab20df, /// 0xaa0
			 0x72b87114, /// 0xaa4
			 0xcd6b8891, /// 0xaa8
			 0xe9767fea, /// 0xaac
			 0x36798c24, /// 0xab0
			 0x6bc08a0a, /// 0xab4
			 0xf536e31f, /// 0xab8
			 0x91062a62, /// 0xabc
			 0xb5e86299, /// 0xac0
			 0x2541dc31, /// 0xac4
			 0xc2a4d0b0, /// 0xac8
			 0x0bae3ef3, /// 0xacc
			 0x71826282, /// 0xad0
			 0x1c4ecd49, /// 0xad4
			 0x606298af, /// 0xad8
			 0xfb8b335e, /// 0xadc
			 0x95b71df8, /// 0xae0
			 0xf7e34c14, /// 0xae4
			 0x0fc043b6, /// 0xae8
			 0xa2165ff6, /// 0xaec
			 0x6d057c8c, /// 0xaf0
			 0x20e01231, /// 0xaf4
			 0x3c9708fd, /// 0xaf8
			 0xac7beca7, /// 0xafc
			 0x3a6e124d, /// 0xb00
			 0x02566162, /// 0xb04
			 0x85f17309, /// 0xb08
			 0x722fae54, /// 0xb0c
			 0xa59f6369, /// 0xb10
			 0xc1eef6b7, /// 0xb14
			 0xb40ba53c, /// 0xb18
			 0xc36a3ec3, /// 0xb1c
			 0x2b5c2e79, /// 0xb20
			 0x82f380db, /// 0xb24
			 0x17adb6ab, /// 0xb28
			 0x56dc7652, /// 0xb2c
			 0xd270c2dc, /// 0xb30
			 0x8a3f3e8d, /// 0xb34
			 0x71012ecc, /// 0xb38
			 0x35fee89b, /// 0xb3c
			 0x195b963a, /// 0xb40
			 0x901e9821, /// 0xb44
			 0x0b410380, /// 0xb48
			 0xe5d83dbd, /// 0xb4c
			 0x93ce2f01, /// 0xb50
			 0x2740ca01, /// 0xb54
			 0x4c38f05a, /// 0xb58
			 0xfcba9c17, /// 0xb5c
			 0x4d07a9e7, /// 0xb60
			 0x5255a410, /// 0xb64
			 0xf2581098, /// 0xb68
			 0xa4005c1e, /// 0xb6c
			 0xa0bd3379, /// 0xb70
			 0xd9ed1a52, /// 0xb74
			 0x0bccc3b0, /// 0xb78
			 0xd4dd9bf6, /// 0xb7c
			 0xe903036e, /// 0xb80
			 0x8867f79c, /// 0xb84
			 0x2e725579, /// 0xb88
			 0xf5c3e079, /// 0xb8c
			 0x8f55b042, /// 0xb90
			 0xf7be7fb9, /// 0xb94
			 0x283c64d2, /// 0xb98
			 0x577e307d, /// 0xb9c
			 0x40007560, /// 0xba0
			 0xf307874a, /// 0xba4
			 0x6682eeff, /// 0xba8
			 0x6fbb662f, /// 0xbac
			 0xd9f06903, /// 0xbb0
			 0xf7008b4e, /// 0xbb4
			 0xb5359130, /// 0xbb8
			 0x6cc2a30e, /// 0xbbc
			 0x158841bb, /// 0xbc0
			 0xdf3b785b, /// 0xbc4
			 0x15b6e522, /// 0xbc8
			 0x69a1109b, /// 0xbcc
			 0xc5198fe0, /// 0xbd0
			 0x772eec9a, /// 0xbd4
			 0xbcef119d, /// 0xbd8
			 0x12500e90, /// 0xbdc
			 0x59b4254b, /// 0xbe0
			 0xa02163ab, /// 0xbe4
			 0x0e2a8e04, /// 0xbe8
			 0xcbfe9808, /// 0xbec
			 0xaab7880b, /// 0xbf0
			 0x79cecc80, /// 0xbf4
			 0x623398f8, /// 0xbf8
			 0x48b1ea03, /// 0xbfc
			 0xa452c291, /// 0xc00
			 0x2cd8ed9b, /// 0xc04
			 0xd8b623b1, /// 0xc08
			 0x0922e11a, /// 0xc0c
			 0x00c3c66f, /// 0xc10
			 0xb1f1b4c8, /// 0xc14
			 0xd849dc3e, /// 0xc18
			 0x04775016, /// 0xc1c
			 0xc72f6efc, /// 0xc20
			 0x87de5df0, /// 0xc24
			 0x7e836151, /// 0xc28
			 0xdf672343, /// 0xc2c
			 0xce7ce387, /// 0xc30
			 0xb63738db, /// 0xc34
			 0xd44b15f3, /// 0xc38
			 0x2c957fe7, /// 0xc3c
			 0x4fa65448, /// 0xc40
			 0x217926b7, /// 0xc44
			 0xfe431bd8, /// 0xc48
			 0xa053cef7, /// 0xc4c
			 0xcf010d08, /// 0xc50
			 0x48794e31, /// 0xc54
			 0x4087af65, /// 0xc58
			 0x5828368d, /// 0xc5c
			 0x26d6a631, /// 0xc60
			 0x26cc0b87, /// 0xc64
			 0x6e7a28f8, /// 0xc68
			 0x82a36f58, /// 0xc6c
			 0xde4209e3, /// 0xc70
			 0xd89f536e, /// 0xc74
			 0xd648d6c6, /// 0xc78
			 0x77a10a39, /// 0xc7c
			 0x9fb98121, /// 0xc80
			 0x3881c14f, /// 0xc84
			 0xa8896d10, /// 0xc88
			 0x9cbd75d1, /// 0xc8c
			 0x17033629, /// 0xc90
			 0x9eb96733, /// 0xc94
			 0x35e36120, /// 0xc98
			 0x093b0036, /// 0xc9c
			 0xaed1a836, /// 0xca0
			 0xa5dc3461, /// 0xca4
			 0x46137c73, /// 0xca8
			 0xe0360a99, /// 0xcac
			 0x223e650a, /// 0xcb0
			 0xc0f28d58, /// 0xcb4
			 0xa1fa7bbe, /// 0xcb8
			 0xdb5d3f4f, /// 0xcbc
			 0xed836304, /// 0xcc0
			 0x4ba4a820, /// 0xcc4
			 0xfd7f79b3, /// 0xcc8
			 0x372d711d, /// 0xccc
			 0x0e006825, /// 0xcd0
			 0x37e813e0, /// 0xcd4
			 0x8e9e947e, /// 0xcd8
			 0x908b0da6, /// 0xcdc
			 0x31a70112, /// 0xce0
			 0xc04f0407, /// 0xce4
			 0xe332c0b6, /// 0xce8
			 0x5e895967, /// 0xcec
			 0xd4c43cf3, /// 0xcf0
			 0xa888d75c, /// 0xcf4
			 0x06f40dcf, /// 0xcf8
			 0x613e0d04, /// 0xcfc
			 0x229c3d46, /// 0xd00
			 0xf0eb8e13, /// 0xd04
			 0x144d23d1, /// 0xd08
			 0x419871a3, /// 0xd0c
			 0x0beb4a73, /// 0xd10
			 0xb1dcd292, /// 0xd14
			 0xfce73e3a, /// 0xd18
			 0x9fb9abec, /// 0xd1c
			 0x2037e242, /// 0xd20
			 0xc8d582bc, /// 0xd24
			 0x36fa380e, /// 0xd28
			 0x10fd8383, /// 0xd2c
			 0x6003fd66, /// 0xd30
			 0x8d42acb5, /// 0xd34
			 0xfc74a781, /// 0xd38
			 0x234155a3, /// 0xd3c
			 0x28b81d39, /// 0xd40
			 0xfcb2b7a6, /// 0xd44
			 0x3b6d21c9, /// 0xd48
			 0x6edd9c6e, /// 0xd4c
			 0x7e14d5ae, /// 0xd50
			 0x1347dcfe, /// 0xd54
			 0xf592a9a9, /// 0xd58
			 0x133b42ca, /// 0xd5c
			 0x5ec63c8c, /// 0xd60
			 0x1de41ca0, /// 0xd64
			 0xaba36cfc, /// 0xd68
			 0x2d6612b9, /// 0xd6c
			 0x90d920c3, /// 0xd70
			 0x258232b7, /// 0xd74
			 0xff11a69c, /// 0xd78
			 0x95a89eaf, /// 0xd7c
			 0xfaf684c9, /// 0xd80
			 0x9b81acfa, /// 0xd84
			 0x42498a4f, /// 0xd88
			 0x5a539b6e, /// 0xd8c
			 0x5cf36b63, /// 0xd90
			 0x09f64de8, /// 0xd94
			 0x6d2405a9, /// 0xd98
			 0x8e91c579, /// 0xd9c
			 0x03bd4f4f, /// 0xda0
			 0x1d2d1b44, /// 0xda4
			 0xdb74a9f7, /// 0xda8
			 0xe1598516, /// 0xdac
			 0x256d74fe, /// 0xdb0
			 0xd5018e1c, /// 0xdb4
			 0x441abfc6, /// 0xdb8
			 0x64040182, /// 0xdbc
			 0xada41fc2, /// 0xdc0
			 0x625b8366, /// 0xdc4
			 0x6902d58a, /// 0xdc8
			 0xe8345228, /// 0xdcc
			 0xb55e8abd, /// 0xdd0
			 0x8ce8fc71, /// 0xdd4
			 0xff43f028, /// 0xdd8
			 0xb25624cc, /// 0xddc
			 0xf2b0a577, /// 0xde0
			 0xc0df8428, /// 0xde4
			 0x5d2a4031, /// 0xde8
			 0x9215dcfe, /// 0xdec
			 0x6068a100, /// 0xdf0
			 0x83b5a27c, /// 0xdf4
			 0x535e4e75, /// 0xdf8
			 0x1ad4c89d, /// 0xdfc
			 0xcda12db9, /// 0xe00
			 0x7551551a, /// 0xe04
			 0xc367aa6b, /// 0xe08
			 0x0c440d63, /// 0xe0c
			 0x7e393d97, /// 0xe10
			 0x8749ad7b, /// 0xe14
			 0x541871ca, /// 0xe18
			 0xcd301bba, /// 0xe1c
			 0x636b1948, /// 0xe20
			 0xc3e8a636, /// 0xe24
			 0x7c667e48, /// 0xe28
			 0x3e243cab, /// 0xe2c
			 0x0aee380c, /// 0xe30
			 0x22d3fd0f, /// 0xe34
			 0x458eff2b, /// 0xe38
			 0xa41f91a1, /// 0xe3c
			 0x22bc7476, /// 0xe40
			 0xe058310e, /// 0xe44
			 0x5a2541e5, /// 0xe48
			 0x18e7d7be, /// 0xe4c
			 0x8e9b2b23, /// 0xe50
			 0x62846f71, /// 0xe54
			 0x3cc20d94, /// 0xe58
			 0x623845bc, /// 0xe5c
			 0xf3e47811, /// 0xe60
			 0x94048a2d, /// 0xe64
			 0x883bbf09, /// 0xe68
			 0xc9149da5, /// 0xe6c
			 0xf8d4a7fc, /// 0xe70
			 0xf91b31f4, /// 0xe74
			 0xc72e77b2, /// 0xe78
			 0x4e73caa9, /// 0xe7c
			 0x1ad89a04, /// 0xe80
			 0xcebfda30, /// 0xe84
			 0x1634858a, /// 0xe88
			 0x50a456d0, /// 0xe8c
			 0x1782183c, /// 0xe90
			 0x95380f0c, /// 0xe94
			 0x2607ae32, /// 0xe98
			 0x805a9082, /// 0xe9c
			 0x7231f125, /// 0xea0
			 0x2b9efcc6, /// 0xea4
			 0x8e50614c, /// 0xea8
			 0x8d3b4fd5, /// 0xeac
			 0xcabb3e4f, /// 0xeb0
			 0xfe5cf190, /// 0xeb4
			 0x8ab62d5b, /// 0xeb8
			 0xc9f7b386, /// 0xebc
			 0x5572b3a9, /// 0xec0
			 0xd9917505, /// 0xec4
			 0x7b9b8a85, /// 0xec8
			 0x10c4c045, /// 0xecc
			 0xd14a34f9, /// 0xed0
			 0xe11a2522, /// 0xed4
			 0x001161c8, /// 0xed8
			 0x402bea18, /// 0xedc
			 0xc9d5075d, /// 0xee0
			 0x0c530f3c, /// 0xee4
			 0x0be0f7b8, /// 0xee8
			 0xaea967e1, /// 0xeec
			 0x39894949, /// 0xef0
			 0x8f733e84, /// 0xef4
			 0x9140617f, /// 0xef8
			 0x777d5aa6, /// 0xefc
			 0xcc6c9a42, /// 0xf00
			 0x0c8c0bf6, /// 0xf04
			 0xf3612efd, /// 0xf08
			 0xdc28dcdb, /// 0xf0c
			 0xc07a0c41, /// 0xf10
			 0xa744be6c, /// 0xf14
			 0xe789d0cb, /// 0xf18
			 0x1424d973, /// 0xf1c
			 0xddb01fde, /// 0xf20
			 0xc2ea10ba, /// 0xf24
			 0xd9d3fbb7, /// 0xf28
			 0xe013d689, /// 0xf2c
			 0x12afe604, /// 0xf30
			 0x7c778d73, /// 0xf34
			 0x1f8a691d, /// 0xf38
			 0x73842f68, /// 0xf3c
			 0x6c7be5dd, /// 0xf40
			 0x5b10af8a, /// 0xf44
			 0xc3975b29, /// 0xf48
			 0x2ab19947, /// 0xf4c
			 0x58b64b36, /// 0xf50
			 0xab87cf5d, /// 0xf54
			 0xffadfdbd, /// 0xf58
			 0xcf80299e, /// 0xf5c
			 0xa3f94139, /// 0xf60
			 0x8d69f9e3, /// 0xf64
			 0x035fae91, /// 0xf68
			 0x71f4222c, /// 0xf6c
			 0x505baa2e, /// 0xf70
			 0xc0f136f0, /// 0xf74
			 0x90df035d, /// 0xf78
			 0x2d32861f, /// 0xf7c
			 0x5067296a, /// 0xf80
			 0x5d7fbdc3, /// 0xf84
			 0x2e5e6aac, /// 0xf88
			 0xe726d44a, /// 0xf8c
			 0xc4dca44e, /// 0xf90
			 0x8aa1f05b, /// 0xf94
			 0x96ef25bf, /// 0xf98
			 0x2d88f2b9, /// 0xf9c
			 0x77432dc9, /// 0xfa0
			 0x41d8c443, /// 0xfa4
			 0xf106ec5f, /// 0xfa8
			 0xd8985729, /// 0xfac
			 0xd66a5e4e, /// 0xfb0
			 0x69958a70, /// 0xfb4
			 0x32199985, /// 0xfb8
			 0x0bf6c6bd, /// 0xfbc
			 0xfc5657f4, /// 0xfc0
			 0x976cf509, /// 0xfc4
			 0x3eedb9ee, /// 0xfc8
			 0xcd023429, /// 0xfcc
			 0xfaeb6138, /// 0xfd0
			 0xa1a3665c, /// 0xfd4
			 0xcdcd1f8a, /// 0xfd8
			 0x869eb6a7, /// 0xfdc
			 0x8ab03b50, /// 0xfe0
			 0x0084f6d0, /// 0xfe4
			 0xc2dda82a, /// 0xfe8
			 0xa7673787, /// 0xfec
			 0x07785837, /// 0xff0
			 0x7e05b8af, /// 0xff4
			 0x4d141f94, /// 0xff8
			 0xc7a56ca2 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x66c326e9, /// 0x0
			 0x8e72c302, /// 0x4
			 0x1790bd66, /// 0x8
			 0x741234bd, /// 0xc
			 0x1d552dd7, /// 0x10
			 0x20c499de, /// 0x14
			 0x8acb4ede, /// 0x18
			 0x14492fdd, /// 0x1c
			 0x3a6c4ae9, /// 0x20
			 0x1094e12c, /// 0x24
			 0xbd03c538, /// 0x28
			 0xc8c06ff7, /// 0x2c
			 0x2588c42a, /// 0x30
			 0xfbbe6444, /// 0x34
			 0x1f1200e4, /// 0x38
			 0x523fc377, /// 0x3c
			 0xa1f58519, /// 0x40
			 0x1f02ee06, /// 0x44
			 0x5617aae3, /// 0x48
			 0xbb172004, /// 0x4c
			 0xf99a481c, /// 0x50
			 0x5c365c47, /// 0x54
			 0x067fd9ee, /// 0x58
			 0x87b2323a, /// 0x5c
			 0xafe5c8d7, /// 0x60
			 0xcaf43cba, /// 0x64
			 0xf4512836, /// 0x68
			 0x065a3e1d, /// 0x6c
			 0x8df1c5ed, /// 0x70
			 0x560380f9, /// 0x74
			 0xa7e3de0f, /// 0x78
			 0xbac705d5, /// 0x7c
			 0x2828b46a, /// 0x80
			 0xe41a3224, /// 0x84
			 0xa20fc6c1, /// 0x88
			 0x55783d2a, /// 0x8c
			 0x05e7071a, /// 0x90
			 0x38acec7a, /// 0x94
			 0xf74fa8fc, /// 0x98
			 0x7302270c, /// 0x9c
			 0x64e755e3, /// 0xa0
			 0xff96b451, /// 0xa4
			 0x61afa9b4, /// 0xa8
			 0x924d19ee, /// 0xac
			 0x5a654bee, /// 0xb0
			 0x82535492, /// 0xb4
			 0x2f4ecfc2, /// 0xb8
			 0x3bc3af4a, /// 0xbc
			 0x32d17cc3, /// 0xc0
			 0x53df9001, /// 0xc4
			 0x6760c3d4, /// 0xc8
			 0xfd3234be, /// 0xcc
			 0xbabfe0fc, /// 0xd0
			 0xa4ccf750, /// 0xd4
			 0x10695792, /// 0xd8
			 0xd5c03b8a, /// 0xdc
			 0x3b40b87a, /// 0xe0
			 0x40cf337b, /// 0xe4
			 0x7d68ada1, /// 0xe8
			 0xb82776b6, /// 0xec
			 0xb9dc49ef, /// 0xf0
			 0xe77f5e5c, /// 0xf4
			 0x2b27b539, /// 0xf8
			 0xca727c7d, /// 0xfc
			 0xcbbad66f, /// 0x100
			 0x10dac62b, /// 0x104
			 0x81bc4067, /// 0x108
			 0x67cf9ae6, /// 0x10c
			 0x86c33154, /// 0x110
			 0xcdcdcf26, /// 0x114
			 0x77b77035, /// 0x118
			 0x23cb2cab, /// 0x11c
			 0x3a640693, /// 0x120
			 0x75165be1, /// 0x124
			 0xbbcba6b3, /// 0x128
			 0xd756eac6, /// 0x12c
			 0x129591d3, /// 0x130
			 0x94588f1e, /// 0x134
			 0x7ea0875f, /// 0x138
			 0xd4338e0b, /// 0x13c
			 0xaea94a73, /// 0x140
			 0x48a9f88f, /// 0x144
			 0xe129c721, /// 0x148
			 0xa56873be, /// 0x14c
			 0x4b27b0af, /// 0x150
			 0x1fed3dc5, /// 0x154
			 0xde9476ab, /// 0x158
			 0x019c65a7, /// 0x15c
			 0x7bda8d61, /// 0x160
			 0x393f7cbd, /// 0x164
			 0xd47e9bfa, /// 0x168
			 0x37bbf0a0, /// 0x16c
			 0x517fc17a, /// 0x170
			 0x98852179, /// 0x174
			 0xfc222062, /// 0x178
			 0xa41fe112, /// 0x17c
			 0x59cc6ee1, /// 0x180
			 0x33ea1aee, /// 0x184
			 0x793c8eba, /// 0x188
			 0xfe72bfb1, /// 0x18c
			 0x596eeb54, /// 0x190
			 0xb17cf245, /// 0x194
			 0xdd9a5a40, /// 0x198
			 0x104bf4af, /// 0x19c
			 0x249dd0dc, /// 0x1a0
			 0xcfab32e7, /// 0x1a4
			 0x8f034d81, /// 0x1a8
			 0x2e3cb6a4, /// 0x1ac
			 0x284cf492, /// 0x1b0
			 0xca9d591a, /// 0x1b4
			 0x1cc9d066, /// 0x1b8
			 0x6ede1c0b, /// 0x1bc
			 0xafa147e8, /// 0x1c0
			 0xb386ceac, /// 0x1c4
			 0xfb23e970, /// 0x1c8
			 0x0df1f231, /// 0x1cc
			 0x0ef94800, /// 0x1d0
			 0x77370a08, /// 0x1d4
			 0xa4e18438, /// 0x1d8
			 0xf97acc5d, /// 0x1dc
			 0x0a377d45, /// 0x1e0
			 0xcd233e89, /// 0x1e4
			 0xca68e63e, /// 0x1e8
			 0x2fbe43df, /// 0x1ec
			 0x944e51c1, /// 0x1f0
			 0x682f36a2, /// 0x1f4
			 0x0269a7b7, /// 0x1f8
			 0x5d86be94, /// 0x1fc
			 0x9e794b89, /// 0x200
			 0x30e60338, /// 0x204
			 0x273e45ca, /// 0x208
			 0xc5d68163, /// 0x20c
			 0x93a99410, /// 0x210
			 0x559f888c, /// 0x214
			 0x6713b9a0, /// 0x218
			 0x4dabac83, /// 0x21c
			 0x3bc55781, /// 0x220
			 0x0f67b9be, /// 0x224
			 0x946c03bd, /// 0x228
			 0xd5f5056c, /// 0x22c
			 0x061f7817, /// 0x230
			 0x82e47e59, /// 0x234
			 0xf8e627e2, /// 0x238
			 0xfcdf20cf, /// 0x23c
			 0x6baced56, /// 0x240
			 0xb46d3740, /// 0x244
			 0x740654a2, /// 0x248
			 0xde0999a9, /// 0x24c
			 0x913745fa, /// 0x250
			 0x32f9fd74, /// 0x254
			 0xbe8f2921, /// 0x258
			 0x59ea1fd0, /// 0x25c
			 0xfabcc7e2, /// 0x260
			 0xa0b6ea3c, /// 0x264
			 0x9b5a0bb6, /// 0x268
			 0x10c96754, /// 0x26c
			 0x606d32c0, /// 0x270
			 0x0bdea851, /// 0x274
			 0xdb11042b, /// 0x278
			 0x622da931, /// 0x27c
			 0x113f252a, /// 0x280
			 0x8ac93411, /// 0x284
			 0xb2a99a4a, /// 0x288
			 0x5d95399c, /// 0x28c
			 0x4c42c5d9, /// 0x290
			 0x28aab39b, /// 0x294
			 0xeca3add9, /// 0x298
			 0x38bc6b24, /// 0x29c
			 0x085635c9, /// 0x2a0
			 0xf62e4725, /// 0x2a4
			 0x329614da, /// 0x2a8
			 0xf656e9dd, /// 0x2ac
			 0x8d04daa9, /// 0x2b0
			 0x02621e03, /// 0x2b4
			 0x67918952, /// 0x2b8
			 0xf9a19b57, /// 0x2bc
			 0x910d5a5e, /// 0x2c0
			 0x881e8396, /// 0x2c4
			 0x54e5c40a, /// 0x2c8
			 0xbdd514dc, /// 0x2cc
			 0x430db779, /// 0x2d0
			 0x512b909e, /// 0x2d4
			 0x185d704d, /// 0x2d8
			 0xb799795e, /// 0x2dc
			 0x17320fe7, /// 0x2e0
			 0x4068647b, /// 0x2e4
			 0x1a032edf, /// 0x2e8
			 0x8d0753fa, /// 0x2ec
			 0x573d0588, /// 0x2f0
			 0xbc90c96c, /// 0x2f4
			 0xf45d5de3, /// 0x2f8
			 0x824280b1, /// 0x2fc
			 0xd8af977f, /// 0x300
			 0x631f9c0d, /// 0x304
			 0x59476dc1, /// 0x308
			 0x215b5b7d, /// 0x30c
			 0xd5f3c6c9, /// 0x310
			 0x49e9a306, /// 0x314
			 0xb35c1a6d, /// 0x318
			 0xa94a4ec8, /// 0x31c
			 0x48e852dd, /// 0x320
			 0xb87fdc3e, /// 0x324
			 0x9576352d, /// 0x328
			 0x86299929, /// 0x32c
			 0x841b874d, /// 0x330
			 0x626eef48, /// 0x334
			 0x9754c8ed, /// 0x338
			 0x855bc4e5, /// 0x33c
			 0xc99c4efe, /// 0x340
			 0xfd9c5b66, /// 0x344
			 0x65a4b64a, /// 0x348
			 0x9cd64a8a, /// 0x34c
			 0xba36b089, /// 0x350
			 0x447fd98b, /// 0x354
			 0x5e0a63c7, /// 0x358
			 0xb3ee0cea, /// 0x35c
			 0x4490be4a, /// 0x360
			 0x4f7d6ec6, /// 0x364
			 0xa6b2da74, /// 0x368
			 0x30e3e63f, /// 0x36c
			 0x2e9ea664, /// 0x370
			 0xd5c2f52f, /// 0x374
			 0xcf93f889, /// 0x378
			 0xb393db46, /// 0x37c
			 0xe038070c, /// 0x380
			 0xdb20de09, /// 0x384
			 0x37def20a, /// 0x388
			 0xc8331b89, /// 0x38c
			 0xd742327b, /// 0x390
			 0xf7c09c31, /// 0x394
			 0x70692ffa, /// 0x398
			 0xc3748509, /// 0x39c
			 0xdf02eb6b, /// 0x3a0
			 0xb85ba8eb, /// 0x3a4
			 0x43ca4a4d, /// 0x3a8
			 0x351d8843, /// 0x3ac
			 0xa8d03d51, /// 0x3b0
			 0xc6cc0d98, /// 0x3b4
			 0x8ea315a1, /// 0x3b8
			 0x868f4d7c, /// 0x3bc
			 0xea8a957e, /// 0x3c0
			 0x4a15f045, /// 0x3c4
			 0xaa366244, /// 0x3c8
			 0x2516841e, /// 0x3cc
			 0x68bec6d6, /// 0x3d0
			 0x6510677a, /// 0x3d4
			 0x896337cb, /// 0x3d8
			 0x384f8996, /// 0x3dc
			 0x209c8f99, /// 0x3e0
			 0xd7742e74, /// 0x3e4
			 0xfa400e84, /// 0x3e8
			 0xd9c32daa, /// 0x3ec
			 0xedc593ac, /// 0x3f0
			 0xe3b69bee, /// 0x3f4
			 0x0ea908fc, /// 0x3f8
			 0x47f233e1, /// 0x3fc
			 0x6f5438d3, /// 0x400
			 0x63c89cac, /// 0x404
			 0xe48535c8, /// 0x408
			 0x72b03b74, /// 0x40c
			 0x49b9377f, /// 0x410
			 0x657b4a6e, /// 0x414
			 0xfad7c306, /// 0x418
			 0xfe84f73a, /// 0x41c
			 0x43010003, /// 0x420
			 0xc88263ff, /// 0x424
			 0xf9d9b97e, /// 0x428
			 0x6079dc46, /// 0x42c
			 0xf54287d9, /// 0x430
			 0xbc0f5b42, /// 0x434
			 0xe4d04fca, /// 0x438
			 0x5d644501, /// 0x43c
			 0x2114a1ea, /// 0x440
			 0x7e81c6bd, /// 0x444
			 0x436ff374, /// 0x448
			 0xdbb1d4ef, /// 0x44c
			 0xbaf762e9, /// 0x450
			 0x1fa1b666, /// 0x454
			 0x6d47a3c2, /// 0x458
			 0x59ef49b1, /// 0x45c
			 0x89cdfdd1, /// 0x460
			 0xfa63aa14, /// 0x464
			 0x1c20fcaa, /// 0x468
			 0x687a4c3d, /// 0x46c
			 0x1dfe9a50, /// 0x470
			 0xb6c7f233, /// 0x474
			 0x05b2df6c, /// 0x478
			 0xaf50a71d, /// 0x47c
			 0xf2a8e4e8, /// 0x480
			 0x284c79c9, /// 0x484
			 0x6062c180, /// 0x488
			 0x65f45698, /// 0x48c
			 0x3142a01c, /// 0x490
			 0x3d402d74, /// 0x494
			 0xd5185c0e, /// 0x498
			 0xc4b9f3e5, /// 0x49c
			 0x5ac2f054, /// 0x4a0
			 0xfea77546, /// 0x4a4
			 0x556e9aef, /// 0x4a8
			 0x5f902c42, /// 0x4ac
			 0x761b18bf, /// 0x4b0
			 0xe15665c2, /// 0x4b4
			 0xcdcdae59, /// 0x4b8
			 0xe68ce934, /// 0x4bc
			 0x8393f8db, /// 0x4c0
			 0x3ef359e2, /// 0x4c4
			 0xa371bae5, /// 0x4c8
			 0x7b876829, /// 0x4cc
			 0x8a938ea3, /// 0x4d0
			 0x4497300a, /// 0x4d4
			 0xb3ee9220, /// 0x4d8
			 0x32fc8bbc, /// 0x4dc
			 0xfd9e5bc9, /// 0x4e0
			 0x34a1116f, /// 0x4e4
			 0xc691c1a2, /// 0x4e8
			 0x242a95a6, /// 0x4ec
			 0xb61f002d, /// 0x4f0
			 0x974ad608, /// 0x4f4
			 0xc255e34c, /// 0x4f8
			 0x3cf38147, /// 0x4fc
			 0x48b355e8, /// 0x500
			 0x6a1fd9b8, /// 0x504
			 0x7d3a4f53, /// 0x508
			 0xb82617a2, /// 0x50c
			 0xe4c63773, /// 0x510
			 0x923fb76e, /// 0x514
			 0xe02edf58, /// 0x518
			 0x6aff369c, /// 0x51c
			 0x6305d04d, /// 0x520
			 0x344df9bf, /// 0x524
			 0x60991312, /// 0x528
			 0xc8d060c4, /// 0x52c
			 0x7f56c7f6, /// 0x530
			 0x913cf082, /// 0x534
			 0x55ead162, /// 0x538
			 0xc08f4729, /// 0x53c
			 0x869f1157, /// 0x540
			 0x622bd629, /// 0x544
			 0x776ba0ca, /// 0x548
			 0x74b41c31, /// 0x54c
			 0x623378ba, /// 0x550
			 0xbd5ccd15, /// 0x554
			 0x79bdc040, /// 0x558
			 0xfb87b963, /// 0x55c
			 0x1c55016a, /// 0x560
			 0xd6eda401, /// 0x564
			 0x5b4c9ec9, /// 0x568
			 0x7f71bc5c, /// 0x56c
			 0xbadc0819, /// 0x570
			 0xa99e82e9, /// 0x574
			 0x4807a6a5, /// 0x578
			 0xd9754fa7, /// 0x57c
			 0x8f7ab2d7, /// 0x580
			 0x586d9855, /// 0x584
			 0x6f36611b, /// 0x588
			 0xa704a73e, /// 0x58c
			 0x2db32e2e, /// 0x590
			 0x1efedaef, /// 0x594
			 0xfe1959fa, /// 0x598
			 0x0df2ef69, /// 0x59c
			 0x4be1cc41, /// 0x5a0
			 0x15cef244, /// 0x5a4
			 0x5313b650, /// 0x5a8
			 0x432f11bc, /// 0x5ac
			 0xd409c5d4, /// 0x5b0
			 0xb5c7686e, /// 0x5b4
			 0xc6611404, /// 0x5b8
			 0xfdc8a36f, /// 0x5bc
			 0x237a863a, /// 0x5c0
			 0x062e46b4, /// 0x5c4
			 0x8e5e6780, /// 0x5c8
			 0x27a0f635, /// 0x5cc
			 0xcb1f386f, /// 0x5d0
			 0x3a617e39, /// 0x5d4
			 0x1e1b5545, /// 0x5d8
			 0x1b1fdd2a, /// 0x5dc
			 0x890cfa69, /// 0x5e0
			 0xd2792b05, /// 0x5e4
			 0x5917e9fb, /// 0x5e8
			 0xaf79b9fe, /// 0x5ec
			 0x46810425, /// 0x5f0
			 0x98b6df89, /// 0x5f4
			 0xb82a328e, /// 0x5f8
			 0x53f0dded, /// 0x5fc
			 0x8d38170a, /// 0x600
			 0xf3a6269e, /// 0x604
			 0xdd66c876, /// 0x608
			 0xeebfa8b8, /// 0x60c
			 0xca757452, /// 0x610
			 0xc32ac2b2, /// 0x614
			 0x89b117f4, /// 0x618
			 0x2b9c8a2f, /// 0x61c
			 0x25062575, /// 0x620
			 0x4e71c0d8, /// 0x624
			 0xa3a34e52, /// 0x628
			 0xc29b2bd3, /// 0x62c
			 0x716738d3, /// 0x630
			 0xbb825c0f, /// 0x634
			 0xfa9fa861, /// 0x638
			 0x561db3d1, /// 0x63c
			 0xa20d1adc, /// 0x640
			 0xecbb02df, /// 0x644
			 0xd0a147bd, /// 0x648
			 0xd3ca9c9f, /// 0x64c
			 0x575ef870, /// 0x650
			 0xbb3139f6, /// 0x654
			 0xe605b519, /// 0x658
			 0x702cc8a6, /// 0x65c
			 0x7d36b5c6, /// 0x660
			 0x933607ef, /// 0x664
			 0x48dfc909, /// 0x668
			 0xf655ec08, /// 0x66c
			 0x774e07dd, /// 0x670
			 0xf265c1bf, /// 0x674
			 0x7456a4d0, /// 0x678
			 0x3fc8a173, /// 0x67c
			 0xbe18f81f, /// 0x680
			 0x3ae1006d, /// 0x684
			 0xefd118da, /// 0x688
			 0xbdab3888, /// 0x68c
			 0x12fb009d, /// 0x690
			 0x7c376be3, /// 0x694
			 0xb17dd5b9, /// 0x698
			 0x3ff25e12, /// 0x69c
			 0xf61e6508, /// 0x6a0
			 0x40123a0c, /// 0x6a4
			 0x6b7ebd82, /// 0x6a8
			 0x94ac0d23, /// 0x6ac
			 0xbfa67a4c, /// 0x6b0
			 0x5fd37448, /// 0x6b4
			 0xe3694557, /// 0x6b8
			 0xeb0531a6, /// 0x6bc
			 0x75293682, /// 0x6c0
			 0xc1fa191b, /// 0x6c4
			 0x3eae0304, /// 0x6c8
			 0x7b7d1156, /// 0x6cc
			 0xf15c60b8, /// 0x6d0
			 0xe9f3d753, /// 0x6d4
			 0x19900b1a, /// 0x6d8
			 0xdf6ae29d, /// 0x6dc
			 0x3ccbe322, /// 0x6e0
			 0x84c9a1e1, /// 0x6e4
			 0x7fa945cd, /// 0x6e8
			 0x2f8da8aa, /// 0x6ec
			 0xe0c13b4e, /// 0x6f0
			 0x695ad6a5, /// 0x6f4
			 0x7452704f, /// 0x6f8
			 0x5e11c773, /// 0x6fc
			 0x0c37ef0b, /// 0x700
			 0xaa660e07, /// 0x704
			 0xfaea4fa5, /// 0x708
			 0xf7bb959c, /// 0x70c
			 0xb5fcc51c, /// 0x710
			 0xfcc8e93a, /// 0x714
			 0x990d92f1, /// 0x718
			 0x7be39500, /// 0x71c
			 0x644ad24f, /// 0x720
			 0x80d032d0, /// 0x724
			 0x061a06d0, /// 0x728
			 0xe40ce473, /// 0x72c
			 0xdbe9e36c, /// 0x730
			 0xb7342d2b, /// 0x734
			 0xf7b1eb01, /// 0x738
			 0x94783267, /// 0x73c
			 0x2c413628, /// 0x740
			 0x119ed1ac, /// 0x744
			 0xa840acef, /// 0x748
			 0x50343ebe, /// 0x74c
			 0x4a392c97, /// 0x750
			 0x8dd6994e, /// 0x754
			 0x73eb2dbf, /// 0x758
			 0xf2dcf8ed, /// 0x75c
			 0xe3baf7e5, /// 0x760
			 0x9046e646, /// 0x764
			 0x8178b307, /// 0x768
			 0xb47402c1, /// 0x76c
			 0xf95dedef, /// 0x770
			 0x1b668a62, /// 0x774
			 0x792bacb6, /// 0x778
			 0xe641840d, /// 0x77c
			 0x40084a50, /// 0x780
			 0x54b676c4, /// 0x784
			 0x025c2bf8, /// 0x788
			 0x6d3d5448, /// 0x78c
			 0x106ecc35, /// 0x790
			 0x92552050, /// 0x794
			 0x233f411f, /// 0x798
			 0x730858d1, /// 0x79c
			 0x7d2303c9, /// 0x7a0
			 0x14072118, /// 0x7a4
			 0x13a3710c, /// 0x7a8
			 0x305ba8bb, /// 0x7ac
			 0x15003c6f, /// 0x7b0
			 0x281e4d2b, /// 0x7b4
			 0xa13b340c, /// 0x7b8
			 0xdee74b41, /// 0x7bc
			 0x47c503ef, /// 0x7c0
			 0x6926b38c, /// 0x7c4
			 0xb9ba7818, /// 0x7c8
			 0xee8e08d8, /// 0x7cc
			 0x1b412715, /// 0x7d0
			 0xcab49c72, /// 0x7d4
			 0xa7d5f1f7, /// 0x7d8
			 0x5513e16e, /// 0x7dc
			 0x958c4a63, /// 0x7e0
			 0xf2fa996a, /// 0x7e4
			 0xa5017808, /// 0x7e8
			 0xac132011, /// 0x7ec
			 0x8814c334, /// 0x7f0
			 0xc276065a, /// 0x7f4
			 0xed49b73e, /// 0x7f8
			 0xb767d52c, /// 0x7fc
			 0xc575a986, /// 0x800
			 0xdb6dc906, /// 0x804
			 0xfaacd492, /// 0x808
			 0x7109d9c1, /// 0x80c
			 0xb809cc66, /// 0x810
			 0xef0bb3e6, /// 0x814
			 0x459eb554, /// 0x818
			 0x4dced3ab, /// 0x81c
			 0x1c960c87, /// 0x820
			 0x4bb9e697, /// 0x824
			 0xb6d282a3, /// 0x828
			 0xbaed07b0, /// 0x82c
			 0xec24873a, /// 0x830
			 0xc5916dc8, /// 0x834
			 0xa25a9f3f, /// 0x838
			 0x6cac40fa, /// 0x83c
			 0x4ab11e78, /// 0x840
			 0x9662d85a, /// 0x844
			 0x490722f5, /// 0x848
			 0x58827015, /// 0x84c
			 0x621278f6, /// 0x850
			 0x8bb12da3, /// 0x854
			 0xc32f8312, /// 0x858
			 0x511c6bd1, /// 0x85c
			 0x920b61a7, /// 0x860
			 0x7c9c90e6, /// 0x864
			 0x0ebfe3b5, /// 0x868
			 0x8ea2597f, /// 0x86c
			 0x75a3bbc1, /// 0x870
			 0x7e6f626b, /// 0x874
			 0xb616adf1, /// 0x878
			 0xcb16f267, /// 0x87c
			 0x497a967b, /// 0x880
			 0x87d71878, /// 0x884
			 0x976afbdf, /// 0x888
			 0xd4c1976a, /// 0x88c
			 0x18241bf5, /// 0x890
			 0xc9d2534a, /// 0x894
			 0xc993eea8, /// 0x898
			 0xc22b7ef6, /// 0x89c
			 0xee131e3c, /// 0x8a0
			 0x84edd109, /// 0x8a4
			 0x18192622, /// 0x8a8
			 0x2909d723, /// 0x8ac
			 0x1c71c6eb, /// 0x8b0
			 0xac023746, /// 0x8b4
			 0xc85f8347, /// 0x8b8
			 0xac8d9e71, /// 0x8bc
			 0xf6430419, /// 0x8c0
			 0x4b2170a0, /// 0x8c4
			 0x16edbf0b, /// 0x8c8
			 0x24d81d9a, /// 0x8cc
			 0x388d585e, /// 0x8d0
			 0x2da30792, /// 0x8d4
			 0xf941da65, /// 0x8d8
			 0xb429a796, /// 0x8dc
			 0x79d92290, /// 0x8e0
			 0x6de92dcb, /// 0x8e4
			 0x60649196, /// 0x8e8
			 0x54aa1aac, /// 0x8ec
			 0x8fda76ff, /// 0x8f0
			 0x9b151ab2, /// 0x8f4
			 0x837e8b2a, /// 0x8f8
			 0x239717e6, /// 0x8fc
			 0x503efbac, /// 0x900
			 0x75f5e65c, /// 0x904
			 0xbfd65227, /// 0x908
			 0x02de3661, /// 0x90c
			 0xebb35b4c, /// 0x910
			 0x5337ba9e, /// 0x914
			 0x240ff2f2, /// 0x918
			 0x3bb247e5, /// 0x91c
			 0xc7bca7b5, /// 0x920
			 0x79a7d08b, /// 0x924
			 0xe08065c0, /// 0x928
			 0x316783c5, /// 0x92c
			 0xa9c8227c, /// 0x930
			 0xb9686d70, /// 0x934
			 0xc1d9b9f5, /// 0x938
			 0x1a64e806, /// 0x93c
			 0x8f9f0012, /// 0x940
			 0x7e0770e3, /// 0x944
			 0x1d8d6c2b, /// 0x948
			 0xe2c9b03d, /// 0x94c
			 0x0a30758f, /// 0x950
			 0x2c44fe7a, /// 0x954
			 0xce87288c, /// 0x958
			 0xf7fd43e3, /// 0x95c
			 0x767028f8, /// 0x960
			 0x8f68ebfe, /// 0x964
			 0x8426aeb1, /// 0x968
			 0x79e55911, /// 0x96c
			 0x4be894a6, /// 0x970
			 0x57810de8, /// 0x974
			 0x754fa02b, /// 0x978
			 0x28c5f341, /// 0x97c
			 0x4ccccae5, /// 0x980
			 0x3261e1a6, /// 0x984
			 0x3f7cb995, /// 0x988
			 0x1428b37c, /// 0x98c
			 0x9939abe2, /// 0x990
			 0x0066d8a6, /// 0x994
			 0xa828dd44, /// 0x998
			 0x91b70f30, /// 0x99c
			 0x2694eda3, /// 0x9a0
			 0xae827c9d, /// 0x9a4
			 0x2a5bd2e1, /// 0x9a8
			 0x9400c71e, /// 0x9ac
			 0x4ff3bb59, /// 0x9b0
			 0x9c760293, /// 0x9b4
			 0xcefc1904, /// 0x9b8
			 0x36a9f075, /// 0x9bc
			 0x1fbf9a16, /// 0x9c0
			 0x9f44fa1f, /// 0x9c4
			 0x0755867c, /// 0x9c8
			 0xe87022dd, /// 0x9cc
			 0x8e412d16, /// 0x9d0
			 0x9e09ac88, /// 0x9d4
			 0xb6e49707, /// 0x9d8
			 0xeb7f237e, /// 0x9dc
			 0xd06968db, /// 0x9e0
			 0xd729d4f5, /// 0x9e4
			 0x47bcdc95, /// 0x9e8
			 0xb83f8091, /// 0x9ec
			 0xd280f44d, /// 0x9f0
			 0x59390955, /// 0x9f4
			 0xdb412390, /// 0x9f8
			 0xabd6034c, /// 0x9fc
			 0x8783d944, /// 0xa00
			 0x492c3450, /// 0xa04
			 0xd43384da, /// 0xa08
			 0x4e2b5902, /// 0xa0c
			 0x3956160d, /// 0xa10
			 0x89ae72ce, /// 0xa14
			 0x838a2123, /// 0xa18
			 0x44b8aa0a, /// 0xa1c
			 0xc95913ad, /// 0xa20
			 0xa4d337e0, /// 0xa24
			 0x1098db7c, /// 0xa28
			 0x829ee3ac, /// 0xa2c
			 0x1027c379, /// 0xa30
			 0x73ff9417, /// 0xa34
			 0x24cf9216, /// 0xa38
			 0x1cd1e9aa, /// 0xa3c
			 0xe2a28d4b, /// 0xa40
			 0x8b004e57, /// 0xa44
			 0xc57435cc, /// 0xa48
			 0x1cc67205, /// 0xa4c
			 0x42b0eae8, /// 0xa50
			 0x28fab1b2, /// 0xa54
			 0x57471e1a, /// 0xa58
			 0xafe2bfb5, /// 0xa5c
			 0x32bb6b66, /// 0xa60
			 0xbb581eba, /// 0xa64
			 0xb6267ae1, /// 0xa68
			 0xf84d617b, /// 0xa6c
			 0xcf7b85fc, /// 0xa70
			 0xc800db96, /// 0xa74
			 0x9983b52e, /// 0xa78
			 0x8d56ff0b, /// 0xa7c
			 0x5fe5d451, /// 0xa80
			 0x467d3e71, /// 0xa84
			 0xf77ae9fb, /// 0xa88
			 0xc9e24c79, /// 0xa8c
			 0x4a650796, /// 0xa90
			 0x733a0449, /// 0xa94
			 0x403b11e7, /// 0xa98
			 0x180761a9, /// 0xa9c
			 0xdd951d18, /// 0xaa0
			 0x390fb4a9, /// 0xaa4
			 0xc6981c50, /// 0xaa8
			 0x57cf493b, /// 0xaac
			 0x0ab2c35c, /// 0xab0
			 0x1ead4bda, /// 0xab4
			 0x6c821422, /// 0xab8
			 0x3f24c58b, /// 0xabc
			 0x28ead6e0, /// 0xac0
			 0xa852a492, /// 0xac4
			 0x929fd3ce, /// 0xac8
			 0x5a683b04, /// 0xacc
			 0x0346db69, /// 0xad0
			 0xbdd978ea, /// 0xad4
			 0x164bc29a, /// 0xad8
			 0x77154378, /// 0xadc
			 0xd62a33c9, /// 0xae0
			 0x23e50fb6, /// 0xae4
			 0xf87f76f3, /// 0xae8
			 0xefae63cb, /// 0xaec
			 0xab8a4036, /// 0xaf0
			 0xd8fc5e12, /// 0xaf4
			 0xb5e5c01e, /// 0xaf8
			 0x26f76944, /// 0xafc
			 0x005467fe, /// 0xb00
			 0x9a227420, /// 0xb04
			 0x5a8f8c0b, /// 0xb08
			 0x49edb1b6, /// 0xb0c
			 0xcc9ac2d7, /// 0xb10
			 0x54e8c47c, /// 0xb14
			 0xc877c836, /// 0xb18
			 0x54cf2542, /// 0xb1c
			 0xf8c7c28b, /// 0xb20
			 0x1cbb0cba, /// 0xb24
			 0x9fab6130, /// 0xb28
			 0x87a2ce4b, /// 0xb2c
			 0x8aa93ffd, /// 0xb30
			 0x75c3912c, /// 0xb34
			 0x24ff238d, /// 0xb38
			 0x2466f763, /// 0xb3c
			 0xd7e2c120, /// 0xb40
			 0x80c0a5f8, /// 0xb44
			 0x02ce3f30, /// 0xb48
			 0x0367e8a8, /// 0xb4c
			 0xacd86766, /// 0xb50
			 0x20a098f1, /// 0xb54
			 0x9e5f6432, /// 0xb58
			 0x15160b02, /// 0xb5c
			 0x0b00a6ff, /// 0xb60
			 0x53aff712, /// 0xb64
			 0xa12612a5, /// 0xb68
			 0xa07e1092, /// 0xb6c
			 0xa6704312, /// 0xb70
			 0xfca1bb9a, /// 0xb74
			 0xf35e4bbc, /// 0xb78
			 0x69beb9f8, /// 0xb7c
			 0x10b45ed4, /// 0xb80
			 0x8da2faf6, /// 0xb84
			 0xf5e4b808, /// 0xb88
			 0xc46f4d42, /// 0xb8c
			 0x8f008836, /// 0xb90
			 0x609f4d2d, /// 0xb94
			 0x80b3e6cb, /// 0xb98
			 0x1503c43b, /// 0xb9c
			 0x3b629675, /// 0xba0
			 0x3a7e370a, /// 0xba4
			 0xe6308897, /// 0xba8
			 0xd0c4c446, /// 0xbac
			 0xde2b8bf9, /// 0xbb0
			 0x190e31a9, /// 0xbb4
			 0xe4e52858, /// 0xbb8
			 0x09c57d50, /// 0xbbc
			 0xdb43f24b, /// 0xbc0
			 0xc9894a52, /// 0xbc4
			 0x04373330, /// 0xbc8
			 0x8c4cd46d, /// 0xbcc
			 0xc4e68ec8, /// 0xbd0
			 0x779134cb, /// 0xbd4
			 0xd4ba65ac, /// 0xbd8
			 0x54353078, /// 0xbdc
			 0xe11918c7, /// 0xbe0
			 0xc9fac30a, /// 0xbe4
			 0xce2f7cf2, /// 0xbe8
			 0x5367c459, /// 0xbec
			 0x8b19f4e9, /// 0xbf0
			 0xac41c4f1, /// 0xbf4
			 0xa9c4f913, /// 0xbf8
			 0x0b1b13f4, /// 0xbfc
			 0x6312aebb, /// 0xc00
			 0x42738170, /// 0xc04
			 0x89d653b8, /// 0xc08
			 0xaadb54e4, /// 0xc0c
			 0x10eb8cc2, /// 0xc10
			 0x0581c58b, /// 0xc14
			 0xcf94ba1b, /// 0xc18
			 0xdd56ef4e, /// 0xc1c
			 0xe8666514, /// 0xc20
			 0xeb35cd29, /// 0xc24
			 0x047a3969, /// 0xc28
			 0x218b9e4a, /// 0xc2c
			 0x6a166eb6, /// 0xc30
			 0xdaab8f99, /// 0xc34
			 0x01bf5a5e, /// 0xc38
			 0x78d2dc2b, /// 0xc3c
			 0x5602aeec, /// 0xc40
			 0x7bdea29e, /// 0xc44
			 0x5ab9fa01, /// 0xc48
			 0x1bdfaa1a, /// 0xc4c
			 0x6652730e, /// 0xc50
			 0x25295896, /// 0xc54
			 0xb098bc6e, /// 0xc58
			 0xfbeec5a6, /// 0xc5c
			 0x76c162e5, /// 0xc60
			 0xaeffd162, /// 0xc64
			 0x3c416bca, /// 0xc68
			 0x1444f71e, /// 0xc6c
			 0x43643741, /// 0xc70
			 0x946f02e6, /// 0xc74
			 0x1e545ca2, /// 0xc78
			 0x9b96b29d, /// 0xc7c
			 0x6220895f, /// 0xc80
			 0x33761ee9, /// 0xc84
			 0x3e8c581c, /// 0xc88
			 0x3daffad8, /// 0xc8c
			 0x4eb9763b, /// 0xc90
			 0x5d8e6889, /// 0xc94
			 0x95301c3b, /// 0xc98
			 0x2b3e5d1f, /// 0xc9c
			 0x1b9ad169, /// 0xca0
			 0xf6992751, /// 0xca4
			 0xede438fa, /// 0xca8
			 0x6d24adba, /// 0xcac
			 0xc38cfa1b, /// 0xcb0
			 0xa3614fb4, /// 0xcb4
			 0x1b307833, /// 0xcb8
			 0xa32daa54, /// 0xcbc
			 0xcfcf9b49, /// 0xcc0
			 0x3e34c3c3, /// 0xcc4
			 0x3186f653, /// 0xcc8
			 0xd6bca97d, /// 0xccc
			 0xbaae7b5b, /// 0xcd0
			 0xfa8cf51e, /// 0xcd4
			 0x26fdb450, /// 0xcd8
			 0x83cbecb3, /// 0xcdc
			 0x37580c0d, /// 0xce0
			 0x15d94a8f, /// 0xce4
			 0x2278dbe3, /// 0xce8
			 0x37e13df0, /// 0xcec
			 0xafb28e29, /// 0xcf0
			 0x809f516c, /// 0xcf4
			 0x26464746, /// 0xcf8
			 0x0f1c7ba7, /// 0xcfc
			 0x501f057f, /// 0xd00
			 0x9ed3ebea, /// 0xd04
			 0x74e4ed2c, /// 0xd08
			 0xd402f7a0, /// 0xd0c
			 0x970912eb, /// 0xd10
			 0x459ec87a, /// 0xd14
			 0xac425c2b, /// 0xd18
			 0x4466483d, /// 0xd1c
			 0x99850cfc, /// 0xd20
			 0xeef8f850, /// 0xd24
			 0x35976547, /// 0xd28
			 0x895d8fe8, /// 0xd2c
			 0x60b57092, /// 0xd30
			 0x065f0e48, /// 0xd34
			 0x576bed98, /// 0xd38
			 0xa8c0d52a, /// 0xd3c
			 0x2fa00a53, /// 0xd40
			 0xa0fe286e, /// 0xd44
			 0xbb4a99bb, /// 0xd48
			 0x731ab276, /// 0xd4c
			 0xb04fb4b6, /// 0xd50
			 0xa6fbdd0c, /// 0xd54
			 0x5fa66a08, /// 0xd58
			 0x9bb30024, /// 0xd5c
			 0x85874969, /// 0xd60
			 0xc338184f, /// 0xd64
			 0xb7060c49, /// 0xd68
			 0xefc0a1f3, /// 0xd6c
			 0xdccdfeea, /// 0xd70
			 0x8ca2cee6, /// 0xd74
			 0x7d6f7413, /// 0xd78
			 0xe4e4cb74, /// 0xd7c
			 0x75bea235, /// 0xd80
			 0x6d401b71, /// 0xd84
			 0x585ef401, /// 0xd88
			 0x24fdcf68, /// 0xd8c
			 0x45cb66fb, /// 0xd90
			 0x6c5567c8, /// 0xd94
			 0xc30bf39a, /// 0xd98
			 0x1779cb84, /// 0xd9c
			 0x8d17363d, /// 0xda0
			 0x1029f7c7, /// 0xda4
			 0xf8688415, /// 0xda8
			 0xe412aaa9, /// 0xdac
			 0xe2ea6a93, /// 0xdb0
			 0x96e8665f, /// 0xdb4
			 0x995aff29, /// 0xdb8
			 0x8abd2abb, /// 0xdbc
			 0x83400aed, /// 0xdc0
			 0x002b3462, /// 0xdc4
			 0x1115ff83, /// 0xdc8
			 0x53f2e49c, /// 0xdcc
			 0x598e23aa, /// 0xdd0
			 0x8f95acba, /// 0xdd4
			 0xfd4b0d0b, /// 0xdd8
			 0xd55d2905, /// 0xddc
			 0xba0f8f73, /// 0xde0
			 0xa0cf3643, /// 0xde4
			 0xb18314b2, /// 0xde8
			 0x3b9b84ab, /// 0xdec
			 0x27475a11, /// 0xdf0
			 0xdf42a7e0, /// 0xdf4
			 0xe31e5314, /// 0xdf8
			 0xaf2d7a59, /// 0xdfc
			 0x16d80cdb, /// 0xe00
			 0xd3976059, /// 0xe04
			 0xe63b3e7e, /// 0xe08
			 0xe7929608, /// 0xe0c
			 0x91155cb3, /// 0xe10
			 0x2a13f1e3, /// 0xe14
			 0x9d09d115, /// 0xe18
			 0x0a50f5fb, /// 0xe1c
			 0x956b9da8, /// 0xe20
			 0xbef3527a, /// 0xe24
			 0x9d526a99, /// 0xe28
			 0x52056f44, /// 0xe2c
			 0x0a4b72ef, /// 0xe30
			 0x0feec175, /// 0xe34
			 0xee148f3b, /// 0xe38
			 0xc3ae9598, /// 0xe3c
			 0xdf6644d7, /// 0xe40
			 0x13ddc351, /// 0xe44
			 0x296ee8d9, /// 0xe48
			 0xee6c247b, /// 0xe4c
			 0xb9705411, /// 0xe50
			 0xc9462ee1, /// 0xe54
			 0x49685569, /// 0xe58
			 0x96ea17e7, /// 0xe5c
			 0xc1e5036c, /// 0xe60
			 0x0471df4e, /// 0xe64
			 0x1957f959, /// 0xe68
			 0x6f0b72ec, /// 0xe6c
			 0x7a1e55e2, /// 0xe70
			 0x518e54ee, /// 0xe74
			 0x174aa2ce, /// 0xe78
			 0x601251b9, /// 0xe7c
			 0xcdc90cea, /// 0xe80
			 0x6095dd4d, /// 0xe84
			 0xd4c12a9d, /// 0xe88
			 0xe5131355, /// 0xe8c
			 0xd35223cb, /// 0xe90
			 0xb061695b, /// 0xe94
			 0x928e4f23, /// 0xe98
			 0x48f8f5ca, /// 0xe9c
			 0xbfa28222, /// 0xea0
			 0xbf93f2e9, /// 0xea4
			 0xd7e2072c, /// 0xea8
			 0x92a186f6, /// 0xeac
			 0x325238ac, /// 0xeb0
			 0xc6f4e03c, /// 0xeb4
			 0x4b8aa36c, /// 0xeb8
			 0xa8149fc3, /// 0xebc
			 0x48ab391e, /// 0xec0
			 0x3553e46b, /// 0xec4
			 0xf35625e1, /// 0xec8
			 0x2d8eeecc, /// 0xecc
			 0x190455bc, /// 0xed0
			 0xd8300845, /// 0xed4
			 0x431c9336, /// 0xed8
			 0x7e94ada2, /// 0xedc
			 0x8be7856e, /// 0xee0
			 0xb0d8f6e0, /// 0xee4
			 0x46f946d3, /// 0xee8
			 0x7428d3e2, /// 0xeec
			 0xfb898381, /// 0xef0
			 0x6fbff598, /// 0xef4
			 0x74b28a48, /// 0xef8
			 0xec63e6fb, /// 0xefc
			 0xdafaf1e5, /// 0xf00
			 0xeab76875, /// 0xf04
			 0x97c14a40, /// 0xf08
			 0x4dbbae2f, /// 0xf0c
			 0x9c79a695, /// 0xf10
			 0xeb28ec44, /// 0xf14
			 0x7b37aee8, /// 0xf18
			 0x8faf9051, /// 0xf1c
			 0x406aea2c, /// 0xf20
			 0x2c0a09e1, /// 0xf24
			 0x65462f92, /// 0xf28
			 0x04e616a0, /// 0xf2c
			 0x7f0aeeb9, /// 0xf30
			 0x0c13e91c, /// 0xf34
			 0x41421bb8, /// 0xf38
			 0x3b051e9f, /// 0xf3c
			 0x6fc76fca, /// 0xf40
			 0xa53f3827, /// 0xf44
			 0x5bd87e4a, /// 0xf48
			 0xd3323dba, /// 0xf4c
			 0x3e86bdb3, /// 0xf50
			 0x7d922914, /// 0xf54
			 0xc5193c32, /// 0xf58
			 0x86dea9d2, /// 0xf5c
			 0xcf9f5670, /// 0xf60
			 0x40245dc4, /// 0xf64
			 0x9f1f59af, /// 0xf68
			 0xd193c8b3, /// 0xf6c
			 0xf9cdc94a, /// 0xf70
			 0xd7e944d4, /// 0xf74
			 0xda862890, /// 0xf78
			 0x33d7de65, /// 0xf7c
			 0x8cedda33, /// 0xf80
			 0x961f1de9, /// 0xf84
			 0x717d37f6, /// 0xf88
			 0x0e33db63, /// 0xf8c
			 0xc4e77b20, /// 0xf90
			 0x24d7d53d, /// 0xf94
			 0x1c99a92c, /// 0xf98
			 0x36f45907, /// 0xf9c
			 0xe29836e7, /// 0xfa0
			 0xc7ee9f62, /// 0xfa4
			 0x821ca0c7, /// 0xfa8
			 0xfceda9d2, /// 0xfac
			 0x40ee4d75, /// 0xfb0
			 0x7ad385f3, /// 0xfb4
			 0xa3e98cab, /// 0xfb8
			 0x2ccca782, /// 0xfbc
			 0x162af10a, /// 0xfc0
			 0x12834568, /// 0xfc4
			 0x623f6736, /// 0xfc8
			 0x1a39bde4, /// 0xfcc
			 0xe424b655, /// 0xfd0
			 0xf9d402fe, /// 0xfd4
			 0xecea996d, /// 0xfd8
			 0xa2df77c0, /// 0xfdc
			 0xd8ac764d, /// 0xfe0
			 0xee3f57a8, /// 0xfe4
			 0x953c08dc, /// 0xfe8
			 0x12ed019c, /// 0xfec
			 0x177bd025, /// 0xff0
			 0x67090a56, /// 0xff4
			 0x2b3f201f, /// 0xff8
			 0x55609b5e /// last
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
			 0x00000570,
			 0x000006e0,
			 0x00000720,
			 0x0000017c,
			 0x000001a4,
			 0x000001a0,
			 0x000000b4,
			 0x000001c4,

			 /// vpu register f2
			 0x41900000,
			 0x40400000,
			 0x41200000,
			 0x40a00000,
			 0x41300000,
			 0x41980000,
			 0x41800000,
			 0x41600000,

			 /// vpu register f3
			 0x41e80000,
			 0x41f00000,
			 0x41e80000,
			 0x41b80000,
			 0x41b80000,
			 0x41a80000,
			 0x42000000,
			 0x41e80000,

			 /// vpu register f4
			 0x41880000,
			 0x41a00000,
			 0x40400000,
			 0x3f800000,
			 0x41600000,
			 0x40400000,
			 0x41800000,
			 0x41a80000,

			 /// vpu register f5
			 0x40a00000,
			 0x41c00000,
			 0x40400000,
			 0x41f80000,
			 0x41c00000,
			 0x41f80000,
			 0x41000000,
			 0x41c80000,

			 /// vpu register f6
			 0x40800000,
			 0x41e80000,
			 0x41d80000,
			 0x41600000,
			 0x40e00000,
			 0x40000000,
			 0x41b80000,
			 0x41f00000,

			 /// vpu register f7
			 0x41900000,
			 0x40800000,
			 0x41a00000,
			 0x40a00000,
			 0x42000000,
			 0x41700000,
			 0x41980000,
			 0x41600000,

			 /// vpu register f8
			 0x41f00000,
			 0x41400000,
			 0x41b00000,
			 0x41800000,
			 0x41700000,
			 0x41200000,
			 0x41500000,
			 0x41500000,

			 /// vpu register f9
			 0x3f800000,
			 0x40400000,
			 0x41600000,
			 0x41000000,
			 0x41900000,
			 0x41b00000,
			 0x41880000,
			 0x40e00000,

			 /// vpu register f10
			 0x41f80000,
			 0x41800000,
			 0x41d80000,
			 0x41e80000,
			 0x41700000,
			 0x41000000,
			 0x41d00000,
			 0x41700000,

			 /// vpu register f11
			 0x41b00000,
			 0x41d00000,
			 0x41c00000,
			 0x41000000,
			 0x41a80000,
			 0x41400000,
			 0x40e00000,
			 0x41900000,

			 /// vpu register f12
			 0x41600000,
			 0x41f00000,
			 0x42000000,
			 0x40800000,
			 0x41300000,
			 0x41d00000,
			 0x41700000,
			 0x41e80000,

			 /// vpu register f13
			 0x41b80000,
			 0x40400000,
			 0x41b80000,
			 0x41c80000,
			 0x41f80000,
			 0x41100000,
			 0x41c80000,
			 0x40c00000,

			 /// vpu register f14
			 0x41880000,
			 0x42000000,
			 0x41900000,
			 0x3f800000,
			 0x41900000,
			 0x41700000,
			 0x41500000,
			 0x41880000,

			 /// vpu register f15
			 0x42000000,
			 0x41880000,
			 0x41e80000,
			 0x41b80000,
			 0x40a00000,
			 0x41e00000,
			 0x41200000,
			 0x41900000,

			 /// vpu register f16
			 0x41d00000,
			 0x41d00000,
			 0x41880000,
			 0x41b80000,
			 0x41500000,
			 0x40000000,
			 0x41600000,
			 0x41d00000,

			 /// vpu register f17
			 0x41200000,
			 0x40c00000,
			 0x42000000,
			 0x41e80000,
			 0x40c00000,
			 0x41a00000,
			 0x41c00000,
			 0x41900000,

			 /// vpu register f18
			 0x41f80000,
			 0x41300000,
			 0x41f00000,
			 0x41b80000,
			 0x41400000,
			 0x41d00000,
			 0x41b80000,
			 0x41000000,

			 /// vpu register f19
			 0x41200000,
			 0x41880000,
			 0x41c80000,
			 0x42000000,
			 0x41f00000,
			 0x41e00000,
			 0x40000000,
			 0x41880000,

			 /// vpu register f20
			 0x41000000,
			 0x41d00000,
			 0x40400000,
			 0x41b80000,
			 0x41a00000,
			 0x41d00000,
			 0x41b80000,
			 0x41e80000,

			 /// vpu register f21
			 0x41b80000,
			 0x41f00000,
			 0x40400000,
			 0x40400000,
			 0x40400000,
			 0x41a00000,
			 0x40e00000,
			 0x41980000,

			 /// vpu register f22
			 0x41980000,
			 0x41d00000,
			 0x41c80000,
			 0x41f00000,
			 0x40800000,
			 0x41300000,
			 0x41a00000,
			 0x40e00000,

			 /// vpu register f23
			 0x41880000,
			 0x41e00000,
			 0x41b00000,
			 0x41500000,
			 0x40a00000,
			 0x41500000,
			 0x41c80000,
			 0x41700000,

			 /// vpu register f24
			 0x41900000,
			 0x41c80000,
			 0x41500000,
			 0x41c00000,
			 0x41e00000,
			 0x41f80000,
			 0x41f80000,
			 0x41f80000,

			 /// vpu register f25
			 0x41880000,
			 0x41a80000,
			 0x41900000,
			 0x41e00000,
			 0x41d80000,
			 0x41980000,
			 0x41f00000,
			 0x41500000,

			 /// vpu register f26
			 0x41200000,
			 0x41980000,
			 0x41500000,
			 0x40800000,
			 0x42000000,
			 0x41c00000,
			 0x41b00000,
			 0x41000000,

			 /// vpu register f27
			 0x41400000,
			 0x41000000,
			 0x41b00000,
			 0x40e00000,
			 0x41d00000,
			 0x41800000,
			 0x40000000,
			 0x40c00000,

			 /// vpu register f28
			 0x41c80000,
			 0x40a00000,
			 0x41b80000,
			 0x41a80000,
			 0x41c00000,
			 0x41a80000,
			 0x41b00000,
			 0x40e00000,

			 /// vpu register f29
			 0x41400000,
			 0x41b00000,
			 0x40e00000,
			 0x41980000,
			 0x41700000,
			 0x41100000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f30
			 0x40800000,
			 0x41700000,
			 0x41e80000,
			 0x3f800000,
			 0x40400000,
			 0x41f80000,
			 0x41200000,
			 0x40000000,

			 /// vpu register f31
			 0x41600000,
			 0x41900000,
			 0x41980000,
			 0x41000000,
			 0x41900000,
			 0x41e00000,
			 0x41900000,
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
		"nop\n"
		"fltm.ps m7, f26, f15\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f22, f19\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f6, f20\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f15, f19\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f30, f2\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f1, f15\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f10, f3\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m2, f19, f18\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f21, f4\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f14, f19\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f14, f27\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f11, f22\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f20, f5\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f21, f16\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f0, f27\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f27, f1\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f15, f6\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f13, f13\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f10, f16\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f24, f24\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f17, f29\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f11, f19\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f8, f24\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f15, f29\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f17, f19\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f21, f14\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f19, f9\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f22, f15\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f2, f1\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f9, f6\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f4, f14\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f21, f24\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f21, f21\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m2, f23, f1\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f5, f19\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f20, f30\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f28, f9\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f24, f4\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f14, f3\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f23, f11\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f18, f11\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f23, f1\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f16, f28\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f13, f8\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f10, f30\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f18, f25\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f4, f5\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f15, f11\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f14, f26\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f15, f1\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f14, f14\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f20, f5\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f12, f12\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m2, f0, f7\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m2, f10, f21\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f10, f4\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f21, f5\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f29, f12\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f29, f19\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m5, f18, f29\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f19, f22\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f1, f29\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f23, f26\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f24, f3\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f28, f8\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f23, f10\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f10, f10\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f13, f22\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m2, f19, f27\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f5, f16\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f21, f25\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f11, f12\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f29, f23\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f1, f24\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f4, f1\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f20, f17\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f7, f18\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f19, f7\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f0, f29\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m3, f13, f16\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f12, f26\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f24, f10\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m1, f1, f11\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f12, f24\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f27, f5\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f1, f25\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m6, f7, f1\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f22, f30\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f20, f20\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f7, f14\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f23, f18\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f23, f17\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f17, f28\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f19, f3\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f10, f30\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m0, f4, f25\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f15, f25\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m4, f12, f22\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.ps m7, f17, f19\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
