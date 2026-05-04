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
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00008
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00010
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00014
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00018
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0001c
			 0x3f028f5c,                                                  // 0.51                                        /// 00020
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00024
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00030
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00034
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00038
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0003c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00044
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0004c
			 0x80000000,                                                  // -zero                                       /// 00050
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0005c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00064
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00068
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00070
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00074
			 0x55555555,                                                  // 4 random values                             /// 00078
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0007c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00084
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00088
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0008c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00090
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00094
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0009c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a8
			 0xcb000000,                                                  // -8388608.0                                  /// 000ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d4
			 0xbf028f5c,                                                  // -0.51                                       /// 000d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00104
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00110
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00114
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00118
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00120
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0e000001,                                                  // Trailing 1s:                                /// 0012c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00134
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00138
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0013c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00148
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0014c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00150
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x55555555,                                                  // 4 random values                             /// 0015c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00160
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00164
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00174
			 0x00000000,                                                  // zero                                        /// 00178
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0017c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00180
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00184
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00194
			 0x80000000,                                                  // -zero                                       /// 00198
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001d8
			 0xcb000000,                                                  // -8388608.0                                  /// 001dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001ec
			 0x33333333,                                                  // 4 random values                             /// 001f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00200
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00204
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0020c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00214
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0021c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00230
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00238
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00240
			 0x33333333,                                                  // 4 random values                             /// 00244
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00248
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00250
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00258
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0025c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00260
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00268
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0026c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00278
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00280
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00284
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00288
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0028c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00290
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00294
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00298
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0029c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00304
			 0x0e000007,                                                  // Trailing 1s:                                /// 00308
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0030c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00314
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00318
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00320
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00328
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00330
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0033c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00340
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00348
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0034c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0xffc00001,                                                  // -signaling NaN                              /// 00354
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00358
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00364
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00370
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00374
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00378
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00380
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00384
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00388
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0038c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00390
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00398
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0039c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003dc
			 0x55555555,                                                  // 4 random values                             /// 003e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00400
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00408
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0040c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00418
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0041c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00420
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00424
			 0x0c700000,                                                  // Leading 1s:                                 /// 00428
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0042c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00438
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0043c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00444
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00448
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0044c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00454
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0045c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00460
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00468
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0046c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00474
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00478
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00480
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00484
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00488
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0048c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00490
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0049c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00504
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00508
			 0x7fc00001,                                                  // signaling NaN                               /// 0050c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00510
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00514
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0051c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00520
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00528
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00530
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00534
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0053c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00540
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x0c600000,                                                  // Leading 1s:                                 /// 0054c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00554
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00558
			 0x0c400000,                                                  // Leading 1s:                                 /// 0055c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00560
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00564
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00568
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00570
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00584
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0058c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00590
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00594
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00598
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0059c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x3f028f5c,                                                  // 0.51                                        /// 005f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00600
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00000000,                                                  // zero                                        /// 0060c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00618
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x4b000000,                                                  // 8388608.0                                   /// 00620
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00624
			 0x3f028f5c,                                                  // 0.51                                        /// 00628
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00630
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00634
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0063c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00640
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x0e000001,                                                  // Trailing 1s:                                /// 00648
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0064c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00654
			 0xbf028f5c,                                                  // -0.51                                       /// 00658
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00660
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0066c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00674
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00678
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00684
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00688
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0068c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00690
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0069c
			 0x7f800000,                                                  // inf                                         /// 006a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x7f800000,                                                  // inf                                         /// 006e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00704
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00708
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0070c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00710
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00714
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0071c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00720
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00724
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00728
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00738
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00744
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00750
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00754
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00758
			 0x80011111,                                                  // -9.7958E-41                                 /// 0075c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00764
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0c600000,                                                  // Leading 1s:                                 /// 0076c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00770
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00774
			 0x0c400000,                                                  // Leading 1s:                                 /// 00778
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0077c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00784
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00790
			 0x00011111,                                                  // 9.7958E-41                                  /// 00794
			 0x0c700000,                                                  // Leading 1s:                                 /// 00798
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00800
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00804
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0080c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00810
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00814
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00818
			 0xff800000,                                                  // -inf                                        /// 0081c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00820
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00824
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0082c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00838
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0083c
			 0x4b000000,                                                  // 8388608.0                                   /// 00840
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00848
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00850
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00858
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0085c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00860
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00868
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00874
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00878
			 0x7f800000,                                                  // inf                                         /// 0087c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00880
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00884
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00894
			 0x0e000001,                                                  // Trailing 1s:                                /// 00898
			 0x0c600000,                                                  // Leading 1s:                                 /// 0089c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00910
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00918
			 0xffc00001,                                                  // -signaling NaN                              /// 0091c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00924
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00930
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00938
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0093c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00944
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00948
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0094c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00950
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00960
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00968
			 0xff800000,                                                  // -inf                                        /// 0096c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00974
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00978
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0097c
			 0x80000000,                                                  // -zero                                       /// 00980
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00984
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00988
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0098c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00990
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00998
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00abc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac0
			 0x00000000,                                                  // zero                                        /// 00ac4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00acc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00adc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ae4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00af0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b38
			 0x80000000,                                                  // -zero                                       /// 00b3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b44
			 0xff800000,                                                  // -inf                                        /// 00b48
			 0x3f028f5c,                                                  // 0.51                                        /// 00b4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b98
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bc0
			 0xff800000,                                                  // -inf                                        /// 00bc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00be4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00be8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bf8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bfc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c38
			 0xff800000,                                                  // -inf                                        /// 00c3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c50
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c90
			 0xbf028f5c,                                                  // -0.51                                       /// 00c94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00ca0
			 0x55555555,                                                  // 4 random values                             /// 00ca4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ca8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cb0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cb4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cb8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cd0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cdc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cf0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cfc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0xffc00001,                                                  // -signaling NaN                              /// 00d58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d84
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00da0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00da8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00db0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00db4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00de0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00de4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00df0
			 0x7f800000,                                                  // inf                                         /// 00df4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dfc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e78
			 0x7fc00001,                                                  // signaling NaN                               /// 00e7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e80
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ea4
			 0x7f800000,                                                  // inf                                         /// 00ea8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ebc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ec0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ecc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ed4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00edc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ee0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ee8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x7f800000,                                                  // inf                                         /// 00ef8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00efc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f40
			 0x7f800000,                                                  // inf                                         /// 00f44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f4c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f70
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fa8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fcc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fd4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fdc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fe0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fe8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ff0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c7ffc00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7e2791b9, /// 0x0
			 0x91cca51b, /// 0x4
			 0x0228d6ef, /// 0x8
			 0xd7a1a0f6, /// 0xc
			 0x296a0ccd, /// 0x10
			 0x6286cfd9, /// 0x14
			 0x49c44662, /// 0x18
			 0xd313b103, /// 0x1c
			 0x6c35ee8c, /// 0x20
			 0x4a895265, /// 0x24
			 0xbdf71f39, /// 0x28
			 0x2fe47c7c, /// 0x2c
			 0x76b37a6d, /// 0x30
			 0x2574524b, /// 0x34
			 0x908b9ec4, /// 0x38
			 0xe4a97d4f, /// 0x3c
			 0xc5e5b014, /// 0x40
			 0xf1eada37, /// 0x44
			 0x28877d47, /// 0x48
			 0x04cf503d, /// 0x4c
			 0x9a403d73, /// 0x50
			 0x916090ed, /// 0x54
			 0x37817b56, /// 0x58
			 0xe99dc64a, /// 0x5c
			 0x212fff4a, /// 0x60
			 0xa52e8b9d, /// 0x64
			 0x5b2c2473, /// 0x68
			 0xe85c50aa, /// 0x6c
			 0xbc4d9bfd, /// 0x70
			 0x46beb0cc, /// 0x74
			 0x86effd20, /// 0x78
			 0xad97f312, /// 0x7c
			 0x44e1b037, /// 0x80
			 0xc4f4a57a, /// 0x84
			 0xa3aafb4b, /// 0x88
			 0xd5ed3ec1, /// 0x8c
			 0x5af1905a, /// 0x90
			 0x9158bc29, /// 0x94
			 0xce88d80c, /// 0x98
			 0x9a0acf48, /// 0x9c
			 0x4e29297f, /// 0xa0
			 0x9b171499, /// 0xa4
			 0x9d777126, /// 0xa8
			 0x263a3753, /// 0xac
			 0xecea15d5, /// 0xb0
			 0xc20b056d, /// 0xb4
			 0x393b498c, /// 0xb8
			 0x912302be, /// 0xbc
			 0xf4016466, /// 0xc0
			 0x47d145bc, /// 0xc4
			 0x3b0e2932, /// 0xc8
			 0x784cc9e0, /// 0xcc
			 0x8da569bd, /// 0xd0
			 0xd6945f0e, /// 0xd4
			 0x892ca562, /// 0xd8
			 0x80fd59b2, /// 0xdc
			 0xb8b0e706, /// 0xe0
			 0x782aba9a, /// 0xe4
			 0xc9959be0, /// 0xe8
			 0x83165380, /// 0xec
			 0x388c8e6d, /// 0xf0
			 0xd82f1c8d, /// 0xf4
			 0x95e8d3b2, /// 0xf8
			 0x1a0d18f2, /// 0xfc
			 0xd49d31ad, /// 0x100
			 0xf83ee309, /// 0x104
			 0x823409da, /// 0x108
			 0x64963dcc, /// 0x10c
			 0x9249c684, /// 0x110
			 0x31dda72f, /// 0x114
			 0x4b0e97fc, /// 0x118
			 0xf0289955, /// 0x11c
			 0x452964ba, /// 0x120
			 0x1b90bf2f, /// 0x124
			 0xc3bb567b, /// 0x128
			 0x1db0fda2, /// 0x12c
			 0xddb69fcf, /// 0x130
			 0x04601ccf, /// 0x134
			 0x95ea6629, /// 0x138
			 0x87c2eec7, /// 0x13c
			 0x2e2ded96, /// 0x140
			 0x1c7b3bab, /// 0x144
			 0x3b84de7c, /// 0x148
			 0x94bdb644, /// 0x14c
			 0x400d78d1, /// 0x150
			 0x32219141, /// 0x154
			 0x33f0ba99, /// 0x158
			 0x07a2cf37, /// 0x15c
			 0xc172cd12, /// 0x160
			 0xe97f7dbd, /// 0x164
			 0xaa8ef704, /// 0x168
			 0x751aeeb3, /// 0x16c
			 0xc8c41718, /// 0x170
			 0x681f485a, /// 0x174
			 0x1fa4713f, /// 0x178
			 0x3291aa2c, /// 0x17c
			 0xe6a1a8fc, /// 0x180
			 0x4fe15f8f, /// 0x184
			 0xe91c8956, /// 0x188
			 0x813208c3, /// 0x18c
			 0xf5545102, /// 0x190
			 0x9d7b9347, /// 0x194
			 0x3498ded5, /// 0x198
			 0x24403415, /// 0x19c
			 0x300c2320, /// 0x1a0
			 0x64d1b17e, /// 0x1a4
			 0x8b1c8203, /// 0x1a8
			 0x757d8004, /// 0x1ac
			 0xb3deafb8, /// 0x1b0
			 0x18c1d61f, /// 0x1b4
			 0x6daf3bf2, /// 0x1b8
			 0xdcb8e006, /// 0x1bc
			 0x9418f6aa, /// 0x1c0
			 0x7328f7c0, /// 0x1c4
			 0xc6bdca5c, /// 0x1c8
			 0x42f1faae, /// 0x1cc
			 0x4a040f46, /// 0x1d0
			 0xde667730, /// 0x1d4
			 0xb8be9574, /// 0x1d8
			 0x5b5a5e0c, /// 0x1dc
			 0x2183ce9e, /// 0x1e0
			 0x657271e3, /// 0x1e4
			 0xc53a8927, /// 0x1e8
			 0x7ccd1c2b, /// 0x1ec
			 0xb9a32633, /// 0x1f0
			 0x45523e51, /// 0x1f4
			 0xe6d46701, /// 0x1f8
			 0x4c3dab7f, /// 0x1fc
			 0xfe2c72fa, /// 0x200
			 0x24f4af4f, /// 0x204
			 0x909229c7, /// 0x208
			 0xd9af6add, /// 0x20c
			 0x9e3d5aa1, /// 0x210
			 0xdb5e6f8c, /// 0x214
			 0x6c85610a, /// 0x218
			 0x269dbd0a, /// 0x21c
			 0x957a8ae9, /// 0x220
			 0xa166a147, /// 0x224
			 0xa95145d6, /// 0x228
			 0x866444e6, /// 0x22c
			 0xf7b208e1, /// 0x230
			 0x08b846b0, /// 0x234
			 0xa3a80310, /// 0x238
			 0x181e2ca6, /// 0x23c
			 0x809afc89, /// 0x240
			 0x6576d4aa, /// 0x244
			 0x4c4a6fed, /// 0x248
			 0x1f5013cb, /// 0x24c
			 0x58018b52, /// 0x250
			 0xda904108, /// 0x254
			 0xba70337a, /// 0x258
			 0x9b87df18, /// 0x25c
			 0xd6ea9ca8, /// 0x260
			 0xc2b2734b, /// 0x264
			 0xdeea2a40, /// 0x268
			 0xadc5af93, /// 0x26c
			 0xdfe7609f, /// 0x270
			 0x0be7c175, /// 0x274
			 0x9144ea16, /// 0x278
			 0x95cf1d28, /// 0x27c
			 0xc43d2d0e, /// 0x280
			 0xd8a05b37, /// 0x284
			 0xc1db5e49, /// 0x288
			 0xb22485d5, /// 0x28c
			 0xa180c856, /// 0x290
			 0xc6531c44, /// 0x294
			 0xa62d3fc3, /// 0x298
			 0xadbddc58, /// 0x29c
			 0x8a420a2c, /// 0x2a0
			 0xe11fa7b4, /// 0x2a4
			 0x6a97990a, /// 0x2a8
			 0x8de38252, /// 0x2ac
			 0x4b4dd41b, /// 0x2b0
			 0xb0951863, /// 0x2b4
			 0x6002cf36, /// 0x2b8
			 0x0e476be6, /// 0x2bc
			 0x87d1d606, /// 0x2c0
			 0x2383d5d4, /// 0x2c4
			 0x3ec4e571, /// 0x2c8
			 0xb06318f6, /// 0x2cc
			 0xb0120896, /// 0x2d0
			 0xc16a8667, /// 0x2d4
			 0x2e1fd186, /// 0x2d8
			 0x00f12765, /// 0x2dc
			 0xc15f250c, /// 0x2e0
			 0xce053f1d, /// 0x2e4
			 0xce6db484, /// 0x2e8
			 0x1bd4a659, /// 0x2ec
			 0x2fb08008, /// 0x2f0
			 0xf7a11ef6, /// 0x2f4
			 0xae370a5a, /// 0x2f8
			 0x3f34c122, /// 0x2fc
			 0x000af446, /// 0x300
			 0xfa8eff04, /// 0x304
			 0x9dc99089, /// 0x308
			 0xbcb5bbcc, /// 0x30c
			 0xc77b1093, /// 0x310
			 0x65ba7464, /// 0x314
			 0xf5f2ccde, /// 0x318
			 0x1d19824d, /// 0x31c
			 0xe60c4e99, /// 0x320
			 0x2c6b722e, /// 0x324
			 0x138fc259, /// 0x328
			 0x9cdfd970, /// 0x32c
			 0x0732be3e, /// 0x330
			 0x8273f4d2, /// 0x334
			 0x94b11ae8, /// 0x338
			 0x1506258c, /// 0x33c
			 0xc3753e8e, /// 0x340
			 0x73b707b4, /// 0x344
			 0xf4608c53, /// 0x348
			 0x5725e907, /// 0x34c
			 0x2d75ea04, /// 0x350
			 0x9586cbb9, /// 0x354
			 0x78e5a423, /// 0x358
			 0x2d04be18, /// 0x35c
			 0x2586bd49, /// 0x360
			 0xf249b0a0, /// 0x364
			 0x6a0c68e3, /// 0x368
			 0x734a1d09, /// 0x36c
			 0xe40e77cf, /// 0x370
			 0x6d4c1e44, /// 0x374
			 0x048ef80d, /// 0x378
			 0xa0c2f4fd, /// 0x37c
			 0x6338f37c, /// 0x380
			 0x3e0d87cb, /// 0x384
			 0x7bff6347, /// 0x388
			 0x6052e9f4, /// 0x38c
			 0x83063651, /// 0x390
			 0x7846048b, /// 0x394
			 0xefeadc86, /// 0x398
			 0xd5e0735e, /// 0x39c
			 0x85a667ee, /// 0x3a0
			 0xc9474c72, /// 0x3a4
			 0x415cffda, /// 0x3a8
			 0x0e7e3049, /// 0x3ac
			 0xc486e5c6, /// 0x3b0
			 0xd4d187af, /// 0x3b4
			 0x55b8603f, /// 0x3b8
			 0x867ee830, /// 0x3bc
			 0x7b6fbc58, /// 0x3c0
			 0xb3dfeb98, /// 0x3c4
			 0xacb5563f, /// 0x3c8
			 0x32ac8103, /// 0x3cc
			 0x4e68bb9d, /// 0x3d0
			 0x62094ef7, /// 0x3d4
			 0x4ab6a2b0, /// 0x3d8
			 0x66cd24e0, /// 0x3dc
			 0x15805ea0, /// 0x3e0
			 0xaf6e8083, /// 0x3e4
			 0x8fa92484, /// 0x3e8
			 0x0301bbf6, /// 0x3ec
			 0xbfd77d53, /// 0x3f0
			 0xb4369cc7, /// 0x3f4
			 0x8e1963e4, /// 0x3f8
			 0xa48b3f7e, /// 0x3fc
			 0xcbb0c5d3, /// 0x400
			 0x6f464426, /// 0x404
			 0xf29f9089, /// 0x408
			 0xa2e4b7b8, /// 0x40c
			 0x972ff2bb, /// 0x410
			 0xcd1100b4, /// 0x414
			 0x8e594c24, /// 0x418
			 0x0b00fe96, /// 0x41c
			 0x2bed4623, /// 0x420
			 0xe33f7670, /// 0x424
			 0xe3fa3786, /// 0x428
			 0x2abf4785, /// 0x42c
			 0x3d2ab8f3, /// 0x430
			 0x1f446beb, /// 0x434
			 0x22606822, /// 0x438
			 0xc12602a3, /// 0x43c
			 0x3b442dfe, /// 0x440
			 0xbc6f9dfa, /// 0x444
			 0x5685d2d7, /// 0x448
			 0x819c8847, /// 0x44c
			 0x74b7f772, /// 0x450
			 0x8a5b0bfe, /// 0x454
			 0xb209f6a7, /// 0x458
			 0x34cee570, /// 0x45c
			 0xb3b29c20, /// 0x460
			 0x88cd808b, /// 0x464
			 0x7fd48f3a, /// 0x468
			 0xc7df0723, /// 0x46c
			 0x479cff4d, /// 0x470
			 0x2529ff29, /// 0x474
			 0xd5d9ecdd, /// 0x478
			 0xccad8e1a, /// 0x47c
			 0x67c5a6a0, /// 0x480
			 0xcbd3ad76, /// 0x484
			 0x9ec175ec, /// 0x488
			 0x4f4dd0e8, /// 0x48c
			 0x8a60ac66, /// 0x490
			 0xdff78404, /// 0x494
			 0x5cf88168, /// 0x498
			 0xf4084654, /// 0x49c
			 0x0ca74d4d, /// 0x4a0
			 0x9c118d35, /// 0x4a4
			 0x5fa80393, /// 0x4a8
			 0x130b8bc8, /// 0x4ac
			 0xa5ad8faf, /// 0x4b0
			 0x16c62f53, /// 0x4b4
			 0x639cfb6b, /// 0x4b8
			 0x58c4a27d, /// 0x4bc
			 0xb2d36493, /// 0x4c0
			 0x70a3ad7c, /// 0x4c4
			 0x9c9d9297, /// 0x4c8
			 0xcc006348, /// 0x4cc
			 0xe69596eb, /// 0x4d0
			 0x1ea65f09, /// 0x4d4
			 0x1adee843, /// 0x4d8
			 0x4dbd3c93, /// 0x4dc
			 0x1d1c3246, /// 0x4e0
			 0x18074c7a, /// 0x4e4
			 0x8a87eee8, /// 0x4e8
			 0x21114472, /// 0x4ec
			 0xd18c6fe5, /// 0x4f0
			 0xebe24a8c, /// 0x4f4
			 0xe763e6a5, /// 0x4f8
			 0xd36ce554, /// 0x4fc
			 0x862cbb09, /// 0x500
			 0x5705c6ac, /// 0x504
			 0x18d822c4, /// 0x508
			 0x20eaf997, /// 0x50c
			 0x802902bb, /// 0x510
			 0x8dc2ae9f, /// 0x514
			 0x3977a58c, /// 0x518
			 0x9ad0a21e, /// 0x51c
			 0xcd00583e, /// 0x520
			 0x3cfa1bba, /// 0x524
			 0xb8ca2dc0, /// 0x528
			 0x412d6a66, /// 0x52c
			 0xd828a450, /// 0x530
			 0x52a893f0, /// 0x534
			 0xa13b2f2b, /// 0x538
			 0x0e406cab, /// 0x53c
			 0x70b0b406, /// 0x540
			 0xb07e2af8, /// 0x544
			 0x1ac5da26, /// 0x548
			 0x2e590eac, /// 0x54c
			 0xe2ba7161, /// 0x550
			 0xfae8b064, /// 0x554
			 0x41b9ba46, /// 0x558
			 0x878f4c70, /// 0x55c
			 0xb63061d7, /// 0x560
			 0xa522a06e, /// 0x564
			 0xf75082b1, /// 0x568
			 0x269060b3, /// 0x56c
			 0xd466af7b, /// 0x570
			 0x884fcec5, /// 0x574
			 0xfa74e0fd, /// 0x578
			 0xaef45b18, /// 0x57c
			 0xfe268676, /// 0x580
			 0x6d7888b1, /// 0x584
			 0x7107d343, /// 0x588
			 0x6ecbb5cc, /// 0x58c
			 0x6480ea1c, /// 0x590
			 0x743d0d2a, /// 0x594
			 0x242917bb, /// 0x598
			 0xddc9787f, /// 0x59c
			 0x3d390b83, /// 0x5a0
			 0xc81804bb, /// 0x5a4
			 0x70dd6a4b, /// 0x5a8
			 0x3e8560dd, /// 0x5ac
			 0xc451c95d, /// 0x5b0
			 0xc7186de5, /// 0x5b4
			 0xbb1e177c, /// 0x5b8
			 0xa9761da6, /// 0x5bc
			 0x9c2447e3, /// 0x5c0
			 0x1ce312f0, /// 0x5c4
			 0x2060aa05, /// 0x5c8
			 0xa0076f04, /// 0x5cc
			 0x8c631fe9, /// 0x5d0
			 0x6fbb99a8, /// 0x5d4
			 0x46fc8827, /// 0x5d8
			 0x6d7b3ca4, /// 0x5dc
			 0x8257d55c, /// 0x5e0
			 0x293e6b8a, /// 0x5e4
			 0xc6fefed3, /// 0x5e8
			 0x7017721c, /// 0x5ec
			 0xd1454689, /// 0x5f0
			 0xc7ac1668, /// 0x5f4
			 0x7270537c, /// 0x5f8
			 0x2831a4c5, /// 0x5fc
			 0xd6256798, /// 0x600
			 0xe593b027, /// 0x604
			 0x32a4b4db, /// 0x608
			 0x9224627b, /// 0x60c
			 0x3ea83b56, /// 0x610
			 0xb5aebc36, /// 0x614
			 0x534837a9, /// 0x618
			 0x0dfef7ba, /// 0x61c
			 0x417b9033, /// 0x620
			 0x6b1412c0, /// 0x624
			 0xb5fa2eac, /// 0x628
			 0x35f21545, /// 0x62c
			 0x7477383d, /// 0x630
			 0x24b264b3, /// 0x634
			 0x79da2f54, /// 0x638
			 0xda29e45d, /// 0x63c
			 0xfc26b2ce, /// 0x640
			 0x1325657b, /// 0x644
			 0x1d8bfe3d, /// 0x648
			 0x7c060ac8, /// 0x64c
			 0xbfb4b6ce, /// 0x650
			 0x8392fa4d, /// 0x654
			 0x7e61eaf1, /// 0x658
			 0x097240e0, /// 0x65c
			 0x3915e4fe, /// 0x660
			 0xf633a496, /// 0x664
			 0xeee62495, /// 0x668
			 0x04dd2ddf, /// 0x66c
			 0x6c2d6db6, /// 0x670
			 0xaeaf996e, /// 0x674
			 0x401fc9c1, /// 0x678
			 0x99409b5f, /// 0x67c
			 0x5995518d, /// 0x680
			 0x3a9569a8, /// 0x684
			 0x63c6bf70, /// 0x688
			 0xe254868d, /// 0x68c
			 0x90e00c18, /// 0x690
			 0x9a0b26e4, /// 0x694
			 0x5e00da4e, /// 0x698
			 0x7e08ea3f, /// 0x69c
			 0x97a3977c, /// 0x6a0
			 0x0b051853, /// 0x6a4
			 0x3c2d1111, /// 0x6a8
			 0x9fe89be7, /// 0x6ac
			 0xe34e2dab, /// 0x6b0
			 0x6093fb1c, /// 0x6b4
			 0xd35d786b, /// 0x6b8
			 0x7c9c21b1, /// 0x6bc
			 0xc2066d29, /// 0x6c0
			 0x4ab235d4, /// 0x6c4
			 0xace84779, /// 0x6c8
			 0x0a82a453, /// 0x6cc
			 0x99e1b3e5, /// 0x6d0
			 0xe3f36460, /// 0x6d4
			 0x1a5ddefc, /// 0x6d8
			 0x4388f5ba, /// 0x6dc
			 0xe980310b, /// 0x6e0
			 0x1fde2d31, /// 0x6e4
			 0x50d12f58, /// 0x6e8
			 0xe9272b51, /// 0x6ec
			 0xe9ace494, /// 0x6f0
			 0x7ea60e74, /// 0x6f4
			 0xc4df8566, /// 0x6f8
			 0x7d6e3052, /// 0x6fc
			 0x6a3317f8, /// 0x700
			 0x3566bf13, /// 0x704
			 0xb303877a, /// 0x708
			 0xac005df0, /// 0x70c
			 0x55a3f1cb, /// 0x710
			 0x3634902e, /// 0x714
			 0x40626d93, /// 0x718
			 0x57c5db09, /// 0x71c
			 0xb2d99323, /// 0x720
			 0xb3ba7b27, /// 0x724
			 0x4c7e26d9, /// 0x728
			 0xc789e97b, /// 0x72c
			 0x30703b07, /// 0x730
			 0x30c28406, /// 0x734
			 0x36ce5e99, /// 0x738
			 0x547f3ca5, /// 0x73c
			 0x83d7185d, /// 0x740
			 0x74a3ae35, /// 0x744
			 0x8837d503, /// 0x748
			 0x9ef667f0, /// 0x74c
			 0xb83907c3, /// 0x750
			 0x2b34d473, /// 0x754
			 0x169b66dc, /// 0x758
			 0xb98e1348, /// 0x75c
			 0xdcdd4ad3, /// 0x760
			 0x3b22fd64, /// 0x764
			 0xc71d7bf5, /// 0x768
			 0x149206f2, /// 0x76c
			 0xbf4838e0, /// 0x770
			 0xa5c673ad, /// 0x774
			 0xa2173647, /// 0x778
			 0xa3b81c5b, /// 0x77c
			 0xa7a4f66f, /// 0x780
			 0x439d1961, /// 0x784
			 0xa76ab87f, /// 0x788
			 0x8aaa7aae, /// 0x78c
			 0xff5be89b, /// 0x790
			 0x61be1ddf, /// 0x794
			 0x6081c20f, /// 0x798
			 0x0129312b, /// 0x79c
			 0x427c06b9, /// 0x7a0
			 0xdbce82c3, /// 0x7a4
			 0x7da094d4, /// 0x7a8
			 0xf100928d, /// 0x7ac
			 0x24e25b59, /// 0x7b0
			 0x0372afc2, /// 0x7b4
			 0xed2ab976, /// 0x7b8
			 0xb2d7c1ad, /// 0x7bc
			 0x26047a42, /// 0x7c0
			 0x94a1ee3c, /// 0x7c4
			 0x53ce690a, /// 0x7c8
			 0x902b73b2, /// 0x7cc
			 0x58c9be3c, /// 0x7d0
			 0x6715e5e5, /// 0x7d4
			 0x2ec78814, /// 0x7d8
			 0x790c896a, /// 0x7dc
			 0x98f1eb47, /// 0x7e0
			 0xb210caba, /// 0x7e4
			 0xdbf7d92d, /// 0x7e8
			 0x173fd928, /// 0x7ec
			 0xf54002d5, /// 0x7f0
			 0x6f972ff7, /// 0x7f4
			 0x848c3f49, /// 0x7f8
			 0x6b183ca6, /// 0x7fc
			 0x1c312c10, /// 0x800
			 0x165d8822, /// 0x804
			 0xaaf20044, /// 0x808
			 0x5b6fc65d, /// 0x80c
			 0x4a101cd9, /// 0x810
			 0x662deb22, /// 0x814
			 0xcb7e9bac, /// 0x818
			 0xf9894cc9, /// 0x81c
			 0xc4101c5e, /// 0x820
			 0xc137ed77, /// 0x824
			 0xf942c6fe, /// 0x828
			 0x6634359d, /// 0x82c
			 0x4fedf73c, /// 0x830
			 0x8b036076, /// 0x834
			 0x97b2b3ff, /// 0x838
			 0x4c1db49b, /// 0x83c
			 0x11739b32, /// 0x840
			 0x71e5851e, /// 0x844
			 0xe831625a, /// 0x848
			 0xf77f7663, /// 0x84c
			 0x8f1c7376, /// 0x850
			 0xf29b421b, /// 0x854
			 0x5f193061, /// 0x858
			 0x17647fcb, /// 0x85c
			 0x8ae98e59, /// 0x860
			 0x1338d472, /// 0x864
			 0xaa725342, /// 0x868
			 0x901d1d81, /// 0x86c
			 0x1283adc8, /// 0x870
			 0x219683a5, /// 0x874
			 0x9accb122, /// 0x878
			 0xa3d13537, /// 0x87c
			 0x10534d3d, /// 0x880
			 0x61898b87, /// 0x884
			 0x8fec0471, /// 0x888
			 0x2eb503aa, /// 0x88c
			 0x167a1d72, /// 0x890
			 0x0e92eb5b, /// 0x894
			 0xb1736624, /// 0x898
			 0x2e138f0b, /// 0x89c
			 0x5a81da95, /// 0x8a0
			 0x66e969f4, /// 0x8a4
			 0x046c8659, /// 0x8a8
			 0x5c5e8749, /// 0x8ac
			 0xe69bf140, /// 0x8b0
			 0xc28e7c89, /// 0x8b4
			 0xf79f0564, /// 0x8b8
			 0xec538d76, /// 0x8bc
			 0xb64af9ed, /// 0x8c0
			 0xdadd5862, /// 0x8c4
			 0xc88dc88a, /// 0x8c8
			 0x43c75ff9, /// 0x8cc
			 0x494c313f, /// 0x8d0
			 0xb0b1ca2b, /// 0x8d4
			 0x7c111e90, /// 0x8d8
			 0x72b0e77f, /// 0x8dc
			 0xf1f57d0d, /// 0x8e0
			 0xbb1cc642, /// 0x8e4
			 0xac214b67, /// 0x8e8
			 0x303801bc, /// 0x8ec
			 0x3c6a4b30, /// 0x8f0
			 0x66e1da7f, /// 0x8f4
			 0x2d0f3b66, /// 0x8f8
			 0xec880dab, /// 0x8fc
			 0x8cb426b8, /// 0x900
			 0xb2624ce9, /// 0x904
			 0xb303f3f1, /// 0x908
			 0x727e453a, /// 0x90c
			 0x8fccdd0e, /// 0x910
			 0xa38a2307, /// 0x914
			 0xc3515663, /// 0x918
			 0xe85a770c, /// 0x91c
			 0xd08be061, /// 0x920
			 0x775ebd2b, /// 0x924
			 0x23ac3272, /// 0x928
			 0xc4641682, /// 0x92c
			 0xbabba9c5, /// 0x930
			 0x5d557271, /// 0x934
			 0x233ecba4, /// 0x938
			 0x1a496ca6, /// 0x93c
			 0x8c3c9229, /// 0x940
			 0x179af1fe, /// 0x944
			 0x54586940, /// 0x948
			 0xcbe36f4e, /// 0x94c
			 0x3d45d1ab, /// 0x950
			 0xd90dba5a, /// 0x954
			 0x08a58803, /// 0x958
			 0x42f6b3b8, /// 0x95c
			 0x2c9c9756, /// 0x960
			 0xd9133371, /// 0x964
			 0x22f0807d, /// 0x968
			 0x22432bab, /// 0x96c
			 0xf356db82, /// 0x970
			 0x287c88e1, /// 0x974
			 0xf9e87de0, /// 0x978
			 0xa5df5f4a, /// 0x97c
			 0xed967a85, /// 0x980
			 0xa4f8c563, /// 0x984
			 0xba24c7a1, /// 0x988
			 0x77d1d38e, /// 0x98c
			 0xf6113927, /// 0x990
			 0x3f8228b8, /// 0x994
			 0xf57568e3, /// 0x998
			 0x4eb44dd0, /// 0x99c
			 0x360802fd, /// 0x9a0
			 0x2decd0f2, /// 0x9a4
			 0x26c288e7, /// 0x9a8
			 0xd9ca12dd, /// 0x9ac
			 0xd20fce14, /// 0x9b0
			 0xaaeb59e7, /// 0x9b4
			 0x9429f5ea, /// 0x9b8
			 0x9d7a911f, /// 0x9bc
			 0xd0d5e728, /// 0x9c0
			 0xfec9c3c8, /// 0x9c4
			 0xc6c245bc, /// 0x9c8
			 0x15e8b501, /// 0x9cc
			 0x8a0beed5, /// 0x9d0
			 0xf8a0ef1b, /// 0x9d4
			 0x7c3756b6, /// 0x9d8
			 0xf5f670b3, /// 0x9dc
			 0x208f65e5, /// 0x9e0
			 0x3dddd3a8, /// 0x9e4
			 0x66f4aca6, /// 0x9e8
			 0x902ab3fc, /// 0x9ec
			 0x7a7430c5, /// 0x9f0
			 0x7affd409, /// 0x9f4
			 0xfb30f032, /// 0x9f8
			 0x7d846bdc, /// 0x9fc
			 0x95f8eb7f, /// 0xa00
			 0xd319dcea, /// 0xa04
			 0xdb59f13a, /// 0xa08
			 0xfe7b1415, /// 0xa0c
			 0x306f3ff3, /// 0xa10
			 0x217234d2, /// 0xa14
			 0x0e8eaa81, /// 0xa18
			 0x78fb6e2b, /// 0xa1c
			 0xee84d8fc, /// 0xa20
			 0xc8a5f05f, /// 0xa24
			 0xd0c1a17c, /// 0xa28
			 0x0a0d4042, /// 0xa2c
			 0x4105d607, /// 0xa30
			 0xb036d183, /// 0xa34
			 0x93f019f9, /// 0xa38
			 0x9a9de5e3, /// 0xa3c
			 0xd2ba612d, /// 0xa40
			 0xbbf07d54, /// 0xa44
			 0x3e48bde0, /// 0xa48
			 0x243e94d3, /// 0xa4c
			 0x36cb45e0, /// 0xa50
			 0x2fddbf09, /// 0xa54
			 0xbbc1d82c, /// 0xa58
			 0x4a7d962b, /// 0xa5c
			 0x1e3cc241, /// 0xa60
			 0x6c3d535d, /// 0xa64
			 0x4a0c4328, /// 0xa68
			 0x9af6cccd, /// 0xa6c
			 0x3ab77e9f, /// 0xa70
			 0x642245ab, /// 0xa74
			 0x227aaa87, /// 0xa78
			 0xe35e651f, /// 0xa7c
			 0xc1a5c7e8, /// 0xa80
			 0x6f386495, /// 0xa84
			 0x582a10ae, /// 0xa88
			 0x66012a7b, /// 0xa8c
			 0xb316d9a7, /// 0xa90
			 0xf62ea53e, /// 0xa94
			 0xdaa060a1, /// 0xa98
			 0xb273bca6, /// 0xa9c
			 0xb8ec9f30, /// 0xaa0
			 0xe0891c7c, /// 0xaa4
			 0xcabef07e, /// 0xaa8
			 0xa191c7b1, /// 0xaac
			 0xfea49bce, /// 0xab0
			 0xcffab336, /// 0xab4
			 0xfb457a69, /// 0xab8
			 0x784b9d94, /// 0xabc
			 0xdfdb6eae, /// 0xac0
			 0x7cd3dcbb, /// 0xac4
			 0xbff3fae8, /// 0xac8
			 0xc13fd981, /// 0xacc
			 0x6b13141c, /// 0xad0
			 0x33aa634a, /// 0xad4
			 0xce6fb6a1, /// 0xad8
			 0x2a7ebf27, /// 0xadc
			 0x7566fea5, /// 0xae0
			 0x71090e75, /// 0xae4
			 0xdbc6e383, /// 0xae8
			 0x784945da, /// 0xaec
			 0xecd1607d, /// 0xaf0
			 0xf35f1ea9, /// 0xaf4
			 0x6f0c6f75, /// 0xaf8
			 0x7c5d2087, /// 0xafc
			 0x68e2a9f1, /// 0xb00
			 0x46bea80d, /// 0xb04
			 0x45bd99a3, /// 0xb08
			 0xd13f450b, /// 0xb0c
			 0xe03e80c1, /// 0xb10
			 0x5164c159, /// 0xb14
			 0x8d0c487c, /// 0xb18
			 0x3de50e73, /// 0xb1c
			 0xe29049ce, /// 0xb20
			 0xec031a13, /// 0xb24
			 0xff4d56a0, /// 0xb28
			 0xeb869865, /// 0xb2c
			 0xc43e0257, /// 0xb30
			 0x5136efc2, /// 0xb34
			 0xe7ee4b6e, /// 0xb38
			 0x56fdd5a2, /// 0xb3c
			 0xe0cf4444, /// 0xb40
			 0xd2a4457f, /// 0xb44
			 0xb49c2e28, /// 0xb48
			 0xdd4a439f, /// 0xb4c
			 0xb34cd9f1, /// 0xb50
			 0xa005114a, /// 0xb54
			 0x99785514, /// 0xb58
			 0xe9c11eac, /// 0xb5c
			 0x79d6f27e, /// 0xb60
			 0x75dda4a8, /// 0xb64
			 0x8a3ab28c, /// 0xb68
			 0x09ef54d4, /// 0xb6c
			 0x3dee9e90, /// 0xb70
			 0x91918971, /// 0xb74
			 0x1ac2e855, /// 0xb78
			 0xc75acffc, /// 0xb7c
			 0xfea616ca, /// 0xb80
			 0xacbf3caa, /// 0xb84
			 0x2be46147, /// 0xb88
			 0xbc63f794, /// 0xb8c
			 0xb37b26fb, /// 0xb90
			 0x03ae7eb0, /// 0xb94
			 0x89c6216d, /// 0xb98
			 0x975ecb48, /// 0xb9c
			 0x5aa120ed, /// 0xba0
			 0x0c0b8ab0, /// 0xba4
			 0x1ed347e9, /// 0xba8
			 0xb0fd31a4, /// 0xbac
			 0xcd135dd4, /// 0xbb0
			 0x7e510281, /// 0xbb4
			 0xfe1d06fd, /// 0xbb8
			 0x77ff353d, /// 0xbbc
			 0x3825d966, /// 0xbc0
			 0x4f31a58e, /// 0xbc4
			 0xf54627e1, /// 0xbc8
			 0xcd4a6dc2, /// 0xbcc
			 0xc6932e92, /// 0xbd0
			 0x09262057, /// 0xbd4
			 0x8ef7967b, /// 0xbd8
			 0x2362e73c, /// 0xbdc
			 0xea012590, /// 0xbe0
			 0xf25cd1ae, /// 0xbe4
			 0x20e08aba, /// 0xbe8
			 0x1f7966e4, /// 0xbec
			 0xa62c20ce, /// 0xbf0
			 0xfff61ecd, /// 0xbf4
			 0x47b7fa1f, /// 0xbf8
			 0xd916d47b, /// 0xbfc
			 0xab3438b0, /// 0xc00
			 0x75bafb4a, /// 0xc04
			 0xbb7f5c12, /// 0xc08
			 0x38b1e4fc, /// 0xc0c
			 0x16b4a57a, /// 0xc10
			 0x4aacab41, /// 0xc14
			 0xdf8425de, /// 0xc18
			 0x6acdcf94, /// 0xc1c
			 0x3d2ba3e5, /// 0xc20
			 0xe05e565a, /// 0xc24
			 0xd19c9fc4, /// 0xc28
			 0x3d5ad3e1, /// 0xc2c
			 0xf37f3c42, /// 0xc30
			 0xeedd52b7, /// 0xc34
			 0xaf23ac98, /// 0xc38
			 0x333609ba, /// 0xc3c
			 0x8b77bb61, /// 0xc40
			 0x2b7d7ed0, /// 0xc44
			 0xa60003a1, /// 0xc48
			 0x9af9b98c, /// 0xc4c
			 0x07e6495a, /// 0xc50
			 0xd18daf53, /// 0xc54
			 0x262274e3, /// 0xc58
			 0x63a8fc4b, /// 0xc5c
			 0x8e26bd06, /// 0xc60
			 0x693d8e2f, /// 0xc64
			 0xdebeb5b7, /// 0xc68
			 0x0469b159, /// 0xc6c
			 0x5f24e4d7, /// 0xc70
			 0x0524280a, /// 0xc74
			 0xd996a7c3, /// 0xc78
			 0x7fbe1a22, /// 0xc7c
			 0xc3645dad, /// 0xc80
			 0x6a9847da, /// 0xc84
			 0x75d67416, /// 0xc88
			 0x2458e9e6, /// 0xc8c
			 0xc6a8dfa5, /// 0xc90
			 0x8cfb2280, /// 0xc94
			 0x35d95975, /// 0xc98
			 0xa36eb880, /// 0xc9c
			 0xe360cf1e, /// 0xca0
			 0x70a2f1ed, /// 0xca4
			 0x35674515, /// 0xca8
			 0xcd18958f, /// 0xcac
			 0xc283ad78, /// 0xcb0
			 0x4ae2676d, /// 0xcb4
			 0x6b8b31f4, /// 0xcb8
			 0x02467825, /// 0xcbc
			 0xee8d07e9, /// 0xcc0
			 0x7bbaf5f7, /// 0xcc4
			 0xdcae0990, /// 0xcc8
			 0xe3a5e368, /// 0xccc
			 0xbc3598de, /// 0xcd0
			 0xb19c2de5, /// 0xcd4
			 0xe1230d0d, /// 0xcd8
			 0x23d2ce72, /// 0xcdc
			 0xfad57aa3, /// 0xce0
			 0x083c99ac, /// 0xce4
			 0x008b784c, /// 0xce8
			 0x2147b0de, /// 0xcec
			 0xd8caa372, /// 0xcf0
			 0x01d6b4ab, /// 0xcf4
			 0x99fa1893, /// 0xcf8
			 0x751a8d81, /// 0xcfc
			 0xfb3fe992, /// 0xd00
			 0xb6cc6322, /// 0xd04
			 0xcfc75612, /// 0xd08
			 0xccf6248c, /// 0xd0c
			 0xe9091602, /// 0xd10
			 0xdea1ba90, /// 0xd14
			 0x09eb9b04, /// 0xd18
			 0xb8537c95, /// 0xd1c
			 0x3bc2c79b, /// 0xd20
			 0x0e411ffe, /// 0xd24
			 0x03f4aa16, /// 0xd28
			 0xc2b2c4ad, /// 0xd2c
			 0x508fa0b8, /// 0xd30
			 0x4a6246b2, /// 0xd34
			 0x2d29d05f, /// 0xd38
			 0x6cd30f0d, /// 0xd3c
			 0xb4b498d4, /// 0xd40
			 0x62b22565, /// 0xd44
			 0x6ff8eb9e, /// 0xd48
			 0xe33198fb, /// 0xd4c
			 0x66fe239b, /// 0xd50
			 0xcb6de875, /// 0xd54
			 0x90896fb7, /// 0xd58
			 0x3cdf2d66, /// 0xd5c
			 0xabe3730f, /// 0xd60
			 0xc7a14897, /// 0xd64
			 0x9ed5394e, /// 0xd68
			 0xeb5379d3, /// 0xd6c
			 0x478bd6d9, /// 0xd70
			 0xdb690c74, /// 0xd74
			 0x2334bdf5, /// 0xd78
			 0x6d55a1ec, /// 0xd7c
			 0xeffeb2f8, /// 0xd80
			 0x0c39fe3d, /// 0xd84
			 0x00e627b2, /// 0xd88
			 0x4fa33105, /// 0xd8c
			 0x59f211c4, /// 0xd90
			 0x01ed64e8, /// 0xd94
			 0x2c6c5a8f, /// 0xd98
			 0x13689caf, /// 0xd9c
			 0x91f0442e, /// 0xda0
			 0xb96a4398, /// 0xda4
			 0x5fcb623f, /// 0xda8
			 0xd69dcc08, /// 0xdac
			 0xf69ee0cb, /// 0xdb0
			 0x543e30b9, /// 0xdb4
			 0x41b12c0d, /// 0xdb8
			 0x0d9687d1, /// 0xdbc
			 0x5fbfec2c, /// 0xdc0
			 0x6994fa6f, /// 0xdc4
			 0x605a890e, /// 0xdc8
			 0x16ff07bb, /// 0xdcc
			 0xa946acbf, /// 0xdd0
			 0x49c60de7, /// 0xdd4
			 0xe64c3fea, /// 0xdd8
			 0xb38612fc, /// 0xddc
			 0xdc5d0e41, /// 0xde0
			 0x714765fc, /// 0xde4
			 0xc79528aa, /// 0xde8
			 0x0805d9f4, /// 0xdec
			 0xdac7924c, /// 0xdf0
			 0xd1397bd7, /// 0xdf4
			 0xbd7a7516, /// 0xdf8
			 0xfabd1d0e, /// 0xdfc
			 0xb495179d, /// 0xe00
			 0xa12fa6f7, /// 0xe04
			 0x026f4220, /// 0xe08
			 0x205ee64b, /// 0xe0c
			 0x6b9cc4f0, /// 0xe10
			 0xe1e75847, /// 0xe14
			 0x70232405, /// 0xe18
			 0x279901e2, /// 0xe1c
			 0xea68ee63, /// 0xe20
			 0xc8abf0c6, /// 0xe24
			 0x7416dad1, /// 0xe28
			 0xbf7ba39c, /// 0xe2c
			 0xfbfdc4cf, /// 0xe30
			 0x0cb0a2b6, /// 0xe34
			 0x5962cd74, /// 0xe38
			 0x074f170b, /// 0xe3c
			 0xe267e30d, /// 0xe40
			 0xe13415ce, /// 0xe44
			 0x90e239e2, /// 0xe48
			 0x6ff2b087, /// 0xe4c
			 0x6cc5ac9c, /// 0xe50
			 0xd214445d, /// 0xe54
			 0x47ddf90d, /// 0xe58
			 0xcb860c48, /// 0xe5c
			 0xcd692f5c, /// 0xe60
			 0x2f3f0284, /// 0xe64
			 0x98dc957e, /// 0xe68
			 0xc0c16808, /// 0xe6c
			 0xd9c3ee79, /// 0xe70
			 0x0607d8c7, /// 0xe74
			 0x8c51086d, /// 0xe78
			 0x258adb19, /// 0xe7c
			 0x4f2bc701, /// 0xe80
			 0x1966155d, /// 0xe84
			 0x07e511b9, /// 0xe88
			 0x43e00dd0, /// 0xe8c
			 0x5989cf9d, /// 0xe90
			 0xfb29a27c, /// 0xe94
			 0xaf738440, /// 0xe98
			 0x953e7826, /// 0xe9c
			 0x093549f2, /// 0xea0
			 0xad5161be, /// 0xea4
			 0xbc043edc, /// 0xea8
			 0x5f3d0942, /// 0xeac
			 0x416bd0cd, /// 0xeb0
			 0x90a3330d, /// 0xeb4
			 0xd1816107, /// 0xeb8
			 0xeb01641d, /// 0xebc
			 0x84c59bbd, /// 0xec0
			 0xa7245dc9, /// 0xec4
			 0xb003a558, /// 0xec8
			 0xcb78c806, /// 0xecc
			 0xe102f19d, /// 0xed0
			 0xd9f8df2a, /// 0xed4
			 0x8af22b21, /// 0xed8
			 0xa8db8b15, /// 0xedc
			 0xc06fdac5, /// 0xee0
			 0x3225a87b, /// 0xee4
			 0xc23d2f0c, /// 0xee8
			 0x418c4e6e, /// 0xeec
			 0x6ac2e107, /// 0xef0
			 0xaeef2e46, /// 0xef4
			 0x39e97b81, /// 0xef8
			 0x4bbf2eef, /// 0xefc
			 0xef0fe5a0, /// 0xf00
			 0x1bcdbd33, /// 0xf04
			 0x7a724dde, /// 0xf08
			 0x48c0c44c, /// 0xf0c
			 0x7e45a383, /// 0xf10
			 0x76641401, /// 0xf14
			 0x8ec09f04, /// 0xf18
			 0x2219a58a, /// 0xf1c
			 0x18db0342, /// 0xf20
			 0x40ff863f, /// 0xf24
			 0x891ae3df, /// 0xf28
			 0x8fe565e5, /// 0xf2c
			 0x3fa63d96, /// 0xf30
			 0x78e6c93e, /// 0xf34
			 0xa8645ff2, /// 0xf38
			 0x3639cfec, /// 0xf3c
			 0xdf2c7d60, /// 0xf40
			 0xef777825, /// 0xf44
			 0x8a123c6e, /// 0xf48
			 0x19aa6acc, /// 0xf4c
			 0xbdff953a, /// 0xf50
			 0xc26b7014, /// 0xf54
			 0xe63e3b6a, /// 0xf58
			 0x63796f5a, /// 0xf5c
			 0x792fcc1d, /// 0xf60
			 0xee1a8723, /// 0xf64
			 0x4b088960, /// 0xf68
			 0x57d9e915, /// 0xf6c
			 0xe73b3c28, /// 0xf70
			 0xce6d9ab5, /// 0xf74
			 0xe2ee6f16, /// 0xf78
			 0xdd992475, /// 0xf7c
			 0xc10a0eff, /// 0xf80
			 0xcf670c0e, /// 0xf84
			 0x264289f2, /// 0xf88
			 0xb98cfa59, /// 0xf8c
			 0x8de7468a, /// 0xf90
			 0xa46d9e2a, /// 0xf94
			 0x42089ed5, /// 0xf98
			 0x8c1f6dd3, /// 0xf9c
			 0x909e26e4, /// 0xfa0
			 0x5b5e4e10, /// 0xfa4
			 0x1476d529, /// 0xfa8
			 0x0412e008, /// 0xfac
			 0x88075967, /// 0xfb0
			 0x170d4025, /// 0xfb4
			 0x9beeebf8, /// 0xfb8
			 0xb206f3ad, /// 0xfbc
			 0x6a95bcc6, /// 0xfc0
			 0xeaeb28ab, /// 0xfc4
			 0xf212a9a0, /// 0xfc8
			 0x58f174b7, /// 0xfcc
			 0x9647e065, /// 0xfd0
			 0x4ea90a6a, /// 0xfd4
			 0x06534a61, /// 0xfd8
			 0xd30541a5, /// 0xfdc
			 0xd423e2af, /// 0xfe0
			 0xb075d31c, /// 0xfe4
			 0xe00f8399, /// 0xfe8
			 0x4ed16c91, /// 0xfec
			 0x8346209b, /// 0xff0
			 0x87d5bb5a, /// 0xff4
			 0x55e645f5, /// 0xff8
			 0xbbfc77a0 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x2ce5e7f8, /// 0x0
			 0xc442b48d, /// 0x4
			 0x565d9fbc, /// 0x8
			 0xe1d9753e, /// 0xc
			 0x4dfed8a3, /// 0x10
			 0x254716e9, /// 0x14
			 0xc0de7ca4, /// 0x18
			 0xb85acfc2, /// 0x1c
			 0x57631371, /// 0x20
			 0xa26fb941, /// 0x24
			 0x528dfb43, /// 0x28
			 0x08f0830d, /// 0x2c
			 0xf204cfd3, /// 0x30
			 0x8dffa51f, /// 0x34
			 0x49e90731, /// 0x38
			 0xa9102cad, /// 0x3c
			 0xa2f19c8e, /// 0x40
			 0x180dd5b0, /// 0x44
			 0x1a6c29fd, /// 0x48
			 0xa661b8e0, /// 0x4c
			 0xa0fa3933, /// 0x50
			 0xa368724b, /// 0x54
			 0x862c0a4c, /// 0x58
			 0xde601195, /// 0x5c
			 0xf6b01389, /// 0x60
			 0x3cfaf06b, /// 0x64
			 0x3a7f8629, /// 0x68
			 0xf13eec2f, /// 0x6c
			 0x88e3b81f, /// 0x70
			 0x9f85a25c, /// 0x74
			 0x29adcf96, /// 0x78
			 0xfa242aad, /// 0x7c
			 0x9330414d, /// 0x80
			 0x99be6a0a, /// 0x84
			 0x46cdab69, /// 0x88
			 0x628a4b0b, /// 0x8c
			 0xb17b390b, /// 0x90
			 0x00aec764, /// 0x94
			 0xa7449ff1, /// 0x98
			 0x3833b252, /// 0x9c
			 0x5573fd33, /// 0xa0
			 0xb60c7e5a, /// 0xa4
			 0x063b77aa, /// 0xa8
			 0x58d9504c, /// 0xac
			 0x314e1cf4, /// 0xb0
			 0x81bf3f01, /// 0xb4
			 0xa88b2fca, /// 0xb8
			 0xe9528e70, /// 0xbc
			 0xdfd66ba0, /// 0xc0
			 0xd68ad076, /// 0xc4
			 0xb5040b32, /// 0xc8
			 0x54d7ea7b, /// 0xcc
			 0xd1cbbe8a, /// 0xd0
			 0x1cb2b812, /// 0xd4
			 0x5173ea1d, /// 0xd8
			 0x116dd422, /// 0xdc
			 0x1ec727b9, /// 0xe0
			 0x6b123e3d, /// 0xe4
			 0xedff275c, /// 0xe8
			 0x53601b57, /// 0xec
			 0x0dcf5216, /// 0xf0
			 0x92b2c7e3, /// 0xf4
			 0x464233b9, /// 0xf8
			 0xd68ca928, /// 0xfc
			 0x313933a3, /// 0x100
			 0x1c0be0ce, /// 0x104
			 0x0984b5c9, /// 0x108
			 0xdac34e93, /// 0x10c
			 0x1f74018a, /// 0x110
			 0x32195dcc, /// 0x114
			 0xfa3b819e, /// 0x118
			 0xbe99291a, /// 0x11c
			 0x75944032, /// 0x120
			 0xb9661e3c, /// 0x124
			 0x92fcbcc2, /// 0x128
			 0x0d717dce, /// 0x12c
			 0xf4bad91f, /// 0x130
			 0x5c436e34, /// 0x134
			 0x6e8a88f0, /// 0x138
			 0x79de11b0, /// 0x13c
			 0xe188a79e, /// 0x140
			 0x874cbd6b, /// 0x144
			 0x7a397ab7, /// 0x148
			 0x35bed9e6, /// 0x14c
			 0xfcbba393, /// 0x150
			 0xd39f3bdc, /// 0x154
			 0x1b66de8a, /// 0x158
			 0x18addaa8, /// 0x15c
			 0xa73ecc7e, /// 0x160
			 0x7b024e71, /// 0x164
			 0x243a1daa, /// 0x168
			 0x31d4e68a, /// 0x16c
			 0x8afbc6d6, /// 0x170
			 0x81839f4a, /// 0x174
			 0xb77138ad, /// 0x178
			 0xcf45efe9, /// 0x17c
			 0x1fa94a0a, /// 0x180
			 0x29227bc3, /// 0x184
			 0xa6c230d9, /// 0x188
			 0xbc2189de, /// 0x18c
			 0xc2ee85f3, /// 0x190
			 0xcc8df609, /// 0x194
			 0x6bb64b80, /// 0x198
			 0x085d97b2, /// 0x19c
			 0x3e14c552, /// 0x1a0
			 0x6412f8b1, /// 0x1a4
			 0xf66752ba, /// 0x1a8
			 0x296490a5, /// 0x1ac
			 0xf195dbd7, /// 0x1b0
			 0xb75c54b4, /// 0x1b4
			 0x1439f490, /// 0x1b8
			 0xea6b8c73, /// 0x1bc
			 0xfc4aa968, /// 0x1c0
			 0x4aa47b3c, /// 0x1c4
			 0xa66eaabd, /// 0x1c8
			 0x2359a7b5, /// 0x1cc
			 0x3b12dff8, /// 0x1d0
			 0x515c0575, /// 0x1d4
			 0xad8f77fd, /// 0x1d8
			 0x63b30c68, /// 0x1dc
			 0x04650492, /// 0x1e0
			 0x8b4673bc, /// 0x1e4
			 0xcd1f3a48, /// 0x1e8
			 0xcf420824, /// 0x1ec
			 0x00ce1fd6, /// 0x1f0
			 0x707166a6, /// 0x1f4
			 0xf45e9aa8, /// 0x1f8
			 0x22793e37, /// 0x1fc
			 0x44580d1c, /// 0x200
			 0x0cc7ec33, /// 0x204
			 0xfcedf893, /// 0x208
			 0x8f23e2ae, /// 0x20c
			 0x609f4411, /// 0x210
			 0xd352aea0, /// 0x214
			 0xdc80b0e4, /// 0x218
			 0xe2342673, /// 0x21c
			 0xe8616cdb, /// 0x220
			 0x1610e1ed, /// 0x224
			 0xc9ed7976, /// 0x228
			 0xa928a11a, /// 0x22c
			 0x36cab211, /// 0x230
			 0x54453c44, /// 0x234
			 0x79e9559c, /// 0x238
			 0x7901d4e3, /// 0x23c
			 0x2f981464, /// 0x240
			 0x40b4905a, /// 0x244
			 0xe8706ef1, /// 0x248
			 0x536460ed, /// 0x24c
			 0x81134a34, /// 0x250
			 0xadba0f8d, /// 0x254
			 0x477ee891, /// 0x258
			 0x387a918f, /// 0x25c
			 0x73d8da6f, /// 0x260
			 0x5760dc2d, /// 0x264
			 0x08712a31, /// 0x268
			 0x2f56bb32, /// 0x26c
			 0xae5b87e5, /// 0x270
			 0x71aa9e0a, /// 0x274
			 0x70272699, /// 0x278
			 0xccd9e140, /// 0x27c
			 0x5a4f8f60, /// 0x280
			 0x288eae60, /// 0x284
			 0xe92508b2, /// 0x288
			 0x05fc8e9c, /// 0x28c
			 0x21646104, /// 0x290
			 0xbf6b7a61, /// 0x294
			 0x5a4d4bc1, /// 0x298
			 0xdafe9cf3, /// 0x29c
			 0x81d4ca48, /// 0x2a0
			 0x0ea0653d, /// 0x2a4
			 0x639abc89, /// 0x2a8
			 0x6f10c43b, /// 0x2ac
			 0x04d0dd8f, /// 0x2b0
			 0x6ad787b2, /// 0x2b4
			 0x21959550, /// 0x2b8
			 0xdfd6fc11, /// 0x2bc
			 0x25e5f9e2, /// 0x2c0
			 0xd59aeb61, /// 0x2c4
			 0x5b422f38, /// 0x2c8
			 0xc6546ab3, /// 0x2cc
			 0xeb544985, /// 0x2d0
			 0xe020442b, /// 0x2d4
			 0x516ec2c2, /// 0x2d8
			 0xa552fbf4, /// 0x2dc
			 0xc7da7006, /// 0x2e0
			 0xf4d6341b, /// 0x2e4
			 0x969c7122, /// 0x2e8
			 0xdac94ed0, /// 0x2ec
			 0xa0ac263b, /// 0x2f0
			 0x71a90da3, /// 0x2f4
			 0x62ee8bda, /// 0x2f8
			 0xb5508655, /// 0x2fc
			 0xd76a66e9, /// 0x300
			 0xd960e0b2, /// 0x304
			 0x2e25b821, /// 0x308
			 0x6105c953, /// 0x30c
			 0x84e50aaa, /// 0x310
			 0x830a4de8, /// 0x314
			 0x59024f59, /// 0x318
			 0x323cda97, /// 0x31c
			 0x7efdecb6, /// 0x320
			 0x69d8bbce, /// 0x324
			 0x8296c730, /// 0x328
			 0xa6d63e39, /// 0x32c
			 0x25cc4df2, /// 0x330
			 0xd5678dd1, /// 0x334
			 0x3ae5d4db, /// 0x338
			 0x925e05e7, /// 0x33c
			 0x760f6f60, /// 0x340
			 0x0c871250, /// 0x344
			 0x7af5082b, /// 0x348
			 0x280a99a1, /// 0x34c
			 0xed43b6eb, /// 0x350
			 0x47ff7f70, /// 0x354
			 0x75ba8d22, /// 0x358
			 0x78c1b337, /// 0x35c
			 0x911a4bfd, /// 0x360
			 0x95dc56aa, /// 0x364
			 0x4e576728, /// 0x368
			 0xece87b17, /// 0x36c
			 0x7ab1d535, /// 0x370
			 0x8855ac42, /// 0x374
			 0x31c57e38, /// 0x378
			 0xf3aaa59f, /// 0x37c
			 0x12045969, /// 0x380
			 0xe7ec1425, /// 0x384
			 0x39925ae0, /// 0x388
			 0xabb88773, /// 0x38c
			 0x814e8f3c, /// 0x390
			 0xda56432b, /// 0x394
			 0xcaacf966, /// 0x398
			 0xbdf1aac0, /// 0x39c
			 0x8bf7e844, /// 0x3a0
			 0xf7d1be54, /// 0x3a4
			 0x9653fd1b, /// 0x3a8
			 0x65bd4e42, /// 0x3ac
			 0xc14d96d1, /// 0x3b0
			 0x4656dae1, /// 0x3b4
			 0xb58c19fc, /// 0x3b8
			 0xebc1e321, /// 0x3bc
			 0x64cb1458, /// 0x3c0
			 0x3f4168d4, /// 0x3c4
			 0x8dc6c359, /// 0x3c8
			 0xedc32e4b, /// 0x3cc
			 0x00c8b39d, /// 0x3d0
			 0x6cdf9a71, /// 0x3d4
			 0x107048c3, /// 0x3d8
			 0xe9853768, /// 0x3dc
			 0x39f3ad54, /// 0x3e0
			 0xf077ad43, /// 0x3e4
			 0x27e7e810, /// 0x3e8
			 0x26827283, /// 0x3ec
			 0x94d091e5, /// 0x3f0
			 0x6f08c32f, /// 0x3f4
			 0xfea4bfe5, /// 0x3f8
			 0xe6b231d5, /// 0x3fc
			 0xee1729d1, /// 0x400
			 0x57e5bb02, /// 0x404
			 0xccb7edcf, /// 0x408
			 0x9c12b6c6, /// 0x40c
			 0x1c720f59, /// 0x410
			 0x246008b3, /// 0x414
			 0xc5b96697, /// 0x418
			 0x567b1ea6, /// 0x41c
			 0x45b7a932, /// 0x420
			 0x0579b2d7, /// 0x424
			 0xc2932e98, /// 0x428
			 0x6464c888, /// 0x42c
			 0x622e07aa, /// 0x430
			 0xa42572aa, /// 0x434
			 0x8700cef6, /// 0x438
			 0xe08fe8d0, /// 0x43c
			 0xe536f1ab, /// 0x440
			 0x83ca6114, /// 0x444
			 0x6c72dd94, /// 0x448
			 0xe232133c, /// 0x44c
			 0xb373ccc3, /// 0x450
			 0xb83ae3d9, /// 0x454
			 0x4ecf433f, /// 0x458
			 0xe3ae21ca, /// 0x45c
			 0xe66f9a7b, /// 0x460
			 0x5e1d20ef, /// 0x464
			 0x13d31db4, /// 0x468
			 0x472c5672, /// 0x46c
			 0xd04cc681, /// 0x470
			 0x0f8d3148, /// 0x474
			 0xee9bdc8d, /// 0x478
			 0x9b6ae76d, /// 0x47c
			 0xf648274d, /// 0x480
			 0x97233d3b, /// 0x484
			 0x45b52f0e, /// 0x488
			 0x670b17fa, /// 0x48c
			 0x282ff383, /// 0x490
			 0xfd3b3fca, /// 0x494
			 0x35360cec, /// 0x498
			 0xe7e05cc2, /// 0x49c
			 0x4767f4c5, /// 0x4a0
			 0x267a9d60, /// 0x4a4
			 0xdf038bde, /// 0x4a8
			 0xc8c2aebf, /// 0x4ac
			 0x0993de75, /// 0x4b0
			 0x0ab31142, /// 0x4b4
			 0xd682f26e, /// 0x4b8
			 0xd3fb785a, /// 0x4bc
			 0x194cf9f2, /// 0x4c0
			 0x89401ff8, /// 0x4c4
			 0x0a42fdf8, /// 0x4c8
			 0x352e1337, /// 0x4cc
			 0xa2ee377f, /// 0x4d0
			 0x05ed1ad6, /// 0x4d4
			 0x4301221f, /// 0x4d8
			 0x5d15f04e, /// 0x4dc
			 0xe9486b79, /// 0x4e0
			 0x2c632db8, /// 0x4e4
			 0x4d6c09c3, /// 0x4e8
			 0x0bab2d0d, /// 0x4ec
			 0x6bfa077b, /// 0x4f0
			 0x32ded0ec, /// 0x4f4
			 0x0ae65184, /// 0x4f8
			 0x94a05ff8, /// 0x4fc
			 0x0220dea4, /// 0x500
			 0x09dfdccc, /// 0x504
			 0x135cce55, /// 0x508
			 0x4a7a44c9, /// 0x50c
			 0x62f9febb, /// 0x510
			 0xe4d0dbb2, /// 0x514
			 0xcaad2fb5, /// 0x518
			 0xe786ec85, /// 0x51c
			 0xc60b640f, /// 0x520
			 0x74f2e10e, /// 0x524
			 0xcd49c8ec, /// 0x528
			 0x6e41aab5, /// 0x52c
			 0x86f6fbe2, /// 0x530
			 0xb51bb91c, /// 0x534
			 0x0609094a, /// 0x538
			 0x982eebc3, /// 0x53c
			 0x74aa0b9f, /// 0x540
			 0x13872caf, /// 0x544
			 0x590add25, /// 0x548
			 0x5eac1c94, /// 0x54c
			 0x1a36ef19, /// 0x550
			 0x9fe57aab, /// 0x554
			 0x4f5575f0, /// 0x558
			 0x517d20a1, /// 0x55c
			 0x3aba3759, /// 0x560
			 0xa26918d1, /// 0x564
			 0xfeb1da5f, /// 0x568
			 0x5f023aac, /// 0x56c
			 0xa1a6c5d9, /// 0x570
			 0x768f5a46, /// 0x574
			 0x166f5137, /// 0x578
			 0x1ec46dab, /// 0x57c
			 0x278c5eae, /// 0x580
			 0xb32c1366, /// 0x584
			 0x27889dc3, /// 0x588
			 0xa2cb56ee, /// 0x58c
			 0x8beccc84, /// 0x590
			 0x6b740420, /// 0x594
			 0xee4b5f24, /// 0x598
			 0xdbdb5dbf, /// 0x59c
			 0x5758b7c0, /// 0x5a0
			 0xd4295c8b, /// 0x5a4
			 0x8f8819fc, /// 0x5a8
			 0xbd8d0f4a, /// 0x5ac
			 0xe45a72f0, /// 0x5b0
			 0xb909bdd6, /// 0x5b4
			 0x8ff3ed9a, /// 0x5b8
			 0x7e77a175, /// 0x5bc
			 0x7b33c35e, /// 0x5c0
			 0x1f43c731, /// 0x5c4
			 0x93c12e49, /// 0x5c8
			 0x8ed6e853, /// 0x5cc
			 0xbcbdd4fb, /// 0x5d0
			 0x94a0a8a4, /// 0x5d4
			 0x9bd8d293, /// 0x5d8
			 0x7df8e9b1, /// 0x5dc
			 0x570613ca, /// 0x5e0
			 0x6338f0bc, /// 0x5e4
			 0x23e2bf0d, /// 0x5e8
			 0x901109a2, /// 0x5ec
			 0x7bff90c0, /// 0x5f0
			 0xbfb6c009, /// 0x5f4
			 0xd74001e2, /// 0x5f8
			 0xbf388fc6, /// 0x5fc
			 0x912b9426, /// 0x600
			 0xe9e82430, /// 0x604
			 0x30298d85, /// 0x608
			 0xce7357c7, /// 0x60c
			 0xa336c8af, /// 0x610
			 0xb3772200, /// 0x614
			 0x7dfddaa7, /// 0x618
			 0x0aba1719, /// 0x61c
			 0x24bfac57, /// 0x620
			 0x8937ee77, /// 0x624
			 0x2d8f0b40, /// 0x628
			 0x0e1523c0, /// 0x62c
			 0xaba47277, /// 0x630
			 0xfe99f38d, /// 0x634
			 0x73db27d7, /// 0x638
			 0x87b14a6c, /// 0x63c
			 0xc178ef1d, /// 0x640
			 0x4af3fdc8, /// 0x644
			 0x4f4c2331, /// 0x648
			 0x1cee286d, /// 0x64c
			 0x5c084eab, /// 0x650
			 0x944e008a, /// 0x654
			 0xb53afc85, /// 0x658
			 0xf759f4cd, /// 0x65c
			 0x27ebb02f, /// 0x660
			 0x60a6ff70, /// 0x664
			 0xb02a267f, /// 0x668
			 0x6b373723, /// 0x66c
			 0x83ff9009, /// 0x670
			 0x79b337a5, /// 0x674
			 0xc52a4828, /// 0x678
			 0xbcc8ef37, /// 0x67c
			 0x7e51947d, /// 0x680
			 0x28c533e9, /// 0x684
			 0xfa1752c1, /// 0x688
			 0x1e0a877a, /// 0x68c
			 0x08204106, /// 0x690
			 0x4e5df2e7, /// 0x694
			 0x83690b6f, /// 0x698
			 0xfef12593, /// 0x69c
			 0xbeed9530, /// 0x6a0
			 0xd2ca8c3e, /// 0x6a4
			 0x5c3358fc, /// 0x6a8
			 0xa08258b8, /// 0x6ac
			 0xbbd3e55e, /// 0x6b0
			 0x3ea7a6ae, /// 0x6b4
			 0x47e61273, /// 0x6b8
			 0xa1b78eec, /// 0x6bc
			 0x01c1e889, /// 0x6c0
			 0xc44e5163, /// 0x6c4
			 0x95ea8165, /// 0x6c8
			 0x49ac5788, /// 0x6cc
			 0xe727464a, /// 0x6d0
			 0x450e4c19, /// 0x6d4
			 0x03677292, /// 0x6d8
			 0x75da7ec5, /// 0x6dc
			 0xea5ec5c2, /// 0x6e0
			 0x31a8caf4, /// 0x6e4
			 0x49555bcf, /// 0x6e8
			 0xa6a39e3b, /// 0x6ec
			 0xeb899088, /// 0x6f0
			 0x1c2fb704, /// 0x6f4
			 0x233c0e5a, /// 0x6f8
			 0x4171f574, /// 0x6fc
			 0x344af6cd, /// 0x700
			 0x0df5a0d6, /// 0x704
			 0xe6f921ba, /// 0x708
			 0x34dc19b7, /// 0x70c
			 0x1b563edf, /// 0x710
			 0xc6da2095, /// 0x714
			 0xd0af3c85, /// 0x718
			 0xcadbb928, /// 0x71c
			 0xad1c6af3, /// 0x720
			 0x1c7fd6c2, /// 0x724
			 0x47da6b9c, /// 0x728
			 0xafb458f1, /// 0x72c
			 0xe6e96bdf, /// 0x730
			 0x092934e9, /// 0x734
			 0xc55824eb, /// 0x738
			 0x88f1e079, /// 0x73c
			 0x83228e7a, /// 0x740
			 0x4d5c2d97, /// 0x744
			 0xbf82fbd4, /// 0x748
			 0x310963f9, /// 0x74c
			 0x333a1186, /// 0x750
			 0x2dde3012, /// 0x754
			 0x6d9162ac, /// 0x758
			 0x85e1da2f, /// 0x75c
			 0xd9f123aa, /// 0x760
			 0xe8ecd66b, /// 0x764
			 0x665b60ef, /// 0x768
			 0x6e2dcb30, /// 0x76c
			 0x08335245, /// 0x770
			 0x6530005c, /// 0x774
			 0xf07f375c, /// 0x778
			 0x450d283d, /// 0x77c
			 0x599b5141, /// 0x780
			 0x25bbd35f, /// 0x784
			 0x4a772b77, /// 0x788
			 0xf46376bf, /// 0x78c
			 0xa05d0253, /// 0x790
			 0xd84fa42f, /// 0x794
			 0x796a22eb, /// 0x798
			 0x2dcff4ab, /// 0x79c
			 0x7e064f99, /// 0x7a0
			 0x8db5e4a3, /// 0x7a4
			 0x521251b8, /// 0x7a8
			 0xdcac372a, /// 0x7ac
			 0x647117c2, /// 0x7b0
			 0x9520331f, /// 0x7b4
			 0x5a4d98f0, /// 0x7b8
			 0x87e39149, /// 0x7bc
			 0xccd556f3, /// 0x7c0
			 0x84717f25, /// 0x7c4
			 0x6da94fc1, /// 0x7c8
			 0x293f42f9, /// 0x7cc
			 0x950550e8, /// 0x7d0
			 0xe37bdcb6, /// 0x7d4
			 0x17392256, /// 0x7d8
			 0xe0054e52, /// 0x7dc
			 0x8317d665, /// 0x7e0
			 0x17f8ada3, /// 0x7e4
			 0x78c5d1cf, /// 0x7e8
			 0xe4e73c05, /// 0x7ec
			 0xa7249a39, /// 0x7f0
			 0xe649659a, /// 0x7f4
			 0x97db321f, /// 0x7f8
			 0x8361144b, /// 0x7fc
			 0xe4231ef3, /// 0x800
			 0x0849441a, /// 0x804
			 0x6db5321b, /// 0x808
			 0x0afbb175, /// 0x80c
			 0x3ea870da, /// 0x810
			 0xc7932128, /// 0x814
			 0x89519ae3, /// 0x818
			 0xbb70778b, /// 0x81c
			 0x2d6b5a8e, /// 0x820
			 0x1bf5325a, /// 0x824
			 0x651e69ae, /// 0x828
			 0xb059dd59, /// 0x82c
			 0xbcafb8f8, /// 0x830
			 0xca2b6e56, /// 0x834
			 0x8bad1f61, /// 0x838
			 0x120ef76c, /// 0x83c
			 0xdbe3cb13, /// 0x840
			 0x7df621b4, /// 0x844
			 0xa316dcad, /// 0x848
			 0x349416f2, /// 0x84c
			 0x7e719878, /// 0x850
			 0x8a1f3075, /// 0x854
			 0x0a547c55, /// 0x858
			 0xf234554c, /// 0x85c
			 0x44fcc6f2, /// 0x860
			 0xf8121985, /// 0x864
			 0xa4717b0b, /// 0x868
			 0x696f003c, /// 0x86c
			 0x4402c58a, /// 0x870
			 0xdee5e431, /// 0x874
			 0x57a4b49c, /// 0x878
			 0x8a7d26cd, /// 0x87c
			 0x0a2b1607, /// 0x880
			 0xf3599941, /// 0x884
			 0xab0ee062, /// 0x888
			 0x9e0e5155, /// 0x88c
			 0x6a4c66de, /// 0x890
			 0x3f8a4095, /// 0x894
			 0x9b5c9787, /// 0x898
			 0x97a7d48f, /// 0x89c
			 0x4a89e42e, /// 0x8a0
			 0xc44378f7, /// 0x8a4
			 0x5bba4cee, /// 0x8a8
			 0x1e04950c, /// 0x8ac
			 0xe0ab85f9, /// 0x8b0
			 0xc70aaa25, /// 0x8b4
			 0x10704e64, /// 0x8b8
			 0x5b77f5cb, /// 0x8bc
			 0x290b079d, /// 0x8c0
			 0x21033777, /// 0x8c4
			 0xc67b40c4, /// 0x8c8
			 0x2631d4f6, /// 0x8cc
			 0x85d39f2c, /// 0x8d0
			 0xd58f53e8, /// 0x8d4
			 0xc5a756c7, /// 0x8d8
			 0xf4e71e2b, /// 0x8dc
			 0x576f1de1, /// 0x8e0
			 0x2ec9663c, /// 0x8e4
			 0xb1ddf651, /// 0x8e8
			 0x3f318011, /// 0x8ec
			 0xf13baf95, /// 0x8f0
			 0x8ec55139, /// 0x8f4
			 0xcefe2ab0, /// 0x8f8
			 0x8138fc2a, /// 0x8fc
			 0x6904d646, /// 0x900
			 0x771707dc, /// 0x904
			 0x4fd9ea52, /// 0x908
			 0x1d136c24, /// 0x90c
			 0x6fa69f1f, /// 0x910
			 0x1d2d11ce, /// 0x914
			 0xb7cae0bf, /// 0x918
			 0xffb82f46, /// 0x91c
			 0xd3acf595, /// 0x920
			 0xdf86397d, /// 0x924
			 0x185696e1, /// 0x928
			 0x8712d7ba, /// 0x92c
			 0x66866a97, /// 0x930
			 0x257e4950, /// 0x934
			 0x4c4a1033, /// 0x938
			 0x162ba284, /// 0x93c
			 0xaed55e4e, /// 0x940
			 0x2398e581, /// 0x944
			 0xadd5a577, /// 0x948
			 0x6c96105f, /// 0x94c
			 0xccf83678, /// 0x950
			 0x12f73d97, /// 0x954
			 0x1817187d, /// 0x958
			 0x577a9220, /// 0x95c
			 0xd9739130, /// 0x960
			 0x5ac35e2e, /// 0x964
			 0xb03fc222, /// 0x968
			 0x5b33091f, /// 0x96c
			 0x8914825e, /// 0x970
			 0x316265ef, /// 0x974
			 0xd5657a2c, /// 0x978
			 0x4808b79a, /// 0x97c
			 0x8acea97f, /// 0x980
			 0x7a552598, /// 0x984
			 0xf80464c2, /// 0x988
			 0xf32231dd, /// 0x98c
			 0xc8dc14b0, /// 0x990
			 0x61213251, /// 0x994
			 0x8d96d194, /// 0x998
			 0x6521a2aa, /// 0x99c
			 0x594767ab, /// 0x9a0
			 0x4ed6b1fa, /// 0x9a4
			 0xf5fcd75b, /// 0x9a8
			 0x629d097f, /// 0x9ac
			 0xa3292adc, /// 0x9b0
			 0xef5ff73e, /// 0x9b4
			 0x9589951f, /// 0x9b8
			 0xb38a32a1, /// 0x9bc
			 0x9e66f722, /// 0x9c0
			 0xa8e95412, /// 0x9c4
			 0x021f1fe3, /// 0x9c8
			 0x30fe59f4, /// 0x9cc
			 0x8a223ea2, /// 0x9d0
			 0xa9c8e092, /// 0x9d4
			 0x2ca1d4b1, /// 0x9d8
			 0x4e2ce267, /// 0x9dc
			 0x1cc80306, /// 0x9e0
			 0x6a551056, /// 0x9e4
			 0x9567ecb0, /// 0x9e8
			 0x24ba383c, /// 0x9ec
			 0x54a7f1e2, /// 0x9f0
			 0x990bcf07, /// 0x9f4
			 0xd542cd43, /// 0x9f8
			 0x9129bcb2, /// 0x9fc
			 0xb3b07fb2, /// 0xa00
			 0xcd179242, /// 0xa04
			 0xca8e20f1, /// 0xa08
			 0xf2f4fb80, /// 0xa0c
			 0x624e2a43, /// 0xa10
			 0x78983c1c, /// 0xa14
			 0xa2050efc, /// 0xa18
			 0x2a9623da, /// 0xa1c
			 0x044b520d, /// 0xa20
			 0x2ca6e3fc, /// 0xa24
			 0xa6577aa6, /// 0xa28
			 0x75c813e4, /// 0xa2c
			 0x893e9498, /// 0xa30
			 0xe6ad7a02, /// 0xa34
			 0xeb091735, /// 0xa38
			 0x373bfd77, /// 0xa3c
			 0x429ee6dc, /// 0xa40
			 0x6696898e, /// 0xa44
			 0x6e208c99, /// 0xa48
			 0x73d84f68, /// 0xa4c
			 0x6b02dc30, /// 0xa50
			 0xe76476d2, /// 0xa54
			 0xa2c0789e, /// 0xa58
			 0x9da3dd26, /// 0xa5c
			 0xaedc149b, /// 0xa60
			 0x8f028e08, /// 0xa64
			 0xfdfa9680, /// 0xa68
			 0xa45f99cb, /// 0xa6c
			 0x45839adb, /// 0xa70
			 0xd49ff5f9, /// 0xa74
			 0x12c45f55, /// 0xa78
			 0x8b862534, /// 0xa7c
			 0x90d14ee7, /// 0xa80
			 0x37b47cbd, /// 0xa84
			 0x09675dfa, /// 0xa88
			 0xa08dcffd, /// 0xa8c
			 0xbac3a927, /// 0xa90
			 0x1abd861e, /// 0xa94
			 0x60f23ba4, /// 0xa98
			 0x2b89a1fa, /// 0xa9c
			 0x47b0d7a3, /// 0xaa0
			 0xbdbed2c4, /// 0xaa4
			 0x5bf20e7e, /// 0xaa8
			 0x518db08b, /// 0xaac
			 0x76e54cf2, /// 0xab0
			 0x1e832e5d, /// 0xab4
			 0x0835e024, /// 0xab8
			 0x2ef5ee7c, /// 0xabc
			 0x65bd6a74, /// 0xac0
			 0xa4267dcf, /// 0xac4
			 0x952ddc7c, /// 0xac8
			 0x4b81f1f2, /// 0xacc
			 0x8d7b0357, /// 0xad0
			 0x29383902, /// 0xad4
			 0xf6da2d42, /// 0xad8
			 0x6c6e3388, /// 0xadc
			 0x2e0893f1, /// 0xae0
			 0x468a7b22, /// 0xae4
			 0x0d8e9c33, /// 0xae8
			 0xb2a44ac8, /// 0xaec
			 0x398cdbf7, /// 0xaf0
			 0x8baa5a78, /// 0xaf4
			 0x7d62187e, /// 0xaf8
			 0xa83195e2, /// 0xafc
			 0xb4887a6b, /// 0xb00
			 0x3c03b4f3, /// 0xb04
			 0x1595607f, /// 0xb08
			 0xc2131899, /// 0xb0c
			 0xa6aa7c37, /// 0xb10
			 0x13bcc15e, /// 0xb14
			 0xc0bf3359, /// 0xb18
			 0xf8c7f01c, /// 0xb1c
			 0x2d495d37, /// 0xb20
			 0x445b77f7, /// 0xb24
			 0xf5d4d0d6, /// 0xb28
			 0xd4ead79c, /// 0xb2c
			 0x779d5605, /// 0xb30
			 0xa8c00211, /// 0xb34
			 0x2138ef3f, /// 0xb38
			 0x0095df23, /// 0xb3c
			 0x61c45b51, /// 0xb40
			 0x78432007, /// 0xb44
			 0x17aaec39, /// 0xb48
			 0xa612635c, /// 0xb4c
			 0xb11c512a, /// 0xb50
			 0x5797c29e, /// 0xb54
			 0x34404164, /// 0xb58
			 0x61c5e7ec, /// 0xb5c
			 0x60c7fc7e, /// 0xb60
			 0x8eca48cc, /// 0xb64
			 0xbaa2017c, /// 0xb68
			 0x6c3fc781, /// 0xb6c
			 0x9dfc388c, /// 0xb70
			 0x59532079, /// 0xb74
			 0x38df7992, /// 0xb78
			 0xb231fe8e, /// 0xb7c
			 0x6d56a772, /// 0xb80
			 0x7f7bcc25, /// 0xb84
			 0xe3d03bec, /// 0xb88
			 0xe6500a79, /// 0xb8c
			 0x9edd387e, /// 0xb90
			 0x3cfe8643, /// 0xb94
			 0x8eca11b2, /// 0xb98
			 0xc90cd50e, /// 0xb9c
			 0xeea3659d, /// 0xba0
			 0x74a9a4bc, /// 0xba4
			 0x9f91caf9, /// 0xba8
			 0x46c530b5, /// 0xbac
			 0x4f2e1d5f, /// 0xbb0
			 0x096adfbe, /// 0xbb4
			 0xaf2664a9, /// 0xbb8
			 0xbd4a5b88, /// 0xbbc
			 0x24be1725, /// 0xbc0
			 0x048105ad, /// 0xbc4
			 0x340738e2, /// 0xbc8
			 0x4bbbf1e3, /// 0xbcc
			 0x53e6694c, /// 0xbd0
			 0x40d9cd08, /// 0xbd4
			 0x7ce6085f, /// 0xbd8
			 0xfc8dc9a8, /// 0xbdc
			 0xefb7eb8e, /// 0xbe0
			 0xcbb2e3c0, /// 0xbe4
			 0xf88f9ef4, /// 0xbe8
			 0x3e069f45, /// 0xbec
			 0xf9ae2df7, /// 0xbf0
			 0x1f1109c2, /// 0xbf4
			 0x64dab4e5, /// 0xbf8
			 0x22a451c7, /// 0xbfc
			 0xf536bfd2, /// 0xc00
			 0xa538bae3, /// 0xc04
			 0x7f139ebe, /// 0xc08
			 0x53fc55e3, /// 0xc0c
			 0x4f10a7d6, /// 0xc10
			 0x7ef52ea5, /// 0xc14
			 0x82250883, /// 0xc18
			 0x11d4de62, /// 0xc1c
			 0x86533d96, /// 0xc20
			 0x9482d278, /// 0xc24
			 0x96fd1814, /// 0xc28
			 0xe30b7212, /// 0xc2c
			 0x6fdd7ec1, /// 0xc30
			 0xc14e2378, /// 0xc34
			 0x5920c446, /// 0xc38
			 0x2b4e9af2, /// 0xc3c
			 0x417a6cb7, /// 0xc40
			 0xe0d2d59c, /// 0xc44
			 0x15bfe4b7, /// 0xc48
			 0x0e611d14, /// 0xc4c
			 0x58054f02, /// 0xc50
			 0x5b17ef87, /// 0xc54
			 0xed86aa54, /// 0xc58
			 0xc8fcc9a8, /// 0xc5c
			 0x2e85073a, /// 0xc60
			 0xc6f57f3d, /// 0xc64
			 0x69cab9e3, /// 0xc68
			 0x1ee688b6, /// 0xc6c
			 0x620b67ef, /// 0xc70
			 0xee5d6fa8, /// 0xc74
			 0xf4928b4b, /// 0xc78
			 0xbb1bfdd6, /// 0xc7c
			 0x5a4708b2, /// 0xc80
			 0x5f5c25ea, /// 0xc84
			 0x159345b0, /// 0xc88
			 0x189184c3, /// 0xc8c
			 0x1edf64f0, /// 0xc90
			 0xc8eb73b3, /// 0xc94
			 0x0e3f8e41, /// 0xc98
			 0x1ff55e3f, /// 0xc9c
			 0x00dd9b71, /// 0xca0
			 0xb7ee0acf, /// 0xca4
			 0x9050e6c0, /// 0xca8
			 0x26465275, /// 0xcac
			 0x7e091bf0, /// 0xcb0
			 0xabfce644, /// 0xcb4
			 0x58a4b26c, /// 0xcb8
			 0xef6977bc, /// 0xcbc
			 0x2d185dbc, /// 0xcc0
			 0x2ea30a65, /// 0xcc4
			 0x647b78e7, /// 0xcc8
			 0x77dfda32, /// 0xccc
			 0xb5d17ca3, /// 0xcd0
			 0xe87caab2, /// 0xcd4
			 0xef9d6815, /// 0xcd8
			 0x5fae1e50, /// 0xcdc
			 0x13511d91, /// 0xce0
			 0xd7fa9256, /// 0xce4
			 0x84d4d5e1, /// 0xce8
			 0x88e23e93, /// 0xcec
			 0xe32dd41e, /// 0xcf0
			 0xb45e634d, /// 0xcf4
			 0x85e065b9, /// 0xcf8
			 0x28b7277d, /// 0xcfc
			 0x2a3a4054, /// 0xd00
			 0x70a71e4d, /// 0xd04
			 0x4cd29333, /// 0xd08
			 0x79ea7acb, /// 0xd0c
			 0x6f23076d, /// 0xd10
			 0x29cc83a4, /// 0xd14
			 0x3184362f, /// 0xd18
			 0x6d2bee57, /// 0xd1c
			 0xcb89a718, /// 0xd20
			 0xde8d586f, /// 0xd24
			 0xa63c82ee, /// 0xd28
			 0xc0d76b16, /// 0xd2c
			 0x5d5fb351, /// 0xd30
			 0xb30b3e2b, /// 0xd34
			 0x6af5a7ca, /// 0xd38
			 0x662807fe, /// 0xd3c
			 0x74b0615b, /// 0xd40
			 0xe3d26afd, /// 0xd44
			 0x026e8384, /// 0xd48
			 0x46532880, /// 0xd4c
			 0x90c30337, /// 0xd50
			 0xf307188d, /// 0xd54
			 0x72443289, /// 0xd58
			 0x7ee93ef1, /// 0xd5c
			 0x1e3ba964, /// 0xd60
			 0xf52daaef, /// 0xd64
			 0xea154150, /// 0xd68
			 0xec0bf337, /// 0xd6c
			 0x1119a143, /// 0xd70
			 0x69867004, /// 0xd74
			 0xae40331a, /// 0xd78
			 0xa04b6dcb, /// 0xd7c
			 0x61617179, /// 0xd80
			 0xe53e0d06, /// 0xd84
			 0x365b3747, /// 0xd88
			 0xb925ee9b, /// 0xd8c
			 0xad44510b, /// 0xd90
			 0xbc57ed46, /// 0xd94
			 0x7bb0d2f3, /// 0xd98
			 0xebda537b, /// 0xd9c
			 0x21cc4416, /// 0xda0
			 0xa9e417e0, /// 0xda4
			 0xa97a26a1, /// 0xda8
			 0x50c5234f, /// 0xdac
			 0x0faef202, /// 0xdb0
			 0xee11a624, /// 0xdb4
			 0x654254cf, /// 0xdb8
			 0x3c30036e, /// 0xdbc
			 0x77939e5e, /// 0xdc0
			 0x00554f86, /// 0xdc4
			 0xddbdb619, /// 0xdc8
			 0x64fd6a82, /// 0xdcc
			 0xc44c6a35, /// 0xdd0
			 0xde898f32, /// 0xdd4
			 0x3a37a2d2, /// 0xdd8
			 0x5470809b, /// 0xddc
			 0x6633c6a3, /// 0xde0
			 0x61f665fa, /// 0xde4
			 0x4b7f2b7a, /// 0xde8
			 0xbe5b8b36, /// 0xdec
			 0x0167613f, /// 0xdf0
			 0xf3851a0e, /// 0xdf4
			 0x396ba99c, /// 0xdf8
			 0x0516d502, /// 0xdfc
			 0x8d26a0e5, /// 0xe00
			 0x6f477acb, /// 0xe04
			 0xaf232c45, /// 0xe08
			 0xfcb15e08, /// 0xe0c
			 0x2756eadc, /// 0xe10
			 0x92f84378, /// 0xe14
			 0x52efc00e, /// 0xe18
			 0x9d29fc6f, /// 0xe1c
			 0xe2e1204d, /// 0xe20
			 0x3b580927, /// 0xe24
			 0x1ddba8b5, /// 0xe28
			 0xa5ad6806, /// 0xe2c
			 0xc12963df, /// 0xe30
			 0xef4582e9, /// 0xe34
			 0xf610b76d, /// 0xe38
			 0x14d1acbf, /// 0xe3c
			 0x3b677169, /// 0xe40
			 0xa5bdb773, /// 0xe44
			 0xb578da21, /// 0xe48
			 0x4c62149b, /// 0xe4c
			 0x5486efc4, /// 0xe50
			 0x2b927b98, /// 0xe54
			 0xe75adc6f, /// 0xe58
			 0xe7ca9a50, /// 0xe5c
			 0x403aa3c0, /// 0xe60
			 0x1b0c36da, /// 0xe64
			 0xd3898d8f, /// 0xe68
			 0x82bfd160, /// 0xe6c
			 0xd71e6d17, /// 0xe70
			 0x6c591d8f, /// 0xe74
			 0x3ea57168, /// 0xe78
			 0xc043412f, /// 0xe7c
			 0x35bdb9d1, /// 0xe80
			 0x19dae1c3, /// 0xe84
			 0xde10f916, /// 0xe88
			 0x48b89e90, /// 0xe8c
			 0xe1d6fb4a, /// 0xe90
			 0x38f19fde, /// 0xe94
			 0x064c02dc, /// 0xe98
			 0xe4c1b7de, /// 0xe9c
			 0xb9a075f1, /// 0xea0
			 0x2070c85b, /// 0xea4
			 0x184b8dd0, /// 0xea8
			 0x1699df52, /// 0xeac
			 0x1a2272b5, /// 0xeb0
			 0xacdc32ef, /// 0xeb4
			 0x84c8a354, /// 0xeb8
			 0x4b9dd73c, /// 0xebc
			 0xb6f0ad03, /// 0xec0
			 0x6d1eeb7d, /// 0xec4
			 0x31d99a66, /// 0xec8
			 0xb4f77fe9, /// 0xecc
			 0xcd4b475e, /// 0xed0
			 0xa3f985ed, /// 0xed4
			 0x2d6e3112, /// 0xed8
			 0xa12ca6f8, /// 0xedc
			 0xa2936d42, /// 0xee0
			 0xb442ee6e, /// 0xee4
			 0xac2a2b8f, /// 0xee8
			 0x729423b1, /// 0xeec
			 0x82112dae, /// 0xef0
			 0xf83c2255, /// 0xef4
			 0x54873735, /// 0xef8
			 0x0ce552d7, /// 0xefc
			 0x4528adcf, /// 0xf00
			 0xebf09785, /// 0xf04
			 0x787971dc, /// 0xf08
			 0x160ee9d9, /// 0xf0c
			 0xdcc1be2b, /// 0xf10
			 0xe802e7fb, /// 0xf14
			 0xc86d17a2, /// 0xf18
			 0xfff20306, /// 0xf1c
			 0xc3a0e9e1, /// 0xf20
			 0x242c2931, /// 0xf24
			 0xbf0557f2, /// 0xf28
			 0xeb530e9d, /// 0xf2c
			 0x7ac0cee3, /// 0xf30
			 0x306280a9, /// 0xf34
			 0xe4ced973, /// 0xf38
			 0xf96b50bb, /// 0xf3c
			 0x5abc8c41, /// 0xf40
			 0x6149fafa, /// 0xf44
			 0x572594e9, /// 0xf48
			 0x508275c7, /// 0xf4c
			 0xfdbacf09, /// 0xf50
			 0xae3fb315, /// 0xf54
			 0x8afb1fbf, /// 0xf58
			 0x1b79db78, /// 0xf5c
			 0x7ce29359, /// 0xf60
			 0x038cc70b, /// 0xf64
			 0x2680f4b8, /// 0xf68
			 0x9ccce0c1, /// 0xf6c
			 0xa04cc640, /// 0xf70
			 0xa76bd5cd, /// 0xf74
			 0xe6ea5fdb, /// 0xf78
			 0xd5942c28, /// 0xf7c
			 0x56c18aa5, /// 0xf80
			 0x7e897c21, /// 0xf84
			 0xdcadb263, /// 0xf88
			 0xb0bdcef8, /// 0xf8c
			 0x95a4d2ab, /// 0xf90
			 0x292c66ea, /// 0xf94
			 0x6534687b, /// 0xf98
			 0x93311dc5, /// 0xf9c
			 0x09b7f1ca, /// 0xfa0
			 0x98279c41, /// 0xfa4
			 0x4d0f9aff, /// 0xfa8
			 0x94d5a3a7, /// 0xfac
			 0xb098465b, /// 0xfb0
			 0x90bedfdf, /// 0xfb4
			 0xd05c7be9, /// 0xfb8
			 0x52f2e536, /// 0xfbc
			 0x4cebe29e, /// 0xfc0
			 0xa6db1136, /// 0xfc4
			 0x2a2f1729, /// 0xfc8
			 0x445286cb, /// 0xfcc
			 0x22c2dc41, /// 0xfd0
			 0xe504f1cf, /// 0xfd4
			 0xc713764e, /// 0xfd8
			 0x659f68ea, /// 0xfdc
			 0xc770e22a, /// 0xfe0
			 0xe44ea7be, /// 0xfe4
			 0x0493e707, /// 0xfe8
			 0x819d3bd1, /// 0xfec
			 0x07310d83, /// 0xff0
			 0xb4fd9ba8, /// 0xff4
			 0xa73bb9c9, /// 0xff8
			 0xa6a2dfb0 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x0c780000,                                                  // Leading 1s:                                 /// 00004
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00014
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0001c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00028
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00030
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00040
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00048
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0004c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00050
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00054
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00060
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00064
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0006c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00074
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00078
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0xbf028f5c,                                                  // -0.51                                       /// 00084
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00088
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00090
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c4
			 0x55555555,                                                  // 4 random values                             /// 000c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0xff800000,                                                  // -inf                                        /// 00108
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00118
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00124
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00128
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00130
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00134
			 0x0c600000,                                                  // Leading 1s:                                 /// 00138
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00144
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00148
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00150
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00158
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00168
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0016c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00170
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00174
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00178
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0017c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0c400000,                                                  // Leading 1s:                                 /// 00184
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0018c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00190
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00194
			 0x0c400000,                                                  // Leading 1s:                                 /// 00198
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001ac
			 0x80000000,                                                  // -zero                                       /// 001b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00200
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00208
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00210
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0021c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00224
			 0x0c400000,                                                  // Leading 1s:                                 /// 00228
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0022c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00230
			 0x0e000003,                                                  // Trailing 1s:                                /// 00234
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00238
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0023c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00244
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00248
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0024c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00250
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00254
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x33333333,                                                  // 4 random values                             /// 0025c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00260
			 0x80000000,                                                  // -zero                                       /// 00264
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0026c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00274
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00278
			 0x0c700000,                                                  // Leading 1s:                                 /// 0027c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00284
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0028c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00294
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00298
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x80000000,                                                  // -zero                                       /// 002a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c4
			 0x4b000000,                                                  // 8388608.0                                   /// 002c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00300
			 0x0e000001,                                                  // Trailing 1s:                                /// 00304
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00308
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00310
			 0x80011111,                                                  // -9.7958E-41                                 /// 00314
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00318
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0031c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00320
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00324
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00328
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0032c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00334
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0033c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00340
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00348
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00350
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00354
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00358
			 0x0c700000,                                                  // Leading 1s:                                 /// 0035c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00360
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00368
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00370
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0037c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0038c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00394
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0039c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00400
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00404
			 0x55555555,                                                  // 4 random values                             /// 00408
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00418
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0041c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00420
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00424
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00438
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00440
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00444
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00448
			 0xbf028f5c,                                                  // -0.51                                       /// 0044c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00454
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00464
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00468
			 0x4b000000,                                                  // 8388608.0                                   /// 0046c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00470
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00474
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00478
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00480
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00484
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00490
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00494
			 0xbf028f5c,                                                  // -0.51                                       /// 00498
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0049c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7fc00001,                                                  // signaling NaN                               /// 004d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00500
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0050c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80011111,                                                  // -9.7958E-41                                 /// 00514
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0051c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00520
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00528
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0052c
			 0x00000000,                                                  // zero                                        /// 00530
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00538
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00540
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00550
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00558
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0055c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00560
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00564
			 0x00011111,                                                  // 9.7958E-41                                  /// 00568
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0056c
			 0x4b000000,                                                  // 8388608.0                                   /// 00570
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00574
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00578
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00580
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00590
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00594
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00598
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0059c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00600
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00604
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00608
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0060c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00610
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00614
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0061c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00620
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00624
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00628
			 0x0c780000,                                                  // Leading 1s:                                 /// 0062c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00638
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00640
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00648
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00650
			 0x80011111,                                                  // -9.7958E-41                                 /// 00654
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00658
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0065c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00664
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00670
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00674
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00678
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00684
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00688
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0068c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00694
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00698
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0069c
			 0x00000000,                                                  // zero                                        /// 006a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x0e000007,                                                  // Trailing 1s:                                /// 0070c
			 0x55555555,                                                  // 4 random values                             /// 00710
			 0x3f028f5c,                                                  // 0.51                                        /// 00714
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00718
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00728
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0072c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00730
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0073c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00740
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00748
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0074c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x80000000,                                                  // -zero                                       /// 00754
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00774
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00778
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0077c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00780
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00784
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00788
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0078c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00790
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00794
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00798
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0079c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x7fc00001,                                                  // signaling NaN                               /// 007d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00800
			 0x4b000000,                                                  // 8388608.0                                   /// 00804
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0080c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00810
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0xff800000,                                                  // -inf                                        /// 00818
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0081c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00820
			 0x0c400000,                                                  // Leading 1s:                                 /// 00824
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0082c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00830
			 0x7f800000,                                                  // inf                                         /// 00834
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0083c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00840
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00844
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00848
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0084c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00854
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00860
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00864
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00868
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0086c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00878
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0087c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00884
			 0x80011111,                                                  // -9.7958E-41                                 /// 00888
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00890
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00894
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00898
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0089c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c8
			 0xffc00001,                                                  // -signaling NaN                              /// 008cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00900
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00904
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00908
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0090c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0091c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00920
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00924
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0e000007,                                                  // Trailing 1s:                                /// 0092c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00930
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00934
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00938
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0093c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00940
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00944
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00948
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00950
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00958
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0095c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00960
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00964
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00970
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00984
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00988
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x3f028f5c,                                                  // 0.51                                        /// 00990
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00998
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00011111,                                                  // 9.7958E-41                                  /// 009a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x80000000,                                                  // -zero                                       /// 009ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xffc00001,                                                  // -signaling NaN                              /// 00a48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aa8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00aac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0xff800000,                                                  // -inf                                        /// 00ab4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ac8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00acc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ad8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ae4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aec
			 0x80000000,                                                  // -zero                                       /// 00af0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b00
			 0x7f800000,                                                  // inf                                         /// 00b04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b08
			 0x7fc00001,                                                  // signaling NaN                               /// 00b0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b18
			 0x00000000,                                                  // zero                                        /// 00b1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0xbf028f5c,                                                  // -0.51                                       /// 00b3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000000,                                                  // -zero                                       /// 00b48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ba4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bc8
			 0x00000000,                                                  // zero                                        /// 00bcc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00be0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00be4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00be8
			 0x7f800000,                                                  // inf                                         /// 00bec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bf0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c40
			 0x55555555,                                                  // 4 random values                             /// 00c44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ca0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cac
			 0x3f028f5c,                                                  // 0.51                                        /// 00cb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ce8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x33333333,                                                  // 4 random values                             /// 00cf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cf8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00d00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d34
			 0xcb000000,                                                  // -8388608.0                                  /// 00d38
			 0xbf028f5c,                                                  // -0.51                                       /// 00d3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d84
			 0xff800000,                                                  // -inf                                        /// 00d88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d94
			 0x00000000,                                                  // zero                                        /// 00d98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00da0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00da4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00da8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00db4
			 0x33333333,                                                  // 4 random values                             /// 00db8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x7f800000,                                                  // inf                                         /// 00dc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00de4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00df0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00df8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x7f800000,                                                  // inf                                         /// 00e24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e7c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ea0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ea8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00eb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ebc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x55555555,                                                  // 4 random values                             /// 00ed0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00edc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ee0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ee4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x80000000,                                                  // -zero                                       /// 00eec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ef0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ef4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00efc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f58
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f5c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fac
			 0x00000000,                                                  // zero                                        /// 00fb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fc0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fe0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ff0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ff4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x00800003                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00000
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x55555555,                                                  // 4 random values                             /// 0000c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00014
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0001c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00030
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00038
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0xcb000000,                                                  // -8388608.0                                  /// 00050
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00058
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0005c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00060
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00064
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000000,                                                  // zero                                        /// 00070
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0xffc00001,                                                  // -signaling NaN                              /// 0007c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00088
			 0xbf028f5c,                                                  // -0.51                                       /// 0008c
			 0x4b000000,                                                  // 8388608.0                                   /// 00090
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00094
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00098
			 0x55555555,                                                  // 4 random values                             /// 0009c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0xff800000,                                                  // -inf                                        /// 000b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00104
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0c780000,                                                  // Leading 1s:                                 /// 0010c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00114
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00118
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0011c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00120
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00124
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00128
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00130
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00138
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00140
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00144
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00148
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0014c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00158
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0015c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00160
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0016c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00174
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0017c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00180
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00188
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00194
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00198
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001c8
			 0x4b000000,                                                  // 8388608.0                                   /// 001cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00200
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00204
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00210
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00214
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00218
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00220
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00224
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00228
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0022c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00234
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00244
			 0x00011111,                                                  // 9.7958E-41                                  /// 00248
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0024c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00250
			 0x4b000000,                                                  // 8388608.0                                   /// 00254
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0025c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00264
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0026c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00274
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x55555555,                                                  // 4 random values                             /// 0028c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00294
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xff800000,                                                  // -inf                                        /// 002b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00300
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00304
			 0xcb000000,                                                  // -8388608.0                                  /// 00308
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00310
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00314
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00318
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00328
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0032c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00330
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00334
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c400000,                                                  // Leading 1s:                                 /// 0033c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00340
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00368
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0036c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00370
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00378
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0037c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x0e000007,                                                  // Trailing 1s:                                /// 0038c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00394
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a4
			 0x80000000,                                                  // -zero                                       /// 003a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00400
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00404
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00408
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0040c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00410
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c780000,                                                  // Leading 1s:                                 /// 0041c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00424
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00434
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0043c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00440
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00444
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00448
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00450
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00454
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00458
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00468
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0046c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00470
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00484
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00488
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0048c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x7f800000,                                                  // inf                                         /// 00494
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0049c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00500
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00510
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00514
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00520
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00524
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00528
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00534
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0053c
			 0xcb000000,                                                  // -8388608.0                                  /// 00540
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00544
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00548
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0054c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00550
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00554
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0055c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0xcb000000,                                                  // -8388608.0                                  /// 00564
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00568
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0056c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00570
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00578
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0057c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00580
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00584
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00588
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0058c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00590
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00594
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00598
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0059c
			 0x4b000000,                                                  // 8388608.0                                   /// 005a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005dc
			 0x7fc00001,                                                  // signaling NaN                               /// 005e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e8
			 0x33333333,                                                  // 4 random values                             /// 005ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00600
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00604
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00608
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0060c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00610
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00614
			 0x0e000001,                                                  // Trailing 1s:                                /// 00618
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x33333333,                                                  // 4 random values                             /// 00624
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00630
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00634
			 0x0e000007,                                                  // Trailing 1s:                                /// 00638
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00640
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00648
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00650
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00654
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00658
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00660
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00664
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00668
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00670
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0067c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00688
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0068c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00694
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00698
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0069c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00700
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00704
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00708
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00718
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0071c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00728
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0072c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0073c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00740
			 0x7fc00001,                                                  // signaling NaN                               /// 00744
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00748
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00750
			 0xbf028f5c,                                                  // -0.51                                       /// 00754
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x4b000000,                                                  // 8388608.0                                   /// 0075c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00760
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00764
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00770
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00778
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0077c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00784
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0078c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00790
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0079c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007cc
			 0x7f800000,                                                  // inf                                         /// 007d0
			 0x4b000000,                                                  // 8388608.0                                   /// 007d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00800
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00804
			 0x00011111,                                                  // 9.7958E-41                                  /// 00808
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0080c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00810
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00818
			 0xff800000,                                                  // -inf                                        /// 0081c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00820
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xcb000000,                                                  // -8388608.0                                  /// 00828
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0082c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00834
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00838
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00844
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0084c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00854
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0085c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00860
			 0x7f800000,                                                  // inf                                         /// 00864
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00868
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00870
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00878
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0087c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x55555555,                                                  // 4 random values                             /// 00884
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00888
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0088c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0e000007,                                                  // Trailing 1s:                                /// 00894
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0089c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008c0
			 0xff800000,                                                  // -inf                                        /// 008c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d8
			 0xffc00001,                                                  // -signaling NaN                              /// 008dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008ec
			 0x80000000,                                                  // -zero                                       /// 008f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00900
			 0x7f800000,                                                  // inf                                         /// 00904
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00910
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00920
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00924
			 0x3f028f5c,                                                  // 0.51                                        /// 00928
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00938
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00940
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00948
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00954
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00958
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0095c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00960
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00964
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00968
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00970
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00974
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00978
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0097c
			 0xcb000000,                                                  // -8388608.0                                  /// 00980
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00984
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00988
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00990
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00994
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00998
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0099c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x80000000,                                                  // -zero                                       /// 009f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x4b000000,                                                  // 8388608.0                                   /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a50
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a64
			 0x4b000000,                                                  // 8388608.0                                   /// 00a68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000000,                                                  // -zero                                       /// 00a80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a94
			 0x33333333,                                                  // 4 random values                             /// 00a98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aa0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00aa4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aac
			 0x55555555,                                                  // 4 random values                             /// 00ab0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ab4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ab8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00abc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ac0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ac4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ad8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00adc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ae0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00afc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b24
			 0x55555555,                                                  // 4 random values                             /// 00b28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b50
			 0xff800000,                                                  // -inf                                        /// 00b54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b80
			 0xcb000000,                                                  // -8388608.0                                  /// 00b84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ba0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bdc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x7fc00001,                                                  // signaling NaN                               /// 00be8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bf0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bfc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00000000,                                                  // zero                                        /// 00c58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c68
			 0xbf028f5c,                                                  // -0.51                                       /// 00c6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c90
			 0x33333333,                                                  // 4 random values                             /// 00c94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x33333333,                                                  // 4 random values                             /// 00cb8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cbc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cc8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cd0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ce0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ce8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x55555555,                                                  // 4 random values                             /// 00d10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0xffc00001,                                                  // -signaling NaN                              /// 00d54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d64
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d68
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x3f028f5c,                                                  // 0.51                                        /// 00d7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00da4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00da8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00db4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00db8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dbc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ddc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0xff800000,                                                  // -inf                                        /// 00de4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00de8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00df0
			 0xffc00001,                                                  // -signaling NaN                              /// 00df4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00df8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xcb000000,                                                  // -8388608.0                                  /// 00e00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e7c
			 0xff800000,                                                  // -inf                                        /// 00e80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0xff800000,                                                  // -inf                                        /// 00e90
			 0x00000000,                                                  // zero                                        /// 00e94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00eb4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ebc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ecc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00edc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ef0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80000000,                                                  // -zero                                       /// 00f74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fcc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00800000 // min norm                                      // SP +ve numbers                              /// last
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
			 0x00000560,
			 0x00000568,
			 0x00000398,
			 0x00000434,
			 0x00000544,
			 0x00000610,
			 0x00000328,
			 0x00000124,

			 /// vpu register f2
			 0x41100000,
			 0x41300000,
			 0x41f00000,
			 0x40e00000,
			 0x41600000,
			 0x41b00000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f3
			 0x41800000,
			 0x41c00000,
			 0x41c00000,
			 0x40400000,
			 0x41e80000,
			 0x41400000,
			 0x41300000,
			 0x42000000,

			 /// vpu register f4
			 0x41e80000,
			 0x41400000,
			 0x42000000,
			 0x41b80000,
			 0x41100000,
			 0x41880000,
			 0x41f80000,
			 0x41d00000,

			 /// vpu register f5
			 0x41980000,
			 0x40c00000,
			 0x41b00000,
			 0x40400000,
			 0x41f80000,
			 0x41d00000,
			 0x41a80000,
			 0x41400000,

			 /// vpu register f6
			 0x40800000,
			 0x41d00000,
			 0x41200000,
			 0x41c80000,
			 0x41f00000,
			 0x40000000,
			 0x41b00000,
			 0x41100000,

			 /// vpu register f7
			 0x41f80000,
			 0x41800000,
			 0x41f80000,
			 0x41b00000,
			 0x41500000,
			 0x40a00000,
			 0x41100000,
			 0x41b00000,

			 /// vpu register f8
			 0x3f800000,
			 0x41100000,
			 0x41e00000,
			 0x3f800000,
			 0x41d00000,
			 0x41400000,
			 0x41b00000,
			 0x41d80000,

			 /// vpu register f9
			 0x41980000,
			 0x3f800000,
			 0x41880000,
			 0x40c00000,
			 0x41100000,
			 0x41800000,
			 0x41c80000,
			 0x41400000,

			 /// vpu register f10
			 0x41b80000,
			 0x41200000,
			 0x3f800000,
			 0x41c00000,
			 0x40e00000,
			 0x40400000,
			 0x41f80000,
			 0x41d80000,

			 /// vpu register f11
			 0x41e00000,
			 0x41b80000,
			 0x40e00000,
			 0x41900000,
			 0x42000000,
			 0x41d00000,
			 0x41500000,
			 0x41600000,

			 /// vpu register f12
			 0x41f80000,
			 0x40000000,
			 0x41b00000,
			 0x3f800000,
			 0x41900000,
			 0x41e80000,
			 0x41700000,
			 0x40c00000,

			 /// vpu register f13
			 0x41e80000,
			 0x40e00000,
			 0x41800000,
			 0x40a00000,
			 0x41d80000,
			 0x41f80000,
			 0x41e80000,
			 0x41700000,

			 /// vpu register f14
			 0x41600000,
			 0x41800000,
			 0x41c80000,
			 0x41000000,
			 0x41e00000,
			 0x41000000,
			 0x40a00000,
			 0x41b80000,

			 /// vpu register f15
			 0x41000000,
			 0x41200000,
			 0x41b80000,
			 0x40e00000,
			 0x41400000,
			 0x40400000,
			 0x41900000,
			 0x40e00000,

			 /// vpu register f16
			 0x40e00000,
			 0x41f00000,
			 0x41600000,
			 0x40800000,
			 0x41600000,
			 0x41980000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f17
			 0x41e80000,
			 0x41500000,
			 0x41f00000,
			 0x41c80000,
			 0x40800000,
			 0x41980000,
			 0x41880000,
			 0x41300000,

			 /// vpu register f18
			 0x41880000,
			 0x41e00000,
			 0x41800000,
			 0x41f00000,
			 0x41f00000,
			 0x41600000,
			 0x40400000,
			 0x41880000,

			 /// vpu register f19
			 0x41880000,
			 0x41f00000,
			 0x41300000,
			 0x41f80000,
			 0x40000000,
			 0x40e00000,
			 0x41400000,
			 0x41100000,

			 /// vpu register f20
			 0x41700000,
			 0x41b00000,
			 0x40e00000,
			 0x41400000,
			 0x41800000,
			 0x41300000,
			 0x41e00000,
			 0x41d80000,

			 /// vpu register f21
			 0x41d00000,
			 0x41a00000,
			 0x41d00000,
			 0x41200000,
			 0x40c00000,
			 0x41200000,
			 0x41600000,
			 0x40400000,

			 /// vpu register f22
			 0x40e00000,
			 0x40000000,
			 0x40400000,
			 0x41000000,
			 0x41600000,
			 0x41300000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f23
			 0x41100000,
			 0x41700000,
			 0x41d00000,
			 0x41c00000,
			 0x41c00000,
			 0x41f80000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f24
			 0x41f00000,
			 0x41b00000,
			 0x41100000,
			 0x40e00000,
			 0x41f00000,
			 0x41500000,
			 0x41900000,
			 0x41a00000,

			 /// vpu register f25
			 0x41b80000,
			 0x41500000,
			 0x41200000,
			 0x41000000,
			 0x41e00000,
			 0x41400000,
			 0x42000000,
			 0x41a80000,

			 /// vpu register f26
			 0x40800000,
			 0x41e00000,
			 0x41e00000,
			 0x41600000,
			 0x41900000,
			 0x41f00000,
			 0x41400000,
			 0x41000000,

			 /// vpu register f27
			 0x41800000,
			 0x41700000,
			 0x41980000,
			 0x41900000,
			 0x41600000,
			 0x41100000,
			 0x41a00000,
			 0x41c80000,

			 /// vpu register f28
			 0x41300000,
			 0x41c00000,
			 0x41300000,
			 0x41a80000,
			 0x41880000,
			 0x41f80000,
			 0x41f00000,
			 0x41a00000,

			 /// vpu register f29
			 0x41f00000,
			 0x41800000,
			 0x41e00000,
			 0x41400000,
			 0x41800000,
			 0x41a80000,
			 0x41a00000,
			 0x40800000,

			 /// vpu register f30
			 0x40e00000,
			 0x41300000,
			 0x41b00000,
			 0x40a00000,
			 0x41200000,
			 0x40c00000,
			 0x42000000,
			 0x40c00000,

			 /// vpu register f31
			 0x41f00000,
			 0x41b00000,
			 0x41e80000,
			 0x41f80000,
			 0x41980000,
			 0x41d80000,
			 0x40000000,
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
		"fcvt.f16.ps f25, f9\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f6, f11\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f4, f7\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f19, f18\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f24, f14\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f9, f4\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f11, f1\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f17, f15\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f16, f27\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f27, f14\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f27, f26\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f7, f24\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f14, f14\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f11, f29\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f6, f22\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f15, f13\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f4, f7\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f27, f13\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f10, f22\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f6, f29\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f13, f18\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f10, f5\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f24, f20\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f6, f28\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f15, f29\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f5, f2\n"                                ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f29, f16\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f10, f3\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f27, f26\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f7, f9\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f28, f10\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f2, f11\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f3, f8\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f5, f1\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f8, f1\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f17, f25\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f29, f19\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f22, f1\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f3, f23\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f24, f15\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f26, f29\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f26, f17\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f8, f12\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f7, f4\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f13, f17\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f17, f29\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f11, f17\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f9, f7\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f22, f11\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f25, f20\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f27, f30\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f6, f12\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f30, f0\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f22, f8\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f17, f10\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f20, f2\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f10, f25\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f12, f3\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f6, f19\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f5, f3\n"                                ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f19, f25\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f9, f17\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f11, f6\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f11, f1\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f11, f13\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f19, f28\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f7, f5\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f2, f6\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f26, f6\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f5, f2\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f9, f26\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f30, f11\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f12, f24\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f28, f17\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f11, f26\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f6, f12\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f21, f12\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f13, f1\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f26, f15\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f28, f5\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f12, f26\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f13, f4\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f8, f11\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f23, f12\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f19, f19\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f10, f27\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f28, f12\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f17, f16\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f9, f28\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f15, f4\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f17, f27\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f5, f10\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f16, f29\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f30, f20\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f27, f24\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f20, f26\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f22, f18\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f5, f4\n"                                ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f16.ps f15, f22\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
